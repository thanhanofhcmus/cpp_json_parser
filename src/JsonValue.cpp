#include "JsonValue.h"
#include <sstream>
#include <cassert>
#include <numeric>

namespace json {

// helpers

inline static auto to_string(Type type) -> char const* {
    switch (type) {
    case Type::Null:    return "null";
    case Type::Boolean: return "boolean";
    case Type::Number:  return "number";
    case Type::String:  return "string";
    case Type::Array:   return "array";
    case Type::Object:  return "object";
    }
    assert(false);
    return "";
}

static auto cast_exception(Type from, Type to) -> JsonException {
    std::stringstream ss;
    ss << "Invalid cast from " << to_string(from) << " to " << to_string(to);
    return JsonException(ss.str());
}

static auto enclose(std::string str,
                    std::string beg,
                    std::string end) -> std::string {
    return std::move(beg) + std::move(str) + std::move(end);
}

template <typename... Ts>
struct overloaded : public Ts... { using Ts::operator()...; };

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// Json member functions

Json::Json(Type type)
    :type_(type) {
    switch (type_) {
    case Type::Null:    value_ = null_t{};    break;
    case Type::Boolean: value_ = boolean_t{}; break;
    case Type::Number:  value_ = number_t{};  break;
    case Type::String:  value_ = string_t{};  break;
    case Type::Array:   value_ = array_t{};   break;
    case Type::Object:  value_ = object_t{};  break;
    }
}

auto Json::is_null() const noexcept -> bool {
    return type_ == Type::Null;
}
auto Json::is_bool() const noexcept -> bool {
    return type_ == Type::Boolean;
}
auto Json::is_number() const noexcept -> bool {
    return type_ == Type::Number;
}
auto Json::is_string() const noexcept -> bool {
    return type_ == Type::String;
}
auto Json::is_array() const noexcept -> bool {
    return type_ == Type::Array;
}
auto Json::is_object() const noexcept -> bool {
    return type_ == Type::Object;
}

auto Json::type() const noexcept -> Type {
    return type_;
}

auto Json::boolean() const -> Json::boolean_t {
    if (not is_bool()) {
        throw cast_exception(type_, Type::Boolean);
    }
    return std::get<boolean_t>(value_);
}

auto Json::number() const -> Json::number_t {
    if (not is_number()) {
        throw cast_exception(type_, Type::Number);
    }
    return std::get<number_t>(value_);
}

auto Json::string() -> Json::string_t& {
    if (not is_string()) {
        throw cast_exception(type_, Type::String);
    }
    return std::get<string_t>(value_);
}

auto Json::string() const -> Json::string_t const& {
    if (not is_string()) {
        throw cast_exception(type_, Type::String);
    }
    return std::get<string_t>(value_);
}

auto Json::array() -> Json::array_t& {
    if (not is_array()) {
        throw cast_exception(type_, Type::Array);
    }
    return std::get<array_t>(value_);
}

auto Json::array() const -> Json::array_t const& {
    if (not is_array()) {
        throw cast_exception(type_, Type::Array);
    }
    return std::get<array_t>(value_);
}

auto Json::object() -> Json::object_t& {
    if (not is_object()) {
        throw cast_exception(type_, Type::Object);
    }
    return std::get<object_t>(value_);
}

auto Json::object() const -> Json::object_t const& {
    if (not is_object()) {
        throw cast_exception(type_, Type::Object);
    }
    return std::get<object_t>(value_);
}

auto Json::to_string() const -> std::string {
    auto separate_by_space = [](std::string a, std::string b) {
        return std::move(a) + ", " + std::move(b);
    };
    auto object_elem_ts = [](object_elem_t elem) {
        auto&& [key, value] = std::move(elem);
        return std::move(enclose(key, "\"", "\"")) + ": " + std::move(value.to_string());
    };

    auto null_ts    = [](null_t)            -> std::string { return "null"; };
    auto boolean_ts = [](boolean_t v)       -> std::string { return v ? "true" : "false"; };
    auto string_ts  = [](string_t const& v) -> std::string { return enclose(v, "\"", "\""); };
    auto number_ts  = [](number_t v)        -> std::string {
        std::stringstream ss;
        ss << v;
        return ss.str();
    };
    auto array_ts   = [separate_by_space](array_t const& v) -> std::string {
        auto s = std::transform_reduce(std::next(v.begin()), v.end(),
            (*v.begin()).to_string(),
            separate_by_space,
            [](Json const& j) { return j.to_string(); });
        return enclose(std::move(s), "[ ", " ]");
    };
    auto object_ts = [separate_by_space, object_elem_ts](object_t const& v) -> std::string {
        auto s = std::transform_reduce(std::next(v.begin()), v.end(),
            object_elem_ts(*v.begin()),
            separate_by_space,
            object_elem_ts);
        return enclose(std::move(s), "{ ", " }");
    };

    return std::visit(overloaded{
        null_ts,
        boolean_ts,
        number_ts,
        string_ts,
        array_ts,
        object_ts
    }, value_);
}

} // namespace json