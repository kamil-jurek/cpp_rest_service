#include <iostream>

#include <usr_interrupt_handler.hpp>
#include <runtime_utils.hpp>

#include "microsvc_controller.hpp"

using namespace web;
using namespace kj;

int main(int argc, const char * argv[]) 
{
    InterruptHandler::hookSIGINT();

    MicroserviceController server;
    server.setEndpoint("http://host_auto_ip4:6502/kj/api");
    
    try 
    {
        // wait for server initialization...
        server.accept().wait();
        std::cout << "Microservice now listening for requests at: " << server.endpoint() << '\n';
        
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
