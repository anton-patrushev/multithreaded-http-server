#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include "./RequestParser.hpp"

class RequestHandler
{
  RequestParser _parser;
  CRITICAL_SECTION *_printingSectionPointer;
  std::vector<std::string> _path;

  void enterPrintSection();
  void leavePrintSection();
  void setPath();
  void convertUrlToPath(std::string url);

  // void sendResponse();

public:
  RequestHandler(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer);
  int handleRequest();
};