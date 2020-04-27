#pragma once

#include "./ServerListener.hpp"

ServerListener::ServerListener(int port, const char *ipAdress)
{
  InitializeCriticalSection(&this->_printSection);

  this->enterPrintingSection();
  std::cout << "Server starting on " << ipAdress << ":" << port << std::endl;
  this->leavePrintingSection();

  this->_port = port;
  this->_ipAddress = ipAdress;
  this->_listenSocket = INVALID_SOCKET;

  WSAData wsaData;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    // todo: handle error

    this->enterPrintingSection();
    std::cout << "WSAStartup error -> " << WSAGetLastError() << std::endl;
    this->enterPrintingSection();
    // WSACleanup();
  };
};

int ServerListener::init()
{
  addrinfo *result = NULL;
  addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // resolve socket address
  const int addrinfoStatus = getaddrinfo(NULL, std::to_string(this->_port).c_str(),
                                         &hints, &result);
  if (addrinfoStatus != 0)
  {
    this->enterPrintingSection();
    std::cout << "getaddrinfo error -> " << WSAGetLastError() << std::endl;
    this->leavePrintingSection();

    WSACleanup();
    return addrinfoStatus;
  }

  // creating server listenSocket
  this->_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (this->_listenSocket == INVALID_SOCKET)
  {
    this->enterPrintingSection();
    std::cout << "create socket error -> " << WSAGetLastError() << std::endl;
    this->leavePrintingSection();
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  const int bindStatus = bind(this->_listenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (bindStatus == SOCKET_ERROR)
  {
    this->enterPrintingSection();
    std::cout << "bind socket error -> " << WSAGetLastError() << std::endl;
    this->leavePrintingSection();
    freeaddrinfo(result);
    closesocket(this->_listenSocket);
    WSACleanup();
    return bindStatus;
  }
  freeaddrinfo(result);

  const int listenStatus = listen(this->_listenSocket, SOMAXCONN);
  if (listenStatus == SOCKET_ERROR)
  {
    this->enterPrintingSection();
    std::cout << "listen socket error -> " << WSAGetLastError() << std::endl;
    this->leavePrintingSection();
    closesocket(this->_listenSocket);
    WSACleanup();
    return listenStatus;
  }

  return 0;
}

int ServerListener::run()
{
  const int initStatus = this->init();
  if (initStatus != 0)
  {
    return initStatus;
  }

  this->enterPrintingSection();
  std::cout << "Waiting for connection ..." << std::endl;
  this->leavePrintingSection();
  bool running = true;
  while (running)
  {
    SOCKET clientSocket = INVALID_SOCKET;

    try
    {
      //accept connection
      clientSocket = accept(this->_listenSocket, NULL, NULL);
      if (clientSocket == INVALID_SOCKET)
      {
        throw "Error";
      }

      this->enterPrintingSection();
      std::cout << "Connected! Socket -> " << clientSocket << std::endl;
      this->leavePrintingSection();
      //run thread & add to threads map
      // this->_threads[clientSocket] = std::thread(ServerListener::requestHandler, clientSocket, &this->_printSection);
      RequestHandlerParameter parameter = {clientSocket, &this->_printSection};
      this->_threads[clientSocket] = CreateThread(NULL, 1024, ServerListener::requestHandler, &parameter, 0, NULL);

      // this->requestHandler(clientSocket);
    }
    catch (...)
    {
      // handle error (accepting error and others)
      this->enterPrintingSection();
      std::cout << "Oops! Something went wrong ..." << std::endl;
      this->leavePrintingSection();
    }
  }
}

DWORD WINAPI ServerListener::requestHandler(CONST LPVOID parameter)
{
  SOCKET acceptedSocket = static_cast<RequestHandlerParameter *>(parameter)->acceptedSocket;
  CRITICAL_SECTION *printSection = static_cast<RequestHandlerParameter *>(parameter)->printSectionPointer;

  const int buffer_size = 1024;
  char buffer[buffer_size];
  bool connected = true;

  while (connected)
  {
    // Sleep(2000);
    const int receieveStatus = recv(acceptedSocket, buffer, buffer_size, NULL);

    if (receieveStatus == SOCKET_ERROR)
    {
      EnterCriticalSection(printSection);
      std::cout << "receive error -> " << WSAGetLastError() << std::endl;
      LeaveCriticalSection(printSection);
      break;
    }

    if (receieveStatus == 0)
    {
      EnterCriticalSection(printSection);
      std::cout << "connection is closed" << std::endl;
      LeaveCriticalSection(printSection);
      break;
    }

    buffer[receieveStatus] = 0;
    EnterCriticalSection(printSection);
    std::cout << "Socket " << acceptedSocket << " -> received " << receieveStatus << "bytes" << std::endl;
    LeaveCriticalSection(printSection);

    // parse received buffer (extract headers, body, url, queryString and etc.)

    RequestHandler handler(buffer, printSection);
    // perform action depends on url (+ body, queryString)

    // send response
    // int sendStatus = requestParser.sendHTMLResponse(acceptedSocket);
  }

  closesocket(acceptedSocket);
  ExitThread(0);
}

void ServerListener::enterPrintingSection() { EnterCriticalSection(&this->_printSection); };
void ServerListener::leavePrintingSection() { LeaveCriticalSection(&this->_printSection); };