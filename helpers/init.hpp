#ifndef INIT_HPP
#define INIT_HPP

#include <signal.h>
#include <iostream>
#include "../classes/db/DBWorker.hpp"

void termitateHandler(int)
{
  std::cout << "Terminating ..." << std::endl;
  // DBWorker::~DBWorker();
  exit(0);
}

void initApp()
{
  // init db
  DBWorker::instance();

  // handle ctrl + c
  signal(SIGINT, &termitateHandler);
}

#endif //INIT_HPP