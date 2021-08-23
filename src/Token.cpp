#include "Token.h"
#include <sstream>
#include <iomanip>

namespace json {

Token::Token(int _line, int _column, TokenType _type, literal_t _literal)
:
    line(_line),
    column(_column),
    type(_type),
    literal(std::move(_literal))
{
}

auto Token::to_string() const -> std::string {
    using TT = TokenType;
    using namespace std;

    std::stringstream ss;
    ss << '[' << line << ':' << column << "]";
    switch (type) {
    case TT::Null:         ss << "[Null: "         << quoted("null");  break;
    case TT::True:         ss << "[True: "         << quoted("true");  break;
    case TT::False:        ss << "[False: "        << quoted("false"); break;
    case TT::Colon:        ss << "[Colon: "        << quoted(":");     break;
    case TT::Comma:        ss << "[Comma: "        << quoted(",");     break;
    case TT::LeftBrace:    ss << "[LeftBrace: "    << quoted("{");     break;
    case TT::RightBrace:   ss << "[RightBrace: "   << quoted("}");     break;
    case TT::LeftBracket:  ss << "[LeftBracket: "  << quoted("[");     break;
    case TT::RightBracket: ss << "[RightBracket: " << quoted("]");     break;
    case TT::Eof:          ss << "[Eof: EOF";                          break;
    case TT::String:       ss << "[String: " << quoted(get<string>(literal)); break;
    case TT::Number:       ss << "[Number: " << get<double>(literal);         break;
    }
    ss << ']';
    return ss.str();
}

} // namespace json