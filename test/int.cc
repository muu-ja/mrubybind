#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"
#include <stdlib.h>

int square(int x) {
  return x * x;
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("square", square);
  }
  if (mrb_gc_arena_save(mrb) != 0) {
    fprintf(stderr, "Arena increased!\n");
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb, "puts square(1111)");
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    return EXIT_FAILURE;
  }

  mrb_close(mrb);
  return EXIT_SUCCESS;
}
