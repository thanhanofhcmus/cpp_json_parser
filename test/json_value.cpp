#include <catch2/catch.hpp>
#include "json_parser/core.h"

TEST_CASE("Constructors", "[Json]") {
    using namespace json;
    SECTION("Default constructor") {
        CHECK( Json{}.type() == Type::Null );
    }

    SECTION("Type constructors") {
        CHECK( Json{ Type::Null    }.type() == Type::Null    );
        CHECK( Json{ Type::Boolean }.type() == Type::Boolean );
        CHECK( Json{ Type::Number  }.type() == Type::Number  );
        CHECK( Json{ Type::String  }.type() == Type::String  );
        CHECK( Json{ Type::Array   }.type() == Type::Array   );
        CHECK( Json{ Type::Object  }.type() == Type::Object  );
    }

    SECTION("Boolean constructor") {
        CHECK( Json{ true  }.type() == Type::Boolean );
        CHECK( Json{ false }.type() == Type::Boolean );
    }

    SECTION("Number constructor") {
        CHECK( Json{ 2.15 }.type() == Type::Number );
    }

    SECTION("String constructor") {
        CHECK( Json{ std::string("a string") }.type() == Type::String );
        CHECK( Json{ "another string there " }.type() == Type::String );
    }

    SECTION("Array constructor") {
        CHECK( Json{ { } }.type() == Type::Array );
        CHECK( Json{ { Json{}, Json{ 1.2 } } }.type() == Type::Array );
        CHECK( Json{ std::vector(12, Json{}) }.type() == Type::Array );
    }

    SECTION("Object constructor") {

    }
}
