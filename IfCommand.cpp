//
// Created by shiraz and nili on 12.12.2019.
//
#ifndef ex3__IFCOMMAND_CPP_
#define ex3__IFCOMMAND_CPP_
#include "ConditionParser.cpp"

class IfCommand : public ConditionParser {
 public:
  Singleton* singleton = Singleton::getSingleton();
  virtual void execute(queue<string> &token, unordered_map<string, Command*> &commands, unordered_map<string, Command *> &symbol_table_program) {
    cout<<"I am executing in If Command"<<endl;
    //initialize the queue of commands and check condition
    ConditionParser::execute(token, commands, symbol_table_program);
    if (condition) {
      int i = 0;
      while(!condition_commands.empty()) {
        i++;
        //the name of the command or var
        string current = condition_commands.front();
        Command *c = singleton->commands.at(current);
        if(c != NULL) {
          c->execute(condition_commands, commands, symbol_table_program);
        }
      }
    }
  }
};
#endif