#include "Endpoint.hpp"

#include <iostream>

using namespace Reminder::Api;
using namespace Reminder::Core;
using namespace crow;
using namespace std;

Endpoint::Endpoint(SimpleApp &givenApp, Manager &givenManager) : app(givenApp),
                                                                 manager(givenManager) {
    registerRoutes();
}

Endpoint::~Endpoint() {
}

void Endpoint::registerRoutes() {
    CROW_ROUTE(app, "/api/todo")
    ([this](const request &req, response &res) {
        std::string jsonToDos = manager.getToDo();
        res.write(jsonToDos);
        res.end();
    });

    CROW_ROUTE(app, "/api/todo/lists")
        .methods("GET"_method, "POST"_method)([this](const request &req, response &res) {
            std::string jsonLists;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonLists = manager.getLists();
                break;
            }
            case HTTPMethod::Post: {
                jsonLists = manager.postList(req.body);
                res.code = 201;
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonLists);
            res.end();
        });

    CROW_ROUTE(app, "/api/todo/lists/<int>")
        .methods("GET"_method, "PUT"_method, "DELETE"_method)([this](const request &req, response &res, int listID) {
            std::string jsonList = "{}";

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonList = manager.getList(listID);
                break;
            }
            case HTTPMethod::Put: {
                jsonList = manager.putList(listID, req.body);
                break;
            }
            case HTTPMethod::Delete: {
                manager.deleteList(listID);
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonList);
            res.end();
        });

    CROW_ROUTE(app, "/api/todo/lists/<int>/items")
        .methods("GET"_method, "POST"_method)([this](const request &req, response &res, int listID) {
            std::string jsonItem;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonItem = manager.getItems(listID);
                break;
            }
            case HTTPMethod::Post: {
                jsonItem = manager.postItem(listID, req.body);
                res.code = 201;
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonItem);
            res.end();
        });

    CROW_ROUTE(app, "/api/todo/lists/<int>/items/<int>")
        .methods("GET"_method, "PUT"_method, "DELETE"_method)([this](const request &req, response &res, int listID, int itemID) {
            std::string jsonItem;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonItem = manager.getItem(listID, itemID);
                break;
            }
            case HTTPMethod::Put: {
                jsonItem = manager.putItem(listID, itemID, req.body);
                break;
            }
            case HTTPMethod::Delete: {
                manager.deleteItem(listID, itemID);
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonItem);
            res.end();
        });
}
