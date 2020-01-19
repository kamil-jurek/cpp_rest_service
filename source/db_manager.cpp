#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>


#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "db_manager.hpp"

#include "kj_utils.hpp"

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
        << "name" << "MongoDB"
        << "type" << "database"
        << "count" << 1
        << "versions" << bsoncxx::builder::stream::open_array
            << "v3.2" << "v3.0" << "v2.6"
        << bsoncxx::builder::stream::close_array
        << "info" << bsoncxx::builder::stream::open_document
            << "x" << 203
            << "y" << 102
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    bsoncxx::document::view view = doc_value.view();

    // Access field
    bsoncxx::document::element element = view["name"];
    std::string name = element.get_utf8().value.to_string();
    TRACE("view[name] = ", name);

    auto db = getDatabase("mydb");
    auto collection = db["test"];
    
    // Insert one document
    bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
        collection.insert_one(view);

    auto cursor = collection.find({});
    for (auto&& doc : cursor) 
    {
        TRACE(bsoncxx::to_json(doc), "\n");
    }
}

    
    
    // Access collection
    //auto collection = db["test"];
    
    
