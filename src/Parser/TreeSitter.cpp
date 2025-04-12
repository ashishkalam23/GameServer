#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <map>

#include <cpp-tree-sitter.h>
#include <Parser/TreeSitter.h>

#include <fstream>
#include <iostream>

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

TreeSitter::TreeSitter(std::string filename)
    : language(tree_sitter_socialgaming()), parser(language), filename(filename) {
      // initalize();

}
/*
  opens the file and reads it into a string
  sets root to the root node of the tree
*/
void TreeSitter::initalize(){
  // Load the tree-sitter socialgaming language.
  fileToString(filename);
  ts::Tree tree = parser.parseString(sourceString);
  // Get the root node of the syntax tree. 
  ts::Node root = tree.getRootNode();
  parseConfig(root);
  createConfig();
  parseState(root);
}

/**
  * @brief Reads the file into a string
 */
void TreeSitter::fileToString(std::string filename){

  std::ifstream file(filename);
  if (!file) {
    fprintf(stderr, "Error opening file!\n");
    return; // Exit if file can't be opened
  }

  // Read the entire file into a string
  std::string sourcecode((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
  sourceString = sourcecode;
}

/*
  * @brief Parses the configuration block of the file
  * @param root The root node of the syntax tree
*/
void TreeSitter::parseConfig(ts::Node root){
  
  //these 3 are in all .game files
  ts::Node config = root.getChildByFieldName("configuration");
  ts::Node name = config.getChildByFieldName("name");
  ts::Node playerRange = config.getChildByFieldName("player_range");
  ts::Node audience = config.getChildByFieldName("has_audience");

  //parse setup 
  parseSetup(config);

  
  data["name"] = getStringText(name);
  data["player_range"] = getStringText(playerRange);
  data["has_audience"] = getStringText(audience);
}

/*
  * @brief Creates the configuration object with the 3 neccessary fields
  will refactor out later
*/
void TreeSitter::createConfig(){
  // Config config;
  config.name = data["name"];
  config.playerRange = parsePlayerRange(data["player_range"]);
  config.hasAudience = stringToBool(data["has_audience"]);


  // return config;
}

/*
  * @brief Parses the player range from the configuration block
  * @param playerRange The string containing the player range
  * @return A pair of integers representing the player range
*/
std::pair<int,int> TreeSitter::parsePlayerRange(std::string playerRange){
  size_t commaPos = playerRange.find(',');
    if (commaPos == std::string::npos) {
        throw std::invalid_argument("Invalid format: missing comma");
    }
  
    std::string firstPart = playerRange.substr(1, commaPos - 1); // From 1 to the character before the comma
    std::string secondPart = playerRange.substr(commaPos + 1, playerRange.size() - commaPos - 2); // After comma to before ')'
    return std::make_pair(std::stoi(firstPart), std::stoi(secondPart));
}

/*
  * @brief converts string to bool 
  * @param hasAudience The string containing the has audience field
  * @return A boolean representing if the game has an audience
*/
bool TreeSitter::stringToBool(std::string hasAudience){
  if (hasAudience == "true") {
        return true;
    } else {
        return false;
    }
}

/*
  * @brief Parses the setup block of the configuration
  * @param config The configuration block node
*/
void TreeSitter::parseSetup(ts::Node config){

  for (uint32_t i = 0; i < config.getNumNamedChildren(); ++i) {
    ts::Node child = config.getNamedChild(i);
    if (child.getType() == "setup_rule") { // Check the type of the node
        ts::Node setupNode = child;
        createSetting(setupNode);
        // std::cout << getStringText(setupNode) << std::endl;
        // std::cout << getStringText(setupNode.getChildByFieldName("name"));
        
    }
  }
}

/*
  * @brief Creates a SetupData object from a setup node
  * @param setting The setup node
*/
void TreeSitter::createSetting(ts::Node setting){
  SetupData setupData;
  ts::Node data = setting.getChildByFieldName("name");
  setupData.name = getStringText(data);
  setupData.kind = getStringText(setting.getChildByFieldName("kind"));
  setupData.prompt = getStringText(setting.getChildByFieldName("prompt"));

  if (!setting.getChildByFieldName("range").isNull()) {
    std::string rangeString = getStringText(setting.getChildByFieldName("range"));
    setupData.range = parsePlayerRange(rangeString);
    // std::cout << "Range: " << setupData.range->first << "," << setupData.range->second << std::endl;
  } else {
    setupData.range = std::nullopt;
    // std::cout << "Range: None" << std::endl;
  }


  // std::cout << setupData.name << "\t" << setupData.kind << "\t" << setupData.prompt << std::endl;
  
  // std::cout << getStringText(setting.getChildByFieldName("choices")) << "\n";

  if (!setting.getChildByFieldName("choices").isNull()) {
    // std::cout << "has choices: " << getStringText(setting.getChildByFieldName("choices")) << setting.getChildByFieldName("choices").getType() << std::endl;
    ts::Node choices = setting.getChildByFieldName("choices");
    std::map<std::string, std::string> choiceMap;
    //     ts::Node choice = setting.getNamedChild(i);
    //     std::cout << "Choice " << i << ": " << getStringText(choice) << "\t" << choice.getType() << "\n";      // std::cout << getStringText(choice) << std::endl;

    //     if (choice.getType() == "enum_description") {
    //       std::cout << "map" << std::endl;
    //     }
    // for (uint32_t i = 0; i < setting.getNumChildren(); ++i) {
    //   ts::Node c = setting.getChild(i);
    //   std::cout << getStringText(c) << "\t" << c.getType() << std::endl;
      // if (setting.getChild(i).getType() == "enum_description" || !setting.getChild(i).isNamed()) {
      //     continue;
      // }
      // std::cout << getStringText(c.getChildByFieldName("name")) << "\t" << getStringText(c.getChildByFieldName("description")) << std::endl;
    }
      // std::string_view choiceType = choice.getType();
      // std::string choiceValue = getStringText(choice);
      // choiceMap[std::string(choiceType)] = std::string(choiceValue);

            // Extract the "name" and "description" fields
            // ts::Node nameNode = choice.getChildByFieldName("name");
            // ts::Node descriptionNode = choice.getChildByFieldName("description");

            //     std::string name = getStringText(nameNode);
            //     std::string description = getStringText(descriptionNode);
            //     std::cout << "Name: " << name << "\tDescription: " << description << std::endl;
            //     // Add the choice to the map
            //     choiceMap[name] = description;

    
  //   setupData.choices = choiceMap;
  //   for (const auto& [key, value] : *setupData.choices) {
  //           std::cout << key << ": " << value << "\n";
  //   }
  // } 
  // else {
  //   std::cout << "No choices" << std::endl;
  //   setupData.choices = std::nullopt;
  // }

  //sets default value depending if the setup object has one
  if (!setting.getChildByFieldName("default").isNull()) {
    ts::Node defaultValue = setting.getChildByFieldName("default");
    setupData.defaultValue = getStringText(defaultValue);
    // std::cout << getStringText(defaultValue) << std::endl;
  } else {
  setupData.defaultValue = std::nullopt;
  }
  config.settings.push_back(setupData);
}

/*
  * @brief Prints the configuration object
*/
void TreeSitter::printConfig(){
  // Config config = createConfig();
  std::cout << "Name: " << config.name << std::endl;
  std::cout << "Player Range: " << config.playerRange.first << ", " << config.playerRange.second << std::endl;
  std::cout << "Has Audience: " << config.hasAudience << std::endl;

  for(auto setting : config.settings){
    std::cout << "Setting Name: " << setting.name << std::endl;
    std::cout << "Setting Kind: " << setting.kind << std::endl;
    std::cout << "Setting Prompt: " << setting.prompt << std::endl;
    if(setting.range.has_value()){
      std::cout << "Setting Range: " << setting.range->first << ", " << setting.range->second << std::endl;
    }
    // if(setting.choices.has_value()){
    //   for(auto choice : setting.choices){
    //     std::cout << "Choice Name: " << choice.first << std::endl;
    //     std::cout << "Choice Description: " << choice.second << std::endl;
    //   }
    // }
    if(setting.defaultValue.has_value()){
    std::visit([](const auto& value) {
            std::cout << "Default value: " << value << std::endl;
        }, setting.defaultValue.value());    }
  }
}


/*
  * @brief Returns the text of a node
  * @param node The node to get the text from
  * @return The text of the node
*/
std::string TreeSitter::getStringText(ts::Node node) {
    auto byteInfo = node.getByteRange();
    uint32_t start_range = byteInfo.start;
    uint32_t end_range = byteInfo.end;
    return sourceString.substr(start_range, end_range - start_range);
}


/*
  * @brief Returns the configuration object
  * @return The configuration object
*/
Config TreeSitter::getConfig(){
  return config;
}

std::string TreeSitter::getSourceString(){
  return sourceString;
}

/**
  * @brief converts the node into its value with given type (number, string, bool)
 */
Literal TreeSitter::typeConversion(ts::Node valueNode){
  std::string_view type = valueNode.getNamedChild(0).getType();
  std::string text = getStringText(valueNode);
    if (type == "number") {
        return {std::stoi(text)}; // Convert to integer
    } else if (type == "quoted_string") {
        return {text}; // Treat as string
    } else if (type == "bool_literal") {
        return {stringToBool(text)}; // Convert to boolean
    }

    // std::cout << "default to string: " << text << std::endl;
    return {text}; // Treat as string
}


/**
  * @brief handles the different types of state that can be stored in the game
  * needs to be refactored to be more readable and maintainable
 */
void TreeSitter::addState(std::string section, std::string key,ts::Node stateNode){
  std::string_view type = stateNode.getNamedChild(0).getType();

  //create function to handle this
  if (type == "list_literal") {
        List list;
        ts::Node elements = stateNode.getNamedChild(0).getChildByFieldName("elements");
        if (elements.isNull()) {
            // std::cout << "No elements in list" << std::endl;
            state.addData(section, key, Value{list});
            return;
        }
        for (uint32_t i = 0; i < elements.getNumNamedChildren(); ++i) {
            ts::Node item = elements.getNamedChild(i);
            // std::cout << getStringText(item) << item.getNamedChild(0).getType() << std::endl;

            //fix this (parse map for tictactoe example)
            if(item.getNamedChild(0).getType() == "value_map"){
              Map map = handleMap(item.getNamedChild(0));
              // for(auto& [key,val] : map){
              // std::cout << key << ": ";
              // std::visit(Printer{},val);
              // }
            list.push_back(Value{map});
            } else {
            list.push_back(Value{typeConversion(item)}); //should only be done if its literal value 
            }
        }
        state.addData(section, key, Value{list});

    //create function to handle this 
    } else if (type == "value_map") {
        // std::cout << "map in addState" << std::endl;
        Map map;
        // std::cout << getStringText(stateNode.getNamedChild(0)) << std::endl;
        ts::Node mapNode = stateNode.getNamedChild(0);
        map = handleMap(mapNode);
        // for (uint32_t i = 0; i < stateNode.getNamedChild(0).getNumNamedChildren(); ++i) {
        //   std::cout << getStringText(mapNode.getNamedChild(i)) << mapNode.getNamedChild(i).getType() << std::endl;
        // }
      //     ts::Node tmp = stateNode.getNamedChild(i);
      //     std::cout << getStringText(tmp.getChildByFieldName("key")) <<  "\t" << getStringText(tmp.getChildByFieldName("value")) << "\t" << tmp.getChildByFieldName("value").getType()  << std::endl;
      //     // ts::Node a = tmp.getChildByFieldName("value");
      //     // addState(s, getStringText(tmp.getChildByFieldName("key")), a);  //deals with what map and what type of data to store in the map
      // }
        //add stuff to map
        state.addData(section, key, Value{map});

    } else {  //literal
        // std::cout << "added literal " << getStringText(stateNode) << std::endl;
        state.addData(section, key, Value{typeConversion(stateNode)});
    }
}


/**
  * @brief for the 4 types of state that rules can use, create a map of the values
 */
void TreeSitter::parseState(ts::Node root){

  std::vector<std::string> stateTypes = {"constants", "variables", "per_player", "per_audience"};

  for (std::string s : stateTypes){
    ts::Node state = root.getChildByFieldName(s);
    if(state.isNull()){
      std::cout << s << " not a valid type" << std::endl;
      continue;
    }
  
  ts::Node child = state.getChildByFieldName("map");
  
  for (uint32_t i = 0; i < child.getNumNamedChildren(); ++i) {
      ts::Node tmp = child.getNamedChild(i);
          // std::cout << getStringText(tmp.getChildByFieldName("key")) <<  "\t" << getStringText(tmp.getChildByFieldName("value"))  << std::endl;
          ts::Node a = tmp.getChildByFieldName("value");
          addState(s, getStringText(tmp.getChildByFieldName("key")), a);  //deals with what map and what type of data to store in the map
      }
    
  }
  
}


Map TreeSitter::handleMap(ts::Node mapNode){
  Map map;
  for (uint32_t i = 0; i < mapNode.getNumNamedChildren(); ++i) {
    ts::Node tmp = mapNode.getNamedChild(i);
    std::string key = getStringText(tmp.getChildByFieldName("key"));
    Literal value = typeConversion(tmp.getChildByFieldName("value"));
    map[key] = value;
  }
  return map;
}


/**
  * @brief given a section and a name, adds the value to the corresponding map
 */
void StateVariables::addData(const std::string& section,const std::string& name, Value value){
  if(section == "constants"){
    constants[name] = value;
  } else if(section == "variables"){
    variables[name] = value;
  } else if(section == "per_player"){
    per_player[name] = value;
  } else if(section == "per_audience"){
    per_audience[name] = value;
  }
}

void StateVariables::printState(){

  std::cout << "Constants: " << std::endl;
  for (const auto& [name, value] : constants) {
            std::cout << name << " : ";
            std::visit(Printer{},value.val);
  }
  std::cout <<"\nVariables: " << std::endl;
  for (const auto& [name, value] : variables) {
            std::cout << name << " : ";
            std::visit(Printer{},value.val);
  }
  std::cout << "\nper_player: " << std::endl;
  for (const auto& [name, value] : per_player) {
            std::cout << name << " : ";
            std::visit(Printer{},value.val);
  }
  std::cout << "\nper_audience: " << std::endl;
  for (const auto& [name, value] : per_audience) {
            std::cout << name << " : ";
            std::visit(Printer{},value.val);
  }
}


void TreeSitter::printState(){
  state.printState();
  
}

ParserObjects TreeSitter::getParserObjects(){
  return {config, state};
}


int main(int argc, char* argv[]){

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    TreeSitter a(argv[1]);
    a.initalize();
    std::cout << "\n";
    a.printState();
    // a.createConfig();
    // a.printConfig();
    // Config b = a.getConfig();
    // std::cout << b.name << std::endl;
    // return 0;
}