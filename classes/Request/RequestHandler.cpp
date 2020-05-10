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
  if (this->_key == constants::routes::login)
    return this->loginUser();

  if (this->_key == constants::routes::signUp)
    return this->registerUser();

  if (this->_key == constants::routes::getTasks)
    return this->getTasks();

  if (this->_key == constants::routes::createTask)
    return this->createTask();

  if (this->_key == constants::routes::updateTask)
    return this->updateTask();

  if (this->_key == constants::routes::deleteTasks)
    return this->deleteTasks();

  return this->defaultHandler();
};

int RequestHandler::defaultHandler()
{
  return this->sendResponse(false);
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

  json res = DBWorker::instance()->performOperation(constants::db::GET_USER, nullptr);

  this->enterPrintSection();
  std::cout << "res -> " << res << std::endl;
  this->leavePrintSection();

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");

  return this->sendResponse(true);
}

int RequestHandler::registerUser()
{
  this->enterPrintSection();
  std::cout << "register handler" << std::endl;
  this->leavePrintSection();

  json body = this->_parser.getBody();

  if (!this->keyExists(body, "email") || !this->keyExists(body, "password"))
    return this->sendResponse(false, {{"error", "invalid body arguments"}});

  std::string email = body["email"];
  std::string password = body["password"];

  if (email.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid email"}}); // TO-DO: send error message

  if (password.size() <= 0)
    return this->sendResponse(false, {{"error", "invalid password"}}); // TO-DO: send error message

  // TO-DO: check email (regex)

  // TO-DO: check password (regex)

  json res = DBWorker::instance()->performOperation(constants::db::CREATE_USER, {{"email", email.c_str()}, {"password", password.c_str()}});

  this->enterPrintSection();
  std::cout << "res -> " << res << std::endl;
  this->leavePrintSection();

  if (res["status"] == "error")
  {
    res.erase("status");
    return this->sendResponse(false, res);
  }

  res.erase("status");

  std::string token = jwt::create()
                          .set_type("JWS")
                          .set_payload_claim("id", jwt::claim(std::string("id")))
                          .sign(jwt::algorithm::hs256{"secret"});

  res["token"] = token;

  return this->sendResponse(true, res);
}

int RequestHandler::getTasks()
{
  this->enterPrintSection();
  std::cout << "get tasks handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::createTask()
{
  this->enterPrintSection();
  std::cout << "create task handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::updateTask()
{
  this->enterPrintSection();
  std::cout << "update task handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::deleteTasks()
{
  this->enterPrintSection();
  std::cout << "delete tasks handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

bool RequestHandler::keyExists(const json &j, const std::string &key) { return j.find(key) != j.end(); }