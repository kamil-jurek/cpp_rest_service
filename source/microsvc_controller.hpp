#pragma once 

#include <basic_controller.hpp>

using namespace kj;

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
    static json::value responseNotImpl(const http::method& method);
    void handleTest(http_request message);
    void handleUserSignUp(http_request message);
    void handleSignOn(http_request message);
    void handleGetUser(http_request message, int userId);
    void handleGetUsers(http_request message);
    void handlePutUser(http_request message, int userId);

    web::http::http_response prepareResponse(http::status_code code);
};