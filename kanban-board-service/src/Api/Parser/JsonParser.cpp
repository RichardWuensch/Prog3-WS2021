#define RAPIDJSON_ASSERT(x)

#include "JsonParser.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <ctime>

using namespace Reminder::Api::Parser;
using namespace Reminder::Core::Model;
using namespace rapidjson;
using namespace std;

string JsonParser::convertToApiString(ToDo &toDo) {
    Document document;
    Document::AllocatorType &allocator = document.GetAllocator();

    Value jsonToDo = getJsonValueFromModel(toDo, allocator);
    return jsonValueToString(jsonToDo);
}

string JsonParser::convertToApiString(List &list) {
    Document document(kObjectType);

    Value jsonList = getJsonValueFromModel(list, document.GetAllocator());
    return jsonValueToString(jsonList);
}

string JsonParser::convertToApiString(std::vector<List> &lists) {
    Document document(kArrayType);
    Document::AllocatorType &allocator = document.GetAllocator();

    for (List &list : lists) {
        Value jsonList = getJsonValueFromModel(list, allocator);
        document.PushBack(jsonList, allocator);
    }

    return jsonValueToString(document);
}

string JsonParser::convertToApiString(Item &item) {

    string result = EMPTY_JSON;
    Document document(kObjectType);

    Value jsonItem = getJsonValueFromModel(item, document.GetAllocator());
    result = jsonValueToString(jsonItem);
    return result;
}

string JsonParser::convertToApiString(std::vector<Item> &items) {
    Document document(kArrayType);
    Document::AllocatorType &allocator = document.GetAllocator();

    for (Item &item : items) {
        Value jsonItem = getJsonValueFromModel(item, allocator);
        document.PushBack(jsonItem, allocator);
    }

    return jsonValueToString(document);
}

std::optional<List> JsonParser::convertListToModel(int listId, std::string &request) {

    std::optional<List> resultList;
    Document document;
    document.Parse(request.c_str());

    if (true == isValidList(document)) {
        std::string name = document["name"].GetString();
        int position = document["position"].GetInt();
        resultList = List(listId, name, position);
    }
    return resultList;
}

std::optional<Item> JsonParser::convertItemToModel(int itemId, std::string &request) {
    std::optional<Item> resultItem;

    Document document;
    document.Parse(request.c_str());
    // std::cout << request.c_str() << std::endl;

    if (true == isValidItem(document)) {
        std::string title = document["title"].GetString();
        int position = document["position"].GetInt();
        int flag = document["flag"].GetBool() ? 1 : 0;
        int done = document["done"].GetBool() ? 1 : 0;
        std::string datum = document["datum"].GetString();
        resultItem = Item(itemId, title, position, flag, done, datum, "");
    }
    return resultItem;
}

string JsonParser::jsonValueToString(rapidjson::Value const &json) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    json.Accept(writer);

    return buffer.GetString();
}

rapidjson::Value JsonParser::getJsonValueFromModel(Item const &item, rapidjson::Document::AllocatorType &allocator) {
    Value jsonItem(kObjectType);

    jsonItem.AddMember("id", item.getId(), allocator);
    jsonItem.AddMember("title", Value(item.getTitle().c_str(), allocator), allocator);
    jsonItem.AddMember("position", item.getPos(), allocator);
    jsonItem.AddMember("flag", item.getFlag(), allocator);
    jsonItem.AddMember("done", item.getDone(), allocator);
    jsonItem.AddMember("datum", Value(item.getDatum().c_str(), allocator), allocator);
    jsonItem.AddMember("timestamp", Value(item.getTimestamp().c_str(), allocator), allocator);
    return jsonItem;
}

rapidjson::Value JsonParser::getJsonValueFromModel(List const &list, rapidjson::Document::AllocatorType &allocator) {
    Value jsonList(kObjectType);

    jsonList.AddMember("id", list.getId(), allocator);
    jsonList.AddMember("name", Value(list.getName().c_str(), allocator), allocator);
    jsonList.AddMember("position", list.getPos(), allocator);

    Value jsonItems(kArrayType);

    for (Item const &item : list.getItems()) {
        Value jsonItem = getJsonValueFromModel(item, allocator);
        jsonItems.PushBack(jsonItem, allocator);
    }

    jsonList.AddMember("items", jsonItems, allocator);

    return jsonList;
}

rapidjson::Value JsonParser::getJsonValueFromModel(ToDo &toDo, rapidjson::Document::AllocatorType &allocator) {
    Value jsonToDo(kObjectType);
    Value jsonLists(kArrayType);

    for (List &list : toDo.getLists()) {
        Value jsonList = getJsonValueFromModel(list, allocator);
        jsonLists.PushBack(jsonList, allocator);
    }

    jsonToDo.AddMember("title", Value(toDo.getTitle().c_str(), allocator), allocator);
    jsonToDo.AddMember("lists", jsonLists, allocator);

    return jsonToDo;
}

bool JsonParser::isValidList(rapidjson::Document const &document) {

    bool isValid = true;

    if (document.HasParseError()) {
        isValid = false;
    }
    if (false == document["name"].IsString()) {
        isValid = false;
    }
    if (false == document["position"].IsInt()) {
        isValid = false;
    }

    return isValid;
}

bool JsonParser::isValidItem(rapidjson::Document const &document) {

    bool isValid = true;
    if (document.HasParseError()) {
        isValid = false;
    }
    if (false == document["title"].IsString()) {
        isValid = false;
    }
    if (false == document["position"].IsInt()) {
        isValid = false;
    }
    if (false == document["datum"].IsString()) {
        isValid = false;
    }
    if (false == isValidDate(document["datum"].GetString())) {
        isValid = false;
    }

    return isValid;
}

bool JsonParser::isValidDate(const std::string &s) {
    int y = stoi(s.substr(0, 4));
    int m = stoi(s.substr(5, 7));
    int d = stoi(s.substr(8, 9));
    if (s.length() == 10) {
        struct tm t = {0};
        t.tm_mday = d;
        t.tm_mon = m - 1;
        t.tm_year = y - 1900;
        t.tm_isdst = -1;

        int dayOfMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)) {
            int dayOfMonth[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (dayOfMonth[m - 1] < d)
                return false;
        } else {
            int dayOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if (dayOfMonth[m - 1] < d)
                return false;
        }
        time_t Zeitstempel;
        tm *nun;
        Zeitstempel = time(0);
        nun = localtime(&Zeitstempel);

        if (t.tm_year > nun->tm_year)
            return true;
        else if (t.tm_year == nun->tm_year && t.tm_mon > nun->tm_mon)
            return true;
        else if (t.tm_mon == nun->tm_mon && t.tm_mday >= nun->tm_mday)
            return true;
        else
            return false;
    }
    return false;
}
