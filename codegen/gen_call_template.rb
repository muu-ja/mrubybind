

MAX_PARAM = 17

HEADER = <<EOD
// This file is generated from gen_template.rb
\#define ARG(mrb, i)  Type<P##i>::get(mrb, args[i])
\#define CHECK(i)  {if(!Type<P##i>::check(mrb, args[i])) return RAISE(i);}
\#define RAISE(i)  raise(mrb, i, Type<P##i>::TYPE_NAME, args[i])

EOD

TMPL = <<EOD

    template<%CLASSES%>MrubyRef call(%ARGS%){
        MrubyArenaStore mas(mrb);
        //%ASSERTS%
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), %ARG_VALS%));
    }

EOD

FOOTER = <<EOD
\#undef ARG
\#undef CHECK
EOD

def embed_template(str, nparam)
  if nparam == 0
    params = 'void'
    args = 'std::string name'
    classes = ''
    asserts = ''
    arg_vals = '0'
  else
    params = (0...nparam).map {|i| "P#{i}"}.join(', ')
    args = (['std::string name'] + (0...nparam).map {|i| "P#{i} a#{i}"}).join(', ')
    classes = (0...nparam).map {|i| "class P#{i}"}.join(', ')
    asserts = (0...nparam).map {|i| "(void)(narg); CHECK(#{i});"}.join(' ')
    arg_vals = ([nparam] + (0...nparam).map {|i| "Type<P#{i}>::ret(mrb, a#{i})"}).join(', ')
  end

  table = {
    '%PARAMS%' => params,
    '%NPARAM%' => nparam.to_s,
    '%ARGS%' => args,
    '%CLASSES%' => classes,
    '%ASSERTS%' => asserts,
    '%ARG_VALS%' => arg_vals,
  }

  return str.gsub(/(#{table.keys.join('|')})/) {|k| table[k]}
end

print HEADER
(1..MAX_PARAM).each do |nparam|
  print embed_template(TMPL, nparam)
end
print FOOTER
