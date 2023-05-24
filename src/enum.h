#pragma once

#include <variant>

#include "util/map.h"

#define NK_ENUM_CREATE_STRUCT(Name) struct Name;
#define NK_ENUM_CREATE_STRUCTS(...) MAP(NK_ENUM_CREATE_STRUCT, __VA_ARGS__)

#define NK_ENUM_EQ(Name) bool operator==(const Name&) const { return true; }
#define NK_ENUM_VALUE_EQ(Name, Type) \
    Type value; \
    bool operator==(const Name& rhs) const { return value == rhs.value; } \

#define NK_ENUM_SET_NAME(Name) Name{ NK_ENUM_EQ(Name) }
#define NK_ENUM_SET_VALUE(Name, Type) Name{ NK_ENUM_VALUE_EQ(Name, Type) }

#define NK_ENUM_HELPER_3_MACRO(_1, _2, X, ...) X

#define NK_ENUM_SET_IMPL(...) NK_ENUM_HELPER_3_MACRO(__VA_ARGS__, NK_ENUM_SET_VALUE, NK_ENUM_SET_NAME, 0)(__VA_ARGS__)
#define NK_ENUM_SET(X) NK_ENUM_SET_IMPL X

#define NK_ENUM_GET_NAME_IMPL(Name, ...) Name
#define NK_ENUM_GET_NAME(X) NK_ENUM_GET_NAME_IMPL X

#define NK_ENUM_GET_NAMES(...) MAP_LIST(NK_ENUM_GET_NAME, __VA_ARGS__)

#define NK_ENUM_CONSTRUCTOR(Type, Name) Name(Type value) : value(value) {}

#define create_enum(Name, ...) \
struct Name { \
private: \
    struct VariantEqualVisitor { \
        template <typename T> \
        bool operator()(const T &lhs, const T &rhs) const { \
            return lhs == rhs; \
        } \
        template <typename T, typename U> \
        bool operator()(const T &lhs, const U &rhs) const { \
            return false; \
        } \
    }; \
    template <typename... Types> \
    bool are_variants_equal(const std::variant<Types...> &lhs, const std::variant<Types...> &rhs) const { \
        if (lhs.index() != rhs.index()) { \
            return false; \
        }\
        return std::visit(VariantEqualVisitor{}, lhs, rhs); \
    } \
public: \
    NK_ENUM_CREATE_STRUCTS(MAP_LIST(NK_ENUM_SET, __VA_ARGS__)) \
    using Name##Type = std::variant<NK_ENUM_GET_NAMES(__VA_ARGS__)>; \
    MAP_ARGUMENTS(NK_ENUM_CONSTRUCTOR, (Name), NK_ENUM_GET_NAMES(__VA_ARGS__)) \
    bool operator==(const Token& rhs) { return are_variants_equal(value, rhs.value); } \
private: \
    Name##Type value; \
}\

// NOTE: Example of code constructed by the macro.

// struct Token {
// private:
//     // Visitor for comparing two variant instances
//     struct VariantEqualVisitor {
//         template<typename T>
//         bool operator()(const T &lhs, const T &rhs) const {
//             return lhs == rhs;
//         }
//
//         template<typename T, typename U>
//         bool operator()(const T&, const U&) const {
//             return false;
//         }
//     };
//
//     // Function to check if two variants are equal
//     template<typename... Types>
//     bool are_variants_equal(const std::variant<Types...> &lhs, const std::variant<Types...> &rhs) {
//         if (lhs.index() != rhs.index()) {
//             return false;
//         }
//
//         VariantEqualVisitor visitor;
//         return std::visit(visitor, lhs, rhs);
//     }
//
// public:
//     struct Illegal{
//         bool operator==(const Illegal&) const { return true; }
//     };
//     struct Eof{
//         bool operator==(const Eof& rhs) const { return true; }
//     };
//     struct Identifier{
//         std::string value;
//         bool operator==(const Identifier& rhs) const { return value == rhs.value; }
//     };
//     struct Number{
//         std::string value;
//         bool operator==(const Number& rhs) const { return value == rhs.value; }
//     };
//     struct Equal{
//         bool operator==(const Equal&) const { return true; }
//     };
//     struct Plus{
//         bool operator==(const Plus&) const { return true; }
//     };
//     struct Comma{
//     };
//     struct Semicolon{
//         bool operator==(const Semicolon&) const { return true; }
//     };
//     struct LParen{
//         bool operator==(const LParen&) const { return true; }
//     };
//     struct RParen{
//         bool operator==(const RParen&) const { return true; }
//     };
//     struct LBrace{
//         bool operator==(const LBrace&) const { return true; }
//     };
//     struct RBrace{
//         bool operator==(const RBrace&) const { return true; }
//     };
//     struct Function{
//         bool operator==(const Function&) const { return true; }
//     };
//
//     using TokenType = std::variant<
//         Illegal,
//         Eof,
//         Identifier,
//         Number,
//         Equal,
//         Plus,
//         Comma,
//         Semicolon,
//         LParen,
//         RParen,
//         LBrace,
//         RBrace,
//         Function
//     >;
//
//     Token(Illegal illegal) : value(illegal) {}
//     Token(Eof eof) : value(eof) {}
//     Token(Identifier id) : value(id) {}
//     Token(Number num) : value(num) {}
//     Token(Equal eq) : value(eq) {}
//     Token(Plus plus) : value(plus) {}
//     Token(Comma comma) : value(comma) {}
//     Token(Semicolon semicolon) : value(semicolon) {}
//     Token(LParen lparen) : value(lparen) {}
//     Token(RParen rparen) : value(rparen) {}
//     Token(LBrace lbrace) : value(lbrace) {}
//     Token(RBrace rbrace) : value(rbrace) {}
//     Token(Function func) : value(func) {}
//
//     bool operator==(const Token& rhs) { return are_variants_equal(value, rhs.value); }
//
// private:
//     TokenType value;
// };
