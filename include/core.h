#pragma once

#include "JsonValue.h" // Json struct and JsonException
#include <string>

namespace json {

auto parse_string(std::string const& source) -> Json;

} // namespace json