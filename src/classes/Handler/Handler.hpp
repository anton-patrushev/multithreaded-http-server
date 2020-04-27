#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include "../../helpers/constants.hpp"
#include "../../helpers/json.hpp"

using json = nlohmann::json;

class Handler
{
  std::string _name;
  std::string _key;
  int _method;
  std::function<int(json)> _handler;
  std::vector<Handler *> _childs;

  void setKey(std::string name);

public:
  Handler(
      std::string name, std::vector<Handler *> childs = {}, int method = -1,
      std::function<int(json)> handler = [](json j) { return 0; });

  std::string getKey();
  std::vector<Handler *> getChilds();
  // std::string getName();
};