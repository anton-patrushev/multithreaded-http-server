#ifndef SERVER_LISTENER_HPP
#define SERVER_LISTENER_HPP

#pragma comment(lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <thread>
#include <map>
#include <iostream>
#include <string>
#include <functional>

// request helpers container
#ifndef REQUEST_HANDLER_HPP
#include "../Request/RequestHandler.hpp"
#endif

struct RequestHandlerParameter
{
  SOCKET acceptedSocket;
  CRITICAL_SECTION *printSectionPointer;
};

class ServerListener
{
private:
  int _port;
  const char *_ipAddress;
  SOCKET _listenSocket;
  CRITICAL_SECTION _printSection;
  std::map<SOCKET, HANDLE> _threads;

  int init();
  static DWORD WINAPI requestHandler(CONST LPVOID parameter);
  void enterPrintingSection();
  void leavePrintingSection();

public:
  ServerListener(int port = 8080, const char *ipAddress = "127.0.0.1");
  int run();
};

#endif //SERVER_LISTENER_HPP