#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>


#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "db_manager.hpp"

#include "kj_utils.hpp"

std::atomic<unsigned int> guserId{1005};

DbManager::DbManager()
{
    // Make a connection 
    mongocxx::instance instance{};
    mongocxx::uri uri{ "mongodb://localhost:27017" };
    this->client = mongocxx::client{ uri };
}

DbManager::~DbManager()
{

}

mongocxx::database DbManager::getDatabase(const std::string& dbName)   
{
    // Access database
    auto db = client[dbName];

    return db;
}  

void DbManager::createDocumemt()
{
    // Create a Document
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "userId" << 1003
        << "email" << "ccc@aaa.com"
        << "password" << "ccc"
        << "name" << "ccc"
        << "lastName" << "ccc"
        << "date" << "2020-01-22"
        << "weight" << 120.0

        << bsoncxx::builder::stream::finalize;

   
    bsoncxx::document::view view = doc_value.view();

    // Access field
    bsoncxx::document::element element = view["name"];
    std::string name = element.get_utf8().value.to_string();
    TRACE("view[name] = ", name);

    auto db = getDatabase("mydb");
    auto users = db["users"];
    
    // Insert one document
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = users.insert_one(view);

    auto cursor = users.find({});
    for (auto&& doc : cursor) 
    {
        TRACE(bsoncxx::to_json(doc), "\n");
    }
}

UserInformation DbManager::getUser(unsigned int userId)
{
    UserInformation user;

    auto db = getDatabase("mydb");
    auto users = db["users"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        users.find_one(bsoncxx::builder::stream::document{} 
            << "userId" << static_cast<int>(userId) 
            << bsoncxx::builder::stream::finalize);
    if(maybe_result) 
    {
        auto result = *maybe_result;
        TRACE(bsoncxx::to_json(result),"\n");
        auto docValueView = result.view();

        user.userId = userId;
        user.name = docValueView["name"].get_utf8().value.to_string();
        user.lastName = docValueView["lastName"].get_utf8().value.to_string();
        user.email = docValueView["email"].get_utf8().value.to_string();
        user.password = docValueView["password"].get_utf8().value.to_string();
        user.weight = docValueView["weight"].get_double().value;
    } 
    else
    {
        TRACE("No user with userId = ", userId);
    }

    return user;
}

std::vector<UserInformation> DbManager::getUsers()
{
    std::vector<UserInformation> users;

    auto db = getDatabase("mydb");
    auto usersDb = db["users"];

    mongocxx::cursor cursor = usersDb.find({});
    for (auto&& doc : cursor) 
    {
        TRACE(bsoncxx::to_json(doc),"\n");
        auto docValueView = doc;

        UserInformation user;
        user.userId = docValueView["userId"].get_int32().value;
        user.name = docValueView["name"].get_utf8().value.to_string();
        user.lastName = docValueView["lastName"].get_utf8().value.to_string();
        user.email = docValueView["email"].get_utf8().value.to_string();
        user.password = docValueView["password"].get_utf8().value.to_string();
        user.weight = docValueView["weight"].get_double().value;

        users.push_back(user);
    } 
    
    return users;
}
    
bool DbManager::signOn(const std::string email, const std::string password, UserInformation& userInfo) 
{
   TRACE("DbManager::signOn() email:", email, " password: ", password);   
   
    auto db = getDatabase("mydb");
    auto users = db["users"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        users.find_one(bsoncxx::builder::stream::document{} 
            << "email" << email
            << bsoncxx::builder::stream::finalize);
    if(maybe_result) 
    {
        auto result = *maybe_result;
        TRACE(bsoncxx::to_json(result),"\n");
        auto docValueView = result.view();

        if(password == docValueView["password"].get_utf8().value.to_string())
        {
            userInfo.userId = docValueView["userId"].get_int32().value;
            userInfo.name = docValueView["name"].get_utf8().value.to_string();
            userInfo.lastName = docValueView["lastName"].get_utf8().value.to_string();
            userInfo.email = docValueView["email"].get_utf8().value.to_string();
            userInfo.password = docValueView["password"].get_utf8().value.to_string();
            userInfo.weight = docValueView["weight"].get_double().value;
            
            return true;
        }
    } 
    
    TRACE("No user with email = ", email);
    return false;
}

void DbManager::signUp(UserInformation& userInfo)
{
   TRACE("DbManager::signUp() email:", userInfo.email); 

    auto db = getDatabase("mydb");
    auto users = db["users"];

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        users.find_one(bsoncxx::builder::stream::document{} 
            << "email" << userInfo.email
            << bsoncxx::builder::stream::finalize);
    if(maybe_result) 
    {
        TRACE("User already exists");
    }
    else 
    {
        // Create a Document
        auto builder = bsoncxx::builder::stream::document{};
        bsoncxx::document::value doc_value = builder
            << "userId" << static_cast<int>(getNextId())
            << "email" << userInfo.email
            << "password" << userInfo.password
            << "name" << userInfo.name
            << "lastName" << userInfo.lastName
            << "date" << userInfo.date
            << "weight" << userInfo.weight

            << bsoncxx::builder::stream::finalize;

   
        bsoncxx::document::view view = doc_value.view();

        // Insert one document
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = users.insert_one(view);
    }   
}

unsigned int DbManager::getNextId()
{
    unsigned int nextUserId = ++guserId;
   TRACE("UserManager::getNextId(), userId: ", nextUserId);
   
   return nextUserId;
}