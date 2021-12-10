#ifndef TYPES_H_
#define TYPES_H_


#include <string>
#include <vector>
#include <iostream>

extern const std::string void_type;
extern const std::string int_type;
extern const std::string byte_type;
extern const std::string bool_type;
extern const std::string string_type;
extern const std::string const_int_type;
extern const std::string const_byte_type;
extern const std::string const_bool_type;


class AbsCls {
public:
  virtual std::string get_name() { std::cerr << "Unexpected error" << std::endl; exit(1); return std::string(); }
  virtual std::string get_type() { std::cerr << "Unexpected error" << std::endl; exit(1); return std::string(); }
  virtual std::vector<std::string> get_args_types() { std::cerr << "Unexpected error" << std::endl; exit(1); return std::vector<std::string>(); }
  virtual std::vector<std::string> get_args_names() { std::cerr << "Unexpected error" << std::endl; exit(1); return std::vector<std::string>(); }
  virtual void add_formal_decl(AbsCls*) { std::cerr << "Unexpected error" << std::endl; exit(1); }
  virtual bool get_is_const() { std::cerr << "Unexpected error" << std::endl; exit(1); return true; }
  virtual ~AbsCls() {}
};


class FormalDeclCls : public AbsCls {
private:
  bool is_const;
  std::string type;
  std::string name;
public:
  FormalDeclCls(bool is_const, std::string type, std::string name);
  std::string get_name() override { return name; }
  std::string get_type() override { return type; }
  bool get_is_const() override { return is_const; }
  virtual ~FormalDeclCls() {}
};


class FormalsListCls : public AbsCls {
private:
  std::vector<std::string> args_types;
  std::vector<std::string> args_names;
public:
  std::vector<std::string> get_args_types() override { return args_types; }
  std::vector<std::string> get_args_names() override { return args_names; }
  FormalsListCls() = default;
  void add_formal_decl(AbsCls*) override;
  virtual ~FormalsListCls() {}
};


class FormalsCls : public AbsCls {
private:
  std::vector<std::string> args_types;
  std::vector<std::string> args_names;
public:
  std::vector<std::string> get_args_types() override { return args_types; }
  std::vector<std::string> get_args_names() override { return args_names; }
  FormalsCls(std::vector<std::string> args_types = std::vector<std::string>(),
             std::vector<std::string> args_names = std::vector<std::string>());
  virtual ~FormalsCls() {}
};


class RetTypeCls : public AbsCls {
private:
  std::string name;
public:
  std::string get_name() override { return name; }
  RetTypeCls(std::string name);
  virtual ~RetTypeCls() {}
};


class TypeCls : public AbsCls {
private:
  std::string name;
public:
  TypeCls(std::string name);
  std::string get_name() override { return name; }
  virtual ~TypeCls() {}
};


class TypeAnnotationCls : public AbsCls {
private:
  bool is_const;
public:
  TypeAnnotationCls(bool is_const);
  bool get_is_const() override { return is_const; }
  virtual ~TypeAnnotationCls() {}
};


class IDCls : public AbsCls {
private:
  std::string name;
public:
  IDCls(std::string name);
  std::string get_name() override { return name; }
  virtual ~IDCls() {}
};


#endif // TYPES_H_
