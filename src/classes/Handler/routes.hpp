#pragma once

#include "./Handler.hpp"

namespace routes
{
// users routes
Handler *login;
Handler *signUp;
Handler *users;

// tasks routes
Handler *getTasks;
Handler *createTask;
Handler *updateTask;
Handler *deleteTasks;

// api route
Handler *api;

Handler *main;
} // namespace routes

Handler *findHandler(std::vector<std::string> path)
{
  Handler *handler = routes::main;
  for (std::string item : path)
  {
    // for(std::string routeName:handler->getChilds)
  }
  return handler;
}

int handleRequest(std::vector<std::string> path, json req)
{
  Handler *currentHandler = findHandler(path);
  return 0; //handle success
}