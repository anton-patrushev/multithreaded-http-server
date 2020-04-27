#include "./RequestHandler.hpp"

/**
 * @todo parse queryString params & store them (maybe as JSON)
*/

RequestHandler::RequestHandler(std::string rawRequest, CRITICAL_SECTION *printSectionPointer)
    : _parser(rawRequest, printSectionPointer)
{
  this->_printingSectionPointer = printSectionPointer;
  this->setPath();

  // parseQueryParams
}

void RequestHandler::setPath() { this->convertUrlToPath(this->_parser.getUrl()); }
void RequestHandler::enterPrintSection() { EnterCriticalSection(this->_printingSectionPointer); }
void RequestHandler::leavePrintSection() { LeaveCriticalSection(this->_printingSectionPointer); }

void RequestHandler::convertUrlToPath(std::string url)
{
  std::stringstream ss(url);
  std::string token;

  while (std::getline(ss, token, '/'))
  {
    int questionCharPosition = 0;
    questionCharPosition = token.find('?', 0);
    if (questionCharPosition != std::string::npos)
    {
      token = token.substr(0, questionCharPosition);
    }

    token.append("_");
    token.append(std::to_string(constants::NOT_FINAL_ROUTE));

    //append to path http_method with key
    this->_path.push_back(token);
  }

  token = this->_path[this->_path.size() - 1];

  token = token.substr(0, token.find('_', 0));
  token.append("_");
  token.append(std::to_string(this->_parser.getRequestType()));

  this->_path[this->_path.size() - 1] = token;
}

int RequestHandler::handleRequest()
{
  for (int index = 0; index < this->_path.size(); ++index)
  {
    std::string route = this->_path[index];
  }

  // this->sendResponse();
  return 0; //success code
}