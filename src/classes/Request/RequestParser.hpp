#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <windows.h>

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

public:
  RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer);

  std::string getUrl();
  int getRequestType();
};

#endif //REQUEST_PARSER_HPP