#include "RequestWorker.h"

std::string RequestWorker::getHTMLResponse(std::string fileName)
{
  int statusCode = 200;
  std::string content;
  std::ifstream file(".\\html\\" + fileName);

  if (file.good())
  {
    // std::cout << "file opened" << std::endl;
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    content = str;
    statusCode = 200;
    // std::cout << "content from file ->" << std::endl
    //           << content << std::endl;
  }

  file.close();

  std::ostringstream oss;
  oss << "HTTP/1.1 " << statusCode << " OK\r\n";
  oss << "Cache-Control: no-cache, private\r\n";
  oss << "Content-Type: text/html\r\n";
  oss << "Content-Length: " << content.size() << "\r\n";
  oss << "\r\n";
  oss << content;

  // std::cout << "parsed content ->" << std::endl
  //           << oss.str() << std::endl;

  return oss.str();
}

int RequestWorker::sendResponse(SOCKET acceptedSocket, std::string response)
{
  const int sendStatus = send(acceptedSocket, response.c_str(), response.size(), 0);
  if (sendStatus == SOCKET_ERROR)
  {
    std::cout << "sending error -> " << WSAGetLastError() << std::endl;
    closesocket(acceptedSocket);
    return sendStatus;
  }

  std::cout << sendStatus << " bytes was successfully sended" << std::endl;
  std::cout << std::endl;
  return 0;
}

int RequestWorker::sendHTMLResponse(SOCKET acceptedSocket, std::string fileName)
{
  return this->sendResponse(acceptedSocket, this->getHTMLResponse(fileName));
}