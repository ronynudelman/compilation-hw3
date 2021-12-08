#include <list>
#include <string>
#include "Tables.h"


SymbolTableEntry::SymbolTableEntry(std::string name, int offset, Type type, bool is_func,
                                  std::vector<std::string> arguments) : name(name),
                                                                        offset(offset),
                                                                        type(type),
                                                                        is_func(is_func),
                                                                        arguments(arguments) {}


std::string SymbolTableEntry::get_name() {
  return name;
}


void SymbolTable::push_entry(std::string name, int offset, Type type, bool is_func,
                            std::vector<std::string> arguments) {
  SymbolTableEntry new_entry(name, offset, type, is_func, arguments);
  symbol_table_entries.push_back(new_entry);
}


void SymbolTable::pop_entry() {
  symbol_table_entries.pop_back();
}

bool SymbolTable::is_symbol_exist(std::string name) {
  for (std::list<SymbolTableEntry>::reverse_iterator it = symbol_table_entries.rbegin(); it != symbol_table_entries.rend(); ++it) {
    if ((*it).get_name() == name) {
      return true;
    }
  }
  return false;
}


SymbolTableStack::SymbolTableStack() : symbol_tables(std::list<SymbolTable>()) {
  SymbolTable new_symbol_table;
  std::vector<std::string> print_func_args;
  print_func_args.push_back("STRING");
  new_symbol_table.push_entry("print", 0, "VOID", true, print_func_args);
  std::vector<std::string> printi_func_args;
  printi_func_args.push_back("INT");
  new_symbol_table.push_entry("printi", 0, "VOID", true, printi_func_args);
  symbol_tables.push_back(new_symbol_table);
}


void SymbolTableStack::push_symbol_table() {
  SymbolTable new_symbol_table;
  symbol_tables.push_back(new_symbol_table);
}


void SymbolTableStack::pop_symbol_table() {
  symbol_tables.pop_back();
}


bool SymbolTableStack::is_symbol_exist(std::string name) {
  for (std::list<SymbolTable>::reverse_iterator it = symbol_tables.rbegin(); it != symbol_tables.rend(); ++it) {
    if ((*it).is_symbol_exist(name)) {
      return true;
    }
  }
  return false;
}


OffsetTableStack::OffsetTableStack() : offsets(std::list<int>()) {
    offsets.push_back(0);
}


void OffsetTableStack::push_offset() {
  int copy_top_offset = offsets.top();
  offsets.push_back(copy_top_offset);
}


void OffsetTableStack::pop_offset() {
  offsets.pop_back();

}


int OffsetTableStack::top_offset() {
  return offsets.back();
}


void OffsetTableStack::inc_top_offset() {
  (offsets.back())++;
}
