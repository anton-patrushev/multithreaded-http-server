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
          "USER_ID   INTEGER             NOT NULL,"
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
  if (operationType == constants::db::GET_USER)
    return this->getUser(content);

  if (operationType == constants::db::CREATE_USER)
    return this->createUser(content);

  if (operationType == constants::db::UPDATE_USER)
    return this->updateUser(content);

  if (operationType == constants::db::DELETE_USER)
    return this->deleteUser(content);

  if (operationType == constants::db::GET_TASKS)
    return this->getTasks(content);

  if (operationType == constants::db::CREATE_TASK)
    return this->createTask(content);

  if (operationType == constants::db::UPDATE_TASK)
    return this->updateTask(content);

  if (operationType == constants::db::DELETE_TASK)
    return this->deleteTask(content);

  return nullptr;
};

json DBWorker::createUser(json content)
{
  std::string email = content["email"];
  std::string password = content["password"];

  std::hash<std::string> hash;
  std::string result = std::to_string(hash(password + constants::db::salt));

  std::string query = "INSERT INTO USERS (EMAIL, PASSWORD)"
                      "VALUES('" +
                      email + "', '" + result + "');";

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
  std::cout << content << std::endl;
  std::string email = content["email"];
  std::string query = "SELECT * FROM USERS WHERE EMAIL = '" + email + "';";

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

json DBWorker::deleteUser(json content) { return nullptr; };
json DBWorker::updateUser(json content) { return nullptr; };

json DBWorker::createTask(json content) { return nullptr; };
json DBWorker::getTasks(json content) { return nullptr; };
json DBWorker::deleteTask(json content) { return nullptr; };
json DBWorker::updateTask(json content) { return nullptr; };

int DBWorker::sqlCallback(void *data, int columns, char **fields, char **columnNames)
{
  json *JSON = (json *)data;
  json localResult;

  for (int i = 0; i < columns; i++)
  {
    const char *field = fields[i] ? fields[i] : "null";
    std::string name(columnNames[i]);

    // property to lower case
    DBWorker::toLowerCase(name);
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

void DBWorker::toLowerCase(std::string &src)
{
  std::transform(src.begin(), src.end(), src.begin(), [](unsigned char c) { return std::tolower(c); });
}

// std::string query = "SELECT * FROM USERS";
// char *messageError = NULL;
// json res;
// res["data"] = {};
// int status = sqlite3_exec(this->_db, query.c_str(), (this->sqlCallback), &res, &messageError);

// std::cout << "json res -> " << res << std::endl;
// if (status != SQLITE_OK)
// {
//   std::string error = messageError;
//   sqlite3_free(messageError);
//   return {{"status", "error"}, {"message", error}};
// }
