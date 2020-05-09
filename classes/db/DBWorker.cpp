#include "./DBWorker.hpp"

const bool firstTime = true;

DBWorker::DBWorker()
{
  std::filesystem::path modulePath = std::filesystem::current_path();

  if (modulePath.filename().string().compare("Debug") || modulePath.filename().string().compare("Release"))
    modulePath = modulePath.parent_path();

  this->_name = modulePath.string() + "/src/db/main.sql";
  std::replace(this->_name.begin(), this->_name.end(), '\\', '/');
  // std::cout << "db path -> " << this->_name << std::endl;

  int openStatus = this->opendb();
  if (openStatus != SQLITE_OK)
  {
    // handle error (should throw error)
    std::cout << "Open db failed" << std::endl;
  }

  if (firstTime)
  {
    int initStatus = this->init();
    if (initStatus != SQLITE_OK)
    {
      // handle error
      std::cout << "Init db tables failed" << std::endl;
    }
  }
}
DBWorker::~DBWorker()
{
  std::cout << "close db" << std::endl;
};

int DBWorker::opendb()
{
  return sqlite3_open(this->_name.c_str(), &this->_db);
}

int DBWorker::init()
{
  std::string script = "CREATE TABLE USERS("
                       "ID        INTEGER PRIMARY KEY NOT NULL,"
                       "EMAIL     TEXT                NOT NULL,"
                       "PASSWORD  TEXT                NOT NULL)";

  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, script.c_str(), NULL, 0, &messageError);

  if (status != SQLITE_OK)
  {
    std::cout << "create USERS table failed" << std::endl;
    std::cout << "Error -> " << messageError << std::endl;
    sqlite3_free(messageError);
    return status;
  }

  script = "CREATE TABLE TASKS("
           "ID        INTEGER PRIMARY KEY NOT NULL,"
           "USER_ID   INTEGER             NOT NULL,"
           "TASK      TEXT                NOT NULL)";

  status = sqlite3_exec(this->_db, script.c_str(), NULL, 0, &messageError);

  if (status != SQLITE_OK)
  {
    std::cout << "create TASKS table failed" << std::endl;
    std::cout << "Error -> " << messageError << std::endl;
    sqlite3_free(messageError);
    return status;
  }
  return 0;
}

int DBWorker::closedb()
{
  sqlite3_open(this->_name.c_str(), &this->_db);
  return 0;
}