#include "./DBWorker.hpp"

const bool firstTime = false;

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
  std::string query = "CREATE TABLE USERS("
                      "ID        INTEGER PRIMARY KEY NOT NULL,"
                      "EMAIL     TEXT                NOT NULL,"
                      "PASSWORD  TEXT                NOT NULL)";

  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), NULL, 0, &messageError);

  if (status != SQLITE_OK)
  {
    std::cout << "create USERS table failed" << std::endl;
    std::cout << "Error -> " << messageError << std::endl;
    sqlite3_free(messageError);
    return status;
  }

  query = "CREATE TABLE TASKS("
          "ID        INTEGER PRIMARY KEY NOT NULL,"
          "USER      INTEGER             NOT NULL,"
          "TASK      TEXT                NOT NULL)";

  status = sqlite3_exec(this->_db, query.c_str(), NULL, 0, &messageError);

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

json DBWorker::performOperation(int operationType, json content)
{
  this->enterDbSection();

  json result;
  switch (operationType)
  {
  case constants::db::GET_USER:
    result = this->getUser(content);
    break;

  case constants::db::CREATE_USER:
    result = this->createUser(content);
    break;

  case constants::db::UPDATE_USER:
    result = this->updateUser(content);
    break;

  case constants::db::DELETE_USER:
    result = this->deleteUser(content);
    break;

  case constants::db::GET_TASKS:
    result = this->getTasks(content);
    break;

  case constants::db::GET_TASK_BY_ID:
    result = this->getTaskById(content);
    break;

  case constants::db::CREATE_TASK:
    result = this->createTask(content);
    break;

  case constants::db::UPDATE_TASK:
    result = this->updateTask(content);
    break;

  case constants::db::DELETE_TASK:
    result = this->deleteTask(content);
    break;

  default:
    break;
  }

  this->leaveDbSection();
  return result;
};

json DBWorker::createUser(json content)
{
  std::string email = content["email"];
  std::string password = content["password"];

  std::string query = "INSERT INTO USERS (EMAIL, PASSWORD)"
                      "VALUES('" +
                      email + "', '" + password + "');";

  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), NULL, 0, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  query = "SELECT * FROM USERS WHERE EMAIL = '" + email + "';";

  messageError = NULL;
  json res;
  status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  // std::cout << "createUser res -> " << res << std::endl;

  res["data"].erase("email");
  res["data"].erase("password");
  return res["data"];
};

json DBWorker::getUser(json content)
{
  std::string query;
  if (keyExists(content, "id"))
  {
    std::string id = content["id"];
    query = "SELECT * FROM USERS WHERE ID = '" + id + "';";
  }
  else if (keyExists(content, "email"))
  {
    std::string email = content["email"];
    query = "SELECT * FROM USERS WHERE EMAIL = '" + email + "';";
  }
  else
    return {{"status", "error"}};

  char *messageError = NULL;
  json res;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  return res;
};

json DBWorker::deleteUser(json content)
{
  if (!keyExists(content, "id"))
    return {{"status", "error"}};

  std::string id = content["id"];
  std::string query = "DELETE FROM USERS WHERE ID = '" + id + "';";

  char *messageError = NULL;
  json res;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  query = "DELETE FROM TASKS WHERE USER = '" + id + "';";
  status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  return res;
};

json DBWorker::updateUser(json content)
{
  std::string id = content["id"];
  std::string newPassword = content["newPassword"];

  std::string query = "UPDATE USERS SET PASSWORD = '" + newPassword + "' WHERE ID = '" + id + "';";

  char *messageError = NULL;
  json res;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  return res;
};

json DBWorker::createTask(json content)
{
  std::string userId = content["userId"];
  std::string task = content["task"];

  std::string query = "INSERT INTO TASKS (USER, TASK)"
                      "VALUES('" +
                      userId + "', '" + task + "');";

  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), NULL, 0, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }
  int lastID = sqlite3_last_insert_rowid(this->_db);

  query = "SELECT * FROM TASKS WHERE ID = '" + std::to_string(lastID) + "';";

  json res;
  messageError = NULL;
  status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  // std::cout << "createUser res -> " << res << std::endl;

  return res;
};

json DBWorker::getTasks(json content)
{
  std::string userId = content["userId"];
  std::string query = "SELECT * FROM TASKS WHERE USER = '" + userId + "';";

  json res;
  res["data"] = json::array();
  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  // std::cout << "createUser res -> " << res << std::endl;

  return res;
};

json DBWorker::getTaskById(json content)
{
  std::string id = content["id"];
  std::string query;
  if (keyExists(content, "userId"))
  {
    std::string userId = content["userId"];
    query = "SELECT * FROM TASKS WHERE ID = '" + id + "' AND USER = '" + userId + "';";
  }
  else
  {
    query = "SELECT * FROM TASKS WHERE ID = '" + id + "';";
  }

  json res;
  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  return res;
};

json DBWorker::deleteTask(json content)
{
  std::string id = content["id"];
  std::string query = "DELETE FROM TASKS WHERE ID = '" + id + "';";

  json res;
  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  // std::cout << "createUser res -> " << res << std::endl;

  return res;
};

json DBWorker::updateTask(json content)
{
  std::string id = content["id"];
  std::string task = content["task"];
  std::string query = "UPDATE TASKS SET TASK = '" + task + "' WHERE ID = '" + id + "';";

  json res;
  char *messageError = NULL;
  int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

  if (status != SQLITE_OK)
  {
    std::string error = messageError;
    sqlite3_free(messageError);
    return {{"status", "error"}, {"message", error}};
  }

  // std::cout << "createUser res -> " << res << std::endl;

  return res;
};

int DBWorker::sqlCallback(void *data, int columns, char **fields, char **columnNames)
{
  json *JSON = (json *)data;
  json localResult;

  for (int i = 0; i < columns; i++)
  {
    const char *field = fields[i] ? fields[i] : "null";
    std::string name(columnNames[i]);

    // property to lower case
    toLowerCase(name);
    localResult[name.c_str()] = field;
  }

  if (keyExists(*JSON, "data") && (*JSON)["data"].is_array())
  {
    (*JSON)["data"].push_back(localResult);
  }
  else
  {
    (*JSON)["data"] = localResult;
  }

  return 0;
}

void DBWorker::enterDbSection() { this->_dbMutex.lock(); }
void DBWorker::leaveDbSection() { this->_dbMutex.unlock(); }