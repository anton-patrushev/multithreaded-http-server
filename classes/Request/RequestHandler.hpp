#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "jwtd.lib")

//#define CPP_JWT_USE_VENDORED_NLOHMANN_JSON

#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// db
#include "../db/DBWorker.hpp"
// Response
#include "../Response/FailureResponse.hpp"
#include "../Response/SuccessResponse.hpp"

// Request Parser
#include "./RequestParser.hpp"

// helpers
#include "../../helpers/json.hpp"
#include "../../helpers/constants.hpp"
#include "../../helpers/helpers.hpp"
#include "../../helpers/jwt.hpp"

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

  // for any routes
  int defaultHandler();

  // user routes
  int loginUser();
  int registerUser();
  int updateUser();
  int deleteUser();

  // task routes
  int getTasks();
  int createTask();
  int updateTask();
  int deleteTasks();

  int sendResponse(bool success);
  int sendResponse(bool success, json res);

public:
  RequestHandler(std::string rawRequest, SOCKET acceptedSocket, CRITICAL_SECTION *printingSectionPointer);
  int handleRequest();
};

#endif //REQUEST_HANDLER_HPP