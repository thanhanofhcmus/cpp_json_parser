#include "json_parser/detail/Scanner.h"
#include "json_parser/JsonException.h"
#include <cctype>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <charconv>

namespace json  {

Scanner::Scanner(std::string const& source)
:
    tokens_(std::vector<Token>{}),
    source_(source),
    start_(0),
    start_line_(0),
    start_column_(0),
    current_(0),
    current_line_(1),
    current_column_(1)
{
}

auto Scanner::scan() && -> std::vector<Token> {
    while (is_not_end()) {
        start_ = current_;
        scan_token();
    }
    add_token(TokenType::Eof);

    return std::move(tokens_);
}

void Scanner::scan_token() {
    update_start_position();
    char const c = advance();
    switch (c) {

    case '{': add_token(TokenType::LeftBrace);    break;
    case '}': add_token(TokenType::RightBrace);   break;
    case '[': add_token(TokenType::LeftBracket);  break;
    case ']': add_token(TokenType::RightBracket); break;
    case ',': add_token(TokenType::Comma);        break;
    case ':': add_token(TokenType::Colon);        break;
    case '"':           scan_string();            break;
    case '+': case '-': scan_number();            break;
    //  DO NOTHING cause advance() takes care of increasing current_line_, current_column_;
    case ' ': case '\n': case '\r': case '\t':    break;
    default: {
        if      (is_on_digit(c)) { scan_number(); }
        else if (isalpha(c))     { scan_identifier(); }
        else    { error(std::string("\"") + c + "\" is an invalid character"); }
        break;
    }

    }
}

void Scanner::scan_number() {
    while (is_on_digit(peek())) {
        advance();
    }

    auto const sv = source_.substr(start_, current_ - start_);
    double num = 0;
    auto const [_, err] = std::from_chars(sv.data(), sv.data() + sv.size(), num);
    if (err == std::errc::invalid_argument) {
        error("Cannot convert " + std::string(sv) + " to number");
    }
    add_token(TokenType::Number, num);
}

void Scanner::scan_string() {
    std::stringstream ss;
    while (is_not_end()) {
        char const c = advance();
        if      (c == '"')  { break; }
        else if (c == '\\') { ss << scan_escape_sequence(); }
        else                { ss << c; }
    }

    // dirty fix since is_not_end() will yield false now if the last character of source_ is '"'
    if (current_ > source_.length()) {
        error("Unterminated string");
    }
    add_token(TokenType::String, ss.str());
}

auto Scanner::scan_escape_sequence() -> char {
    char const c = advance();

    switch (c) {

    case '\\': return '\\';
    case '"':  return '"';
    case '/':  return '/';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'n':  return '\n';
    case 'r':  return '\r';
    case 't':  return '\t';
    case 'u':  return scan_hex_digit();
    default: {
        update_start_position();
        error(std::string("\\") + c + " is an invalid escape character");
        break;
    }

    }
    return '\0';
}

auto Scanner::scan_hex_digit() -> char {
    auto const factors = { 12u, 8u, 4u, 0u };
    unsigned int hex_value = 0;
    for (auto const factor : factors) {
        auto const c = advance();
        if      ('0' <= c && c <= '9') { hex_value += static_cast<unsigned int>(c - '0') << factor; }
        else if ('a' <= c && c <= 'f') { hex_value += static_cast<unsigned int>(c - 'a') << factor; }
        else if ('A' <= c && c <= 'F') { hex_value += static_cast<unsigned int>(c - 'A') << factor; }
        else {
            std::stringstream ss;
            update_start_position();
            ss << "Escape hex-code character can only be in [0, 9] or [a, z] or [A. Z], found ";
            ss << '"' << c << '"';
            error(ss.str());
        }
    }

    assert(0x000 <= hex_value && hex_value <= 0xFFFF);
    if(0x000 <= hex_value && hex_value <= 0xFFFF) {
        error("Escape hex-code value can only in range [0, 2^16-1(65535)], computed " + std::to_string(hex_value));
    }
    return static_cast<char>(hex_value);
}

void Scanner::scan_identifier() {
    while (isalpha(peek())) {
        advance();
    }

    auto const iden = source_.substr(start_, current_ - start_);

    if      (iden == "null")  { add_token(TokenType::Null); }
    else if (iden == "true")  { add_token(TokenType::True); }
    else if (iden == "false") { add_token(TokenType::False); }
    else                      { error('"' + std::string(iden) + "\" is an invalid literal"); }
}

auto Scanner::is_on_digit(char const c) const -> bool {
    return isdigit(c) || tolower(c) == 'e' ||
           c == '+' || c == '-' || c == '.';
}

auto Scanner::is_not_end() const -> bool {
    return current_ < source_.length();
}

auto Scanner::advance() -> char {
    char const c = source_[current_++];
    if (c == '\n') {
        ++current_line_;
        current_column_ = 1;
    } else {
        ++current_column_;
    }
    return c;
}

auto Scanner::peek() const -> char {
    return source_[current_];
}

void Scanner::update_start_position() {
    start_column_ = current_column_;
    start_line_ = current_line_;
}

void Scanner::add_token(TokenType type, Token::literal_t literal) {
    tokens_.emplace_back(start_line_, start_column_, type, std::move(literal));
}

void Scanner::error(std::string const& message) const {
    std::stringstream ss;
    ss << "Scan error at "
       << '[' << start_line_ << ':' << start_column_ << "]: " << message;

    throw JsonException(ss.str());
}

} // namespace json
