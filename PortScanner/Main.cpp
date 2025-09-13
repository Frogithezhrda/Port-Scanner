#pragma comment (lib, "ws2_32.lib")
#include "WinInitializer.hpp"
#include "Scanner.h"

constexpr const int MAX_ARGS = 5;

int main(int argc, char** argv)
{
	WSAInitializer wsa;
	if (argc < MAX_ARGS)
	{
		std::cout << "Not Enough Args... (Thread Count,IP SrcPort DestPort)" << std::endl;
	}
	else
	{
		try
		{
			Scanner scanner = Scanner(unsigned short(std::stoi(argv[1])), argv[2], unsigned short(std::stoi(argv[3])), unsigned short(std::stoi(argv[4])));
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what();
		}
	}
	return 0;
}