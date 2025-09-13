#pragma once
#include <vector>
#include <thread>
#include "ScannerException.hpp"
#include "ConnectionException.hpp"
#include <mutex>

#include <Windows.h>

#define CALC_PORT(start, threadNum, amount) ((start) + ((threadNum) * (amount)))

class Scanner
{
public:
	Scanner(const unsigned short numOfThreads, const std::string targetIP, const unsigned short startingPort, const unsigned short endingPort);
	~Scanner() = default;


private:
	std::vector<std::thread> m_scanThreads;
	std::string m_targetIP;
	std::vector<int> m_portsOpen;
	std::mutex mtx;

	void scanForPorts(const unsigned short startPort, const unsigned short endPort);
	void createSocket(SOCKET& sock);
	void printOpenPorts();
};