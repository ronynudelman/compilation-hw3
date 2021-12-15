#include <cassert>
#include <string>
#include <vector>
#include "Utilities.h"
#include "Tables.h"
#include "hw3_output.hpp"


SymbolTableStack symbol_table_stack;
OffsetTableStack offset_table_stack;
bool is_inside_while;

static bool is_type_starts_with_const(std::string type){
	return std::string::npos != type.find("CONST");
}

static SymbolTableEntry& get_last_func() {
	return symbol_table_stack.first_symbol_table().top_symbol_table_entry();
}

static std::string strip_const(std::string type){
	if(std::string::npos  == type.find("CONST")){
		return type;
	}
	std::string stripped_type = type.replace(0, 6, "");
	return stripped_type;
}

static std::vector<std::string> strip_const(std::vector<std::string> types){
	std::vector<std::string> stripped_types;
	for (std::vector<std::string>::iterator it = types.begin(); it != types.end(); ++it) {
		stripped_types.push_back(strip_const(*it));
	}
	return stripped_types;
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
	if (strip_const(type_a) != strip_const(type_b)){
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


std::string check_valid_func_call(std::string func_name, std::vector<std::string> args_types) {
	SymbolTableEntry* func_entry = symbol_table_stack.get_entry_by_name(func_name);
	if(!func_entry || func_entry->get_is_func() == false) {
		 output::errorUndefFunc(yylineno, func_name);
		 exit(1);
	}
	std::vector<std::string> reverse_args_types;
	for (std::vector<std::string>::reverse_iterator it = args_types.rbegin(); it != args_types.rend(); ++it) {
		reverse_args_types.push_back(*it);
	}
	std::vector<std::string> func_entry_args(func_entry->get_arguments());
	std::vector<std::string> stripped_func_entry_args = strip_const(func_entry_args);
	std::vector<std::string> stripped_reverse_args_types = strip_const(reverse_args_types);
	
	if(stripped_func_entry_args != stripped_reverse_args_types){
		output::errorPrototypeMismatch(yylineno, func_name, func_entry_args);
		exit(1);
	}
	return func_entry->get_type();
}


void check_valid_ret_type(std::string ret_type) {
	check_matching_types(get_last_func().get_type(), ret_type);
}


void check_legal_break(){
	if(!is_inside_while){
		output::errorUnexpectedBreak(yylineno);
		exit(1);
	}
}


void check_legal_continue(){
	if(!is_inside_while){
		output::errorUnexpectedContinue(yylineno);
		exit(1);
	}
}








