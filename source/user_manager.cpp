#include <mutex>
#include "user_manager.hpp"

UserDatabase usersDB;
std::mutex usersDBMutex;

void UserManager::signUp(const UserInformation& userInfo)
{
   
   std::unique_lock<std::mutex> lock { usersDBMutex };
   if (usersDB.find(userInfo.email) != usersDB.end()) 
   {
      throw UserManagerException("user already exists!");
   }
   usersDB.insert(std::pair<std::string, UserInformation>(userInfo.email, userInfo));   
}

bool UserManager::signOn(const std::string email, const std::string password, UserInformation& userInfo) 
{   
   if (usersDB.find(email) != usersDB.end()) 
   {
      auto ui = usersDB[email];
      if (ui.password == password) 
      {
         userInfo = ui;
         return true;
      }         
   }
   return false;
}

std::vector<UserInformation> UserManager::getUsers()
{
   std::vector<UserInformation> users;
      for(auto const& kv : usersDB)
      {
         users.push_back(kv.second);
      }

   return users;
}