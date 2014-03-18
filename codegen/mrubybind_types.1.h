// Describe type conversion between C type value and mruby value.

#include "mruby/string.h"
#include "mruby/proc.h"
#include "mruby/array.h"
#include "mruby/hash.h"
#include "mruby/variable.h"
#include <string>
#include <functional>
#include <memory>

namespace mrubybind {

template<class T> class Deleter{
    mrb_state* mrb_;
    mrb_value avoid_gc_table_;
    mrb_value v_;
public:
    Deleter(mrb_state* mrb, mrb_value avoid_gc_table, mrb_value v){
        mrb_ = mrb;
        avoid_gc_table_ = avoid_gc_table;
        v_ = v;
    }
    ~Deleter(){

    }
    void operator()(T* p) const {
        mrb_hash_delete_key(mrb_, avoid_gc_table_, v_);
        delete p;
    }

};

template<class T> using sp_mrb_obj = std::shared_ptr<T>;
//template<class T> using sp_mrb_func = std::shared_ptr<std::function<T> >;

template<class T> class sp_mrb_func{
    std::shared_ptr<std::function<T> > p;
public:
    sp_mrb_func(){

    }
    template<class D>sp_mrb_func(std::function<T>* pt, D d) : p(pt, d){

    }
    ~sp_mrb_func(){

    }
    std::shared_ptr<std::function<T> >& ref(){
        return p;
    }
    std::function<T>& func(){
        return *p.get();
    }
    operator bool() {
        if(!p.get()){
            return false;
        }
        return (bool)*p.get();
    }
    void reset(){
        p.reset();
    }
    template<class Y> void reset(Y* y){
        p.reset(y);
    }
    template<class Y, class D> void reset(Y* y, D d){
        p.reset(y, d);
    }
    template<class Y, class D, class A> void reset(Y* y, D d, A a){
        p.reset(y, d, a);
    }
};

template<class T> Deleter<T> set_avoid_gc(mrb_state* mrb, mrb_value v){
    RClass* mrubybind = mrb_define_module(mrb, "MrubyBind");
    mrb_value avoid_gc_table = mrb_obj_iv_get(mrb, (RObject*)mrubybind,
                                         mrb_intern_cstr(mrb, "__ untouchable __"));
    mrb_hash_set(mrb, avoid_gc_table, v, v);
    return Deleter<T>(mrb, avoid_gc_table, v);
}

template<class T> sp_mrb_obj<T> make_sp_mrb_obj(Deleter<T> d, T t){
    T* pt = new T();
    *pt = t;
    return sp_mrb_obj<T>(pt, d);
}

template<class T> sp_mrb_func<T> make_sp_mrb_func(Deleter<std::function<T> > d, std::function<T> t){
    std::function<T>* pt = new std::function<T>();
    *pt = t;
    return sp_mrb_func<T>(pt, d);
}


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
struct TypeFuncBase{
    static const char TYPE_NAME[];
};

template<class R>
struct Type<sp_mrb_func<R()> > :public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static sp_mrb_func<R()> get(mrb_state* mrb, mrb_value v) {
      Deleter<std::function<R()> > d = set_avoid_gc<std::function<R()> >(mrb, v);
      return make_sp_mrb_func<R()>(d, [=](){
          return Type<R>::get(mrb, mrb_yield(mrb, v, mrb_nil_value()));
      });
  }
  static mrb_value ret(mrb_state* mrb, sp_mrb_func<R()> p) {
      // don't call.
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

template<>
struct Type<sp_mrb_func<void()> > :public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static sp_mrb_func<void()> get(mrb_state* mrb, mrb_value v) {
      Deleter<std::function<void()> > d = set_avoid_gc<std::function<void()> >(mrb, v);
      return make_sp_mrb_func<void()>(d, [=](){
          mrb_yield(mrb, v, mrb_nil_value());
      });
  }
  static mrb_value ret(mrb_state* mrb, sp_mrb_func<void()> p) {
      // don't call.
      (void)mrb; (void)p; return mrb_nil_value();
  }
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
