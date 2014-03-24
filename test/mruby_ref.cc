
#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

mrubybind::MrubyRef mruby_ref;
mrubybind::MrubyRef mruby_ref_a, mruby_ref_b;

void set_mruby_ref(mrubybind::MrubyRef r){
    mruby_ref = r;
}

void set_mruby_ref_pair(mrubybind::MrubyRef a, mrubybind::MrubyRef b){
    mruby_ref_a = a;
    mruby_ref_b = b;
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("set_mruby_ref", set_mruby_ref);
    b.bind("set_mruby_ref_pair", set_mruby_ref_pair);
  }

  int result_code = EXIT_SUCCESS;
  mrubybind::load_string(mrb, 
                    "set_mruby_ref \"3test\"\n"
                    "s = \"aaa\"\n"
                    "set_mruby_ref_pair s, s\n"
                    "set_mruby_ref_pair :s, :s\n"
                    );
  std::cout << "mruby_ref = " << mruby_ref.to_s() << std::endl;
  std::cout << "mruby_ref = " << mruby_ref.to_i() << std::endl;
  std::cout << "mruby_ref = " << mruby_ref.call("gsub", "te", "toa").to_s() << std::endl;
  std::cout << ":a == :a = " << mruby_ref_a.obj_equal(mruby_ref_b) << std::endl;
  std::cout << "arena_index = " << mrb_gc_arena_save(mrb) << std::endl;
  
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return result_code;
}

