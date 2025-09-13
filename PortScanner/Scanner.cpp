#include "Scanner.h"

std::atomic<int> counter{0};
std::atomic<int> scannedPorts{0};

Scanner::Scanner(const unsigned short numOfThreads, const std::string targetIP, const unsigned short startingPort, const unsigned short endingPort, websocketpp::server<websocketpp::config::asio>& server, websocketpp::connection_hdl& hdl)
{
	int amountOfPorts = (endingPort - startingPort) / numOfThreads;
	unsigned short endPort = 0;
	unsigned short startPort = 0;
	m_targetIP = targetIP;
	m_server = &server;
	m_hdl = hdl;
	if (startingPort > endingPort)
	{
		throw ScannerException("Ending Port Can't Be Less than starting port");
	}
	
	for (unsigned short i = 0; i < numOfThreads; i++)
	{
		endPort = CALC_PORT(startingPort, i + 1, amountOfPorts);
		startPort = CALC_PORT(startingPort, i, amountOfPorts);
		if (endPort > endingPort) endPort = endingPort;
		std::thread thread(&Scanner::scanForPorts, this, startPort, endPort);
		m_scanThreads.push_back(std::move(thread));
	}
	for (auto& thread : m_scanThreads) 
	{
		if (thread.joinable()) thread.join();
	}
	while (counter < numOfThreads);
	printOpenPorts();
	json msg;
	msg["finished"] = true;           
	msg["scanned"] = endingPort - startingPort; 
	msg["total"] = endingPort - startingPort; 
	m_server->send(m_hdl, msg.dump(), websocketpp::frame::opcode::text);
}

void Scanner::scanForPorts(const unsigned short startPort, const unsigned short endPort)
{
	SOCKET clientSocket;
	struct sockaddr_in sa = { 0 };
	int totalScanned = endPort - startPort;
	for (unsigned short i = startPort; i < endPort; i++)
	{
		scannedPorts++;
		createSocket(clientSocket);
		sa.sin_port = htons(i); // port that server will listen for
		sa.sin_family = AF_INET;   // must be AF_INET
		sa.sin_addr.s_addr = inet_addr(m_targetIP.c_str());    // when there are few ip's for the machine. We will use always "INADDR_ANY"
		if (connect(clientSocket, (struct sockaddr*)&sa, sizeof(sa)) != SOCKET_ERROR)
		{
			std::lock_guard<std::mutex> lock(mtx);
			json msg;
			msg["port"] = i;           // the open port
			msg["scanned"] = scannedPorts.load();  // how many ports have been scanned so far
			msg["total"] = totalScanned; // total ports to scan
			m_server->send(m_hdl, msg.dump(), websocketpp::frame::opcode::text);
			m_portsOpen.push_back(i);
		}
	}
	counter++;
}

void Scanner::createSocket(SOCKET& sock)
{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		//getting function name and socket error
		throw ConnectionException(__FUNCTION__);
	}
}

void Scanner::printOpenPorts()
{
	for (int i = 0; i < m_portsOpen.size(); i++)
	{
		std::cout << "Port Open At - " << m_portsOpen[i] << std::endl;
	}
}
