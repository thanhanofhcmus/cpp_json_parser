#include "Parser.h"
#include <algorithm>
#include <cassert>

namespace json {

using TT = TokenType;

static inline void error(std::string const& message, Token const& token) {
    throw JsonException("Parse error at " + token.to_string() + ": " + message);
}

Parser::Parser(std::vector<Token> tokens)
:
    source_(std::move(tokens)),
    current_(0)
{
}

auto Parser::parse() -> Json {
    if (not is_not_end()) { error("Empty string", peek()); }
    auto res = parse_element();
    if (is_not_end()) { error("Unexpected token after parsing element", peek()); }
    return res;
}

auto Parser::parse_element() -> Json {
    if      (match(TT::LeftBracket)) { return parse_array(); }
    else if (match(TT::LeftBrace))   { return parse_object(); }
    else                             { return parse_literal(); }
}

auto Parser::parse_literal() -> Json {
    auto token = advance();
    switch (token.type) {
    case TT::Null:   return Json{};
    case TT::True:   return Json{true};
    case TT::False:  return Json{false};
    case TT::Number: return Json{std::get<Json::number_t>(token.literal)};
    case TT::String: return Json{std::get<Json::string_t>(token.literal)};
    default: error("Invalid literal", previous()); return Json{};
    }
    assert(false);
    return Json{};
}

auto Parser::parse_array() -> Json {
    auto array = Json::array_t{};

    if (match(TT::RightBracket)) { return array; }

    array.push_back(parse_element());

    while (is_not_end() && not check(TT::RightBracket)) {
        consume(TT::Comma, "Expected comma \",\" after element in array");
        array.push_back(parse_element());
    }
    consume(TT::RightBracket, "Need right bracket \"]\" to terminate an array");

    return array;
}

auto Parser::parse_object() -> Json {
    auto object = Json::object_t{};

    if (match(TT::RightBrace)) { return object; }

    insert_object_elem(object);

    while (is_not_end() && not check(TT::RightBrace)) {
        consume(TT::Comma, "Expected comma \",\" after element in object");
        insert_object_elem(object);
    }
    consume(TT::RightBrace, "Need right brace \"}\" to terminate an object");

    return object;
}

auto Parser::parse_object_elem() -> ObjectElemParsingResult {
    auto const& key_token = consume(TT::String, "Key of object element must be a string");
    auto key = std::get<Json::string_t>(key_token.literal);
    consume(TT::Colon, "Object must have a colon \":\" to separate a key-value pair");
    auto value = parse_element();

    return { Json::object_elem_t{ key, value }, key_token };
}

void Parser::insert_object_elem(Json::object_t& object) {
    auto [kv, key_token] = parse_object_elem();
    auto [key, value] = kv;
    if (not object.insert(std::make_pair(key, std::move(value))).second) {
        error("Key \"" + key + "\" already exist", key_token);
    }
}

auto Parser::is_not_end() const noexcept -> bool {
    return source_[current_].type != TT::Eof;
}

auto Parser::match(TokenType expected) -> bool {
    if (peek().type == expected) {
        advance();
        return true;
    } else {
        return false;
    }
}

auto Parser::match(std::initializer_list<TokenType> list) -> bool {
    return std::any_of(list.begin(), list.end(),
        [this](TokenType type) { return match(type); });
}

auto Parser::check(TokenType type) const -> bool {
    return peek().type == type;
}

auto Parser::advance() -> Token& {
    if (is_not_end()) { ++ current_; }
    return source_[current_ - 1];
}

auto Parser::peek() const -> const Token& {
    return source_[current_];
}

auto Parser::previous() const -> const Token& {
    return source_[current_ - 1];
}

auto Parser::consume(TokenType type, std::string const& message) -> Token& {
    if (check(type)) {
        return advance();
    }
    error(message, peek());
    assert(false);
}

} // namespace json