#include <mutex>
#include "user_manager.hpp"
#include "kj_utils.hpp"

UserDatabase usersDB;
std::mutex usersDBMutex;
std::atomic<unsigned int> userId{1000};

void UserManager::signUp(UserInformation& userInfo)
{
   TRACE("UserManager::signUp() email:", userInfo.email); 
   std::unique_lock<std::mutex> lock { usersDBMutex };
   if (usersDB.find(userInfo.email) != usersDB.end()) 
   {
      throw UserManagerException("user already exists!");
   }

   userInfo.userId = getNextId();
   usersDB.insert(std::pair<std::string, UserInformation>(userInfo.email, userInfo));   
}

bool UserManager::signOn(const std::string email, const std::string password, UserInformation& userInfo) 
{
   TRACE("UserManager::signOn() email:", email, " password: ", password);   
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

bool UserManager::setUserWeight(const std::string email, double weight)
{
   TRACE("UserManager::setUserWeight() email:", email, " weight: ", weight);
   if (usersDB.find(email) != usersDB.end()) 
   {
      std::unique_lock<std::mutex> lock { usersDBMutex };
      usersDB[email].weight = weight;

      return true;
   }

   return false;
}

std::vector<UserInformation> UserManager::getUsers()
{
   TRACE("UserManager::getUsers()");
   std::vector<UserInformation> users;
   for(auto const& kv : usersDB)
   {
      users.push_back(kv.second);
   }

   return users;
}

UserInformation UserManager::getUser(unsigned int userId)
{
   TRACE("UserManager::getUser(userId:", userId, ")");
   UserInformation user;
   for(auto const& kv : usersDB)
   {
      if (kv.second.userId == userId)
      {  
         user = kv.second;
      }    
   }

   return user;
}

unsigned int UserManager::getNextId()
{  
   unsigned int nextUserId = ++userId;
   TRACE("UserManager::getNextId(), userId: ", nextUserId);
   
   return nextUserId;
}