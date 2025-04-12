#pragma once

#include "RulesInterpreterStructs.h"
#include "../State/GameState.h"
#include <iostream>
#include <fstream>
// #include "../CachupaServer.h"
// #include "../Support.h"

/**
 * @brief Utility visitor struct for printing different kinds of Values
 */
struct ValuePrinter
{
    void operator()(const Literal &l)
    {
        std::visit([](auto &&a)
                   { std::cout << a; }, l);
    }

    void operator()(const List &l)
    {
        std::cout << "[" << std::endl;
        for (auto &e : l)
        {
            std::visit(ValuePrinter{}, e.val);
            std::cout << std::endl;
        }
        std::cout << "]" << std::endl;
    }

    void operator()(const Map &m)
    {
        std::cout << "{" << std::endl;
        for (auto &[key, value] : m)
        {
            std::cout << "{ " << key << " : ";
            std::visit(ValuePrinter{}, value.val);
            std::cout << "}" << std::endl;
        }
        std::cout << "}" << std::endl;
    }
};

void operator<<(std::ostream &out, const Value &v)
{
    std::visit(ValuePrinter{}, v.val);
}

// TODO: Plug this into the acutal server i/o. Just a prototype
/**
 * @brief Interface for server i/o
 */
class IOInterface
{
public:
    virtual ~IOInterface() = default;

    // Input Methods
    virtual std::string readString() = 0;
    virtual int readInt() = 0;
    virtual bool readBool() = 0;

    // Output Methods
    virtual void writeString(const std::string &str) = 0;
    virtual void writeInt(int value) = 0;
    virtual void writeBool(bool value) = 0;
    virtual void writeLine(const std::string &str) = 0;
};

/**
 * @brief Interface for Console IO extending server
 */
class ConsoleIO : public IOInterface
{
public:
    std::string readString() override
    {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    int readInt() override
    {
        int input;
        std::cin >> input;
        return input;
    }

    bool readBool() override
    {
        bool input;
        std::cin >> input;
        return input;
    }

    void writeString(const std::string &str) override
    {
        std::cout << str;
    }

    void writeInt(int value) override
    {
        std::cout << value;
    }

    void writeBool(bool value) override
    {
        std::cout << (value ? "true" : "false");
    }

    void writeLine(const std::string &str) override
    {
        std::cout << str << std::endl;
    }
};
/**
 * @brief Interface for FileIO extending server
 */
class FileIO : public IOInterface
{
public:
    FileIO(const std::string &filename) : file(filename)
    {
        if (!file.is_open())
        {
            throw std::ios_base::failure("Failed to open file.");
        }
    }

    std::string readString() override
    {
        std::string input;
     //   std::getline(file, input);
        return input;
    }

    int readInt() override
    {
        int input;
      //  file >> input;
        return input;
    }

    bool readBool() override
    {
        bool input;
     //   file >> input;
        return input;
    }

    void writeString(const std::string &str) override
    {
        file << str;
    }

    void writeInt(int value) override
    {
        file << value;
    }

    void writeBool(bool value) override
    {
        file << (value ? "true" : "false");
    }

    void writeLine(const std::string &str) override
    {
        file << str << std::endl;
    }

private:
    std::ofstream file;
};

/**
 * @brief Mock interface extending server
 * for testing in state/main.cpp
 */
class MockIOInterface : public IOInterface {
public:
    MockIOInterface() = default;

    // Setters for behaviors (used in tests)
    void setReadStringBehavior(const std::string& returnValue) {
        readStringReturnValue = returnValue;
    }

    void setReadIntBehavior(int returnValue) {
        readIntReturnValue = returnValue;
    }

    void setWriteStringBehavior(const std::function<void(const std::string&)>& behavior) {
        writeStringBehavior = behavior;
    }

    void setWriteIntBehavior(const std::function<void(int)>& behavior) {
        writeIntBehavior = behavior;
    }

    void setReadBoolBehavior(bool returnValue) {
        readBoolReturnValue = returnValue;
    }

    void setWriteBoolBehavior(const std::function<void(bool)>& behavior) {
        writeBoolBehavior = behavior;
    }

    void setWriteLineBehavior(const std::function<void(const std::string&)>& behavior) {
        writeLineBehavior = behavior;
    }

    // Override methods to return predefined values or execute custom behaviors
    std::string readString() override {
        return readStringReturnValue;
    }

    int readInt() override {
        return readIntReturnValue;
    }

    void writeString(const std::string& str) override {
        if (writeStringBehavior) {
            writeStringBehavior(str);
        }
    }

    void writeInt(int value) override {
        if (writeIntBehavior) {
            writeIntBehavior(value);
        }
    }

    bool readBool() override {
        return readBoolReturnValue;
    }

    void writeBool(bool value) override {
        if (writeBoolBehavior) {
            writeBoolBehavior(value);
        }
    }

    void writeLine(const std::string& line) override {
        if (writeLineBehavior) {
            writeLineBehavior(line);
        }
    }

private:
    std::string readStringReturnValue = "Default String";
    int readIntReturnValue = 42;
    bool readBoolReturnValue = true;

    std::function<void(const std::string&)> writeStringBehavior = [](const std::string& str) {
        std::cout << "Writing string: " << str << std::endl;
    };
    std::function<void(int)> writeIntBehavior = [](int value) {
        std::cout << "Writing int: " << value << std::endl;
    };
    std::function<void(bool)> writeBoolBehavior = [](bool value) {
        std::cout << "Writing bool: " << value << std::endl;
    };
    std::function<void(const std::string&)> writeLineBehavior = [](const std::string& line) {
        std::cout << "Writing line: " << line << std::endl;
    };
};



/**
 * @brief Visitor struct that handles every different kind of Rule as defined in grammar.js
 */
struct RuleExecuter
{
    RuleExecuter() {}
   //  RuleExecuter(unique_ptr<IOInterface> io, unique_ptr<GameState> gs, unique_ptr<Rule> mainRule) :  gs(std::move(gs)), io(std::move(io)) {}

    struct RuleVisitor
    {
        void operator()(const Body &b)
        {
            for (const auto &r : b)
            {
                std::visit(RuleVisitor{}, r.rule);
            }
        }
        void operator()(const For &f)
        {
        }
        template <typename T>
        void operator()(const T &) const
        {
        }
        // TODO: Add handler for more Rules (while, discard, etc.)
    };

    void executeRule()
        {
            // Read an integer from the IO
            int input = io->readInt();
            // Write a result to the IO
            io->writeLine("Result: " + std::to_string(input * 2));
        }


    /**
     * @brief: a helper function that stores pointer to the lastest accessed global or players variable.
     * This will be used for assingment and other modifying operations.
     * @param: type_name is the the type of the Value we wish to store... "int", "List", "Map"
     */
    void setTempValAsLastExpr(Value *val, string type_name)
    {
        lastValueExpr = val;
        lastValType = type_name;
    }

    /**
     * @brief: a helper function that stores the result of the last evaluated expression.
     * It can store different kind of Values. Rn it is saving in the localVarsMap, so the user
     * can break the interpreter if they have a local variable with similar name as the one we
     * use in this function. e.g. lastint + 4
     * @param: type_name is the the type of the Value we wish to store... "int", "List", "Map"
     */
    // TODO: Fix this hacky way to avoid exploits
    template <class T>
    void setTempValAsLastExpr(const T &val, string type_name)
    {
        localVarsMap["last" + type_name].val = val;
        lastValueExpr = (Value *)&(localVarsMap["last" + type_name]);
        lastValType = type_name;
    }

    void debugState() const {
    std::cout << "Last Value Expr: ";
    if (lastValueExpr) {
        std::visit(ValuePrinter{}, lastValueExpr->val); // Corrected `std::visit`
    } else {
        std::cout << "nullptr";
    }
    std::cout << "\nLast Value Type: " << lastValType << "\n";
    std::cout << "Local Variables Map:\n";
    for (const auto &[key, value] : localVarsMap) {
        std::cout << key << " : ";
        std::visit(ValuePrinter{}, value.val);
        std::cout << "\n";
    }
}

    Value *lastValueExpr; // We need to have this pointer in case we wish to do an assignment.
    string lastValType;   // Will be used later to do type checking
    // QuotedString lastQuotedStringExpr;
    Map localVarsMap; // A map holding all the local including for loop vars

     unique_ptr<IOInterface> io;
     unique_ptr<GameState> gs;
};

struct SimpleExpressionVisitor
{
    SimpleExpressionVisitor(RuleExecuter &re, GameState &gs) : re(re), gs(gs) {}
    void operator()(const Identifier &i)
    {
        if (i.identName == "players")
        {
            re.setTempValAsLastExpr(gs.getPlayerVariablesList(), "list");
        }
        else
        {
            if (re.localVarsMap.count(i.identName) > 0)
                re.lastValueExpr = &re.localVarsMap[i.identName];
            else
            {
                auto globalLookup = gs.getGameVariable(i.identName);
                if (globalLookup.has_value())
                    re.lastValueExpr = globalLookup.value();
            }
        }
    }
    void operator()(const int &i)
    {
        re.setTempValAsLastExpr<int>(i, "int");
    }

    void operator()(const bool &b)
    {
        re.setTempValAsLastExpr<bool>(b, "bool");
    }

    void operator()(const QuotedString &str)
    {
        // re.setTempValAsLastExpr<QuotedString>(str, "qstring");
        // TODO: Handle Quoted String
    }

    void operator()(const List &list)
    {
        re.setTempValAsLastExpr<List>(list, "list");
    }

    void operator()(const Map &map)
    {
        re.setTempValAsLastExpr<Map>(map, "map");
    }
    RuleExecuter &re;
    GameState &gs;
};

struct ExpressionVisitor
{
    ExpressionVisitor(RuleExecuter &re, GameState &gs) : re(re), gs(gs) {}
    void operator()(const SimpleExpression &se)
    {
        std::visit(SimpleExpressionVisitor{re, gs}, se);
    }

    void operator()(const BinaryExpression &be)
    {
        if (be.op == ".")
        {
            std::visit(ExpressionVisitor{re, gs}, be.left->expr);
            auto ident = std::get<Identifier>(std::get<SimpleExpression>((be.right->expr)));
            if (ident.identName == "elements")
            {
                auto list = std::get<List>(re.lastValueExpr->val);
                Map res;
                List temp;
                for (auto &[key, value] : std::get<Map>(list[0].val))
                {
                    res[key] = Value{List{}}; // Initialize the list with appropriate keys
                }
                for (auto &e : list)
                {
                    for (auto &[key, value] : std::get<Map>(e.val))
                    {
                        std::get<List>(res[key].val).push_back(value);
                    }
                }
                re.setTempValAsLastExpr<Map>(res, "map");
            }
            else
            {
                re.lastValueExpr = (Value *)&(std::get<Map>(re.lastValueExpr->val).find(ident.identName))->second;
            }
            return;
        }
        if (be.op == "=")
        {
            std::visit(ExpressionVisitor{re, gs}, be.left->expr);
            Value v;
            v.val = re.lastValueExpr->val;
            std::visit(ExpressionVisitor{re, gs}, be.right->expr);
            if (v.val == re.lastValueExpr->val)
                re.setTempValAsLastExpr<bool>(true, "bool");
            else
                re.setTempValAsLastExpr<bool>(false, "bool");
        }
    }

    void operator()(const BuiltinExpression &be)
    {
        List res, tmp;
        switch (be.op)
        {
        case BuiltinExpression::SIZE:
            re.setTempValAsLastExpr<int>(std::get<List>(re.lastValueExpr->val).size(), "int");
            break;
        case BuiltinExpression::UPFROM:

            for (int i = std::get<int>(std::get<SimpleExpression>(be.args->expr)); i < std::get<int>(std::get<Literal>(re.lastValueExpr->val)); i++)
            {
                res.push_back(Value{i});
            }
            re.setTempValAsLastExpr<List>(res, "list");
            break;
        case BuiltinExpression::COLLECT:
            tmp = std::get<List>(re.lastValueExpr->val);
            for (auto &e : tmp)
            {
            }

            break;
        }
    }

    RuleExecuter &re;
    GameState &gs;
};