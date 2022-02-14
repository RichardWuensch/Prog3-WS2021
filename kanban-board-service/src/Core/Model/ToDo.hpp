#pragma once

#include "List.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Reminder {
namespace Core {
namespace Model {

class ToDo {
  public:
    ToDo(std::string givenTitle);
    ~ToDo() {}

    std::string getTitle() const;

    std::vector<List> &getLists();
    void setLists(std::vector<List> const &lists);

  private:
    std::string title;
    std::vector<List> lists;
};

} // namespace Model
} // namespace Core
} // namespace Reminder
