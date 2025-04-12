#include <Parser/RuleParser.h>


RuleParser::RuleParser(std::vector<std::string> rules) : rulesData(rules){
    
}

//get the rule type based off the string
//change this to correspond to class/nodes of RuleParser later
RuleParser::RuleType RuleParser::getRuleType(std::string s){
    if(s == "control"){
        return RuleType::CONTROL;
    }
    else if(s == "list"){
        return RuleType::LIST_OPERATION;
    }
    else if(s == "timing"){
        return RuleType::TIMING;
    }
    else if(s == "input"){
        return RuleType::INPUT;
    }
    else if(s == "output"){
        return RuleType::OUTPUT;
    }
    else if ( s == "var_expression"){
        return RuleType::VAR_EXPRESSION;
    }
    return RuleType::UNKNOWN;
}


void RuleParser::executeTurn(){
    //execute all lines in the rules section
    for(auto& line :rulesData){
        executeLine(getRuleType(line), line);
    }
}

void RuleParser::executeLine(RuleType type, std::string line){
    switch(type){
        case RuleType::CONTROL:
            //execute line
            break;
        case RuleType::LIST_OPERATION:
            //execute line
            break;
        case RuleType::TIMING:
            //execute line
            break;
        case RuleType::INPUT:
            //execute line
            break;
        case RuleType::OUTPUT:
            //execute line
            break;
        case RuleType::VAR_EXPRESSION:
            //execute line
            break;
        case RuleType::UNKNOWN:
            std::cerr << "Unknown rule type: " << line << "\n";
            break;
    }
}

void RuleParser::addVariable(std::string name, std::any value){
    variables[name] = value;
}

std::unordered_map<std::string, std::any> RuleParser::getVariables(){
    return variables;
}

