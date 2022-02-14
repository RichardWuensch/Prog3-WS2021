#pragma once

#include "Api/Parser/ParserIf.hpp"
#include "Repository/RepositoryIf.hpp"

namespace Reminder {
namespace Core {
class Manager {
  private:
    Reminder::Repository::RepositoryIf &repository;
    Reminder::Api::Parser::ParserIf &parser;

  public:
    Manager(Reminder::Api::Parser::ParserIf &givenParser, Reminder::Repository::RepositoryIf &givenRepository);
    ~Manager();

    std::string getToDo();
    std::string getLists();
    std::string getList(int listId);
    std::string postList(std::string request);
    std::string putList(int listId, std::string request);
    void deleteList(int listId);

    std::string getItems(int listId);
    std::string getItem(int listId, int itemId);
    std::string postItem(int listId, std::string request);
    std::string putItem(int listId, int itemId, std::string request);
    void deleteItem(int listId, int itemId);
};

} // namespace Core
} // namespace Reminder
