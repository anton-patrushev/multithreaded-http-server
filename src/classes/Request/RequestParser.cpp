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

  // extract headers

  // extract body

  // extract query params
}

void RequestParser::setRequestType(std::string httpMethod)
{
  this->enterPrintSection();
  std::cout << "HTTP METHOD -> " << httpMethod << std::endl;
  this->leavePrintSection();

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

  this->_url = this->_rawRequest.substr(startIndex, length);

  this->enterPrintSection();
  std::cout << "URL -> " << this->_url << std::endl;
  this->leavePrintSection();
}

std::string RequestParser::getUrl() { return this->_url; }
int RequestParser::getRequestType() { return this->_requestType; }

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