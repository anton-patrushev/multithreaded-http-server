#include "./RequestParser.hpp"

RequestParser::RequestParser(std::string rawRequest, CRITICAL_SECTION *printingSectionPointer)
{
  this->_rawRequest = rawRequest;
  this->_printingSectionPointer = printingSectionPointer;

  this->parseRequest();
}

void RequestParser::enterPrintSection() { EnterCriticalSection(this->_printingSectionPointer); }
void RequestParser::leavePrintSection() { LeaveCriticalSection(this->_printingSectionPointer); }

void RequestParser::parseRequest()
{
  // extract method type
  std::regex httpMethodRegex("^[A-Z]{3,}");
  this->setRequestType(findFirstMatch(this->_rawRequest, httpMethodRegex));

  // extract url
  this->parseUrl();

  // extract query params
  this->parseQueryString();

  // extract headers (only necessary)
  this->parseHeaders();

  // extract body
  this->parseBody();

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

  this->_queryParams = json::parse(this->formatQueryString());
}

void RequestParser::logRequest()
{
  this->enterPrintSection();
  std::cout << std::endl;
  std::cout << "Request ->" << std::endl;

  // std::cout << "Raw Request ->" << std::endl;
  // std::cout << this->_rawRequest << std::endl;
  // std::cout << std::endl;

  std::cout << "HTTP METHOD -> " << this->getHttpMethod() << std::endl;
  std::cout << "full url -> \'" << this->_fullUrl << "\'" << std::endl;
  std::cout << "url -> \'" << this->_url << "\'" << std::endl;
  std::cout << "queryParams -> " << this->_queryParams << std::endl;
  std::cout << "usefull headers -> " << this->_headers << std::endl;
  std::cout << "body -> " << this->_body << std::endl;
  std::cout << std::endl;
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

  return NULL;
}

void RequestParser::parseHeaders()
{
  std::string headerValue = "Authorization";

  size_t position = this->_rawRequest.find(headerValue);
  if (position != std::string::npos)
  {
    size_t length = 0;
    for (size_t index = position + headerValue.size(); this->_rawRequest[index] != '\n' && this->_rawRequest[index] != '\r'; ++index, ++length)
    {
      // calculating end position (length of value)
    }

    std::string authToken = this->_rawRequest.substr(position + headerValue.size() + 2, length - 2);
    this->_headers["Authorization"] = authToken;
  }
}

void RequestParser::parseBody()
{
  size_t position = this->_rawRequest.find("\r\n\r\n");

  if (position == std::string::npos)
    return;
  position += 4;
  if (position == this->_rawRequest.size())
    return;

  std::string body = this->_rawRequest.substr(position, this->_rawRequest.size() - position);
  this->_body = json::parse(body);
}

std::string RequestParser::getUrl() { return this->_url; }
int RequestParser::getRequestType() { return this->_requestType; }
json RequestParser::getQueryParams() { return this->_queryParams; }
json RequestParser::getBody() { return this->_body; }
json RequestParser::getHeaders() { return this->_headers; }

std::string RequestParser::formatQueryString()
{
  std::string temp = this->_queryString.substr(1, this->_queryString.size() - 1);

  temp.insert(temp.begin(), '{');
  temp.insert(temp.end(), '}');
  temp = this->replaceAll(temp, "=", "\":\"");
  temp = this->replaceAll(temp, "&", "\",\"");
  temp = this->replaceAll(temp, "{", "{\"");
  temp = this->replaceAll(temp, "}", "\"}");

  return temp;
}
std::string RequestParser::replaceAll(std::string source, std::string searching, std::string inserting)
{
  size_t index = 0;
  while (index != std::string::npos)
  {
    // Locate the substring to replace.
    index = source.find(searching.c_str(), index);
    if (index == std::string::npos)
      break;

    // Make the replacement.
    source.replace(index, searching.size(), inserting.c_str());

    // Advance index forward so the next iteration doesn't pick it up as well.
    index += inserting.size();
  }
  return source;
}