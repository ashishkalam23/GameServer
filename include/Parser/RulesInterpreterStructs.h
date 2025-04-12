#pragma once

#include "PrimitiveTypes.h"


// /**
//  * @brief The runtime environment (State) for Social Gaming interpreter.
//  * Stores the variables, constants for each game
//  */
// struct LocalEnvironment
// {
//     void set(const string &name, const Value &value)
//     {
//         state[name] = value;
//     }

//     std::optional<Value> get(const string &name)
//     {
//         if (state.find(name) == state.end())
//         {
//             return std::nullopt;
//         }
//         return state[name];
//     }

// private:
//     unordered_map<string,Value&> state; // Maps names to their values
// };

struct Rule;
using Body = vector<Rule>;

struct For {
    using Mode = enum {SEQ, PAR}; // Sequential vs Parallel
    Identifier loopVar;
    unique_ptr<Expression> listExpr;
    unique_ptr<Body> body;
    Mode loopMode;
};

struct While {
    unique_ptr<Expression> cond;
    unique_ptr<Body> body;
};

struct InParallel {
    unique_ptr<Body> body;
};

struct MatchEntry {
    unique_ptr<Expression> guard;
    unique_ptr<Body> body;
};

struct Match {
    unique_ptr<Expression> target;
    vector<MatchEntry> cases;
};

struct Extend {
    unique_ptr<QualifiedIdentifier> target;
    unique_ptr<Expression> value;
};

struct Reverse {
    unique_ptr<QualifiedIdentifier> target;
};

struct Shuffle {
    unique_ptr<QualifiedIdentifier> target;
};

struct Sort {
    unique_ptr<QualifiedIdentifier> target;
    unique_ptr<QualifiedIdentifier> key;
};

using All = struct {};

using PlayerSet = variant<All,Expression>;

struct Deal {
    unique_ptr<PlayerSet> targets;
    unique_ptr<Expression> source, count;
};

struct Discard {
    unique_ptr<Expression> count;
    unique_ptr<QualifiedIdentifier> source;
};

struct Assignment {
    unique_ptr<Expression> value;
    unique_ptr<QualifiedIdentifier> target;
};

struct Timer {
    using Mode = enum {ATMOST, EXACTLY};
    unique_ptr<Expression> duration;
    Mode mode = ATMOST;
    optional<unique_ptr<QualifiedIdentifier>> flag;
    unique_ptr<Body> body;
};

struct InputChoice {
    unique_ptr<QualifiedIdentifier> player, target;
    unique_ptr<Expression> prompt, choices;
    optional<unique_ptr<Expression>> timeout;
};

struct InputText {
    unique_ptr<QualifiedIdentifier> player, target;
    unique_ptr<Expression> prompt;
    optional<unique_ptr<Expression>> timeout;
};

struct InputRange {
    unique_ptr<QualifiedIdentifier> player, target;
    unique_ptr<Expression> prompt;
    unique_ptr<NumberRange> range;
    optional<unique_ptr<Expression>> timeout;
};

struct InputVote {
    unique_ptr<QualifiedIdentifier> target;
    unique_ptr<PlayerSet> players;
    unique_ptr<Expression> prompt, choices;
    optional<unique_ptr<Expression>> timeout;
};

struct Message {
    unique_ptr<PlayerSet> players;
    unique_ptr<Expression> content;
};

struct Scores {
    unique_ptr<List> keys;
};

struct Rule {
    variant<Body,For,While,InParallel,Match,Extend,Reverse,Shuffle,Sort,Deal,
    Discard, Assignment, Timer, InputChoice, InputRange, InputText, InputVote,
    Message, Scores> rule;
};