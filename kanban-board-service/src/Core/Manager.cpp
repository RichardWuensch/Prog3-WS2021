#include "Manager.hpp"
#include <iostream>

using namespace Reminder::Core;
using namespace Reminder::Core::Model;
using namespace Reminder::Api::Parser;
using namespace Reminder::Repository;
using namespace std;

Manager::Manager(ParserIf &givenParser, RepositoryIf &givenRepository)
    : parser(givenParser), repository(givenRepository) {
}

Manager::~Manager() {
}

std::string Manager::getToDo() {
    ToDo toDo = repository.getToDo();

    return parser.convertToApiString(toDo);
}

std::string Manager::getLists() {
    std::vector<List> lists = repository.getLists();

    return parser.convertToApiString(lists);
}

std::string Manager::getList(int listId) {

    std::optional<List> list = repository.getList(listId);
    if (list) {
        return parser.convertToApiString(list.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::postList(std::string request) {

    int const dummyId = -1;
    std::optional<List> parsedListOptional = parser.convertListToModel(dummyId, request);
    if (false == parsedListOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    List parsedList = parsedListOptional.value();

    std::optional<List> postedList = repository.postList(parsedList.getName(), parsedList.getPos());

    if (postedList) {
        return parser.convertToApiString(postedList.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putList(int listId, std::string request) {

    std::optional<List> parsedListOptional = parser.convertListToModel(listId, request);

    if (false == parsedListOptional.has_value()) {
        return parser.getEmptyResponseString();
    }
    List list = parsedListOptional.value();
    std::optional<List> putList = repository.putList(listId, list.getName(), list.getPos());

    if (putList) {
        return parser.convertToApiString(putList.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

void Manager::deleteList(int listId) {
    repository.deleteList(listId);
}

std::string Manager::getItems(int listId) {
    std::vector<Item> items = repository.getItems(listId);

    return parser.convertToApiString(items);
}

std::string Manager::getItem(int listId, int itemId) {

    std::optional<Item> item = repository.getItem(listId, itemId);

    if (item) {
        return parser.convertToApiString(item.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::postItem(int listId, std::string request) {

    int const dummyId = -1;
    std::optional parsedItemOptional = parser.convertItemToModel(dummyId, request);
    if (false == parsedItemOptional.has_value()) {
        std::cout << "parser.getEmptyResponseString" << std::endl;
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> postedItem = repository.postItem(listId, item.getTitle(), item.getPos(), item.getDatum());
    if (postedItem) {
        return parser.convertToApiString(postedItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putItem(int listId, int itemId, std::string request) {

    std::optional parsedItemOptional = parser.convertItemToModel(itemId, request);
    if (false == parsedItemOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> putItem = repository.putItem(listId, itemId, item.getTitle(), item.getPos(), item.getFlag(), item.getDone(), item.getDatum());

    if (putItem) {
        return parser.convertToApiString(putItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

void Manager::deleteItem(int listId, int itemId) {
    repository.deleteItem(listId, itemId);
}
