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

  mrb_load_string(mrb,
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

  mrb_load_string(mrb,
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
    RClass* mod = mrb_define_module(mrb, "Mod");
    mrubybind::MrubyBind b(mrb, mod);
    b.bind("modfunc", modfunc);
    b.bind_const("FOO_VALUE", 1234);
  }

  mrb_load_string(mrb,
                  "Mod.modfunc(Mod::FOO_VALUE)\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
//


std::string call_callback(mrubybind::sp_mrb_func<void()> f) {
  if(f)
  {
      cout << "pre f\n";
      f.func()();
      cout << "post f\n";
  }
  return "call_callback called\n";
}

std::string call_callback_a1(mrubybind::sp_mrb_func<void(int a0)> f) {
  if(f)
  {
      f.func()(23);
  }
  return "call_callback_a1 called\n";
}

std::string call_callback_a2(mrubybind::sp_mrb_func<void(int a0, std::string a1)> f) {
  if(f)
  {
      f.func()(23, "string");
  }
  return "call_callback_a2 called\n";
}

std::string call_callback_a1_int(mrubybind::sp_mrb_func<int(int a0)> f) {
  std::stringstream s;
  s << "call_callback_a1_int return this ->" << f.func()(23);
  return  s.str();
}

mrubybind::sp_mrb_func<void()> old_f;

void set_old_f(mrubybind::sp_mrb_func<void()> f) {
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

    int func_test(mrubybind::sp_mrb_func<void(int a0)> f)
    {
        f.func()(a);
        return a;
    }

    std::string func_a2_string(mrubybind::sp_mrb_func<std::string(int a0, std::string a1)> f) {
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

  mrb_load_string(mrb,
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
  mrb_load_string(mrb,
                  "GC.start\n"
                  );
  mrb_load_string(mrb,
                  "puts \"later...\"\n"
                  "call_old_f\n"
                  );
  old_f.reset();
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
  }
}

//=============================================================================
int main() {
  mrb_state* mrb = mrb_open();

  BindFunctionTest(mrb);
  BindClassTest(mrb);
  UseModuleTest(mrb);
  CallbackFunctionTest(mrb);

  mrb_close(mrb);
  return 0;
}
