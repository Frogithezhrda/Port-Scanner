#include "Scanner.h"

static int counter = 0;

Scanner::Scanner(const unsigned short numOfThreads, const std::string targetIP, const unsigned short startingPort, const unsigned short endingPort)
{
	int amountOfPorts = (endingPort - startingPort) / numOfThreads;
	unsigned short endPort = 0;
	unsigned short startPort = 0;
	m_targetIP = targetIP;
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
		m_scanThreads[i].detach();
	}
	while (counter < numOfThreads);
	printOpenPorts();
}

void Scanner::scanForPorts(const unsigned short startPort, const unsigned short endPort)
{
	SOCKET clientSocket;
	struct sockaddr_in sa = { 0 };
	for (unsigned short i = startPort; i < endPort; i++)
	{
		createSocket(clientSocket);
		sa.sin_port = htons(i); // port that server will listen for
		sa.sin_family = AF_INET;   // must be AF_INET
		sa.sin_addr.s_addr = inet_addr(m_targetIP.c_str());    // when there are few ip's for the machine. We will use always "INADDR_ANY"
		if (connect(clientSocket, (struct sockaddr*)&sa, sizeof(sa)) != SOCKET_ERROR)
		{
			std::lock_guard<std::mutex> lock(mtx);
			std::cout << "Open Port At: " << i << " Amount Of Threads Closed: " << counter << std::endl;
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
