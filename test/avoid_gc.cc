
#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

typedef mrubybind::FuncPtr<void(void)> Func;

Func f0;
Func f1;

void set_f0(Func f)
{
    f0 = f;
}

void set_f1(Func f)
{
    f1 = f;
}

void clear_f0()
{
    f0.reset();
}

void call_f1()
{
    f1.func()();
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("set_f0", set_f0);
    b.bind("set_f1", set_f1);
    b.bind("clear_f0", clear_f0);
    b.bind("call_f1", call_f1);
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
                  "end\n"
                  "f\n"
                  "clear_f0\n"
                  "GC.start\n"
                  "call_f1\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return result_code;
}




