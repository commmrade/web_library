#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <server/CustomController.hpp>
#include<print>




int main() {
    auto &app = HttpServer::instance();
    MyController::init_path_routing();

    
   
    app.listen_start();
    
    
    return 0;
}





