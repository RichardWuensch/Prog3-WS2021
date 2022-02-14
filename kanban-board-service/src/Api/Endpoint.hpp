#pragma once

#include "Core/Manager.hpp"
#include "crow.h"

namespace Reminder {
namespace Api {

class Endpoint {
  public:
    Endpoint(crow::SimpleApp &givenApp, Reminder::Core::Manager &givenManager);
    ~Endpoint();

    void registerRoutes();

  private:
    crow::SimpleApp &app;
    Reminder::Core::Manager &manager;
};

} // namespace Api
} // namespace Reminder
