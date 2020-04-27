#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include "../../helpers/json.hpp" // C++ JSON
#include "../../helpers/constants.hpp"

class RequestParser
{
  CRITICAL_SECTION *_printingSectionPointer;
  std::string _rawRequest;
  std::string _url;
  int _requestType;

  //something for headers (if necessary)

  //something for body (think about C++ JSON)

  //something for query params (data structure, maybe similar to JSON (key -> value))

  void parseUrl();
  void setRequestType(std::string httpMethod);
  std::string findFirstMatch(std::string str, std::regex regex);
  void enterPrintSection();
  void leavePrintSection();
  void parseRequest();
  std::string getHTMLResponse(std::string fileName = "index.html");

public:
  RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer);
  int sendResponse(SOCKET acceptedSocket, std::string response);
  int sendHTMLResponse(SOCKET acceptedSocket, std::string fileName = "index.html");

  std::string getUrl();
  int getRequestType();
};