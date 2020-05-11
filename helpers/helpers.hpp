#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "./json.hpp"
#include <string>
#include <regex>

using json = nlohmann::json;

static bool keyExists(const json &j, const std::string &key) { return j.find(key) != j.end(); }

static std::string findFirstMatch(std::string str, std::regex regex)
{
  std::smatch matches;
  std::regex_search(str, matches, regex);

  while (!matches.ready())
  {
    //waiting until matches full filled and established
  }

  return matches[0];
}

#endif //HELPERS