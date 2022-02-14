#pragma once

#include "Core/Model/ToDo.hpp"
#include "optional"

namespace Reminder {
namespace Repository {
class RepositoryIf {
  public:
    virtual ~RepositoryIf() {}

    virtual Reminder::Core::Model::ToDo getToDo() = 0;
    virtual std::vector<Reminder::Core::Model::List> getLists() = 0;
    virtual std::optional<Reminder::Core::Model::List> getList(int id) = 0;
    virtual std::optional<Reminder::Core::Model::List> postList(std::string name, int position) = 0;
    virtual std::optional<Reminder::Core::Model::List> putList(int id, std::string name, int position) = 0;
    virtual void deleteList(int id) = 0;
    virtual std::vector<Reminder::Core::Model::Item> getItems(int listId) = 0;
    virtual std::optional<Reminder::Core::Model::Item> getItem(int listId, int itemId) = 0;
    virtual std::optional<Reminder::Core::Model::Item> postItem(int listId, std::string title, int position, std::string datum) = 0;
    virtual std::optional<Reminder::Core::Model::Item> putItem(int listId, int itemId, std::string title, int position, int flag, int done, std::string datum) = 0;
    virtual void deleteItem(int listId, int itemId) = 0;
};

} // namespace Repository
} // namespace Reminder
