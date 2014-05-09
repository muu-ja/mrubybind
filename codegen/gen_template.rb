
MAX_PARAM = 10

HEADER = <<EOD
// This file is generated from gen_template.rb
\#define ARG(mrb, i)  Type<P##i>::get(mrb, args[i])
\#define ARGSHIFT(mrb, i, j)  Type<P##i>::get(mrb, args[j])
\#define CHECK(i)  {if(!Type<P##i>::check(mrb, args[i])) return RAISE(i);}
\#define CHECKSHIFT(i, j)  {if(!Type<P##i>::check(mrb, args[j])) return RAISE(j);}
\#define RAISE(i)  raise(mrb, i, Type<P##i>::TYPE_NAME, args[i])
\#define CHECKNARG(narg)  {if(narg != NPARAM) RAISENARG(narg);}
\#define RAISENARG(narg)  raisenarg(mrb, mrb_cfunc_env_get(mrb, 1), narg, NPARAM)

EOD

FUNC_TMPL = <<EOD
// void f(%PARAMS%);
template<%CLASSES0%>
struct Binder<void (*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM%;
  static mrb_value call(mrb_state* mrb, mrb_value /*self*/) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    mrb_value cfunc = mrb_cfunc_env_get(mrb, 0);
    void (*fp)(%PARAMS%) = (void (*)(%PARAMS%))mrb_cptr(cfunc);
    fp(%ARGS%);
    return mrb_nil_value();
  }
};

// R f(%PARAMS%);
template<class R%CLASSES1%>
struct Binder<R (*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM%;
  static mrb_value call(mrb_state* mrb, mrb_value /*self*/) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    mrb_value cfunc = mrb_cfunc_env_get(mrb, 0);
    R (*fp)(%PARAMS%) = (R (*)(%PARAMS%))mrb_cptr(cfunc);
    R result = fp(%ARGS%);
    return Type<R>::ret(mrb, result);
  }
};

// C* ctor(%PARAMS%);
template<class C%CLASSES1%>
struct ClassBinder<C* (*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM%;
  static mrb_value ctor(mrb_state* mrb, mrb_value self) {
    DATA_TYPE(self) = &ClassBinder<C>::type_info;
    DATA_PTR(self) = NULL;
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    mrb_value cfunc = mrb_cfunc_env_get(mrb, 0);
    C* (*ctor)(%PARAMS%) = (C* (*)(%PARAMS%))mrb_cptr(cfunc);
    if(ctor)
    {
      C* instance = ctor(%ARGS%);
      DATA_PTR(self) = instance;
    }
    return self;
  }
};

EOD

METHOD_TMPL = <<EOD
// class C { void f(%PARAMS%) };
template<class C%CLASSES1%>
struct ClassBinder<void (C::*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM%;
  static mrb_value call(mrb_state* mrb, mrb_value self) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    C* instance = static_cast<C*>(DATA_PTR(self));
    mrb_value cmethod = mrb_cfunc_env_get(mrb, 0);
    typedef void (C::*M)(%PARAMS%);
    M mp = *(M*)RSTRING_PTR(cmethod);
    (instance->*mp)(%ARGS%);
    return mrb_nil_value();
  }
};

// class C { R f(%PARAMS%) };
template<class C, class R%CLASSES1%>
struct ClassBinder<R (C::*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM%;
  static mrb_value call(mrb_state* mrb, mrb_value self) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    C* instance = static_cast<C*>(DATA_PTR(self));
    mrb_value cmethod = mrb_cfunc_env_get(mrb, 0);
    typedef R (C::*M)(%PARAMS%);
    M mp = *(M*)RSTRING_PTR(cmethod);
    R result = (instance->*mp)(%ARGS%);
    return Type<R>::ret(mrb, result);
  }
};

EOD

CUSTOM_METHOD_TMPL = <<EOD

// custom method
template<%CLASSES0%>
struct CustomClassBinder<void (*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM% - 1;
  static mrb_value call(mrb_state* mrb, mrb_value self) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    P0* instance = static_cast<P0*>(DATA_PTR(self));
    mrb_value cmethod = mrb_cfunc_env_get(mrb, 0);
    typedef void (*M)(%PARAMS%);
    M mp = *(M*)RSTRING_PTR(cmethod);
    mp(*instance%ARGS1%);
    return mrb_nil_value();
  }
};

template<class R%CLASSES1%>
struct CustomClassBinder<R (*)(%PARAMS%)> {
  static const int NPARAM = %NPARAM% - 1;
  static mrb_value call(mrb_state* mrb, mrb_value self) {
    mrb_value* targs;
    int narg;
    mrb_value block = mrb_nil_value();
    std::vector<mrb_value> args;
    mrb_get_args(mrb, "*|&", &targs, &narg, &block);
    args.resize(narg);
    if(narg > 0){
      ::memmove(&args[0], &targs[0], narg * sizeof(mrb_value));
    }
    if(mrb_test(block)){
      args.push_back(block);
      narg++;
    }
    %ASSERTS%
    P0* instance = static_cast<P0*>(DATA_PTR(self));
    mrb_value cmethod = mrb_cfunc_env_get(mrb, 0);
    typedef R (*M)(%PARAMS%);
    M mp = *(M*)RSTRING_PTR(cmethod);
    R result = mp(*instance%ARGS1%);
    return Type<R>::ret(mrb, result);
  }
};


EOD

FOOTER = <<EOD
\#undef ARG
\#undef CHECK
EOD

def embed_template(str, nparam)
  if nparam == 0
    params = 'void'
    args = ''
    classes = ''
    asserts = ''
  else
    params = (0...nparam).map {|i| "P#{i}"}.join(', ')
    args = (0...nparam).map {|i| "ARG(mrb, #{i})"}.join(', ')
    classes = (0...nparam).map {|i| "class P#{i}"}.join(', ')
    asserts = (0...nparam).map {|i| " CHECK(#{i});"}.join('')
  end

  table = {
    '%PARAMS%' => params,
    '%NPARAM%' => nparam.to_s,
    '%ARGS%' => args,
    '%CLASSES0%' => classes,
    '%CLASSES1%' => classes.empty? ? '' : ', ' + classes,
    '%ASSERTS%' => 'CHECKNARG(narg);' + asserts
  }

  return str.gsub(/(#{table.keys.join('|')})/) {|k| table[k]}
end

def embed_template_custom(str, nparam)
  if nparam == 0
    params = 'void'
    args = ''
    classes = ''
    asserts = ''
  else
    params = (0...nparam).map {|i| "P#{i}"}.join(', ')
    args = (1...nparam).map {|i| "ARGSHIFT(mrb, #{i}, #{i - 1})"}.join(', ')
    classes = (0...nparam).map {|i| "class P#{i}"}.join(', ')
    asserts = (1...nparam).map {|i| " CHECKSHIFT(#{i}, #{i - 1});"}.join('')
  end

  table = {
    '%PARAMS%' => params,
    '%NPARAM%' => nparam.to_s,
    '%ARGS%' => args,
    '%ARGS1%' => args.empty? ? '' : ', ' + args,
    '%CLASSES0%' => classes,
    '%CLASSES1%' => classes.empty? ? '' : ', ' + classes,
    '%ASSERTS%' => 'CHECKNARG(narg);' + asserts
  }

  return str.gsub(/(#{table.keys.join('|')})/) {|k| table[k]}
end

print HEADER
(0..MAX_PARAM).each do |nparam|
  print embed_template(FUNC_TMPL, nparam)
  print embed_template(METHOD_TMPL, nparam)
  print embed_template_custom(CUSTOM_METHOD_TMPL, nparam) if nparam != 0
end
print FOOTER

#
