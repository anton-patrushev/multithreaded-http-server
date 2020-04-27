#pragma once

#include <signal.h>
#include <iostream>

#include "../classes/Handler/routes.hpp"

int cleanUp()
{
  delete routes::deleteTasks;
  delete routes::updateTask;
  delete routes::createTask;
  delete routes::getTasks;
  delete routes::login;
  delete routes::signUp;
  delete routes::users;
  delete routes::api;
}

void terminateHandler(int s)
{
  std::cout << "Terminating ..." << std::endl;
  if (cleanUp() != 0)
  {
    std::cout << "CleanUp failed" << std::endl;
    exit(-2);
  };
  exit(0);
}

int setTerminateHandler()
{
  signal(SIGINT, terminateHandler);
  return 0;
}