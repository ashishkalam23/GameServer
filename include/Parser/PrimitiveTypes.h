#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <memory>
#include <optional>

using std::vector, std::unordered_map, std::variant, std::string, std::unique_ptr, std::string_view, std::optional;

struct Value;

using Literal = std::variant<int, std::string, bool>;
using List = std::vector<Value>;
using Map = std::unordered_map<std::string, Value>;

struct Value
{
    Value() {}
    Value(const Literal &l)
    {
        val = l;
    }
    Value(const List &l)
    {
        val = l;
    }
    Value(const Map &m) {
        val = m;
    }
    bool operator==(const Value& v) const{
        return val==v.val;
    }
    std::variant<Literal, List, Map> val;
};

struct NumberRange
{
    int low, high;
};

struct QuotedString
{
    string val;
};

struct Identifier
{
    string identName; // The name of the identifier. Will be used to look
                      // up its value.
};

struct QualifiedIdentifier
{
    vector<string> identifiers;
};

struct Expression;

using SimpleExpression = variant<int, bool, QuotedString, List, Map, Identifier>;
struct BinaryExpression
{
    Expression* left, *right;
    string op;
};

struct BuiltinExpression
{
    using BuiltinOp = enum { UPFROM,
                             SIZE,
                             CONTAINS,
                             COLLECT };
    Expression* expr;
    BuiltinOp op;
    Expression* args;
};

struct Expression
{
    variant<SimpleExpression, BinaryExpression, BuiltinExpression> expr;
};