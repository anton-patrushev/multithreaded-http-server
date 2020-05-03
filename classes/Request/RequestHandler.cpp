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
    return this->loginUser(json());

  if (this->_key == constants::routes::signUp)
    return this->registerUser(json());

  if (this->_key == constants::routes::getTasks)
    return this->getTasks(json());

  if (this->_key == constants::routes::createTask)
    return this->createTask(json());

  if (this->_key == constants::routes::updateTask)
    return this->updateTask(json());

  if (this->_key == constants::routes::deleteTasks)
    return this->deleteTasks(json());

  return this->defaultHandler();
};

int RequestHandler::defaultHandler()
{
  return this->sendResponse(false);
}

int RequestHandler::sendResponse(bool success)
{
  if (success)
  {
    SuccessResponse response(this->_printingSectionPointer);
    return response.sendResponse(this->_socket);
  }

  FailureResponse response(this->_printingSectionPointer);
  return response.sendResponse(this->_socket);
}

int RequestHandler::sendResponse(bool success, json res)
{
  // use Response class to return value with json content
  return 0;
}

int RequestHandler::loginUser(json req)
{
  this->enterPrintSection();
  std::cout << "login handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::registerUser(json req)
{
  this->enterPrintSection();
  std::cout << "register handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::getTasks(json req)
{
  this->enterPrintSection();
  std::cout << "get tasks handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::createTask(json req)
{
  this->enterPrintSection();
  std::cout << "create task handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::updateTask(json req)
{
  this->enterPrintSection();
  std::cout << "update task handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}

int RequestHandler::deleteTasks(json req)
{
  this->enterPrintSection();
  std::cout << "delete tasks handler" << std::endl;
  this->leavePrintSection();

  return this->sendResponse(true);
}