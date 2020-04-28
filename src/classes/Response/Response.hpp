#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <iostream>
#include "../../helpers/json.hpp";

using json = nlohmann::json;

class Response
{
protected:
  CRITICAL_SECTION *_printSection;
  json _res;

  int _sendResponse(SOCKET socket, std::string response);
  void enterPrintSection();
  void leavePrintSection();
  std::string createResponse(int statusCode, std::string message);

public:
  Response(CRITICAL_SECTION *printSectionPointer);

  int sendResponse(SOCKET socket);
};

#endif //RESPONSE_HPP