#include "./Response.hpp"

Response::Response(CRITICAL_SECTION *printSectionPointer)
{
  this->_printSection = printSectionPointer;
}

void Response::enterPrintSection() { EnterCriticalSection(this->_printSection); }
void Response::leavePrintSection() { LeaveCriticalSection(this->_printSection); }

std::string Response::createResponse(int statusCode, std::string message)
{
  std::string temp = this->_res.dump();
  std::ostringstream oss;
  oss << "HTTP/1.1 " << statusCode << " " << message << "\r\n";
  oss << "Cache-Control: no-cache, private\r\n";
  oss << "Content-Type: text/json\r\n";
  oss << "Content-Length: " << temp.size() << "\r\n";
  oss << "\r\n";

  oss << temp << "\r\n";

  // this->enterPrintSection();
  // std::cout << oss.str() << std::endl;
  // this->leavePrintSection();

  return oss.str();
}

int Response::sendResponse(SOCKET socket)
{
  std::string response = this->createResponse(200, "Success");

  const int sendStatus = send(socket, response.c_str(), response.size(), 0);
  if (sendStatus == SOCKET_ERROR)
  {
    this->enterPrintSection();
    std::cout << "sending error -> " << WSAGetLastError() << std::endl;
    this->leavePrintSection();
    closesocket(socket);
    return sendStatus;
  }

  this->enterPrintSection();
  std::cout << sendStatus << " bytes was successfully sended" << std::endl;
  std::cout << std::endl;
  this->leavePrintSection();

  return 0;
}

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