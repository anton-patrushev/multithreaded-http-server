#ifndef DB_WORKER_HPP
#define DB_WORKER_HPP

#include <windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <memory>

#include "../../helpers/sqlite3.h"
#include "../../helpers/json.hpp"
#include "../../helpers/constants.hpp"

using json = nlohmann::json;

class DBWorker
{
  std::string _name;
  sqlite3 *_db;

  // helpers
  int opendb();
  int closedb();
  int init();

  // workers
  json createUser(json content);
  json getUser(json content);
  json deleteUser(json content);
  json updateUser(json content);

  json createTask(json content);
  json getTasks(json content);
  json deleteTask(json content);
  json updateTask(json content);

  DBWorker();

public:
  DBWorker(DBWorker const &) = delete;
  DBWorker &operator=(DBWorker const &) = delete;

  static std::shared_ptr<DBWorker> instance()
  {
    static std::shared_ptr<DBWorker> s(new DBWorker);
    return s;
  }

  json performOperation(int operationType, json content);
  static int sqlCallback(void*, int columns, char** fields, char** columnNames);

  ~DBWorker();
};

#endif //DB_WORKER_HPP