#pragma once

#include "./cleanup.hpp"
#include "../classes/Handler/init.hpp"

int initApp()
{
  if (initRoutes() != 0)
  {
    std::cout << "Failed to init routes" << std::endl;
    return 1;
  }

  if (setTerminateHandler() != 0)
  {
    std::cout << "Failed to set exit function" << std::endl;
    return 1;
  }

  return 0;
}