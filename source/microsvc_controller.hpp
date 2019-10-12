#pragma once 

#include <basic_controller.hpp>

using namespace cfx;

class MicroserviceController : public BasicController, Controller 
{
public:
    MicroserviceController() : BasicController() {}
    ~MicroserviceController() {}
    
    void handleGet(http_request message) override;
    void handlePut(http_request message) override;
    void handlePost(http_request message) override;
    void handlePatch(http_request message) override;
    void handleDelete(http_request message) override;
    void handleHead(http_request message) override;
    void handleOptions(http_request message) override;
    void handleTrace(http_request message) override;
    void handleConnect(http_request message) override;
    void handleMerge(http_request message) override;
    void initRestOpHandlers() override;    

private:
    static json::value responseNotImpl(const http::method & method);
};