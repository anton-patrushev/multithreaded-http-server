#ifndef SUCCESS_RESPONSE_HPP
#define SUCCESS_RESPONSE_HPP

#include "./Response.hpp";

using json = nlohmann::json;

class SuccessResponse : public Response
{
  int sendSuccessResponse();

public:
  SuccessResponse(CRITICAL_SECTION *printSection);
  SuccessResponse(CRITICAL_SECTION *printSection, json res);
};

#endif //SUCCESS_RESPONSE_HPP