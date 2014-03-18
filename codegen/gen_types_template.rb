
MAX_PARAM = 10

HEADER = <<EOD
// This file is generated from gen_template.rb
\#define ARG(mrb, i)  Type<P##i>::get(mrb, args[i])
\#define CHECK(i)  {if(!Type<P##i>::check(args[i])) return RAISE(i);}
\#define RAISE(i)  raise(mrb, i, Type<P##i>::TYPE_NAME, args[i])

EOD

TYPE_TMPL = <<EOD

// callback R(%PARAMS%)
template<class R, %CLASSES%>
struct Type<sp_mrb_func<R(%PARAMS%)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static sp_mrb_func<R(%PARAMS%)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(%PARAMS%)> > d = set_avoid_gc<std::function<R(%PARAMS%)> >(mrb, v);
    return make_sp_mrb_func<R(%PARAMS%)>(d, [=](%ARGS%){
      mrb_value a[] = {%ARG_VALS%};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, %NPARAM%, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, sp_mrb_func<R(%PARAMS%)> p) {
      // don't call.
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(%PARAMS%)
template<%CLASSES%>
struct Type<sp_mrb_func<void(%PARAMS%)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static sp_mrb_func<void(%PARAMS%)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(%PARAMS%)> > d = set_avoid_gc<std::function<void(%PARAMS%)> >(mrb, v);
    return make_sp_mrb_func<void(%PARAMS%)>(d, [=](%ARGS%){
      mrb_value a[] = {%ARG_VALS%};
      mrb_yield_argv(mrb, v, %NPARAM%, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, sp_mrb_func<void(%PARAMS%)> p) {
      // don't call.
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

EOD


FOOTER = <<EOD
\#undef ARG
\#undef CHECK
EOD

def embed_template(str, nparam)
  params = (0...nparam).map {|i| "P#{i}"}.join(', ')
  classes = (0...nparam).map {|i| "class P#{i}"}.join(', ')
  arg_vals = (0...nparam).map {|i| "Type<P#{i}>::ret(mrb, a#{i})"}.join(', ')
  args = (0...nparam).map {|i| "P#{i} a#{i}"}.join(', ')

  table = {
      '%PARAMS%' => params,
      '%NPARAM%' => nparam.to_s,
      '%ARG_VALS%' => arg_vals,
      '%CLASSES%' => classes,
      '%ARGS%' => args,
    }

  return str.gsub(/(#{table.keys.join('|')})/) {|k| table[k]}
end

print HEADER
(1..MAX_PARAM).each do |nparam|
  print embed_template(TYPE_TMPL, nparam)
end
print FOOTER

