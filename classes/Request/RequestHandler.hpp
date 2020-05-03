#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "../Response/FailureResponse.hpp"
#include "../Response/SuccessResponse.hpp"

#include "./RequestParser.hpp"
#include "../../helpers/json.hpp" //C++ JSON
#include "../../helpers/constants.hpp"

using json = nlohmann::json;

class RequestHandler
{
  RequestParser _parser;
  CRITICAL_SECTION *_printingSectionPointer;
  std::string _key;
  SOCKET _socket;

  void setKey();

  void enterPrintSection();
  void leavePrintSection();

  int defaultHandler();
  int loginUser(json req);
  int registerUser(json req);
  int getTasks(json req);
  int createTask(json req);
  int updateTask(json req);
  int deleteTasks(json req);

  int sendResponse(bool success);
  int sendResponse(bool success, json res);

public:
  RequestHandler(std::string rawRequest, SOCKET acceptedSocket, CRITICAL_SECTION *printingSectionPointer);
  int handleRequest();
};

#endif //REQUEST_HANDLER_HPP