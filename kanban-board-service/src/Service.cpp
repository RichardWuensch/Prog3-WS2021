#include <iostream>
#include <string>

#include "Api/Endpoint.hpp"
#include "Api/Parser/JsonParser.hpp"
#include "Core/Manager.hpp"
#include "Repository/SQLite/SQLiteRepository.hpp"
#include "crow.h"

int main() {
    crow::SimpleApp crowApplication;
    Reminder::Repository::SQLite::SQLiteRepository sqlRepository;
    Reminder::Api::Parser::JsonParser jsonParser;

    Reminder::Core::Manager manager(jsonParser, sqlRepository);
    Reminder::Api::Endpoint endpoint(crowApplication, manager);

    crowApplication.port(8080)
        //        .multithreaded()
        .run();
}
