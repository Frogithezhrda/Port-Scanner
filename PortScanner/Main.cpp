#pragma comment (lib, "ws2_32.lib")
#include "WinInitializer.hpp"
#include "Scanner.h"


constexpr const int MAX_ARGS = 5;

typedef websocketpp::server<websocketpp::config::asio> server;

int main(void)
{
	WSAInitializer wsa;
	server webServer;

	webServer.init_asio();

	webServer.set_open_handler([](websocketpp::connection_hdl hdl) {
		std::cout << "Client connected\n";
		});
	webServer.set_message_handler([&webServer](websocketpp::connection_hdl hdl, server::message_ptr msg) {
		json data = json::parse(msg->get_payload());
		unsigned short numberOfThreads = data["numberOfThreads"];
		std::string targetIP = data["targetIP"];
		unsigned short startPort = data["startPort"];
		unsigned short endPort = data["endPort"];
		try
		{
			Scanner scanner = Scanner(numberOfThreads, targetIP, startPort, endPort, webServer, hdl);
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what();
		}
		});


	webServer.listen(8080);
	webServer.start_accept();
	webServer.run();
	return 0;
}