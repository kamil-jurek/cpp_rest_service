#pragma once
#include <map>
#include <vector>
#include <std_micro_service.hpp>

typedef struct 
{
   std::string email;
   std::string password;
   std::string name;
   std::string lastName;
   double weight;   

} UserInformation;

class UserManagerException : public std::exception 
{
   std::string _message;

public:
   UserManagerException(const std::string & message) : _message(message) { }
   const char * what() const throw() 
   {
      return _message.c_str();
   }
};

// alias declaration of our In Memory database...
using UserDatabase = std::map<std::string, UserInformation>;

class UserManager 
{
  public:
   void signUp(const UserInformation & userInfo);
   
   bool signOn(const std::string email, const std::string password, UserInformation& userInfo);

   bool setUserWeight(const std::string email, double weight);

   std::vector<UserInformation> getUsers();
};