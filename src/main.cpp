#include "./classes/Server/ServerListener.hpp"
#include "./helpers/init.hpp"

int main()
{
  // if (initApp() != 0)
  // {
  //   std::cout << "Failed to start server" << std::endl;
  //   return 1;
  // }

  return ServerListener().run();
}