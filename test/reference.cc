
#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

class V{
public:
    int a;
    V(){
        a = 0;
    }
};

V create_v()
{
    return V();
}

void add_v(V& v)
{
    v.a++;
}

int v_a(V& v)
{
    return v.a;
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);;
    b.bind_class<V>("V");
    b.bind("create_v", create_v);
    b.bind("add_v", add_v);
    b.bind("v_a", v_a);
  }

  int result_code = EXIT_SUCCESS;
  mrb_load_string(mrb,
                  "puts \"start ReferenceTest\"\n"
                  "v = create_v\n"
                  "add_v v\n"
                  "puts \"v_a = #{v_a v}\"\n"
                  "add_v v\n"
                  "puts \"v_a = #{v_a v}\"\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return result_code;
}


