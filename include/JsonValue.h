#pragma once

#include <variant>
#include <vector>
#include <string>
#include <map>
#include "JsonException.h"

namespace json {

enum class Type {
    Null,
    Boolean,
    Number,
    String,
    Array,
    Object,
};

class Json {
public:

    struct NullValue { };
    using null_t        = NullValue;
    using boolean_t     = bool;
    using number_t      = double;
    using string_t      = std::string;
    using array_t       = std::vector<Json>;
    // Ideally we would want to use unordered_map but
    // most STL implementation of
    // unordered_map requires complete type and map does not
    using object_t      = std::map<std::string, Json>;
    using object_elem_t = typename object_t::value_type;

    using value_t       =
        std::variant<null_t, boolean_t, number_t, string_t, array_t, object_t>;

public:

    Json()
        : type_(Type::Null), value_(null_t{}) { }

    Json(Type type);

    Json(boolean_t boolean)
        : type_(Type::Boolean), value_(boolean) { }

    Json(number_t number)
        : type_(Type::Number), value_(number) { }

    Json(string_t string)
        : type_(Type::String), value_(std::move(string)) { }

    Json(array_t array)
        : type_(Type::Array), value_(std::move(array)) { }

    Json(object_t object)
        : type_(Type::Object), value_(std::move(object)) { }

    [[nodiscard]] auto is_null()   const noexcept -> bool;
    [[nodiscard]] auto is_bool()   const noexcept -> bool;
    [[nodiscard]] auto is_number() const noexcept -> bool;
    [[nodiscard]] auto is_string() const noexcept -> bool;
    [[nodiscard]] auto is_array()  const noexcept -> bool;
    [[nodiscard]] auto is_object() const noexcept -> bool;

    [[nodiscard]] auto type() const noexcept -> Type;

    [[nodiscard]] auto boolean() const -> boolean_t;
    [[nodiscard]] auto number() const -> number_t;
    
    [[nodiscard]] auto string() -> string_t&;
    [[nodiscard]] auto string() const -> string_t const&;

    [[nodiscard]] auto array() -> array_t&;
    [[nodiscard]] auto array() const -> array_t const&;

    [[nodiscard]] auto object() -> object_t&;
    [[nodiscard]] auto object() const -> object_t const&;

    [[nodiscard]] auto to_string() const -> std::string;

private:

    Type type_;
    value_t value_;
};

} // namespace json