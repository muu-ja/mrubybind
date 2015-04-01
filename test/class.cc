// Simple usage for binding C++ class.

#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

class Foo {
public:
  Foo(int x) : x_(x) {
    cout << "Foo::ctor(" << x << ")" << endl;
  }
  virtual ~Foo() {
    cout << "Foo::dtor()" << endl;
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

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind_class("Foo", new_foo);
    b.bind_instance_method("Foo", "bar", &Foo::bar);
    b.bind_static_method("Foo", "baz", &Foo::baz);
  }
  if (mrb_gc_arena_save(mrb) != 0) {
    fprintf(stderr, "Arena increased!\n");
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb,
                  "foo = Foo.new(123)\n"
                  "p foo.bar(567)\n"
                  "p Foo.baz(9999)"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    return EXIT_FAILURE;
  }

  mrb_close(mrb);
  return EXIT_SUCCESS;
}
