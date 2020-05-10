#include "./FailureResponse.hpp"

FailureResponse::FailureResponse(CRITICAL_SECTION *printSection)
    : Response(printSection)
{
  this->_res["status"] = "failure";
  this->_res["response"]["statusCode"] = 400;
}

FailureResponse::FailureResponse(CRITICAL_SECTION *printSection, json res)
    : Response(printSection)
{
  this->_res["status"] = "failure";
  this->_res["response"]["statusCode"] = 400;
  this->_res["response"]["data"] = res;
}