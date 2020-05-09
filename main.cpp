#include "./classes/Server/ServerListener.hpp"
#include "./helpers/init.hpp"

int main()
{
  initApp();
  return ServerListener().run();
}