#ifndef FAILURE_RESPONSE_HPP
#define FAILURE_RESPONSE_HPP

#include "./Response.hpp";

using json = nlohmann::json;

class FailureResponse : public Response
{
public:
  FailureResponse(CRITICAL_SECTION *printSection);
  FailureResponse(CRITICAL_SECTION *printSection, json res);
};

#endif //FAILURE_RESPONSE_HPP