#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"
#include <stdlib.h>

double mul(float x, double y) {
  return x * y;
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("mul", mul);
  }
  if (mrb_gc_arena_save(mrb) != 0) {
    fprintf(stderr, "Arena increased!\n");
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb, "puts mul(12.0, 34.0)");
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    return EXIT_FAILURE;
  }

  mrb_close(mrb);
  return EXIT_SUCCESS;
}
