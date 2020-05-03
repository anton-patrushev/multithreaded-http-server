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

using json = nlohmann::json;

class RequestParser
{
  CRITICAL_SECTION *_printingSectionPointer;
  int _requestType;
  std::string _rawRequest;
  std::string _fullUrl;
  std::string _url;
  std::string _queryString;
  json _queryParams;

  //something for headers (if necessary)

  //something for body (think about C++ JSON)

  //something for query params (data structure, maybe similar to JSON (key -> value))

  void parseQueryString();
  void parseUrl();
  void setRequestType(std::string httpMethod);
  std::string findFirstMatch(std::string str, std::regex regex);
  void enterPrintSection();
  void leavePrintSection();
  void parseRequest();
  void logRequest();
  std::string getHttpMethod();

public:
  RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer);

  std::string getUrl();
  int getRequestType();
};

#endif //REQUEST_PARSER_HPP