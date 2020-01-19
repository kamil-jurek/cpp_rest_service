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

bool UserManager::updateUser(unsigned int userId, 
                             const std::string name, 
                             const std::string lastName, 
                             const std::string date,
                             double weight)
{
   TRACE("UserManager::updateUserWeight() userId:", 
          userId, " name:", name, " lastName:", lastName, " date:", date, "weight: ", weight);
   
   for(auto& emailUserInfoPair : usersDB)
   {
      if (emailUserInfoPair.second.userId == userId)
      {  
         std::unique_lock<std::mutex> lock { usersDBMutex };
         emailUserInfoPair.second.name = name;
         emailUserInfoPair.second.lastName = lastName;
         emailUserInfoPair.second.date = date;
         emailUserInfoPair.second.weight = weight;

         return true;
      }    
   }

   return false;
}

std::vector<UserInformation> UserManager::getUsers()
{
   TRACE("UserManager::getUsers()");
   std::vector<UserInformation> users;
   for(auto const& emailUserInfoPair : usersDB)
   {
      users.push_back(emailUserInfoPair.second);
   }

   return users;
}

UserInformation UserManager::getUser(unsigned int userId)
{
   TRACE("UserManager::getUser(userId:", userId, ")");
   UserInformation user;
   for(auto const& emailUserInfoPair : usersDB)
   {
      if (emailUserInfoPair.second.userId == userId)
      {  
         user = emailUserInfoPair.second;
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