#include "./RequestParser.hpp"

/**
 * @todo parse queryString params & store them (maybe as JSON)
*/

RequestParser::RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer)
{
  this->_rawRequest = rawRequest;
  this->_printingSectionPointer = printingSectionPointer;

  this->parseRequest();
}

void RequestParser::enterPrintSection() { EnterCriticalSection(this->_printingSectionPointer); }
void RequestParser::leavePrintSection() { LeaveCriticalSection(this->_printingSectionPointer); }

std::string RequestParser::findFirstMatch(std::string str, std::regex regex)
{
  std::smatch matches;
  std::regex_search(this->_rawRequest, matches, regex);

  while (!matches.ready())
  {
    //waiting until matches full filled and established
  }

  return matches[0];
}

void RequestParser::parseRequest()
{
  // extract method type
  std::regex httpMethodRegex("^[A-Z]{3,}");
  this->setRequestType(this->findFirstMatch(this->_rawRequest, httpMethodRegex));

  // extract url
  this->parseUrl();

  // extract query params
  // this->parseQueryString();

  // extract headers

  // extract body

  //log request info
  this->logRequest();
}

void RequestParser::setRequestType(std::string httpMethod)
{
  if (httpMethod.compare("GET") == 0)
  {
    this->_requestType = constants::http::GET_HTTP;
    return;
  }

  if (httpMethod.compare("POST") == 0)
  {
    this->_requestType = constants::http::POST_HTTP;
    return;
  }

  if (httpMethod.compare("PATCH") == 0)
  {
    this->_requestType = constants::http::PATCH_HTTP;
    return;
  }

  if (httpMethod.compare("DELETE") == 0)
  {
    this->_requestType = constants::http::DELETE_HTTP;
    return;
  }

  this->enterPrintSection();
  std::cout << "wrong HTTP method" << std::endl;
  this->leavePrintSection();
  return;
}

void RequestParser::parseUrl()
{
  int startIndex = 0;
  int length = 0;

  for (int i = 0; i < this->_rawRequest.size(); ++i)
    if (this->_rawRequest[i] == ' ')
    {
      // store index url first symbol
      startIndex = i + 1;
      break;
    }

  for (int i = startIndex; i < this->_rawRequest.size(); ++i, ++length)
    if (this->_rawRequest[i] == ' ')
      break;

  //full url with queryString;
  this->_fullUrl = this->_rawRequest.substr(startIndex, length);

  this->enterPrintSection();
  std::cout << "full url -> \'" << this->_fullUrl << "\'" << std::endl;
  this->leavePrintSection();

  length = 0;
  for (int i = 0; i < this->_fullUrl.size(); ++i, ++length)
    if (this->_fullUrl[length] == '?')
      break;

  if (length == this->_fullUrl.size())
    this->_url = this->_fullUrl;
  else
    this->_url = this->_fullUrl.substr(0, length);

  this->_queryString = this->_fullUrl.substr(length, this->_fullUrl.size() - length);
}

//  parsing scheme:
//  paramName=value -> into json -> { "paramName": value }
void RequestParser::parseQueryString()
{
  // if empty or only '?' symbol
  if (this->_queryString.size() <= 1)
    return;

  // bad query string
  if (this->_queryString[0] != '?')
    return;

  // bad query string
  if (this->_queryString.find('=') == std::string::npos)
    return;

  std::string temp = this->_queryString.substr(1, this->_queryString.size());
  std::stringstream ss(temp);
}

std::string RequestParser::getUrl() { return this->_url; }
int RequestParser::getRequestType() { return this->_requestType; }

void RequestParser::logRequest()
{
  this->enterPrintSection();
  std::cout << "HTTP METHOD -> " << this->getHttpMethod() << std::endl;
  std::cout << "url -> \'" << this->_url << "\'" << std::endl;
  std::cout << "queryString -> \'" << this->_queryString << "\'" << std::endl;
  this->leavePrintSection();
}

std::string RequestParser::getHttpMethod()
{
  if (this->_requestType == constants::http::GET_HTTP)
    return "GET";

  if (this->_requestType == constants::http::POST_HTTP)
    return "POST";

  if (this->_requestType == constants::http::PATCH_HTTP)
    return "PATCH";

  if (this->_requestType == constants::http::DELETE_HTTP)
    return "DELETE";
}
// std::string RequestParser::getHTMLResponse(std::string fileName)
// {
//   int statusCode = 200;
//   std::string content;
//   // set relative from project root
//   std::ifstream file("./src/html/index.html");

//   if (file.good())
//   {
//     std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//     content = str;
//     statusCode = 200;
//   }

//   file.close();

//   std::ostringstream oss;
//   oss << "HTTP/1.1 " << statusCode << " OK\r\n";
//   oss << "Cache-Control: no-cache, private\r\n";
//   oss << "Content-Type: text/html\r\n";
//   oss << "Content-Length: " << content.size() << "\r\n";
//   oss << "\r\n";
//   oss << content;

//   return oss.str();
// }

// int RequestParser::sendHTMLResponse(SOCKET acceptedSocket, std::string fileName)
// {
//   return this->sendResponse(acceptedSocket, this->getHTMLResponse(fileName));
// }