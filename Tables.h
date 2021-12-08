#include <list>
#include <vector>
#include <string>
#include "Types.h"


class SymbolTableEntry {
private:
  std::string name;
  int offset;
  std::string type; // for functions, it's the return type
  bool is_func;
  std::vector<std::string> arguments;
public:
  SymbolTableEntry(std::string name, int offset, Type type, bool is_func = false,
                   std::vector<std::string> arguments = std::vector<std::string>());
  std::string get_name();
};


class SymbolTable {
private:
  std::list<SymbolTableEntry> symbol_table_entries;
public:
  SymbolTable();
  void push_entry(std::string name, int offset, Type type, bool is_func = false,
                  std::vector<std::string> arguments = std::vector<std::string>());
  void pop_entry();
  bool is_symbol_exist(std::string name); // TODO
};


class SymbolTableStack {
private:
  std::list<SymbolTable> symbol_tables;
public:
  SymbolTableStack();
  void push_symbol_table();
  void pop_symbol_table();
  bool is_symbol_exist(std::string name); // TODO
};


class OffsetTableStack {
private:
  std::list<int> offsets;
public:
  OffsetTableStack();
  void push_offset();
  void pop_offset();
  int top_offset();
  void inc_top_offset();
}
