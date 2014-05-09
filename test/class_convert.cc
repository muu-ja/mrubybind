#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

class TA{
    int num;
public:
    TA(){
        num = 1;
    }
    
    virtual int get_num(){
        return num;
    }

};

class TB : public TA{
    int num2;
public:
    TB(){
        num2 = 2;
    }
    
    virtual int get_num(){
        return num2;
    }

};

std::shared_ptr<TA> create_a()
{
    return std::make_shared<TA>();
}

std::shared_ptr<TB> create_b()
{
    return std::make_shared<TB>();
}

int get_num_a(std::shared_ptr<TA> a)
{
    return a->get_num();
}

int get_num_b(std::shared_ptr<TB> b)
{
    return b->get_num();
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind_class<std::shared_ptr<TA> >("TA");
    b.bind_class<std::shared_ptr<TB> >("TB");
    b.bind_static_method("TA", "create", create_a);
    b.bind_static_method("TB", "create", create_b);
    b.bind_static_method("TA", "get_num", get_num_a);
    b.bind_static_method("TB", "get_num", get_num_b);
    b.add_convertable("TA", "TB");
  }

  int result_code = EXIT_SUCCESS;
  mrb_load_string(mrb,
                  "a = TA.create\n"
                  "b = TB.create\n"
                  "puts \"TA -> a = #{TA.get_num a}\"\n"
                  "puts \"TB -> b = #{TB.get_num b}\"\n"
                  "puts \"TA -> b = #{TA.get_num b}\"\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return result_code;
}

