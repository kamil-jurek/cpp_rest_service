#pragma once

#include <mongocxx/client.hpp>

#include <vector>

#include "user_manager.hpp"

class DbManager 
{
  public:
    DbManager();
    ~DbManager();

    mongocxx::database getDatabase(const std::string&);
    void createDocumemt();

    void signUp(UserInformation & userInfo);
    bool signOn(const std::string email, const std::string password, UserInformation& userInfo);
    UserInformation getUser(unsigned int userId);
    std::vector<UserInformation> getUsers();
  
  private:
    mongocxx::client client;
    unsigned int getNextId();
};