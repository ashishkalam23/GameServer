add_library(Parser)
target_sources(Parser PRIVATE GameData.cpp TreeSitter.cpp RuleParser.cpp)

target_include_directories(Parser PRIVATE ../include/Parser)

add_executable(TreeSitter
            GameData.cpp
            TreeSitter.cpp
            RuleParser.cpp)

 target_link_libraries(Parser PRIVATE
        tree-sitter-socialgaming
        cpp-tree-sitter
)

target_link_libraries(TreeSitter PRIVATE
        Parser
        tree-sitter-socialgaming
        cpp-tree-sitter
)