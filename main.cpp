#include "server/HttpServer.hpp"
#include <server/CustomController.hpp>
#include <server/CustomFilter.hpp>
#include <server/CustomFilter2.hpp>




int main() {

    MyController a{};

    //MyFilter b{};

   
    HttpServer::instance().listen_start();
    
    
    return 0;
}





