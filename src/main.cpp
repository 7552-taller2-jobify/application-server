#include <iostream>
#include "Server.h"

using namespace std;

int main(int argc, char *argv[]){

	Server* server = new Server();
	server->start();

	return 0;
}
