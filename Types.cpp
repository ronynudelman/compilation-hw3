#include <string>
#include "Types.h"


const std::string void_type = "VOID";
const std::string int_type = "INT";
const std::string byte_type = "BYTE";
const std::string bool_type = "BOOL";
const std::string string_type = "STRING";
const std::string const_int_type = "CONST INT";
const std::string const_byte_type = "CONST BYTE";
const std::string const_bool_type = "CONST BOOL";


FormalDeclCls::FormalDeclCls(bool is_const, std::string type, std::string name) : is_const(is_const),
                                                                                  type(type),
                                                                                  name(name)
                                                                                  {}


void FormalsListCls::add_formal_decl(AbsCls* new_decl) {
  std::string new_type;
  if (new_decl->get_is_const()) {
    new_type = std::string("CONST") + new_decl->get_type();
  } else {
    new_type = new_decl->get_type();
  }
  std::string new_name(new_decl->get_name());
  args_types.push_back(new_type);
  args_names.push_back(new_name);
}


FormalsCls::FormalsCls(std::vector<std::string> args_types,
                       std::vector<std::string> args_names) : args_types(args_types),
                                                              args_names(args_names)
                                                              {}


RetTypeCls::RetTypeCls(std::string name) : name(name) {}


TypeCls::TypeCls(std::string name) : name(name) {}


IDCls::IDCls(std::string name) : name(name) {}


TypeAnnotationCls::TypeAnnotationCls(bool is_const) : is_const(is_const) {}
