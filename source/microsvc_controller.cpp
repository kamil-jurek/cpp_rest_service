#include <std_micro_service.hpp>
#include "microsvc_controller.hpp"

#include "user_manager.hpp"
#include "kj_utils.hpp"
#include <runtime_utils.hpp>


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
    listener.support(methods::OPTIONS, std::bind(&MicroserviceController::handleOptions, this, std::placeholders::_1));
}

void MicroserviceController::handleGet(http_request message) 
{
    auto path = requestPath(message);  
    auto requestPathStr =  requestPathString(message);
    TRACE("Request path: ", requestPathStr);

    if (!path.empty()) 
    {
        if (requestPathStr == "/test") 
        {                    
            auto response = handleTest();
            message.reply(status_codes::OK, response);
        }

        else if (path.size() == 2 && path[0] == "users" && std::stoi(path[1]) != 0)
        {
            handleGetUser(message, std::stoi(path[1]));
        }

        else if (requestPathStr == "/users/signon") 
        {   
            pplx::create_task([=]() -> std::tuple<bool, UserInformation> 
            {
                TRACE("pplx::create_task([=]() -> std::tuple<bool, UserInformation>");
                auto headers = message.headers();
                if (message.headers().find("Authorization") == headers.end()) 
                {    
                    throw std::exception();
                }
                
                auto authHeader = headers["Authorization"];
                TRACE("authHeader: ", authHeader);

                auto credsPos = authHeader.find("Basic");
                if (credsPos == std::string::npos) 
                {    
                    throw std::exception();
                }

                auto base64 = authHeader.substr(credsPos + std::string("Basic").length() + 1);
                if (base64.empty()) 
                {
                    throw std::exception();
                }

                auto bytes = utility::conversions::from_base64(base64);
                std::string creds(bytes.begin(), bytes.end());
                TRACE("creds:" , creds);

                auto colonPos = creds.find(":");
                if (colonPos == std::string::npos) 
                {
                    throw std::exception();
                }
                
                auto useremail = creds.substr(0, colonPos);
                auto password = creds.substr(colonPos + 1, creds.size() - colonPos - 1);            
                TRACE("useremail: ", useremail, " password: ", password);    

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
                        // message.reply(status_codes::OK, response);

                        http_response httpResponse = prepareResponse(status_codes::OK);
                        message.reply(httpResponse);
   
                    }
                    else 
                    {
                        //message.reply(status_codes::Unauthorized);
                        http_response httpResponse = prepareResponse(status_codes::Unauthorized);
                        message.reply(httpResponse);
                    }
                }
                catch(std::exception) 
                {
                    //message.reply(status_codes::Unauthorized);
                    http_response httpResponse = prepareResponse(status_codes::Unauthorized);
                    message.reply(httpResponse);
                }
            });
        }
        else if (requestPathStr == "/users")
        {
            handleGetUsers(message);     
        }
   }

    else {
        TRACE("NotFound");
        message.reply(status_codes::NotFound);
    }
}

void MicroserviceController::handlePatch(http_request message) {
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::PATCH));
}

void MicroserviceController::handlePut(http_request message) 
{    
    auto path = requestPath(message);
    auto requestPathStr =  requestPathString(message);
    TRACE("Request path: ", requestPathStr);
    
    if (path.size() >= 2 && path[0] == "users") 
    {       
        TRACE("headers.content_type: ", message.headers().content_type());
        
        std::string email = path[1];
        TRACE("email: ", email);
        //TRACE("request: ", message.extract_string().get());
        
        message.extract_json().then([=](json::value request) 
        {
            TRACE("0,5");
            try 
            {               
                double weight = request.at("weight").as_double();
                TRACE("weight: ", weight);

                UserManager users;
                bool result = users.setUserWeight(email, weight);
                
                json::value responseJson;
                if (result)
                {
                    responseJson["message"] = json::value::string("succesful update!");
                    http_response response(status_codes::OK);
                    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                    response.set_body(responseJson);
                    message.reply(response);
                }
                else 
                {
                    responseJson["message"] = json::value::string("unsuccesful update!");
                    http_response response(status_codes::BadRequest);
                    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
                    response.set_body(responseJson);
                    message.reply(response);
                }            
            }
            catch(UserManagerException & e) 
            {
                TRACE("UserManagerException: ", e.what());
                message.reply(status_codes::BadRequest, e.what());
            }
            catch(json::json_exception & e) 
            {
                TRACE("json::json_exception: ", e.what());
                message.reply(status_codes::BadRequest);
            }
            catch(...) 
            {   
                TRACE("Exception ...");
                RuntimeUtils::printStackTrace();
            }
        });
    }
    else
    {
        TRACE("MicroserviceController::handlePost not users/signup");
    }
}

void MicroserviceController::handlePost(http_request message) {
    TRACE("MicroserviceController::handlePost");
    
    auto requestPathStr =  requestPathString(message);
    TRACE("Request path: ", requestPathStr);
    
    if (requestPathStr == "/users/signup") 
    {       
        handleUserSignUp(message);
    }
}

void MicroserviceController::handleDelete(http_request message) {    
    message.reply(status_codes::NotImplemented, responseNotImpl(methods::DEL));
}

void MicroserviceController::handleHead(http_request message) {
    TRACE("MicroserviceController::handleOptions");
    
    http_response response = prepareResponse(status_codes::OK);
    message.reply(response);
}

void MicroserviceController::handleOptions(http_request message) 
{
    TRACE("MicroserviceController::handleOptions");
    
    http_response response = prepareResponse(status_codes::OK);
    message.reply(response);
   
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

json::value MicroserviceController::handleTest()
{
    auto currentTime = std::chrono::system_clock::now();
    auto response = json::value::object();

    response["version"] = json::value::string("0.1.1");
    response["status"] = json::value::string("ready!");
    response["time"] = json::value::string(kj::timePointAsString(currentTime));
    
    return response;
}

void MicroserviceController::handleUserSignUp(http_request message)
{   
    TRACE("MicroserviceController::handleUserSignUp()");
    TRACE("headers.content_type: ", message.headers().content_type());

    message.extract_json().then([=](json::value request) 
    {
        try 
        {   
            UserInformation userInfo 
            { 
                0,
                request.at("email").as_string(),
                request.at("password").as_string(),
                request.at("name").as_string(),
                request.at("lastName").as_string(),
                0.0
            };
            
            UserManager users;
            users.signUp(userInfo);
            
            json::value responseJson;
            responseJson["message"] = json::value::string("succesful registration!");
            
            //message.reply(status_codes::OK, response);

            http_response response(status_codes::OK);
            response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
            response.set_body(responseJson);
            message.reply(response);
        }
        catch(UserManagerException & e) 
        {
            TRACE("UserManagerException: ", e.what());
            message.reply(status_codes::BadRequest, e.what());
        }
        catch(json::json_exception & e) 
        {
            TRACE("json::json_exception: ", e.what());
            message.reply(status_codes::BadRequest);
        }
        catch(...) 
        {   
            TRACE("Exception ...");
            RuntimeUtils::printStackTrace();
        }
    });
}

void MicroserviceController::handleGetUsers(http_request message)
{
    TRACE("MicroserviceController::handleGetUsers()");
    TRACE("headers.content_type: ", message.headers().content_type());

    pplx::create_task([=]() 
    {
        auto responseJson = json::value::object();
        std::vector<web::json::value> users;
        UserManager userManager;

        auto usersVector = userManager.getUsers();
        for(auto const& userDb : usersVector)
        {
            json::value user;
            user["email"] = json::value::string(userDb.email);
            user["name"] = json::value::string(userDb.name);
            user["lastName"] = json::value::string(userDb.lastName); 
            user["weight"] = json::value::number(userDb.weight); 
            user["userId"] = json::value::number(userDb.userId); 

            users.push_back(user);
        }
        responseJson["users"] = json::value::array(users);

        // message.reply(status_codes::OK, response);  
        http_response response(status_codes::OK);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.set_body(responseJson);

        message.reply(response);
    }); 
}

void MicroserviceController::handleGetUser(http_request message, int userId)
{
    TRACE("MicroserviceController::handleGetUser(", userId, ")");
    TRACE("headers.content_type: ", message.headers().content_type());

    pplx::create_task([=]() 
    {
        auto responseJson = json::value::object();
        UserManager userManager;

        auto userDb = userManager.getUser(userId);         
        responseJson["email"] = json::value::string(userDb.email);
        responseJson["name"] = json::value::string(userDb.name);
        responseJson["lastName"] = json::value::string(userDb.lastName); 
        responseJson["weight"] = json::value::number(userDb.weight); 
        responseJson["userId"] = json::value::number(userDb.userId); 

        // message.reply(status_codes::OK, response);  
        http_response response(status_codes::OK);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.set_body(responseJson);

        message.reply(response);
    }); 
}

web::http::http_response MicroserviceController::prepareResponse(http::status_code code)
{
    http_response response(code);
    response.headers().add(U("Allow"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type, Authorization"));  

    return response; 
}