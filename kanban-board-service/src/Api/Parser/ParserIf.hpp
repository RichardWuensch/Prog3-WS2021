#pragma once

#include "Core/Model/ToDo.hpp"
#include "optional"

namespace Reminder {
namespace Api {
namespace Parser {

class ParserIf {
  public:
    virtual ~ParserIf() {}

    virtual std::string getEmptyResponseString() = 0;

    virtual std::string convertToApiString(Reminder::Core::Model::ToDo &toDo) = 0;
    virtual std::string convertToApiString(Reminder::Core::Model::List &list) = 0;
    virtual std::string convertToApiString(std::vector<Reminder::Core::Model::List> &lists) = 0;

    virtual std::string convertToApiString(Reminder::Core::Model::Item &item) = 0;
    virtual std::string convertToApiString(std::vector<Reminder::Core::Model::Item> &items) = 0;

    virtual std::optional<Reminder::Core::Model::List> convertListToModel(int listId, std::string &request) = 0;
    virtual std::optional<Reminder::Core::Model::Item> convertItemToModel(int itemId, std::string &request) = 0;
};

} // namespace Parser
} // namespace Api
} // namespace Reminder
