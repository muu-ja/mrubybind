// Simple usage for binding function under some module.

#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"
#include <stdlib.h>


class ModClass {
public:
  int a;
  
  ModClass(int a)
  {
    this->a = a;
  }
};

std::shared_ptr<ModClass> create_mod_class(int a)
{
    return std::shared_ptr<ModClass>(new ModClass(a));
}

int mod_class_get_a(std::shared_ptr<ModClass> c)
{
    return c->a;
}

void modfunc(int v) {
  printf("modfunc called: %d\n", v);
}

int main() {
  mrb_state* mrb = mrb_open();

  RClass* mod = mrb_define_module(mrb, "Mod");
  int arena = mrb_gc_arena_save(mrb);
  {
    mrubybind::MrubyBind b(mrb, mod);
    b.bind("modfunc", modfunc);
    b.bind_const("FOO_VALUE", 1234);
    
    b.bind_class<std::shared_ptr<ModClass> >("Mod", "ModClass");
    b.bind_const("Mod", "ModClass", "CNST", 12345);
    b.bind_static_method("Mod", "ModClass", "create", create_mod_class);
    b.bind_custom_method("Mod", "ModClass", "get_a", mod_class_get_a);
  }
  if (mrb_gc_arena_save(mrb) != arena) {
    fprintf(stderr, "Arena increased!\n");
    return EXIT_FAILURE;
  }

  mrb_load_string(mrb, "Mod.modfunc(Mod::FOO_VALUE)\n"
            "c = Mod::ModClass.create 4\n"
            "puts \"Mod::ModClass::CNST #{Mod::ModClass::CNST}\"\n"
            "puts \"c.get_a #{c.get_a}\"\n"
        );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    return EXIT_FAILURE;
  }

  mrb_close(mrb);
  return EXIT_SUCCESS;
}
