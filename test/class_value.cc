

#include <mruby.h>
#include <mruby/compile.h>
#include "mrubybind.h"

#include <iostream>
using namespace std;

class ClassValue{
public:
    int a;
    mrubybind::FuncPtr<std::string(int a0, std::string a1)> block;
    
    ClassValue(){
        std::cout << "ClassValue construct.\n";
        a = 7;
    }
    
    ~ClassValue(){
        std::cout << "ClassValue destruct.\n";
    }
    
    void decriment(){
        a--;
    }
    
    void set_block(mrubybind::FuncPtr<std::string(int a0, std::string a1)> block){
        this->block = block;
    }
    
    std::string call_block(){
        return block.func()(a, "test");
    }
};

std::shared_ptr<ClassValue> create_class_value()
{
    return std::shared_ptr<ClassValue>(new ClassValue());
}

void class_value_increment(std::shared_ptr<ClassValue> cv)
{
    cv->a++;
}

int class_value_get_a(std::shared_ptr<ClassValue> cv)
{
    return cv->a;
}

void class_value_decriment(std::shared_ptr<ClassValue> cv)
{
    cv->decriment();
}

std::weak_ptr<ClassValue> convert_to_weak_class_value(std::shared_ptr<ClassValue> cv){
    return cv;
}

int weak_class_value_get_a(std::weak_ptr<ClassValue> cv)
{
    if(auto ptr = cv.lock()){
        return ptr->a;
    }
    return 0;
}

void class_value_set_block(std::shared_ptr<ClassValue> cv, mrubybind::FuncPtr<std::string(int a0, std::string a1)> block)
{
    cv->set_block(block);
}

std::string class_value_call_block(std::shared_ptr<ClassValue> cv)
{
    return cv->call_block();
}

int main() {
  mrb_state* mrb = mrb_open();

  {
    mrubybind::MrubyBind b(mrb);
    b.bind("create_class_value", create_class_value);
    b.bind_class<std::shared_ptr<ClassValue> >("ClassValue");
    b.bind_class<std::weak_ptr<ClassValue> >("WeakClassValue");
    b.bind("class_value_increment", class_value_increment);
    b.bind("class_value_get_a", class_value_get_a);
    b.bind_custom_method(NULL, "ClassValue", "decriment", class_value_decriment);
    b.bind("convert_to_weak_class_value", convert_to_weak_class_value);
    b.bind("weak_class_value_get_a", weak_class_value_get_a);
    b.bind_custom_method("ClassValue", "set_block", class_value_set_block);
    b.bind_custom_method("ClassValue", "call_block", class_value_call_block);
  }

  int result_code = EXIT_SUCCESS;
  mrb_load_string(mrb,
                  "puts \"start ClassPointerTest\"\n"
                  "cv = create_class_value\n"
                  "puts \"cv -> #{class_value_get_a(cv)}\"\n"
                  "class_value_increment(cv)\n"
                  "puts \"cv -> #{class_value_get_a cv}\"\n"
                  "cv.decriment\n"
                  "puts \"cv -> #{class_value_get_a cv}\"\n"
                  "cv.set_block do |a0, a1|\n"
                  "  a1.to_s + a0.to_s\n"
                  "end\n"
                  "puts \"cv.call_block -> #{cv.call_block}\"\n"
                  "wk = convert_to_weak_class_value cv\n"
                  "puts \"wk->#{weak_class_value_get_a wk}\"\n"
                  "cv = nil\n"
                  "puts \"GC\"\n"
                  "GC.start\n"
                  "puts \"wk->#{weak_class_value_get_a wk}\"\n"
                  );
  if (mrb->exc) {
    mrb_p(mrb, mrb_obj_value(mrb->exc));
    result_code = EXIT_FAILURE;
  }

  mrb_close(mrb);
  return result_code;
}


