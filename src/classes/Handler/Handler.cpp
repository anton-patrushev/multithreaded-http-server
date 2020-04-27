#include "./Handler.hpp"

Handler::Handler(std::string name, std::vector<Handler *> childs, int method, std::function<int(json)> handler)
{
  // init
  this->_name = name;
  this->_method = method;
  this->_handler = handler;
  this->_childs = childs;
  this->setKey(name);
}

void Handler::setKey(std::string name)
{
  //key = "routeName_methodName"
  std::stringstream ss(this->_name);
  ss << constants::ROUTE_KEY_DELIMITER << this->_method;

  this->_key = ss.str();
}

std::string Handler::getKey() { return this->_key; }
std::vector<Handler *> Handler::getChilds() { return this->_childs; }