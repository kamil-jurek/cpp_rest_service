#include <iostream>

#include <usr_interrupt_handler.hpp>
#include <runtime_utils.hpp>

#include "microsvc_controller.hpp"
#include "kj_utils.hpp"

#include "db_manager.hpp"

using namespace web;
using namespace kj;

int main() 
{   
    DbManager dbManager;
    // dbManager.createDocumemt();

    //dbManager.getUser(1002);

    InterruptHandler::hookSIGINT();

    MicroserviceController server;
    server.setEndpoint("http://host_auto_ip4:6502/kj/api");
    server.setDbManager(&dbManager);

    try 
    {
        // wait for server initialization...
        server.accept().wait();
        TRACE("Microservice now listening for requests at: ",  server.endpoint());
        
        InterruptHandler::waitForUserInterrupt();

        server.shutdown().wait();
    }
    catch(std::exception & e) 
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    catch(...) 
    {
        RuntimeUtils::printStackTrace();
    }

    return 0;
}
