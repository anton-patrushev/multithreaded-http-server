#ifndef DB_WORKER_HPP
#define DB_WORKER_HPP

#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <memory>

#include "../../helpers/sqlite3.h"

class DBWorker
{
  std::string _name;
  sqlite3 *_db;

  // helpers
  int opendb();
  int closedb();
  int init();

  DBWorker();

public:
  DBWorker(DBWorker const &) = delete;
  DBWorker &operator=(DBWorker const &) = delete;

  static std::shared_ptr<DBWorker> instance()
  {
    static std::shared_ptr<DBWorker> s(new DBWorker);
    return s;
  }

  ~DBWorker();
};

#endif //DB_WORKER_HPP