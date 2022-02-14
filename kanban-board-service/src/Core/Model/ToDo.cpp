#include "ToDo.hpp"

using namespace Reminder::Core::Model;

ToDo::ToDo(std::string givenTitle) : title(givenTitle) {}

std::string ToDo::getTitle() const {
    return title;
}

std::vector<List> &ToDo::getLists() {
    return lists;
}

void ToDo::setLists(std::vector<List> const &lists) {
    this->lists = lists;
}
