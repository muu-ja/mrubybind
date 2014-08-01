// Describe type conversion between C type value and mruby value.

#include "mruby/string.h"
#include "mruby/proc.h"
#include "mruby/array.h"
#include "mruby/hash.h"
#include "mruby/variable.h"
#include <string>
#include <functional>
#include <memory>
#include <map>
#include <iostream>

namespace mrubybind {

extern const char* untouchable_table;
extern const char* untouchable_object;

class MrubyArenaStore{
    mrb_state* mrb;
    int ai;
public:
    MrubyArenaStore(mrb_state* mrb)
    {
        this->mrb = mrb;
        this->ai = mrb_gc_arena_save(mrb);
    }
    
    ~MrubyArenaStore()
    {
        mrb_gc_arena_restore(mrb, ai);
    }

};

class MrubyBindStatus{

public:

    struct Data;
    typedef std::shared_ptr<Data> Data_ptr;
    typedef std::map<mrb_state*, Data_ptr > Table;
    
    static Table& get_living_table(){
        static Table table;
        return table;
    }

    struct Data{
        typedef std::map<std::string, std::map<std::string, bool> > ClassConvertableTable;
    
        mrb_state* mrb;
        mrb_value avoid_gc_table;
        ClassConvertableTable class_convertable_table;
        
        Data(){
            
        }
        ~Data(){
            
        }
        
        mrb_state* get_mrb(){
            return mrb;
        }
        
        mrb_value get_avoid_gc_table(){
            return avoid_gc_table;
        }
        
        void set_class_conversion(const std::string& s, const std::string& d, bool c){
            class_convertable_table[s][d] = c;
        }
        
        bool is_convertable(const std::string& s, const std::string& d)
        {
            auto fs = class_convertable_table.find(s);
            if(fs != class_convertable_table.end()){
                auto fd = fs->second.find(d);
                if(fd != fs->second.end()){
                    return fd->second;
                }
            }
            return false;
        }
        
    };
    
    MrubyBindStatus(){
        
    }

    MrubyBindStatus(mrb_state* mrb, mrb_value avoid_gc_table){
    
        Table& living_table = get_living_table();
        data = std::make_shared<Data>();
        data->mrb = mrb;
        data->avoid_gc_table = avoid_gc_table;
        living_table[mrb] = data;
    }
    
    ~MrubyBindStatus(){
    
        Table& living_table = MrubyBindStatus::get_living_table();
        living_table.erase(data->mrb);
        data->mrb = NULL;
        
    }
    
    static bool is_living(mrb_state* mrb){
        Table& living_table = get_living_table();
        if(living_table.find(mrb) != living_table.end()){
            return living_table[mrb].get();
        }
        return false;
    }
    
    static Data_ptr search(mrb_state* mrb){
        Table& living_table = get_living_table();
        if(living_table.find(mrb) != living_table.end()){
            return living_table[mrb];
        }
        return Data_ptr(NULL);
    }
    
    
    
private:
    std::shared_ptr<Data> data;
};

template<class T> class Deleter{
    MrubyBindStatus::Data_ptr mrbsp;
    mrb_value v_;
public:
    Deleter()
    {
        
    }

    Deleter(mrb_state* mrb, mrb_value v){
        mrbsp = MrubyBindStatus::search(mrb);
        mrb_value avoid_gc_table = mrbsp->get_avoid_gc_table();
        mrb_int v_id = mrb_obj_id(v);
        mrb_value id = mrb_fixnum_value(v_id);
        mrb_value s = mrb_hash_get(mrb, avoid_gc_table, id);
        if(mrb_test(s)){
            mrb_value a = s;
            mrb_value nv = mrb_ary_ref(mrb, a, 1);
            mrb_int n = mrb_fixnum(nv);
            n++;
            mrb_ary_set(mrb, a, 1, mrb_fixnum_value(n));
        }
        else{
            mrb_value a = mrb_ary_new(mrb);
            mrb_ary_push(mrb, a, v);
            mrb_ary_push(mrb, a, mrb_fixnum_value(1));
            s = a;
            mrb_hash_set(mrb, avoid_gc_table, id, s);
        }
        v_ = v;
    }
    ~Deleter(){

    }
    mrb_state* get_mrb(){
        return mrbsp->mrb;
    }
    void operator()(T* p) const {
        if(mrbsp.get()){
            mrb_state* mrb = mrbsp->get_mrb();
            if(mrb){
                mrb_value avoid_gc_table = mrbsp->get_avoid_gc_table();
                mrb_int v_id = mrb_obj_id(v_);
                mrb_value id = mrb_fixnum_value(v_id);
                mrb_value a = mrb_hash_get(mrb, avoid_gc_table, id);
                mrb_value nv = mrb_ary_ref(mrb, a, 1);
                mrb_int n = mrb_fixnum(nv);
                n--;
                if(n <= 0)
                {
                    mrb_hash_delete_key(mrb, avoid_gc_table, id);
                }
                else
                {
                    mrb_ary_set(mrb, a, 1, mrb_fixnum_value(n));
                }
            }
        }
        if(p){
            delete p;
        }
    }

};

template<class T> using obj_ptr = std::shared_ptr<T>;
//template<class T> using FuncPtr = std::shared_ptr<std::function<T> >;

template<class T> class FuncPtr{
    mrb_state* mrb;
    std::shared_ptr<std::function<T> > p;
public:
    FuncPtr(){

    }
    template<class D>FuncPtr(std::function<T>* pt, D d) : p(pt, d){
        mrb = d.get_mrb();
    }
    ~FuncPtr(){

    }
    const std::shared_ptr<std::function<T> >& ref() const{
        return p;
    }
    bool is_living() const{
        return MrubyBindStatus::is_living(mrb);
    }
    std::function<T>& func() const{
        if(!p.get()){
            throw std::runtime_error("empty function.");
        }
        return *p.get();
    }
    operator bool() const {
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
    return Deleter<T>(mrb, v);
}

template<class T> obj_ptr<T> make_obj_ptr(Deleter<T> d, T t){
    T* pt = new T();
    *pt = t;
    return obj_ptr<T>(pt, d);
}

template<class T> FuncPtr<T> make_FuncPtr(Deleter<std::function<T> > d, std::function<T> t){
    std::function<T>* pt = new std::function<T>();
    *pt = t;
    return FuncPtr<T>(pt, d);
}

template <class T>
struct Type;

class MrubyRef{
    mrb_state* mrb;
    std::shared_ptr<mrb_value> v;
public:
    
    MrubyRef();
    MrubyRef(mrb_state* mrb, const mrb_value& v);
    ~MrubyRef();
    
    bool is_living() const;
    mrb_value get_v()const;
    bool empty() const;
    bool test() const;
    bool obj_equal(const MrubyRef& r) const;
    std::string to_s() const;
    int to_i() const;
    float to_float() const;
    double to_double() const;
    
    MrubyRef call(std::string name);
    
#include "mrubybind_call_generated.h"

};

//===========================================================================
// C <-> mruby type converter.

//template <class T>
//struct Type {
  // Type name used for error message.
  // static const char TYPE_NAME[];

  // Returns whether the given mrb_value can be converted into type T.
  //static int check(mrb_value v) = 0;

  // Converts mrb_value to type T value.
  //static T get(mrb_value v) = 0;

  // Converts type T value to mrb_value.
  //static mrb_value ret(mrb_state*, T i) = 0;
//};

// Fixnum
template<>
struct Type<int> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_fixnum_p(v) || mrb_float_p(v); }
  static int get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_fixnum_p(v) ? mrb_fixnum(v) : mrb_float(v); }
  static mrb_value ret(mrb_state*, int i) { return mrb_fixnum_value(i); }
};

template<>
struct Type<unsigned int> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_fixnum_p(v) || mrb_float_p(v); }
  static unsigned int get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_fixnum_p(v) ? mrb_fixnum(v) : mrb_float(v); }
  static mrb_value ret(mrb_state*, unsigned int i) { return mrb_fixnum_value(i); }
};

// float
template<>
struct Type<float> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_float_p(v) || mrb_fixnum_p(v); }
  static float get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_float_p(v) ? mrb_float(v) : mrb_fixnum(v); }
  static mrb_value ret(mrb_state* mrb, float f) { return mrb_float_value(mrb, f); }
};

// double
template<>
struct Type<double> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_float_p(v) || mrb_fixnum_p(v); }
  static double get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_float_p(v) ? mrb_float(v) : mrb_fixnum(v); }
  static mrb_value ret(mrb_state* mrb, double f) { return mrb_float_value(mrb, f); }
};

// String
template<>
struct Type<const char*> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_string_p(v); }
  static const char* get(mrb_state* mrb, mrb_value v) { (void)mrb; return RSTRING_PTR(v); }
  static mrb_value ret(mrb_state* mrb, const char* s) { return mrb_str_new_cstr(mrb, s); }
};

template<>
struct Type<std::string> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

template<>
struct Type<const std::string> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

template<>
struct Type<const std::string&> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_string_p(v); }
  static const std::string get(mrb_state* mrb, mrb_value v) { (void)mrb; return std::string(RSTRING_PTR(v), RSTRING_LEN(v)); }
  static mrb_value ret(mrb_state* mrb, const std::string& s) { return mrb_str_new(mrb, s.c_str(), s.size()); }
};

// Boolean
template<>
struct Type<bool> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value /*v*/) { return 1; }
  static bool get(mrb_state* mrb, mrb_value v) { (void)mrb; return mrb_test(v); }
  static mrb_value ret(mrb_state* /*mrb*/, bool b) { return b ? mrb_true_value() : mrb_false_value(); }
};

// Raw pointer
template<>
struct Type<void*> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value v) { return mrb_cptr_p(v); }
  static void* get(mrb_state*, mrb_value v) { return mrb_cptr(v); }
  static mrb_value ret(mrb_state* mrb, void* p) { return mrb_cptr_value(mrb, p); }
};

// Function
struct TypeFuncBase{
    static const char TYPE_NAME[];
};

template<class R>
struct Type<FuncPtr<R()> > :public TypeFuncBase {
  static int check(mrb_state*, mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R()> get(mrb_state* mrb, mrb_value v) {
      Deleter<std::function<R()> > d = set_avoid_gc<std::function<R()> >(mrb, v);
      return make_FuncPtr<R()>(d, [=](){
          MrubyArenaStore mas(mrb);
          return Type<R>::get(mrb, mrb_yield(mrb, v, mrb_nil_value()));
      });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R()> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R()> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

template<>
struct Type<FuncPtr<void()> > :public TypeFuncBase {
  static int check(mrb_state*, mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void()> get(mrb_state* mrb, mrb_value v) {
      Deleter<std::function<void()> > d = set_avoid_gc<std::function<void()> >(mrb, v);
      return make_FuncPtr<void()>(d, [=](){
          MrubyArenaStore mas(mrb);
          mrb_yield(mrb, v, mrb_nil_value());
      });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void()> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void()> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// mruby value
template<>
struct Type<MrubyRef> {
  static const char TYPE_NAME[];
  static int check(mrb_state*, mrb_value) { return 1; }
  static MrubyRef get(mrb_state* mrb, mrb_value v) { (void)mrb; return MrubyRef(mrb, v); }
  static mrb_value ret(mrb_state*, MrubyRef r) { return r.get_v(); }
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

template <class T>
struct CustomClassBinder {
  // Template specialization.
  //static mrb_value call(mrb_state* mrb, void* p, mrb_value* args, int narg) = 0;
};

// Other Class
struct TypeClassBase{
    static const char TYPE_NAME[];
};

template<class T> struct Type<T&> :public TypeClassBase {
    static std::string class_name;
    static int check(mrb_state* mrb, mrb_value v) {
        return mrb_type(v) == MRB_TT_DATA &&
            MrubyBindStatus::search(mrb)->is_convertable(mrb_obj_classname(mrb, v), class_name);
    }
    static T& get(mrb_state* mrb, mrb_value v) {
        (void)mrb; return *(T*)DATA_PTR(v);
    }
    static mrb_value ret(mrb_state* mrb, T t) {
        RClass* cls;
        mrb_value v;
        cls = mrb_class_get(mrb, class_name.c_str());
        v = mrb_class_new_instance(mrb, 0, NULL, cls);
        DATA_TYPE(v) = &ClassBinder<T>::type_info;
        T* nt = new T();
        *nt = t;
        DATA_PTR(v) = nt;
        return v;
    }
};

template<class T> std::string Type<T&>::class_name = "";

template<class T> struct Type :public TypeClassBase {
    static std::string class_name;
    static int check(mrb_state* mrb, mrb_value v) { 
        return mrb_type(v) == MRB_TT_DATA &&
            MrubyBindStatus::search(mrb)->is_convertable(mrb_obj_classname(mrb, v), class_name); 
    }
    static T get(mrb_state* mrb, mrb_value v) { 
            (void)mrb; return *(T*)DATA_PTR(v); 
        }
    static mrb_value ret(mrb_state* mrb, T t) { 
        RClass* cls;
        mrb_value v;
        cls = mrb_class_get(mrb, class_name.c_str());
        v = mrb_class_new_instance(mrb, 0, NULL, cls);
        DATA_TYPE(v) = &ClassBinder<T>::type_info;
        T* nt = new T();
        *nt = t;
        DATA_PTR(v) = nt;
        return v;
    }
};

template<class T> std::string Type<T>::class_name = "";

//
mrb_value raise(mrb_state *mrb, int parameter_index,
                const char* required_type_name, mrb_value value);
mrb_value raisenarg(mrb_state *mrb, mrb_value func_name, int narg, int nparam);

// Includes generated template specialization.
//#include "mrubybind.inc"
