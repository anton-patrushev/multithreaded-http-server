#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "./constants.hpp"
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

static bool verifyQueryString(std::string query, std::regex regex)
{
  std::smatch matches;
  std::regex_search(query, matches, regex);

  while (!matches.ready())
  {
    //waiting until matches full filled and established
  }

  return matches.size() != 0;
}

static void toLowerCase(std::string &src)
{
  std::transform(src.begin(), src.end(), src.begin(), [](unsigned char c) { return std::tolower(c); });
}

static void toUpperCase(std::string &src)
{
  std::transform(src.begin(), src.end(), src.begin(), [](unsigned char c) { return std::toupper(c); });
}

static std::string hashPassword(std::string password)
{
  std::hash<std::string> hash;
  return std::to_string(hash(password + constants::db::salt));
}

static bool verifyPassword(std::string password, std::string hashed)
{
  std::hash<std::string> hash;
  return hashed.compare(std::to_string(hash(password + constants::db::salt))) == 0;
}

#endif //HELPERS