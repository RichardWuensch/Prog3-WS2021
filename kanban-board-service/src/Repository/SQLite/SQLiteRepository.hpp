#pragma once

#include "Repository/RepositoryIf.hpp"
#include "sqlite3.h"

namespace Reminder {
namespace Repository {
namespace SQLite {

class SQLiteRepository : public RepositoryIf {
  private:
    sqlite3 *database;

    void initialize();
    void createDummyData();
    void handleSQLError(int statementResult, char *errorMessage);

    static bool isValid(int id) {
        return id != INVALID_ID;
    }

    static Reminder::Core::Model::Item getItemFromCallback(char **fieldValues, int startIndex);
    static Reminder::Core::Model::List getListFromCallback(char **fieldValues, int startIndex);

    static int queryListsCallback(void *data, int numberOfLists, char **fieldValues, char **listNames);
    static int queryItemsCallback(void *data, int numberOfLists, char **fieldValues, char **listNames);

  public:
    SQLiteRepository();
    virtual ~SQLiteRepository();

    virtual Reminder::Core::Model::ToDo getToDo();
    virtual std::vector<Reminder::Core::Model::List> getLists();
    virtual std::optional<Reminder::Core::Model::List> getList(int id);
    virtual std::optional<Reminder::Core::Model::List> postList(std::string name, int position);
    virtual std::optional<Reminder::Core::Model::List> putList(int id, std::string name, int position);
    virtual void deleteList(int id);
    virtual std::vector<Reminder::Core::Model::Item> getItems(int listId);
    virtual std::optional<Reminder::Core::Model::Item> getItem(int listId, int itemId);
    virtual std::optional<Reminder::Core::Model::Item> postItem(int listId, std::string title, int position, std::string datum);
    virtual std::optional<Reminder::Core::Model::Item> putItem(int listId, int itemId, std::string title, int position, int flag, int done, std::string datum);
    virtual void deleteItem(int listId, int itemId);

    static inline std::string const toDoTitle = "Reminder App";
    static inline int const INVALID_ID = -1;

    static std::string const databaseFile;
};

} // namespace SQLite
} // namespace Repository
} // namespace Reminder
