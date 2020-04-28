#include "./FailureResponse.hpp"

FailureResponse::FailureResponse(CRITICAL_SECTION *printSection)
    : Response(printSection)
{
  this->_res["status"] = "failure";
  this->_res["response"]["statusCode"] = 400;
}

FailureResponse::FailureResponse(CRITICAL_SECTION *printSection, std::string message)
    : Response(printSection)
{
  this->_res["status"] = "failure";
  this->_res["response"]["statusCode"] = 400;
  this->_res["response"]["data"] = {"message", message.c_str()};
}

FailureResponse::FailureResponse(CRITICAL_SECTION *printSection, std::string message, json res)
    : Response(printSection)
{
  this->_res["status"] = "failure";
  this->_res["response"]["statusCode"] = 400;
  this->_res["response"]["data"] = {{"message", message.c_str()}, {"data", res}};
}