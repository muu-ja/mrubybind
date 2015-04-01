#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"
#include <stdlib.h>

void* myopen() {
  return stdout;
}

void mywrite(void* fp, const char* string) {
  fwrite(string, 1, strlen(string), (FILE*)fp);
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("myopen", myopen);
    b.bind("mywrite", mywrite);
  }
  if (mrb_gc_arena_save(mrb) != 0) {
    fprintf(stderr, "Arena increased!\n");
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb,
                  "f = myopen()\n"
                  "mywrite(f, 'cptr test')");
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    return EXIT_FAILURE;
  }

  mrb_close(mrb);
  return EXIT_SUCCESS;
}
