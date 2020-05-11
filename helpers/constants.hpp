#ifndef CONSTANS_HPP
#define CONSTANS_HPP

#include <regex>

namespace constants
{
namespace http
{
const int GET_HTTP = 0;
const int POST_HTTP = 1;
const int PATCH_HTTP = 2;
const int DELETE_HTTP = 3;

const char ROUTE_KEY_DELIMITER = '_';
} // namespace http

namespace routes
{
const std::string updateUser = "/api/users/_2";
const std::string deleteUser = "/api/users/_3";
const std::string login = "/api/users/login/_1";
const std::string signUp = "/api/users/register/_1";
const std::string getTasks = "/api/tasks/_0";
const std::string createTask = "/api/tasks/_1";
const std::string updateTask = "/api/tasks/_2";
const std::string deleteTasks = "/api/tasks/_3";

} // namespace routes

namespace db
{
const int GET_USER = 0;
const int CREATE_USER = 1;
const int UPDATE_USER = 2;
const int DELETE_USER = 3;

const int GET_TASKS = 4;
const int CREATE_TASK = 5;
const int UPDATE_TASK = 6;
const int DELETE_TASK = 7;

const std::string salt = "unique_salt";
const std::string jwtSecret = "jwt-secret";
} // namespace db

namespace validation
{
const std::regex emailRegex("^(([^<>()\\[\\]\\.,;:\\s@\"]+(\\.[^<>()\\[\\]\\.,;:\\s@\"]+)*)|(\".+\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\])|(([a-zA-Z\\-0-9]+\\.)+[a-zA-Z]{2,}))$", std::regex_constants::ECMAScript);
} // namespace validation

} // namespace constants

#endif //CONSTANS_HPP