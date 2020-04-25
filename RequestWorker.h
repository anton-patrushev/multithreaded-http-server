#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class RequestWorker
{
  std::string getHTMLResponse(std::string fileName = "index.html");

public:
  int sendResponse(SOCKET acceptedSocket, std::string response);
  int sendHTMLResponse(SOCKET acceptedSocket, std::string fileName = "index.html");
};