#ifndef CONSTANS_HPP
#define CONSTANS_HPP

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
const std::string login = "/api/users/login/_1";
const std::string signUp = "/api/users/register/_1";
const std::string getTasks = "/api/tasks/_0";
const std::string createTask = "/api/tasks/_1";
const std::string updateTask = "/api/tasks/_2";
const std::string deleteTasks = "/api/tasks/_3";

} // namespace routes
} // namespace constants

#endif //CONSTANS_HPP