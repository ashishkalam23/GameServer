#include "../Parser/RuleExecute.h"
#include <iostream>
#include <cassert>
// #include "../Parser/MockIOInterface.h"

GameState initwithListofMaps() {
    Map m, n, k;
    List l;
    Identifier list{"list"}, twoid{"two"};
    // l.push_back(two);
    // l.push_back(helloworld);
    m["num"] = Value{2};
    m["str"] = Value{"esi"};
    n["num"] = Value{3};
    n["str"] = Value{"Mohsen"};
    k["num"] = Value{4};
    k["str"] = Value{"Reza"};
    l.push_back(m);
    l.push_back(n);
    l.push_back(k);

    // std::cout<<two;
    // std::cout<<std::endl<<helloworld;
    // std::cout<<std::endl<<l;
    GameState gs;
    gs.setGameVariable("list", l);
    return gs;
} 

void testElementAgg()
{
    GameState gs = initwithListofMaps();
    // std::cout<<*(gs.getGameVariable("two").value());
    // IOInterface io;
    // Rule r;
    RuleExecuter re;

   
  //  RuleExecuter re(std::move(io), std::move(gameState), std::move(rule));
    


    BinaryExpression be, outer;
    Expression left, right, lrexp, exp, mainexpr;
    left.expr = Identifier{"list"};
    right.expr = Identifier{"elements"};
    be.left = &left;
    be.right = &right;
    be.op = ".";
    lrexp.expr = be;
    outer.left = &lrexp;
    exp.expr = Identifier{"str"};
    outer.op = ".";
    outer.right = &exp;
    mainexpr.expr = outer;
    std::visit(ExpressionVisitor{re, gs}, mainexpr.expr);
    // std::cout << *(re.lastValueExpr);
    List correctOutput;
    correctOutput.push_back(Value{"esi"});
    correctOutput.push_back(Value{"Mohsen"});
    correctOutput.push_back(Value{"Reza"});
    assert(std::get<List>(re.lastValueExpr->val) == correctOutput && "Element Agg is not working correctly");
}

void testEqualityOp() {
    GameState gs;
    RuleExecuter re;
   
  //  RuleExecuter re(std::move(io), std::move(gameState), std::move(rule));
    
    gs.setGameVariable("x",Value{4});
    gs.setGameVariable("y",Value{4});
    Expression left, right, mainexpr;
    BinaryExpression be;
    left.expr = Identifier{"x"};
    right.expr = Identifier{"y"};
    be.left = &left;
    be.right = &right;
    be.op="=";
    mainexpr.expr = be;
    std::visit(ExpressionVisitor{re, gs}, mainexpr.expr);
    bool correctOutput = true;
    assert(std::get<bool>(std::get<Literal>((re.lastValueExpr->val))) == correctOutput && "Equal x and y didn't equal!");
    std::visit(ExpressionVisitor{re, gs}, left.expr);
    *(re.lastValueExpr) = Value{5};
    std::visit(ExpressionVisitor{re, gs}, mainexpr.expr);
    correctOutput = false;
    assert(std::get<bool>(std::get<Literal>((re.lastValueExpr->val))) == correctOutput && "Equal x and y equal! (x=5, y=4)");
}

int main()
{
    testElementAgg();
    testEqualityOp();
    return 0;
}