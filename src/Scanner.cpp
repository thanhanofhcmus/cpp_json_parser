#include "Scanner.h"
#include "JsonException.h"
#include <cctype>
#include <cassert>
#include <sstream>

namespace json  {

Scanner::Scanner(std::string const& source)
:
    tokens_(std::vector<Token>{}),
    source_(std::move(source)),
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
    start_line_ = current_line_;
    start_column_ = current_column_;
    char c = advance();
    switch (c) {

    case '{': add_token(TokenType::LeftBrace);    break;
    case '}': add_token(TokenType::RightBrace);   break;
    case '[': add_token(TokenType::LeftBracket);  break;
    case ']': add_token(TokenType::RightBracket); break;
    case ',': add_token(TokenType::Comma);        break;
    case ':': add_token(TokenType::Colon);        break;
    case '"':           scan_string();            break;
    case '+': case '-': scan_number();            break;
    //  DO NOTHING cause advance() takes care of increasing lines, column;
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

    auto s = source_.substr(start_, current_ - start_);
    try {
        // TODO: checking for invalid number
        double literal = std::stod(s);
        add_token(TokenType::Number, literal);
    } catch (std::invalid_argument& ignored) {
        error("Cannot covert " + s + " to number");
    }
}

// TODO: parsing escape sequence, hex digit
void Scanner::scan_string() {
    while (is_not_end() && peek() != '"') {
        advance();
    }
    if (not is_not_end()) {
        error("Unterminated string");
    }
    auto literal = source_.substr(start_ + 1, current_ - start_  - 1);
    advance();
    add_token(TokenType::String, literal);
}

void Scanner::scan_identifier() {
    while (isalpha(peek())) {
        advance();
    }

    auto iden = source_.substr(start_, current_ - start_);

    if      (iden == "null")  { add_token(TokenType::Null); }
    else if (iden == "true")  { add_token(TokenType::True); }
    else if (iden == "false") { add_token(TokenType::False); }
    else                      { error('"' + iden + "\" is an invalid literal"); }
}

auto Scanner::is_on_digit(char c) const -> bool {
    return isdigit(c) || tolower(c) == 'e' ||
           c == '+' || c == '-' || c == '.';
}

auto Scanner::is_not_end() const -> bool {
    return current_ < source_.length();
}

auto Scanner::advance() -> char {
    char c = source_[current_++];
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
