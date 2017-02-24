#include<functional>
#include "tcp_server.h"
#include "tcp_client.h"
#include <session.h>
#include "packer.h"

using namespace Bull;
char data[1024] = "Hello, world!";
const char* default_ip = "192.168.1.17";
using Client = TcpClient<Session<Handler<64>>>;
using Server = TcpServer<Session<Handler<64>>>;
Client* client;
Server* server;


void request() {
	client->request(data, strlen(data) + 1, [](bool success, CircularBuf<Handler<64>::CircularBufCapacity::Value>* pcb) {
		                if (success) {
			                request();
		                }
	                });
}

void push() {
	client->push(data, strlen(data) + 1, [](bool status) {
		             if (status) {
			             push();
		             }
	             });
}

int main(int argc, char** argv) {
	if (argc > 1) {
		printf("%s %s\n", argv[0], argv[1]);

		if (strcmp(argv[1], "s") == 0) {
			server = new Server("0.0.0.0", 5001);
			server->startup();
			RUN_UV_DEFAULT_LOOP();
			server->shutdown();
			delete server;
		}
		else if (strcmp(argv[1], "c") == 0) {
			client = new Client(argc > 2 ? argv[2] : default_ip, 5001, []() {
				                    request();
			                    }, []() { }, true);
			client->startup();
			RUN_UV_DEFAULT_LOOP();
			client->shutdown();
			delete client;
		}
		return 0;
	}

	return -1;
}
