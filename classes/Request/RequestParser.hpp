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
  // raw data
  std::string _rawRequest;
  std::string _fullUrl;
  std::string _queryString;

  //parsed data
  int _requestType;
  std::string _url;
  json _queryParams;
  json _headers;
  json _body;

  void parseRequest();
  void parseUrl();
  void parseQueryString();
  void parseHeaders();
  void parseBody();
  void setRequestType(std::string httpMethod);

  void enterPrintSection();
  void leavePrintSection();
  void logRequest();
  std::string getHttpMethod();
  std::string findFirstMatch(std::string str, std::regex regex);
  std::string replaceAll(std::string source, std::string searching, std::string inserting);
  std::string formatQueryString();

public:
  RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer);

  std::string getQueryParams();
  std::string getBody();
  std::string getHeaders();
  std::string getUrl();
  int getRequestType();
};

#endif //REQUEST_PARSER_HPP