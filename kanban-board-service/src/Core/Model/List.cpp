#include "List.hpp"

using namespace Reminder::Core::Model;

List::List(int id, std::string givenName, int givenPosition)
    : id(id), name(givenName), position(givenPosition) {}

int List::getId() const {
    return id;
}

std::string List::getName() const {
    return name;
}

int List::getPos() const {
    return position;
}

std::vector<Item> List::getItems() const {
    return items;
}

void List::setID(int givenId) {
    id = givenId;
}

void List::setName(std::string givenName) {
    name = givenName;
}

void List::setPos(int givenPos) {
    position = givenPos;
}

void List::addItem(Item &givenItem) {
    items.push_back(givenItem);
}
