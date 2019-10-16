#include "basic_controller.hpp"
#include "network_utils.hpp"

namespace kj 
{
    BasicController::BasicController() 
    {}

    BasicController::~BasicController() 
    {}
    
    void BasicController::setEndpoint(const std::string& value) 
    {
        uri endpointURI(value);
        uri_builder endpointBuilder;

        endpointBuilder.set_scheme(endpointURI.scheme());
        if (endpointURI.host() == "host_auto_ip4") 
        {
            endpointBuilder.set_host(NetworkUtils::hostIP4());        
        }
        else if (endpointURI.host() == "host_auto_ip6") 
        {
            endpointBuilder.set_host(NetworkUtils::hostIP6());
        }
        endpointBuilder.set_port(endpointURI.port());
        endpointBuilder.set_path(endpointURI.path());

        listener = http_listener(endpointBuilder.to_uri());
    }

    std::string BasicController::endpoint() const 
    {
        return listener.uri().to_string();
    }

    pplx::task<void> BasicController::accept() 
    {
        initRestOpHandlers();
        return listener.open();
    }

    pplx::task<void> BasicController::shutdown() 
    {
        return listener.close();
    }

    std::vector<utility::string_t> BasicController::requestPathVector(const http_request & message) 
    {
        auto relativePath = uri::decode(message.relative_uri().path());
        return uri::split_path(relativePath);        
    }

    utility::string_t BasicController::requestPath(const http_request & message) 
    {
        auto relativePath = uri::decode(message.relative_uri().path());
        return relativePath;        
    }
}