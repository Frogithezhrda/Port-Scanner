#pragma once
#include <vector>
#include <thread>
#include "ScannerException.hpp"
#include "ConnectionException.hpp"
#include <mutex>


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define ASIO_STANDALONE

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "json.hpp"


using json = nlohmann::json;

#define CALC_PORT(start, threadNum, amount) ((start) + ((threadNum) * (amount)))

class Scanner
{
public:
	Scanner(const unsigned short numOfThreads, const std::string targetIP, const unsigned short startingPort, const unsigned short endingPort, websocketpp::server<websocketpp::config::asio>& server, websocketpp::connection_hdl& hdl);
	~Scanner() = default;


private:
	std::vector<std::thread> m_scanThreads;
	std::string m_targetIP;
	std::vector<int> m_portsOpen;
	std::mutex mtx;
	websocketpp::server<websocketpp::config::asio>* m_server;
	websocketpp::connection_hdl m_hdl;

	void scanForPorts(const unsigned short startPort, const unsigned short endPort);
	void createSocket(SOCKET& sock);
	void printOpenPorts();
};