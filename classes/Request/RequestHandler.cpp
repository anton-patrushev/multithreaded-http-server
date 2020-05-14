#include "./RequestHandler.hpp"

RequestHandler::RequestHandler(std::string rawRequest, SOCKET acceptedSocket, CRITICAL_SECTION *printSectionPointer)
    : _parser(rawRequest, printSectionPointer)
{
  this->_socket = acceptedSocket;
  this->_printingSectionPointer = printSectionPointer;
  this->setKey(); // called after parser initialization
}

void RequestHandler::setKey()
{
  this->_key = this->_parser.getUrl();

  // verify key for certain format
  if (this->_key.find_last_of("/") != this->_key.size() - 1)
    this->_key.append("/");

  this->_key.append("_");
  this->_key.append(std::to_string(this->_parser.getRequestType()));
}

void RequestHandler::enterPrintSection() { EnterCriticalSection(this->_printingSectionPointer); }
void RequestHandler::leavePrintSection() { LeaveCriticalSection(this->_printingSectionPointer); }

int RequestHandler::handleRequest()
{
  try
  {
    if (this->_key == constants::routes::login)
      return this->loginUser();

    if (this->_key == constants::routes::signUp)
      return this->registerUser();

    if (this->_key == constants::routes::updateUser)
      return this->updateUser();

    if (this->_key == constants::routes::deleteUser)
      return this->deleteUser();

    if (this->_key == constants::routes::getTasks)
      return this->getTasks();

    if (this->_key == constants::routes::createTask)
      return this->createTask();

    if (this->_key == constants::routes::updateTask)
      return this->updateTask();

    if (this->_key == constants::routes::deleteTasks)
      return this->deleteTask();

    return this->defaultHandler();
  }
  catch (...)
  {
    return this->sendResponse(false, {{"message", "something went wrong ..."}});
  }
};

int RequestHandler::defaultHandler()
{
  // TO-DO: create info message
  return this->sendResponse(false, {{"message", "invalid route"}});
}

int RequestHandler::sendResponse(bool success)
{
  if (success)
    return SuccessResponse(this->_printingSectionPointer).sendResponse(this->_socket);

  return FailureResponse(this->_printingSectionPointer).sendResponse(this->_socket);
}

int RequestHandler::sendResponse(bool success, json res)
{
  if (success)
    return SuccessResponse(this->_printingSectionPointer, res).sendResponse(this->_socket);

  return FailureResponse(this->_printingSectionPointer, res).sendResponse(this->_socket);
}

int RequestHandler::loginUser()
{
  this->enterPrintSection();
  std::cout << "login handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();

  if (!keyExists(body, "email") || !keyExists(body, "password"))
    return this->sendResponse(false, {{"error", "invalid body arguments"}});

  std::string email = body["email"];
  std::string password = body["password"];

  if (email.size() <= 0 || findFirstMatch(email, constants::validation::emailRegex).size() <= 0)
    return this->sendResponse(false, {{"error", "invalid email"}});

  if (password.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid password"}});

  json searchResult = DBWorker::instance()->performOperation(constants::db::GET_USER, {{"email", email}});

  if (searchResult.empty())
  {
    return this->sendResponse(false, {{"message", "user with this email doesn't exist"}});
  }

  std::hash<std::string> hash;
  std::string hashedPassword = std::to_string(hash(password + constants::db::salt));

  if (!verifyPassword(password, searchResult["data"]["password"]))
    return this->sendResponse(false, {{"message", "wrong password"}});

  json res;

  if (keyExists(searchResult, "data") && searchResult["data"].is_object() && keyExists(searchResult["data"], "id"))
    res["id"] = searchResult["data"]["id"];

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");
  res["token"] = jwtToken::signToken(res["id"]);

  return this->sendResponse(true, res);
}

int RequestHandler::registerUser()
{
  this->enterPrintSection();
  std::cout << "register handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();

  if (!keyExists(body, "email") || !keyExists(body, "password"))
    return this->sendResponse(false, {{"error", "invalid body arguments"}});

  std::string email = body["email"];
  std::string password = body["password"];

  if (email.size() <= 0 || findFirstMatch(email, constants::validation::emailRegex).size() <= 0)
    return this->sendResponse(false, {{"error", "invalid email"}}); // TO-DO: send error message

  if (password.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid password"}});

  json searchResult = DBWorker::instance()->performOperation(constants::db::GET_USER, {{"email", email.c_str()}});

  if (!searchResult.empty())
    return this->sendResponse(false, {{"message", "user already exists"}});

  std::string hashedPassword = hashPassword(password);
  json res = DBWorker::instance()->performOperation(constants::db::CREATE_USER, {{"email", email}, {"password", hashedPassword}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");
  res["token"] = jwtToken::signToken(res["id"]);

  return this->sendResponse(true, res);
}

int RequestHandler::updateUser()
{
  this->enterPrintSection();
  std::cout << "update user handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();
  json queryParams = this->_parser.getQueryParams();
  json headers = this->_parser.getHeaders();

  if (!keyExists(queryParams, "id"))
    return this->sendResponse(false, {{"error", "invalid query string parameters"}});

  std::string queryId = queryParams["id"];

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string id = jwtToken::parseToken(token);

  if (id.compare(queryId) != 0)
    return this->sendResponse(false, {{"message", "token id doesn't match id provided with query string"}});

  // get user by id
  json res = DBWorker::instance()->performOperation(constants::db::GET_USER, {{"id", id}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");

  if (!keyExists(body, "password"))
    return this->sendResponse(false, {{"error", "invalid body arguments"}});

  std::string password = body["password"];

  if (password.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid password"}});

  // if comparison failed
  // send failure response
  if (!verifyPassword(password, res["data"]["password"]))
    return this->sendResponse(false, {{"error", "wrong current password"}});

  // if success
  if (!keyExists(body, "newPassword"))
    return this->sendResponse(false, {{"error", "'newPassword' is missed"}});

  std::string newPassword = body["newPassword"];

  if (newPassword.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid new password"}});

  // create hashed password
  std::string hash = hashPassword(newPassword);

  // update user password
  res = DBWorker::instance()->performOperation(constants::db::UPDATE_USER, {{"id", id}, {"newPassword", hash}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }
  res.erase("status");

  // send success response
  return this->sendResponse(true, {{"message", "password was successfully updated"}});
}

int RequestHandler::deleteUser()
{
  this->enterPrintSection();
  std::cout << "delete user handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();
  json queryParams = this->_parser.getQueryParams();
  json headers = this->_parser.getHeaders();

  if (!keyExists(queryParams, "id"))
    return this->sendResponse(false, {{"error", "invalid query string parameters"}});

  std::string id = queryParams["id"];

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string parsedTokenId = jwtToken::parseToken(token);

  if (id.compare(parsedTokenId) != 0)
    return this->sendResponse(false, {{"message", "token id doesn't match id provided with query string"}});

  // get user by id
  json res = DBWorker::instance()->performOperation(constants::db::GET_USER, {{"id", id}});

  if (res.empty() || res["status"] == "error")
  {
    res.empty() ? 0 : res.erase("status");
    return this->sendResponse(false);
  }

  res.erase("status");

  // compare given password with stored in db
  if (!keyExists(body, "password"))
    return this->sendResponse(false, {{"error", "invalid body arguments"}});

  std::string password = body["password"];
  // if comparison failed
  // send failure response
  if (!verifyPassword(password, res["data"]["password"]))
    return this->sendResponse(false, {{"error", "wrong current password"}});

  // if success
  // delete user
  res = DBWorker::instance()->performOperation(constants::db::DELETE_USER, {{"id", id}});
  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");

  // send success response
  return this->sendResponse(true, {{"message", "user was successfully deleted"}});
}

// TO-DO - pagination
int RequestHandler::getTasks()
{
  this->enterPrintSection();
  std::cout << "get tasks handler" << std::endl;
  this->leavePrintSection();

  // json queryParams = this->_parser.getQueryParams();
  json headers = this->_parser.getHeaders();

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string parsedTokenId = jwtToken::parseToken(token);

  // get tasks
  json res = DBWorker::instance()->performOperation(constants::db::GET_TASKS, {{"userId", parsedTokenId}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }
  res.erase("status");

  return this->sendResponse(true, res);
}

int RequestHandler::createTask()
{
  this->enterPrintSection();
  std::cout << "create task handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();
  json queryParams = this->_parser.getQueryParams();
  json headers = this->_parser.getHeaders();

  // get user id from query string
  if (!keyExists(queryParams, "userId"))
    return this->sendResponse(false, {{"error", "invalid query string parameters - 'userId' parameter was missed"}});

  std::string userId = queryParams["userId"];

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string parsedTokenId = jwtToken::parseToken(token);

  // compare token and query string id
  if (userId.compare(parsedTokenId) != 0)
    return this->sendResponse(false, {{"message", "token id doesn't match id provided with query string"}});

  // check body
  if (!keyExists(body, "data"))
    return this->sendResponse(false, {{"error", "Invalid body - 'data' property was missed"}});

  std::string task = body["data"];

  // create task
  json res = DBWorker::instance()->performOperation(constants::db::CREATE_TASK, {{"userId", userId}, {"task", task}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");

  return this->sendResponse(true, res);
}

int RequestHandler::updateTask()
{
  this->enterPrintSection();
  std::cout << "update task handler" << std::endl;
  this->leavePrintSection();

  json queryParams = this->_parser.getQueryParams();
  json body = this->_parser.getBody();
  json headers = this->_parser.getHeaders();

  // check query string (task-id)
  if (!keyExists(queryParams, "id"))
    return this->sendResponse(false, {{"error", "invalid query params - `id` parameter is missed"}});

  std::string taskId = queryParams["id"];

  // check query string (userId)
  if (!keyExists(queryParams, "userId"))
    return this->sendResponse(false, {{"error", "invalid query params - `userId` parameter is missed"}});

  std::string userId = queryParams["userId"];

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string parsedTokenId = jwtToken::parseToken(token);

  if (userId.compare(parsedTokenId) != 0)
    return this->sendResponse(false, {{"message", "token id doesn't match id provided with query string"}});

  // check body
  if (!keyExists(body, "data"))
    return this->sendResponse(false, {{"error", "invalid body - 'data' property was missed"}});

  std::string task = body["data"];

  // get task by id
  json searchResult = DBWorker::instance()->performOperation(constants::db::GET_TASK_BY_ID, {{"id", taskId}, {"userId", userId}});

  if (searchResult.empty())
    return this->sendResponse(false, {{"message", "task doesn't exist"}});

  // update task
  json res = DBWorker::instance()->performOperation(constants::db::UPDATE_TASK, {{"id", taskId}, {"task", task}});

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }
  res.erase("status");

  return this->sendResponse(true, {{"message", "task was successfully updated"}});
}

int RequestHandler::deleteTask()
{
  this->enterPrintSection();
  std::cout << "delete tasks handler" << std::endl;
  this->leavePrintSection();

  json queryParams = this->_parser.getQueryParams();
  json body = this->_parser.getBody();
  json headers = this->_parser.getHeaders();

  // check query string (userId)
  if (!keyExists(queryParams, "userId"))
    return this->sendResponse(false, {{"error", "invalid body arguments - 'userId' parameter was missed"}});

  std::string userId = queryParams["userId"];

  // check query string (task-id)
  if (!keyExists(queryParams, "id"))
    return this->sendResponse(false, {{"error", "invalid body arguments - 'id' parameter was missed"}});

  std::string taskId = queryParams["id"];

  // check auth header (token)
  if (!keyExists(headers, "Authorization"))
    return this->sendResponse(false, {{"error", "Authorization header is missed"}});

  std::string token = headers["Authorization"];

  // parse token -> get user id
  std::string parsedTokenId = jwtToken::parseToken(token);

  if (userId.compare(parsedTokenId) != 0)
    return this->sendResponse(false, {{"message", "token id doesn't match id provided with query string"}});

  // get task by id
  json searchResult = DBWorker::instance()->performOperation(constants::db::GET_TASK_BY_ID, {{"id", taskId}, {"userId", userId}});

  if (searchResult.empty())
    return this->sendResponse(false, {{"message", "task doesn't exist"}});

  // delete task
  json res = DBWorker::instance()->performOperation(constants::db::DELETE_TASK, {{"id", taskId}});

  std::cout << "res -> " << res << std::endl;

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }
  res.erase("status");

  return this->sendResponse(true, {{"message", "task was successfully deleted"}});
}