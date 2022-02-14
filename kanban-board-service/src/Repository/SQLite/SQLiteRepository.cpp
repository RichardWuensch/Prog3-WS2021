#include "SQLiteRepository.hpp"
#include <filesystem>
#include <string.h>

using namespace Reminder::Repository::SQLite;
using namespace Reminder::Core::Model;
using namespace std;

#ifdef RELEASE_SERVICE
string const SQLiteRepository::databaseFile = "./data/reminder-app.db";
#else
string const SQLiteRepository::databaseFile = "../data/reminder-app.db";
#endif

SQLiteRepository::SQLiteRepository() : database(nullptr) {

    string databaseDirectory = filesystem::path(databaseFile).parent_path().string();

    if (filesystem::is_directory(databaseDirectory) == false) {
        filesystem::create_directory(databaseDirectory);
    }

    int result = sqlite3_open(databaseFile.c_str(), &database);

    if (SQLITE_OK != result) {
        cout << "Cannot open database: " << sqlite3_errmsg(database) << endl;
    }

    initialize();
}

SQLiteRepository::~SQLiteRepository() {
    sqlite3_close(database);
}

void SQLiteRepository::initialize() {
    int result = 0;
    char *errorMessage = nullptr;

    string sqlCreateTableList =
        "create table if not exists list("
        "id integer not null primary key autoincrement,"
        "name text not null,"
        "position integer not null UNIQUE);";

    string sqlCreateTableItem =
        "create table if not exists item("
        "id integer not null primary key autoincrement,"
        "title text not null,"
        "date text not null,"
        "position integer not null,"
        "flag integer not null,"
        "done integer not null,"
        "datum text not null,"
        "list_id integer not null,"
        "unique (position, list_id),"
        "foreign key (list_id) references list (id));";

    result = sqlite3_exec(database, sqlCreateTableList.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
    result = sqlite3_exec(database, sqlCreateTableItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

ToDo SQLiteRepository::getToDo() {

    ToDo toDo = ToDo(toDoTitle);

    vector<List> lists = getLists();
    toDo.setLists(lists);

    return toDo;
}

std::vector<List> SQLiteRepository::getLists() {

    std::vector<List> lists;

    string sqlQueryLists =
        "SELECT list.id, list.name, list.position, item.id, item.title, item.position, item.date, item.flag, item.done, item.datum from list "
        "left join item on item.list_id = list.id "
        "order by list.position, item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryLists.c_str(), queryListsCallback, &lists, &errorMessage);
    handleSQLError(result, errorMessage);

    return lists;
}

std::optional<List> SQLiteRepository::getList(int id) {

    std::vector<List> lists;

    string sqlQueryLists =
        "SELECT list.id, list.name, list.position, item.id, item.title, item.position, item.date, item.flag, item.done, item.datum from list "
        "left join item on item.list_id = list.id where list.id = " +
        to_string(id) +
        " order by list.position, item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryLists.c_str(), queryListsCallback, &lists, &errorMessage);
    handleSQLError(result, errorMessage);

    if (lists.size() == 1) {
        return lists.front();
    } else {
        return std::nullopt;
    }
}

std::optional<List> SQLiteRepository::postList(std::string name, int position) {

    string sqlPostItem =
        "INSERT INTO list('name', 'position') "
        "VALUES('" +
        name + "', '" + to_string(position) + "')";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        int listId = sqlite3_last_insert_rowid(database);
        return List(listId, name, position);
    }

    return std::nullopt;
}

std::optional<Reminder::Core::Model::List> SQLiteRepository::putList(int id, std::string name, int position) {

    string sqlUpdateList =
        "UPDATE list "
        "SET name = '" +
        name + "', position = '" + to_string(position) +
        "' WHERE id = " + to_string(id);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlUpdateList.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    return getList(id);
}

void SQLiteRepository::deleteList(int id) {

    string sqlDeleteListItems =
        "DELETE FROM item "
        "WHERE item.list_id = " +
        to_string(id);

    string sqlDeleteList =
        "DELETE FROM list "
        "WHERE list.id = " +
        to_string(id);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlDeleteListItems.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    result = sqlite3_exec(database, sqlDeleteList.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

std::vector<Item> SQLiteRepository::getItems(int listId) {

    std::vector<Item> items;

    string sqlQueryItems =
        "SELECT item.id, item.title, item.position, item.date, item.flag, item.done, item.datum from item "
        "where item.list_id = " +
        std::to_string(listId) +
        " order by item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItems.c_str(), queryItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    return items;
}

std::optional<Item> SQLiteRepository::getItem(int listId, int itemId) {

    std::vector<Item> items;

    string sqlQueryItems =
        "SELECT item.id, item.title, item.position, item.date, item.flag, item.done, item.datum from item "
        "where item.list_id = " +
        std::to_string(listId) + " and item.id = " + std::to_string(itemId) +
        " order by item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItems.c_str(), queryItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    if (items.size() == 1) {
        return items.front();
    } else {
        return std::nullopt;
    }
}

std::optional<Item> SQLiteRepository::postItem(int listId, std::string title, int position, std::string datum) {

    string sqlPostItem =
        "INSERT INTO item ('title', 'date', 'position', 'flag', 'done', 'datum', 'list_id')"
        "VALUES ('" +
        title + "', datetime(), " + to_string(position) + ", " + "0, 0, '" + datum + "', " + to_string(listId) + ");";
    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    int itemId = INVALID_ID;
    if (SQLITE_OK == result) {
        itemId = sqlite3_last_insert_rowid(database);
    }

    return getItem(listId, itemId);
}

std::optional<Reminder::Core::Model::Item> SQLiteRepository::putItem(int listId, int itemId, std::string title, int position, int flag, int done, std::string datum) {

    string sqlUpdateItem =
        "UPDATE item SET title = '" + title + "', position = " + to_string(position) + ", flag = " + to_string(flag) + ", done = " + to_string(done) + ", datum = '" + datum + "'"
                                                                                                                                                                               " WHERE item.list_id = " +
        to_string(listId) + " AND item.id = " + to_string(itemId);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlUpdateItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    return getItem(listId, itemId);
}

void SQLiteRepository::deleteItem(int listId, int itemId) {
    string sqlQueryItem =
        "DELETE FROM item "
        "WHERE item.list_id = " +
        to_string(listId) +
        " AND item.id = " + to_string(itemId);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

Item SQLiteRepository::getItemFromCallback(char **fieldValues, int startIndex) {
    int index = startIndex;

    int itemId = fieldValues[index] ? atoi(fieldValues[index]) : INVALID_ID;
    index++;

    string title = fieldValues[index] ? fieldValues[index] : "";
    index++;

    int position = fieldValues[index] ? atoi(fieldValues[index]) : 0;
    index++;

    string timestamp = fieldValues[index] ? fieldValues[index] : "";
    index++;

    int flag = fieldValues[index] ? atoi(fieldValues[index]) : 0;
    index++;

    int done = fieldValues[index] ? atoi(fieldValues[index]) : 0;
    index++;

    std::string datum = fieldValues[index] ? fieldValues[index] : "";

    Item item(itemId, title, position, flag, done, datum, timestamp);
    return item;
}

List SQLiteRepository::getListFromCallback(char **fieldValues, int startIndex) {
    int index = startIndex;

    int listId = fieldValues[index] ? atoi(fieldValues[index]) : INVALID_ID;
    index++;

    string name = fieldValues[index] ? fieldValues[index] : "";
    index++;

    int position = fieldValues[index] ? atoi(fieldValues[index]) : 0;
    index++;

    List list(listId, name, position);
    return list;
}

int SQLiteRepository::queryListsCallback(void *data, int numberOfLists, char **fieldValues, char **listNames) {

    std::vector<List> *pLists = static_cast<std::vector<List> *>(data);
    std::vector<List> &lists = *pLists;

    Item item = getItemFromCallback(fieldValues, 3);

    int listId = fieldValues[0] ? atoi(fieldValues[0]) : INVALID_ID;
    bool wasListAlreadyAdded = false;

    for (List &list : lists) {
        if (list.getId() == listId) {
            wasListAlreadyAdded = true;
            if (isValid(item.getId())) {
                list.addItem(item);
            }
            break;
        }
    }

    if (false == wasListAlreadyAdded) {
        List list = getListFromCallback(fieldValues, 0);

        if (isValid(item.getId())) {
            list.addItem(item);
        }
        if (isValid(list.getId())) {
            lists.push_back(list);
        }
    }
    return 0;
}

int SQLiteRepository::queryItemsCallback(void *data, int numberOfLists, char **fieldValues, char **listNames) {

    std::vector<Item> *result = static_cast<std::vector<Item> *>(data);

    Item item = getItemFromCallback(fieldValues, 0);
    if (isValid(item.getId())) {
        result->push_back(item);
    }
    return 0;
}

void SQLiteRepository::handleSQLError(int statementResult, char *errorMessage) {

    if (statementResult != SQLITE_OK) {
        cout << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}
