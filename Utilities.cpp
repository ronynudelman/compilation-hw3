#include <cassert>
#include <string>
#include <vector>
#include "Utilities.h"
#include "hw3_output.hpp"


SymbolTableStack symbol_table_stack;
OffsetTableStack offset_table_stack;


void check_main_exist() {
  if (!symbol_table_stack.is_main_func_exist()) {
    output::errorMainMissing();
    exit(1);
  }
}


void open_scope() {
  symbol_table_stack.push_symbol_table();
  offset_table_stack.push_offset();
}


void close_scope() {
  output::endScope();
  symbol_table_stack.print_top_symbol_table();
  symbol_table_stack.pop_symbol_table();
  offset_table_stack.pop_offset();
}


void add_func_to_symbol_table(std::string name, std::string ret_type, std::vector<std::string> arguments) {
  SymbolTable top_symbol_table = symbol_table_stack.top_symbol_table();
  top_symbol_table.push_entry(name, 0, ret_type, true, arguments);
}


void add_func_args_to_symbol_table(std::vector<std::string> args_types, std::vector<std::string> args_names) {
  assert(args_types.size() == args_names.size());
  SymbolTable top_symbol_table = symbol_table_stack.top_symbol_table();
  int arg_offset = -1;
  for (size_t i = 0; i < args_types.size(); i++) {
    top_symbol_table.push_entry(args_names[i], arg_offset, args_types[i]);
    arg_offset--;
  }
}
