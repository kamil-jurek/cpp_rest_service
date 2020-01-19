#pragma once

#include <mongocxx/client.hpp>

class DbManager 
{
  public:
    DbManager();
    ~DbManager();

    mongocxx::database getDatabase(const std::string&);
    void createDocumemt();
  
  private:
    mongocxx::client client;
};