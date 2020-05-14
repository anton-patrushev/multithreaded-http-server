#ifndef JWT_HELPER_HPP
#define JWT_HELPER_HPP

#include "./json.hpp"
using json = nlohmann::json;

#define NOMINMAX
#undef max
#undef min
#include "jwt-cpp/jwt.h"

namespace jwtToken
{
static std::string signToken(std::string id)
{
  return jwt::create()
      .set_type("JWS")
      .set_payload_claim("id", jwt::claim(id))
      .sign(jwt::algorithm::hs256{"secret"});
};

static std::string parseToken(std::string token)
{
  return jwt::decode(token).get_payload_claim("id").as_string();
};

} // namespace jwtToken

#endif // JWT_HELPER_HPP