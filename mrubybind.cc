#include "mrubybind.h"
#include "mruby/compile.h"
#include "mruby/dump.h"
#include "mruby/proc.h"
#include "mruby/string.h"
#include "mruby/variable.h"

namespace mrubybind {

const char Type<int>::TYPE_NAME[] = "Fixnum";
const char Type<unsigned int>::TYPE_NAME[] = "Fixnum";
const char Type<float>::TYPE_NAME[] = "Float";
const char Type<double>::TYPE_NAME[] = "Float";
const char Type<const char*>::TYPE_NAME[] = "String";
const char Type<std::string>::TYPE_NAME[] = "String";
const char Type<const std::string>::TYPE_NAME[] = "String";
const char Type<const std::string&>::TYPE_NAME[] = "String";
const char Type<bool>::TYPE_NAME[] = "Bool";
const char Type<void*>::TYPE_NAME[] = "Voidp";
const char Type<std::function<void()> >::TYPE_NAME[] = "Func";
const char TypeFuncBase::TYPE_NAME[] = "Func";

mrb_value raise(mrb_state *mrb, int parameter_index,
                const char* required_type_name, mrb_value value) {
  const char * argument_class_name = mrb_obj_classname(mrb, value);
  mrb_raisef(mrb, E_TYPE_ERROR, "can't convert %S into %S, argument %S(%S)",
             mrb_str_new_cstr(mrb, argument_class_name),
             mrb_str_new_cstr(mrb, required_type_name),
             mrb_fixnum_value(parameter_index + 1), value);
  return mrb_nil_value();
}

mrb_value raisenarg(mrb_state *mrb, mrb_value func_name, int narg, int nparam) {
  mrb_raisef(mrb, E_ARGUMENT_ERROR, "'%S': wrong number of arguments (%S for %S)",
             func_name,
             mrb_fixnum_value(narg),
             mrb_fixnum_value(nparam));
  return mrb_nil_value();
}


void
MrubyBind::mrb_define_class_method_raw(mrb_state *mrb, struct RClass *c, mrb_sym mid, struct RProc *p)
{
  mrb_define_class_method(mrb, c, mrb_sym2name(mrb, mid), NULL, MRB_ARGS_ANY());  // Dummy registration.
  mrb_define_method_raw(mrb, ((RObject*)c)->c, mid, p);
}


MrubyBind::MrubyBind(mrb_state* mrb) : mrb_(mrb), mod_(mrb_->kernel_module) {
  Initialize();
}

MrubyBind::MrubyBind(mrb_state* mrb, RClass* mod) : mrb_(mrb), mod_(mod) {
  Initialize();
}

MrubyBind::~MrubyBind() {
  mrb_gc_arena_restore(mrb_, arena_index_);
}

void MrubyBind::Initialize() {
  arena_index_ = mrb_gc_arena_save(mrb_);
}

struct RClass* MrubyBind::GetClass(const char* class_name) {
  mrb_value mod = mrb_obj_value(mod_);
  mrb_value klass_v = mrb_const_get(mrb_, mod, mrb_intern_cstr(mrb_, class_name));
  return mrb_class_ptr(klass_v);
}

void MrubyBind::BindInstanceMethod(
    const char* class_name, const char* method_name,
    mrb_value original_func_v,
    mrb_value (*binder_func)(mrb_state*, mrb_value)) {
  mrb_sym method_name_s = mrb_intern_cstr(mrb_, method_name);
  mrb_value env[] = {
    original_func_v,                  // 0: c function pointer
    mrb_symbol_value(method_name_s),  // 1: method name
  };
  struct RProc* proc = mrb_proc_new_cfunc_with_env(mrb_, binder_func, 2, env);
  struct RClass* klass = GetClass(class_name);
  mrb_define_method_raw(mrb_, klass, method_name_s, proc);
}

}  // namespace mrubybind
