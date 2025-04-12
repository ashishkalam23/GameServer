#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <variant>
#include <memory>
#include "GameState.h"

using std::vector, std::unordered_map, std::variant, std::string, std::shared_ptr, std::string_view;

/**
 * @brief The runtime environment (State) for Social Gaming interpreter.
 * Stores the variables, constants for each game
 */
struct LocalEnvironment
{
    void set(const string &name, const GameState::Value &value)
    {
        state[name] = value;
    }

    std::optional<GameState::Value> get(const string &name)
    {
        if (state.find(name) == state.end())
        {
            return std::nullopt;
        }
        return state[name];
    }

private:
    unordered_map<string,GameState::Value&> state; // Maps names to their values
};

/**
 * @brief Base AST Node
 */

struct ASTNode
{
};

// For input/output, list operations, control structures
struct StatementNode : public ASTNode
{
    virtual void exec(GameState &gs, LocalEnvironment &localEnv) = 0;
};

struct ExpressionNode : public ASTNode
{ // For variables, constants, literals
  // Base struct for expressions
    virtual GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) = 0;
};

struct IntNode : ExpressionNode
{
    int val;
};

struct BoolNode : ExpressionNode
{
    bool val;
};

struct QuotedStringNode : ExpressionNode
{
    string val;
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        // TODO: If there is expression in the string, calculated the final
        //                                  resulting string and return it.
    }
};

struct ListLiteralNode : ExpressionNode
{
    GameState::List val;
};

struct IdentifierNode : ExpressionNode
{
    string identName; // The name of the identifier. Will be used to look
                      // up its value.
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        if (localEnv.get(identName) == std::nullopt)
        { // First local vars then global
            return gs.getGameVariable(identName).value();
        }
        else
            return localEnv.get(identName).value();
    }

    GameState::Value& getVarReference() {

    }
};

struct QualifiedIdentifierNode : ExpressionNode
{
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        if (identifiers.size() == 1)
        {
            auto identName = identifiers[0];
            if (localEnv.get(identName) == std::nullopt)
            { // First local vars then global
                return gs.getGameVariable(identName).value();
            }
            else
                return localEnv.get(identName).value();
        }
    }
    void setValue(GameState &gs, LocalEnvironment &localEnv, GameState::Value val)
    {
    }
    vector<string> identifiers;
};

struct ValueMapNode : ExpressionNode
{
    GameState::Map val;
};

// Binary Expressions

struct BinaryExpressionNode : ExpressionNode
{
    using Operation = enum {
        OR,
        AND,
        EQ,
        INEQ,
        LT,
        LTE,
        GT,
        GTE,
        PLUS,
        MINUS,
        MULT,
        DIV,
        MOD
    };
    BinaryExpressionNode(shared_ptr<ExpressionNode> l, shared_ptr<ExpressionNode> r, Operation operation) : left(l), right(r), op(operation) {}
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        switch (op)
        {
        case OR:
            return GameState::Literal{std::get<bool>(left->eval(gs, localEnv).val) || std::get<bool>(right->eval(gs, localEnv).val)};
            break;
        case AND:
            return GameState::Literal{std::get<bool>(left->eval(gs, localEnv).val) && std::get<bool>(right->eval(gs, localEnv).val)};
            break;
        case EQ:
            return GameState::Literal{left->eval(gs, localEnv).val == right->eval(gs, localEnv).val};
            break;
        case INEQ:
            return GameState::Literal{left->eval(gs, localEnv).val != right->eval(gs, localEnv).val};
            break;
        case LT:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) < std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case LTE:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) <= std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case GT:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) > std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case GTE:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) >= std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case PLUS:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) + std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case MINUS:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) - std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case MULT:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) * std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case DIV:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) / std::get<int>(right->eval(gs, localEnv).val)};
            break;
        case MOD:
            return GameState::Literal{std::get<int>(left->eval(gs, localEnv).val) % std::get<int>(right->eval(gs, localEnv).val)};
            break;
        default:
            throw std::runtime_error("Unknown operation");
            break;
        }
    }

private:
    shared_ptr<ExpressionNode> left, right;
    Operation op;
};

struct LogicalNotExpressionNode : ExpressionNode
{
    LogicalNotExpressionNode(shared_ptr<ExpressionNode> expression) : expr(expression) {}
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        return GameState::Literal{!std::get<bool>(expr->eval(gs, localEnv).val)};
    }

private:
    shared_ptr<ExpressionNode> expr;
};

struct DotExpressionNode : ExpressionNode
{
    DotExpressionNode(const shared_ptr<ExpressionNode> &expression,
                      const shared_ptr<IdentifierNode> &id) : expr(expression), identifier(id) {}
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        GameState::List res;
        if (identifier->identName == "elements")
        {
            return expr->eval(gs, localEnv);
        }
        else
        {
            GameState::List list = std::get<GameState::List>(expr->eval(gs, localEnv).val);
            for (auto el : list)
            {
                res.push_back(GameState::Value{std::get<GameState::Map>(el.val)[identifier->identName]});
            }
        }
        return res;
    }
    shared_ptr<ExpressionNode> expr;
    shared_ptr<IdentifierNode> identifier;
};

struct IntegerUpFromNode : ExpressionNode
{
    IntegerUpFromNode(int x, shared_ptr<ExpressionNode> expr) : val(x),
                                                                expression(expr) {}
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        int high_lim = std::get<int>(expression->eval(gs, localEnv).val);
        GameState::List res;
        for (int i = val; i <= high_lim; i++)
        {
            res.push_back(GameState::Literal{i});
        }
        return res;
    }

private:
    int val;
    shared_ptr<ExpressionNode> expression;
};

struct ListSizeNode : ExpressionNode
{
    ListSizeNode(shared_ptr<ExpressionNode> expr) : listExpr(expr) {}
    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        GameState::List list = std::get<GameState::List>(listExpr->eval(gs, localEnv).val);
        return GameState::Literal{(int)list.size()};
    }

private:
    shared_ptr<ExpressionNode> listExpr;
};

struct ListContainsNode : ExpressionNode
{
    ListContainsNode(shared_ptr<ExpressionNode> expr,
                     const GameState::Value &el) : listExpr(expr), element(el) {}

    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        GameState::List list = std::get<GameState::List>(listExpr->eval(gs, localEnv).val);
        bool found = false;
        for (auto x : list)
        {
            if (x.val == element.val)
                found = true;
        }
        return GameState::Literal{found};
    }

private:
    shared_ptr<ExpressionNode> listExpr;
    GameState::Value element;
};

struct ListCollectNode : ExpressionNode
{
    ListCollectNode(shared_ptr<ExpressionNode> expr,
                    shared_ptr<IdentifierNode> el, shared_ptr<ExpressionNode> pred)
        : listExpr(expr), element(el), predicate(pred) {}

    GameState::Value eval(GameState &gs, LocalEnvironment &localEnv) override
    {
        GameState::List list = std::get<GameState::List>(listExpr->eval(gs, localEnv).val), res;
        for (auto x : list)
        {
            localEnv.set(element->identName, x);
            if (std::get<bool>(predicate->eval(gs, localEnv).val))
                res.push_back(x);
        }
        return GameState::List{res};
    }

private:
    shared_ptr<ExpressionNode> listExpr, predicate;
    shared_ptr<IdentifierNode> element;
};

// TODO: Add support for parallel loop
struct ForEachNode : StatementNode
{
    using Mode = enum {
        PARALLEL,
        SEQUENTIAL
    };

    ForEachNode(Mode m, shared_ptr<ExpressionNode> expr,
                shared_ptr<IdentifierNode> lvar, vector<shared_ptr<StatementNode>> lbody)
        : listExpr(expr), loopVar(lvar), loopBody(lbody), mode(m) {}
    void exec(GameState &gs, LocalEnvironment &localEnv) override
    {
        GameState::List list = std::get<GameState::List>(listExpr->eval(gs, localEnv).val);
        for (auto x : list)
        {
            localEnv.set(loopVar->identName, x);
            for (auto stmt : loopBody)
            {
                stmt->exec(gs, localEnv);
            }
        }
    }

private:
    shared_ptr<ExpressionNode> listExpr;
    shared_ptr<IdentifierNode> loopVar;
    vector<shared_ptr<StatementNode>> loopBody;
    Mode mode;
};

struct WhileLoopNode : StatementNode
{
    WhileLoopNode(shared_ptr<ExpressionNode> cond,
                  vector<shared_ptr<StatementNode>> lbody) : condition(cond), loopBody(lbody) {}
    void exec(GameState &gs, LocalEnvironment &localEnv) override
    {
        bool cond = std::get<bool>(condition->eval(gs, localEnv).val);
        while (cond)
        {
            for (auto stmt : loopBody)
            {
                stmt->exec(gs, localEnv);
            }
            cond = std::get<bool>(condition->eval(gs, localEnv).val);
        }
    }

private:
    shared_ptr<ExpressionNode> condition;
    vector<shared_ptr<StatementNode>> loopBody;
};

// TODO: Add support for in parallel block

struct MatchGuardsNode : StatementNode
{
    using MatchEntry = struct
    {
        shared_ptr<ExpressionNode> guard;
        vector<shared_ptr<StatementNode>> matchBody;
    };
    MatchGuardsNode(shared_ptr<ExpressionNode> targ,
                    const vector<MatchEntry> &mbody) : target(targ), cases(std::move(mbody)) {}
    void exec(GameState &gs, LocalEnvironment &localEnv) override
    {
        auto targetVal = target->eval(gs, localEnv);
        for (auto matchCase : cases)
        {
            if (matchCase.guard->eval(gs, localEnv).val == targetVal.val)
            {
                for (auto stmt : matchCase.matchBody)
                {
                    stmt->exec(gs, localEnv);
                }
            }
        }
    }

private:
    shared_ptr<ExpressionNode> target;
    vector<MatchEntry> cases;
};

// TODO: Handle the case where variable is initialized with empty value
// (no value is specified)
struct VariableNode : public ExpressionNode
{
public:
    VariableNode(const Value &value, string_view var_name) : val(value), name(var_name) {}
    Value val;
    string name; // Variable Name
};

struct ConstantNode : public ExpressionNode
{
public:
    ConstantNode(const Value &value, string_view const_name) : val(value),
                                                               name(const_name) {}
    const Value val;
    string name; // Constant Name
};

struct AssignmentNode : public StatementNode
{
    AssignmentNode(std::shared_ptr<QualifiedIdentifierNode> var,
                   std::shared_ptr<ExpressionNode> val)
        : variable(var), value(val) {}

    void exec(GameState &gs, LocalEnvironment &localEnv) override
    {
        gs.store(var->name, var->val); // Update the value in runtime state
    }

private:
    std::shared_ptr<QualifiedIdentifierNode> variable;
    std::shared_ptr<ExpressionNode> value;
}; // Assignment operation e.g. x <- [ "a", "literal", "list" ];

struct InputNode : public StatementNode
{
}; // Human Input operations

struct TimedNode : public StatementNode
{
}; // Timed block

// A list operation such as shuffle, reverse
struct ListOperationNode : public StatementNode
{
};

// A block holding statements used in Foreach, in parallel, match case
struct StatementBlockNode
{
public:
    vector<shared_ptr<StatementNode>> statements;
};

struct ForEachNode : public StatementNode
{
public:
    shared_ptr<VariableNode> var_node;
    shared_ptr<ExpressionNode> list_expr;
    shared_ptr<StatementBlockNode> statement_block;
};
