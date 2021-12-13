#include <cassert>
#include <string>
#include <vector>
#include "Utilities.h"
#include "Tables.h"
#include "hw3_output.hpp"


SymbolTableStack symbol_table_stack;
OffsetTableStack offset_table_stack;

static bool is_type_starts_with_const(std::string type){
	return std::string::npos != type.find("CONST");
}

void check_main_exist() {
  SymbolTableEntry* main_entry = symbol_table_stack.get_entry_by_name("main");
  if (!main_entry || !(main_entry->get_type() == "VOID" && main_entry->get_is_func() && main_entry->get_arguments().empty())) {
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
  if (symbol_table_stack.get_entry_by_name(name)) {
  	output::errorDef(yylineno, name);
  	exit(1);
  }
  SymbolTable& top_symbol_table = symbol_table_stack.top_symbol_table();
  std::vector<std::string> reverse_args;
  
  for (std::vector<std::string>::reverse_iterator it = arguments.rbegin(); it != arguments.rend(); ++it) {
	reverse_args.push_back(*it);
  }
  top_symbol_table.push_entry(name, 0, ret_type, true, reverse_args);
}


void add_func_args_to_symbol_table(std::vector<std::string> args_types, std::vector<std::string> args_names) {
  assert(args_types.size() == args_names.size());
  if (!args_types.empty()){
	 SymbolTable& top_symbol_table = symbol_table_stack.top_symbol_table();
	 int arg_offset = -1;
	 for (size_t i = (args_types.size() - 1); i >= 0; i--) {
		top_symbol_table.push_entry(args_names[i], arg_offset, args_types[i]);
		arg_offset--;
		if (i == 0){
			break;
		}
	 }
  }
}


void add_var_to_symbol_table (bool is_const, std::string type, std::string name){
	if (symbol_table_stack.get_entry_by_name(name)) {
		output::errorDef(yylineno, name);
		exit(1);
	}
	SymbolTable& top_symbol_table = symbol_table_stack.top_symbol_table();
	type = is_const ? "CONST " + type : type;
	top_symbol_table.push_entry(name, offset_table_stack.top_offset(), type); 
	offset_table_stack.inc_top_offset();
}

void check_matching_types(std::string type_a, std::string type_b){
	if (type_a != type_b){
		output::errorMismatch(yylineno);
		exit(1);
	}
}


void check_var_valid_for_assign(std::string name){
	SymbolTableEntry* var_entry = symbol_table_stack.get_entry_by_name(name);
	if (!var_entry || var_entry->get_is_func()) {
		output::errorUndef(yylineno, name);
		exit(1);
	}
	else if(is_type_starts_with_const(var_entry->get_type())){
		output::errorConstMismatch(yylineno);
		exit(1);
	}
}


std::string get_id_type(std::string name){
	SymbolTableEntry* id_entry = symbol_table_stack.get_entry_by_name(name);
	if (id_entry == nullptr){
		output::errorUndef(yylineno, name);
		exit(1);
	}
	return id_entry->get_type();
}


void check_assign_to_const(bool is_const){
	if(is_const){
		output::errorConstDef(yylineno);
		exit(1);
	}
}


std::string handle_binop_exp(std::string type_a, std::string type_b){
	if (std::string::npos == type_a.find("INT") && std::string::npos == type_a.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
	if (std::string::npos == type_b.find("INT") && std::string::npos == type_b.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
	if (std::string::npos != type_a.find("INT") || std::string::npos != type_b.find("INT")){
		return "INT";
	}
	
	return "BYTE";
}


void handle_relop_exp(std::string type_a, std::string type_b){
	if (std::string::npos == type_a.find("INT") && std::string::npos == type_a.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
	if (std::string::npos == type_b.find("INT") && std::string::npos == type_b.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
}

void handle_cast_exp(std::string type_a, std::string type_b){
	if (std::string::npos == type_a.find("INT") && std::string::npos == type_a.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
	if (std::string::npos == type_b.find("INT") && std::string::npos == type_b.find("BYTE")){
		output::errorMismatch(yylineno);
		exit(1);
	}
}


void check_valid_types_for_assign(std::string left_type, std::string right_type){
	if (!(std::string::npos != left_type.find("INT") && std::string::npos != right_type.find("BYTE"))){
		check_matching_types(left_type, right_type);
	}
}

























