#include <std_micro_service.hpp>
#include "microsvc_controller.hpp"

#include "user_manager.hpp"
#include "kjUtils.hpp"

#include <tuple>
#include <chrono>
#include <ctime>    

using namespace web;
using namespace http;

void MicroserviceController::initRestOpHandlers() 
{
    listener.support(methods::GET, std::bind(&MicroserviceController::handleGet, this, std::placeholders::_1));
    listener.support(methods::PUT, std::bind(&MicroserviceController::handlePut, this, std::placeholders::_1));
    listener.support(methods::POST, std::bind(&MicroserviceController::handlePost, this, std::placeholders::_1));
    listener.support(methods::DEL, std::bind(&MicroserviceController::handleDelete, this, std::placeholders::_1));
    listener.support(methods::PATCH, std::bind(&MicroserviceController::handlePatch, this, std::placeholders::_1));
}

void MicroserviceController::handleGet(http_request message) 
{
    auto path = requestPath(message);
    
    TRACE("MicroserviceController::handleGet");
    TRACE("Request path: ", requestPathString(message));

    if (!path.empty()) 
    {
        if (path[0] == "test") 
        {                    
            auto currentTime = std::chrono::system_clock::now();
            std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

            auto response = json::value::object();
            response["version"] = json::value::string("0.1.1");
            response["status"] = json::value::string("ready!");
            response["time"] = json::value::string(std::ctime(&currentTime_t));
            message.reply(status_codes::OK, response);
        }
    
        else if (path.size() == 2 &&
                 path[0] == "users" && 
                 path[1] == "signon") 
        {   
            std::cout << "KJ: 0";
            pplx::create_task([=]() -> std::tuple<bool, UserInformation> 
            {
                std::cout << "KJ: 1";
                auto headers = message.headers();
                if (message.headers().find("Authorization") == headers.end()) 
                {    
                    throw std::exception();
                }
                
                std::cout << "KJ: 2";
                auto authHeader = headers["Authorization"];
                
                auto credsPos = authHeader.find("Basic");
                if (credsPos == std::string::npos) 
                {    
                    throw std::exception();
                }
                std::cout << "KJ: 3";
                auto base64 = authHeader.substr(credsPos + std::string("Basic").length() + 1);
                std::cout << "KJ: 4 " << base64 << "\n";
                if (base64.empty()) 
                {
                    throw std::exception();
                }
                std::cout << "KJ: 5";
                auto bytes = utility::conversions::from_base64(base64);
                std::string creds(bytes.begin(), bytes.end());
                auto colonPos = creds.find(":");
                if (colonPos == std::string::npos) 
                {
                    throw std::exception();
                }
                
                auto useremail = creds.substr(0, colonPos);
                auto password = creds.substr(colonPos + 1, creds.size() - colonPos - 1);            
                        
                UserManager users;
                UserInformation userInfo;            
                if (users.signOn(useremail, password, userInfo)) {
                    return std::make_tuple(true, userInfo);
                }
                else {
                    return std::make_tuple(false, UserInformation {});
                }
            })
            .then([=](pplx::task<std::tuple<bool, UserInformation>> resultTsk) 
            {
                try 
                {
                    auto result = resultTsk.get();
                    if (std::get<0>(result) == true) 
                    {
                        json::value response;
                        response["success"] = json::value::string("welcome " + std::get<1>(result).name + "!");                    
                        message.reply(status_codes::OK, response);
                    }
                    else 
                    {
                        message.reply(status_codes::Unauthorized);
                    }
                }
                catch(std::exception) 
                {
                    message.reply(status_codes::Unauthorized);
                }
            });
        }
         else if (path.size() == 1 && path[0] == "users")
        {
            pplx::create_task([=]() 
            {
                auto response = json::value::object();
                std::vector<web::json::value> users;
                UserManager userManager;

                auto usersVector = userManager.getUsers();
                for(auto const& userDb : usersVector)
                {
                    json::value user;
                    user["email"] = json::value::string(userDb.email);
                    user["lastName"] = json::value::string(userDb.lastName); 

                    users.push_back(user);
                }
                response["users"] = json::value::array(users);

                message.reply(status_codes::OK, response);  
            });      
        }
   }

    else {
        message.reply(status_codes::NotFound);
    }
}

void MicroserviceController::handlePatch(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::PATCH));
}

void MicroserviceController::handlePut(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::PUT));
}

void MicroserviceController::handlePost(http_request message) {
    TRACE("MicroserviceController::handlePost");
    TRACE("Request path: ", requestPathString(message));
    
    auto path = requestPath(message);
    if (path.size() == 2 && 
        path[0] == "users" && 
        path[1] == "signup") 
    {
        //TRACE("message: ", message.extract_string().get());
        
        message.
        extract_json().
        then([=](json::value request) 
        {
            try 
            {
                UserInformation userInfo 
                { 
                    request.at("email").as_string(),
                    request.at("password").as_string(),
                    request.at("name").as_string(),
                    request.at("lastName").as_string()
                };
                
                UserManager users;
                users.signUp(userInfo);
                json::value response;
                response["message"] = json::value::string("succesful registration!");
                message.reply(status_codes::OK, response);
            }
            catch(UserManagerException & e) 
            {
                message.reply(status_codes::BadRequest, e.what());
            }
            catch(json::json_exception & e) 
            {
                message.reply(status_codes::BadRequest);
            }
        });
    }
}

void MicroserviceController::handleDelete(http_request message) {    
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::DEL));
}

void MicroserviceController::handleHead(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::HEAD));
}

void MicroserviceController::handleOptions(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::OPTIONS));
}

void MicroserviceController::handleTrace(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::TRCE));
}

void MicroserviceController::handleConnect(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::CONNECT));
}

void MicroserviceController::handleMerge(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::MERGE));
}

json::value MicroserviceController::responseNotImpl(const http::method & method) {
    auto response = json::value::object();
    response["serviceName"] = json::value::string("C++ Mircroservice Sample");
    response["http_method"] = json::value::string(method);
    return response ;
}