
#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

typedef mrubybind::FuncPtr<void(void)> Func;

Func f0;
Func f1;
Func f2;

void set_f0(Func f)
{
    f0 = f;
}

void set_f1(Func f)
{
    f1 = f;
}

void set_f2(Func f)
{
    f2 = f;
}

void clear_f0()
{
    f0.reset();
}

void clear_f1()
{
    f1.reset();
}

void clear_f2()
{
    f2.reset();
}

void call_f2()
{
    f2.func()();
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("set_f0", set_f0);
    b.bind("set_f1", set_f1);
    b.bind("set_f2", set_f2);
    b.bind("clear_f0", clear_f0);
    b.bind("clear_f1", clear_f1);
    b.bind("clear_f2", clear_f2);
    b.bind("call_f2", call_f2);
  }

  int result_code = EXIT_SUCCESS;
  mrb_load_string(mrb,
                  "puts \"start avoid_gc\"\n"
                  "def f\n"
                  "  fa = Proc.new do\n"
                  "    puts \"called!\"\n"
                  "  end\n"
                  "  set_f0 fa\n"
                  "  set_f1 fa\n"
                  "  set_f2 fa\n"
                  "end\n"
                  "f\n"
                  "clear_f0\n"
                  "clear_f1\n"
                  "GC.start\n"
                  "call_f2\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
    mrb_close(mrb);
    return result_code;
  }
  
  mrb_load_string(mrb,
                  "puts \"start avoid_gc 2\"\n"
                  "def f(&block)\n"
                  "  fa = Proc.new do\n"
                  "    clear_f2\n"
                  "    GC.start\n"
                  "    puts \"called!\"\n"
                  "    yield\n"
                  "  end\n"
                  "  set_f0 fa\n"
                  "  set_f1 fa\n"
                  "  set_f2 fa\n"
                  "  fa = nil\n"
                  "end\n"
                  "def g\n"
                  "  f do\n"
                  "    puts \"block called!\"\n"
                  "  end\n"
                  "end\n"
                  "g\n"
                  "clear_f0\n"
                  "clear_f1\n"
                  "GC.start\n"
                  "call_f2\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
    mrb_close(mrb);
    return result_code;
  }

  mrb_close(mrb);
  return result_code;
}




