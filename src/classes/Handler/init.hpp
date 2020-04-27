#pragma once

#include <windows.h>
#include <iostream>

#include "./Handler.hpp"
#include "../../helpers/json.hpp" //C++ json
#include "./routes.hpp"           //routes declaration

using json = nlohmann::json;

/**
 * @todo Resolve `routes` lifecycle problem
*/

int createUsersRoute()
{
  // POST api/users/login/
  std::function<int(json)> loginHandler = [](json j) {
    std::cout << "login in handler" << std::endl;
    return 0;
  };

  routes::login = new Handler("login", {}, constants::POST_HTTP, loginHandler);

  // POST api/users/register/
  std::function<int(json)> registerHandler = [](json j) {
    std::cout << "register in handler" << std::endl;
    return 0;
  };

  routes::signUp = new Handler("register", {}, constants::POST_HTTP, registerHandler);

  // combined register and login routes on api/users/
  Handler users("users", {routes::login, routes::signUp});

  return 0;
}

int createTasksRoute()
{
  // GET api/tasks/
  std::function<int(json)> getTasksHandler = [](json j) {
    std::cout << "get tasks in handler" << std::endl;
    return 0;
  };

  routes::getTasks = new Handler("tasks", {}, constants::GET_HTTP, getTasksHandler);

  // POST api/tasks/
  std::function<int(json)> createTaskHandler = [](json j) {
    std::cout << "create task in handler" << std::endl;
    return 0;
  };

  routes::createTask = new Handler("tasks", {}, constants::GET_HTTP, createTaskHandler);

  // PATCH api/tasks/
  std::function<int(json)> updateTaskHandler = [](json j) {
    std::cout << "update task in handler" << std::endl;
    return 0;
  };

  routes::updateTask = new Handler("tasks", {}, constants::PATCH_HTTP, updateTaskHandler);

  // DELETE api/tasks/
  std::function<int(json)> deleteTasksHandler = [](json j) {
    std::cout << "delete tasks in handler" << std::endl;
    return 0;
  };

  routes::deleteTasks = new Handler("tasks", {}, constants::DELETE_HTTP, deleteTasksHandler);

  return 0;
}

int createApiRoute()
{
  // group all `tasks` and `users` routes in one route named `api`
  routes::api = new Handler("api", {routes::users, routes::getTasks, routes::createTask, routes::updateTask, routes::deleteTasks});
  return 0;
}

// call on initApp
int initRoutes()
{
  if (createUsersRoute() != 0)
  {
    std::cout << "Failed to create `/api/users/` route" << std::endl;
    return 1;
  }

  if (createTasksRoute() != 0)
  {
    std::cout << "Failed to create `/api/tasks/` route" << std::endl;
    return 1;
  }

  if (createApiRoute() != 0)
  {
    std::cout << "Failed to create `/api/` route" << std::endl;
    return 1;
  }

  routes::main = new Handler("", {routes::api});

  return 0;
}
