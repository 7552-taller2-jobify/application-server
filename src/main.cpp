// "Copyright 2016 <Jobify>"

#include <iostream>
#include "Server.h"

int main(int argc, char *argv[]) {
    Server* server = new Server();
    server->start();
    return 0;
}
