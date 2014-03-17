// Describe type conversion between C type value and mruby value.

#include "mruby/string.h"
#include "mruby/proc.h"
#include "mruby/array.h"
#include <string>
#include <functional>

namespace mrubybind {

//===========================================================================
// C <-> mruby type converter.

// Base template class.
template <class T>
struct Type {
  // Type name used for error message.
  // static const char TYPE_NAME[];

  // Returns whether the given mrb_value can be converted into type T.
  //static int check(mrb_value v) = 0;

  // Converts mrb_value to type T value.
  //static T get(mrb_value v) = 0;

  // Converts type T value to mrb_value.
  //static mrb_value ret(mrb_state*, T i) = 0;
};

// Fixnum
template<>
struct Type<int> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_fixnum_p(v) || mrb_float_p(v); }
  static int get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_fixnum_p(v) ? mrb_fixnum(v) : mrb_float(v); }
  static mrb_value ret(mrb_state*, int i) { return mrb_fixnum_value(i); }
};

template<>
struct Type<unsigned int> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_fixnum_p(v) || mrb_float_p(v); }
  static unsigned int get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_fixnum_p(v) ? mrb_fixnum(v) : mrb_float(v); }
  static mrb_value ret(mrb_state*, unsigned int i) { return mrb_fixnum_value(i); }
};

// float
template<>
struct Type<float> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_float_p(v) || mrb_fixnum_p(v); }
  static float get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_float_p(v) ? mrb_float(v) : mrb_fixnum(v); }
  static mrb_value ret(mrb_state* mrb, float f) { return mrb_float_value(mrb, f); }
};

// double
template<>
struct Type<double> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_float_p(v) || mrb_fixnum_p(v); }
  static double get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_float_p(v) ? mrb_float(v) : mrb_fixnum(v); }
  static mrb_value ret(mrb_state* mrb, double f) { return mrb_float_value(mrb, f); }
};

// String
template<>
struct Type<const char*> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_string_p(v); }
  static const char* get(mrb_state* mrb, mrb_value v) { (void)mrb; return RSTRING_PTR(v); }
  static mrb_value ret(mrb_state* mrb, const char* s) { return mrb_str_new_cstr(mrb, s); }
};

template<>
struct Type<std::string> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

template<>
struct Type<const std::string> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

template<>
struct Type<const std::string&> {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

// Boolean
template<>
struct Type<bool> {
  static const char TYPE_NAME[];
  static int check(mrb_value /*v*/) { return 1; }
  static bool get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_test(v); }
  static mrb_value ret(mrb_state* /*mrb*/, bool b) { return b ? mrb_true_value() : mrb_false_value(); }
};

// Raw pointer
template<>
struct Type<void*> {
  static const char TYPE_NAME[];
<<<<<<< HEAD
  static int check(mrb_value v) { return mrb_cptr_p(v); }
  static void* get(mrb_value v) { return mrb_cptr(v); }
  static mrb_value ret(mrb_state* mrb, void* p) { return mrb_cptr_value(mrb, p); }
=======
  static int check(mrb_value v) { return mrb_voidp_p(v); }
  static void* get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_voidp(v); }
  static mrb_value ret(mrb_state* mrb, void* p) { return mrb_voidp_value(mrb, p); }
>>>>>>> call back.
};

// Function
template<>
struct Type<std::function<void()> > {
  static const char TYPE_NAME[];
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static std::function<void()> get(mrb_state* mrb, mrb_value v) { return [=](){
      mrb_yield(mrb, v, mrb_nil_value());
  }; }
  static mrb_value ret(mrb_state* mrb, std::function<void()> p) {
      // don't call.
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

struct TypeFuncBase{
    static const char TYPE_NAME[];
};

#include "mrubybind_types_generated.h"

//===========================================================================
// Binder

// Template class for Binder.
// Binder template class is specialized with type.
template <class T>
struct Binder {
  // Template specialization.
  //static mrb_value call(mrb_state* mrb, void* p, mrb_value* args, int narg) = 0;
};

// Template class for Binder.
// Binder template class is specialized with type.
template <class C>
struct ClassBinder {
  static struct mrb_data_type type_info;
  static void dtor(mrb_state*, void* p) {
    C* instance = static_cast<C*>(p);
    delete instance;
  }

  // Template specialization.
  //static void ctor(mrb_state* mrb, mrb_value self, void* new_func_ptr, mrb_value* args, int narg) {
};
template<class C>
mrb_data_type ClassBinder<C>::type_info = { "???", dtor };

mrb_value raise(mrb_state *mrb, int parameter_index,
                const char* required_type_name, mrb_value value);
mrb_value raisenarg(mrb_state *mrb, mrb_value func_name, int narg, int nparam);

// Includes generated template specialization.
//#include "mrubybind.inc"
