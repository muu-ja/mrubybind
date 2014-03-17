#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
#include <string>
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

static mrb_state* mrb_callback = NULL;

std::string call_callback(std::function<void()> f) {
  if(f)
  {
      f();
  }
  //std::cout << "call call_callback\n";
  return "call_callback called\n";
}

std::string call_callback_a1(std::function<void(int a0)> f) {
  if(f)
  {
      f(23);
  }
  //std::cout << "call call_callback\n";
  return "call_callback called\n";
}

void CallbackFunctionTest(mrb_state* mrb) {
  {
    mrubybind::MrubyBind b(mrb);
    b.bind("call_callback", call_callback);
    b.bind("call_callback_a1", call_callback_a1);
  }

  mrb_callback = mrb;

  mrb_load_string(mrb,
                  "puts call_callback(lambda do\n"
                  "  puts \"??\"\n"
                  "end)\n"
                  "puts call_callback_a1(lambda do |a0|\n"
                  "  puts \"a0 = #{a0.to_s}\"\n"
                  "end)\n"
                  "puts emphasize('Hello, mruby!')\n"
                  );
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
