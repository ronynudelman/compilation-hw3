#ifndef UTILITIES_H_
#define UTILITIES_H_


#include <string>
#include <vector>
#include "Tables.h"


extern SymbolTableStack symbol_table_stack;
extern OffsetTableStack offset_table_stack;


void check_main_exist();
void open_scope();
void close_scope();
void add_func_to_symbol_table(std::string name, std::string ret_type, std::vector<std::string> arguments);
void add_func_args_to_symbol_table(std::vector<std::string> args_types, std::vector<std::string> args_names);


#endif // UTILITIES_H_
