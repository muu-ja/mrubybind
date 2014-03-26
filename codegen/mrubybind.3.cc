
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
const char Type<MrubyRef>::TYPE_NAME[] = "MrubyRef";
const char TypeFuncBase::TYPE_NAME[] = "Func";
const char TypeClassBase::TYPE_NAME[] = "CppClass";

const char* untouchable_table = "__ untouchable table __";
const char* untouchable_object = "__ untouchable object __";
const char* untouchable_last_exception = "__ untouchable last exception __";

mrb_value raise(mrb_state *mrb, int parameter_index,
                const char* required_type_name, mrb_value value) {
  const char * argument_class_name = mrb_obj_classname(mrb, value);
  mrb_raisef(mrb, E_TYPE_ERROR, "can't convert %S into %S, argument %S(%S)",
             mrb_str_new_cstr(mrb, argument_class_name),
             mrb_str_new_cstr(mrb, required_type_name),
             mrb_fixnum_value(parameter_index + 1), value);
  return mrb_nil_value();
}

static mrb_value call_cfunc(mrb_state *mrb, mrb_value /*self*/) {
  mrb_value binder;
  mrb_value func_ptr_v;
  mrb_value* args;
  int narg;
  mrb_get_args(mrb, "oo*", &binder, &func_ptr_v, &args, &narg);
  typedef mrb_value (*BindFunc)(mrb_state*, void*, mrb_value*, int);
  BindFunc binderp = reinterpret_cast<BindFunc>(mrb_voidp(binder));
  return binderp(mrb, mrb_voidp(func_ptr_v), args, narg);
}

static mrb_value call_ctorfunc(mrb_state *mrb, mrb_value self) {
  mrb_value binder;
  mrb_value self_v;
  mrb_value new_func_ptr_v;
  mrb_value* args;
  int narg;
  mrb_get_args(mrb, "ooo*", &binder, &self_v, &new_func_ptr_v, &args, &narg);
  typedef mrb_value (*BindFunc)(mrb_state*, mrb_value, void*, mrb_value*, int);
  BindFunc binderp = reinterpret_cast<BindFunc>(mrb_voidp(binder));
  binderp(mrb, self_v, mrb_voidp(new_func_ptr_v), args, narg);
  return self;
}

static mrb_value call_imethod(mrb_state *mrb, mrb_value /*self*/) {
  mrb_value binder;
  mrb_value self_v;
  mrb_value method_pptr_v;
  mrb_value* args;
  int narg;
  mrb_get_args(mrb, "ooS*", &binder, &self_v, &method_pptr_v, &args, &narg);
  typedef mrb_value (*BindFunc)(mrb_state*, mrb_value, void*, mrb_value*, int);
  BindFunc binderp = reinterpret_cast<BindFunc>(mrb_voidp(binder));
  return binderp(mrb, self_v, RSTRING_PTR(method_pptr_v), args, narg);
}

static mrb_value call_smethod(mrb_state *mrb, mrb_value self) {
  mrb_value binder;
  mrb_value method_pptr_v;
  mrb_value* args;
  int narg;
  mrb_get_args(mrb, "oS*", &binder, &method_pptr_v, &args, &narg);
  typedef mrb_value (*BindFunc)(mrb_state*, mrb_value, void*, mrb_value*, int);
  BindFunc binderp = reinterpret_cast<BindFunc>(mrb_voidp(binder));
  return binderp(mrb, self, RSTRING_PTR(method_pptr_v), args, narg);
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
  mrb_sym sym_mrubybind = mrb_intern_cstr(mrb_, "MrubyBind");
  RClass* mrubybind = mrb_define_module(mrb_, "MrubyBind");
  if (mrb_const_defined(mrb_, mrb_obj_value(mrb_->kernel_module),
                        sym_mrubybind)) {
    mod_mrubybind_ = mrb_const_get(mrb_, mrb_obj_value(mrb_->kernel_module),
                                   sym_mrubybind);
    avoid_gc_table_ = mrb_obj_iv_get(mrb_, (RObject*)mrubybind,
                                     mrb_intern_cstr(mrb_, untouchable_table));
  } else {
    mrb_const_set(mrb_, mrb_obj_value(mrb_->kernel_module), sym_mrubybind, mrb_obj_value(mrubybind));
  
    mod_mrubybind_ = mrb_obj_value(mrubybind);
    mrb_define_module_function(mrb_, mrubybind, "call_cfunc", call_cfunc,
                               ARGS_REQ(2) | ARGS_REST());
    mrb_define_module_function(mrb_, mrubybind, "call_ctorfunc", call_ctorfunc,
                               ARGS_REQ(3) | ARGS_REST());
    mrb_define_module_function(mrb_, mrubybind, "call_imethod", call_imethod,
                               ARGS_REQ(3) | ARGS_REST());
    mrb_define_module_function(mrb_, mrubybind, "call_smethod", call_smethod,
                               ARGS_REQ(2) | ARGS_REST());
    mrb_irep* irep = mrb_read_irep(mrb_, binder);
    if (irep != NULL) {
      mrb_run(mrb_, mrb_proc_new(mrb_, irep), mrb_top_self(mrb_));
    }
    
    {
        avoid_gc_table_ = mrb_hash_new(mrb_);
        mrb_obj_iv_set(mrb_, (RObject*)mrubybind,
                   mrb_intern_cstr(mrb_, untouchable_table), avoid_gc_table_);
        std::shared_ptr<MrubyBindStatus> mrbs = std::make_shared<MrubyBindStatus>(mrb_, avoid_gc_table_);
        mrubybind::MrubyBind b(mrb_);
        b.bind_class<std::shared_ptr<MrubyBindStatus> >("MrubyBind", "MrubyBindStatusPtr");
        mrb_value msbpv = Type<std::shared_ptr<MrubyBindStatus> >::ret(mrb_, mrbs);
        mrb_obj_iv_set(mrb_, (RObject*)mrubybind,
                       mrb_intern_cstr(mrb_, untouchable_object), msbpv);
    }
  }
}

MrubyRef load_string(mrb_state* mrb, std::string code)
{
    mrubybind::MrubyArenaStore mas(mrb);
    
    RClass* mrubybind = mrb_define_module(mrb, "MrubyBind");
    mrb->exc = NULL;
    mrb_value r = mrb_load_string(mrb,
                    code.c_str());
    if(mrb->exc){
        mrb_obj_iv_set(mrb, (RObject*)mrubybind,
                       mrb_intern_cstr(mrb, untouchable_last_exception), mrb_obj_value(mrb->exc));
        r = mrb_nil_value();
    }
    else{
        mrb_obj_iv_set(mrb, (RObject*)mrubybind,
                       mrb_intern_cstr(mrb, untouchable_last_exception), mrb_nil_value());
    }
    
    return MrubyRef(mrb, r);
}

MrubyRef::MrubyRef(){
    
}

MrubyRef::MrubyRef(mrb_state* mrb, const mrb_value& v){
    this->mrb = mrb;
    mrb_value* p = new mrb_value();
    *p = v;
    this->v = std::shared_ptr<mrb_value>(p, set_avoid_gc<mrb_value>(mrb, v));
}

MrubyRef::~MrubyRef(){
    
}

mrb_value MrubyRef::get_v() const{
    if(v.get()){
        return *(this->v.get());
    }
    return mrb_nil_value();
}

bool MrubyRef::empty() const{
    if(!v.get()){
        return true;
    }
    return false;
}

bool MrubyRef::test() const{
    if(v.get()){
        mrb_value v = *(this->v.get());
        return mrb_test(v);
    }
    return false;
}

bool MrubyRef::obj_equal(const MrubyRef& r) const{
    if(!empty() && !r.empty()){
        return mrb_obj_equal(mrb, get_v(), r.get_v());
    }
    else{
        return empty() == r.empty();
    }
}

std::string MrubyRef::to_s() const{
    if(v.get()){
        MrubyArenaStore mas(mrb);
        mrb_value v = *(this->v.get());
        if(mrb_string_p(v)){
            return std::string(RSTRING_PTR(v), RSTRING_LEN(v));
        }
        else{
            v = mrb_funcall(mrb, v, "to_s", 0);
            return std::string(RSTRING_PTR(v), RSTRING_LEN(v));
        }
    }
    else{
        return "";
    }
}

int MrubyRef::to_i() const{
    if(v.get()){
        mrb_value v = *(this->v.get());
        if(mrb_fixnum_p(v)){
            return mrb_fixnum(v);
        }
        else{
            v = mrb_funcall(mrb, v, "to_i", 0);
            return mrb_fixnum(v);
        }
    }
    else{
        return 0;
    }
}

float MrubyRef::to_float() const{
    if(v.get()){
        mrb_value v = *(this->v.get());
        if(mrb_float_p(v)){
            return mrb_float(v);
        }
        else{
            v = mrb_funcall(mrb, v, "to_f", 0);
            return mrb_float(v);
        }
    }
    else{
        return 0.0f;
    }
}

double MrubyRef::to_double() const{
    if(v.get()){
        mrb_value v = *(this->v.get());
        if(mrb_float_p(v)){
            return mrb_float(v);
        }
        else{
            v = mrb_funcall(mrb, v, "to_f", 0);
            return (double)mrb_float(v);
        }
    }
    else{
        return 0.0;
    }
}


}  // namespace mrubybind
