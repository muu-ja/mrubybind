#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
#include <string>
#include <sstream>
#include <functional>
using namespace std;

//=============================================================================
// Simple usage for binding function.

int square(int x) {
  return x * x;
}

string emphasize(const char* str) {
  return "* " + string(str) + " *";
}

void BindFunctionTest(mrb_state* mrb) {
  {
    mrubybind::MrubyBind b(mrb);
    b.bind("square", square);
    b.bind("emphasize", emphasize);
  }

  mrubybind::load_string(mrb, 
                  "puts square(1111)\n"
                  "puts emphasize('Hello, mruby!')\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
// Simple usage for binding C++ class.

class Foo {
public:
  Foo(int x) : x_(x) {
    cout << "Foo::ctor(" << x << "), " << this << endl;
  }
  virtual ~Foo() {
    cout << "Foo::dtor(), " << this << endl;
  }
  int bar(int y) {
    return x_ + y;
  }
  static int baz(int z) {
    return z * z;
  }

private:
  int x_;
};

Foo* new_foo(int x) {
  return new Foo(x);
}

void BindClassTest(mrb_state* mrb) {
  {
    mrubybind::MrubyBind b(mrb);
    b.bind_class("Foo", new_foo);
    b.bind_instance_method("Foo", "bar", &Foo::bar);
    b.bind_static_method("Foo", "baz", &Foo::baz);
  }

  mrubybind::load_string(mrb, 
                  "foo = Foo.new(123)\n"
                  "p foo\n"
                  "p foo.bar(567)\n"
                  "foo = nil\n"
                  "p Foo.baz(9999)"
                  );

  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
// Simple usage for binding function under some module.

void modfunc(int v) {
  printf("modfunc called: %d\n", v);
}

void UseModuleTest(mrb_state* mrb) {
  {
    mrubybind::MrubyArenaStore mas(mrb);
    RClass* mod = mrb_define_module(mrb, "Mod");
    mrubybind::MrubyBind b(mrb, mod);
    b.bind("modfunc", modfunc);
    b.bind_const("FOO_VALUE", 1234);
  }

  mrubybind::load_string(mrb,
                  "Mod.modfunc(Mod::FOO_VALUE)\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
//


std::string call_callback(mrubybind::FuncPtr<void()> f) {
  if(f)
  {
      cout << "pre f\n";
      f.func()();
      cout << "post f\n";
  }
  return "call_callback called\n";
}

std::string call_callback_a1(mrubybind::FuncPtr<void(int a0)> f) {
  if(f)
  {
      f.func()(23);
  }
  return "call_callback_a1 called\n";
}

std::string call_callback_a2(mrubybind::FuncPtr<void(int a0, std::string a1)> f) {
  if(f)
  {
      f.func()(23, "string");
  }
  return "call_callback_a2 called\n";
}

std::string call_callback_a1_int(mrubybind::FuncPtr<int(int a0)> f) {
  std::stringstream s;
  s << "call_callback_a1_int return this ->" << f.func()(23);
  return  s.str();
}

mrubybind::FuncPtr<void()> old_f;

void set_old_f(mrubybind::FuncPtr<void()> f) {
  old_f = f;
}

void call_old_f() {
  old_f.func()();
}


class Callbacker
{
    int a;
public:
    Callbacker()
    {
        a = 5;
    }

    int func_test(mrubybind::FuncPtr<void(int a0)> f)
    {
        f.func()(a);
        return a;
    }

    std::string func_a2_string(mrubybind::FuncPtr<std::string(int a0, std::string a1)> f) {
      return f.func()(48, "str");
    }
};

Callbacker* new_callbacker()
{
    return new Callbacker();
}

void CallbackFunctionTest(mrb_state* mrb) {
  {
    mrubybind::MrubyBind b(mrb);
    b.bind("call_callback", call_callback);
    b.bind("call_callback_a1", call_callback_a1);
    b.bind("call_callback_a2", call_callback_a2);
    b.bind("call_callback_a1_int", call_callback_a1_int);
    b.bind("set_old_f", set_old_f);
    b.bind("call_old_f", call_old_f);

    b.bind_class("Callbacker", new_callbacker);
    b.bind_instance_method("Callbacker", "func_test", &Callbacker::func_test);
    b.bind_instance_method("Callbacker", "func_a2_string", &Callbacker::func_a2_string);
  }

  mrubybind::load_string(mrb, 
                  "v = call_callback do\n"
                  "  puts \"?? called\n\""
                  "end\n"
                  "puts v\n"
                  "puts call_callback_a1 { |a0|\n"
                  "  puts \"a0 = #{a0}\"\n"
                  "}\n"
                  "puts call_callback_a2 { |a0, a1|\n"
                  "  puts \"a0 = #{a0}, a1 = #{a1}\"\n"
                  "}\n"
                  "puts call_callback_a1_int { |a0|\n"
                  "  puts \"a0 = #{a0}\"\n"
                  "}\n"
                  "puts \"Callbacker.new.func_test \" + Callbacker.new.func_test {|a0|\n"
                  "  puts \"class a0 = #{a0}\"\n"
                  "}.to_s\n"
                  "puts \"Callbacker.new.func_a2_string \" + Callbacker.new.func_a2_string {|a0, a1|\n"
                  "  puts \"class a0 = #{a0}, a1 = #{a1}\"\n"
                  "  \"cat #{a0} #{a1}\"\n"
                  "}.to_s\n"
                  "set_old_f do puts \"call old_f\"; end"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
  mrubybind::load_string(mrb,
                  "GC.start\n"
                  );
  mrubybind::load_string(mrb,
                  "puts \"later...\"\n"
                  "call_old_f\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
//

class ClassValue{
public:
    int a;
    
    ClassValue(){
        std::cout << "ClassValue construct.\n";
        std::cout.flush();
        a = 7;
    }
    
    ~ClassValue(){
        std::cout << "ClassValue destruct.\n";
        std::cout.flush();
    }
    
    void decriment(){
        a--;
    }
};

std::shared_ptr<ClassValue> create_class_value()
{
    return std::shared_ptr<ClassValue>(new ClassValue());
    //return ClassValue();
}

void class_value_increment(std::shared_ptr<ClassValue> cv)
{
    cv->a++;
}

int class_value_get_a(std::shared_ptr<ClassValue> cv)
{
    return cv->a;
}

void class_value_decriment(std::shared_ptr<ClassValue> cv)
{
    cv->decriment();
}

std::weak_ptr<ClassValue> convert_to_weak_class_value(std::shared_ptr<ClassValue> cv){
    return cv;
}

int weak_class_value_get_a(std::weak_ptr<ClassValue> cv)
{
    if(auto ptr = cv.lock()){
        return ptr->a;
    }
    return 0;
}

void ClassPointerTest(mrb_state* mrb){
    
    {
        mrubybind::MrubyBind b(mrb);
        b.bind("create_class_value", create_class_value);
        b.bind_class<std::shared_ptr<ClassValue> >("ClassValue");
        b.bind_class<std::weak_ptr<ClassValue> >("WeakClassValue");
        b.bind("class_value_increment", class_value_increment);
        b.bind("class_value_get_a", class_value_get_a);
        b.bind_custom_method(NULL, "ClassValue", "decriment", class_value_decriment);
        b.bind("convert_to_weak_class_value", convert_to_weak_class_value);
        b.bind("weak_class_value_get_a", weak_class_value_get_a);
    }
    
    mrubybind::load_string(mrb, 
                    "puts \"start ClassPointerTest\"\n"
                    "cv = create_class_value\n"
                    "puts \"cv -> #{class_value_get_a(cv)}\"\n"
                    "class_value_increment(cv)\n"
                    "puts \"cv -> #{class_value_get_a cv}\"\n"
                    "cv.decriment\n"
                    "puts \"cv -> #{class_value_get_a cv}\"\n"
                    "wk = convert_to_weak_class_value cv\n"
                    "puts \"wk->#{weak_class_value_get_a wk}\"\n"
                    "cv = nil\n"
                    "puts \"GC\"\n"
                    "GC.start\n"
                    "puts \"wk->#{weak_class_value_get_a wk}\"\n"
                    );
    if (mrb->exc) {
        mrb_p(mrb, mrb_obj_value(mrb->exc));
    }
}

//=============================================================================
//

mrubybind::MrubyRef mruby_ref;
mrubybind::MrubyRef mruby_ref_a, mruby_ref_b;

void set_mruby_ref(mrubybind::MrubyRef r){
    mruby_ref = r;
}

void set_mruby_ref_pair(mrubybind::MrubyRef a, mrubybind::MrubyRef b){
    mruby_ref_a = a;
    mruby_ref_b = b;
}

void MrubyRefTest(mrb_state* mrb){

    {
        mrubybind::MrubyBind b(mrb);
        b.bind("set_mruby_ref", set_mruby_ref);
        b.bind("set_mruby_ref_pair", set_mruby_ref_pair);
        
    }
    
    mrubybind::load_string(mrb, 
                    "set_mruby_ref \"3test\"\n"
                    "s = \"aaa\"\n"
                    "set_mruby_ref_pair s, s\n"
                    "set_mruby_ref_pair :s, :s\n"
                    );
    //
    std::cout << "mruby_ref = " << mruby_ref.to_s() << std::endl;
    std::cout << "mruby_ref = " << mruby_ref.to_i() << std::endl;
    std::cout << "mruby_ref = " << mruby_ref.call("gsub", "te", "toa").to_s() << std::endl;
    std::cout << ":a == :a = " << mruby_ref_a.obj_equal(mruby_ref_b) << std::endl;
    std::cout << "arena_index = " << mrb_gc_arena_save(mrb) << std::endl;
    
    if (mrb->exc) {
        mrb_p(mrb, mrb_obj_value(mrb->exc));
    }
}


//=============================================================================
int main() {
  mrb_state* mrb = mrb_open();

  try{
    BindFunctionTest(mrb);
    BindClassTest(mrb);
    UseModuleTest(mrb);
    CallbackFunctionTest(mrb);
    ClassPointerTest(mrb);
    MrubyRefTest(mrb);
  }
  catch(std::runtime_error e){
    std::cout << "std::runtime_error -> " << e.what() << std::endl;
  }
  catch(...){
    std::cout << "unknown error!" << std::endl;
    std::cout.flush();
    throw;
  }
  

  mrb_close(mrb);
  return 0;
}
