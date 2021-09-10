#pragma once

#include <stdexcept>

namespace json {

class JsonException : public std::exception {

public:

    JsonException(std::string message)
        : msg(std::move(message)) { }
    ~JsonException() override { }

    const char* what() const noexcept override {
        return msg.c_str();
    }

private:

    std::string msg;
};

} // namespace json