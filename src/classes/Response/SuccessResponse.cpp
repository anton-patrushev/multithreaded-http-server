#include "./SuccessResponse.hpp"

SuccessResponse::SuccessResponse(CRITICAL_SECTION *printSection)
    : Response(printSection)
{
  this->_res["status"] = "success",
  this->_res["response"]["statusCode"] = 200;
}

SuccessResponse::SuccessResponse(CRITICAL_SECTION *printSection, json res)
    : Response(printSection)
{
  this->_res["status"] = "success",
  this->_res["response"]["statusCode"] = 200;
  this->_res["response"]["data"] = res;
}