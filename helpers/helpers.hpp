#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "./json.hpp"

using json = nlohmann::json;

static bool keyExists(const json &j, const std::string &key) { return j.find(key) != j.end(); }

#endif //HELPERS