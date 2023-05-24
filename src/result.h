#pragma once

#include <variant>
#include <string>

template <typename T>
struct Ok { T value; };
struct Err { std::string message; };

template <typename T>
using Result = std::variant<Ok<T>, Err>;
