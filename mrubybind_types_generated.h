// This file is generated from gen_template.rb
#define ARG(mrb, i)  Type<P##i>::get(mrb, args[i])
#define CHECK(i)  {if(!Type<P##i>::check(args[i])) return RAISE(i);}
#define RAISE(i)  raise(mrb, i, Type<P##i>::TYPE_NAME, args[i])


// callback R(P0)
template<class R, class P0>
struct Type<FuncPtr<R(P0)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0)> > d = set_avoid_gc<std::function<R(P0)> >(mrb, v);
    return make_FuncPtr<R(P0)>(d, [=](P0 a0){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 1, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0)
template<class P0>
struct Type<FuncPtr<void(P0)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0)> > d = set_avoid_gc<std::function<void(P0)> >(mrb, v);
    return make_FuncPtr<void(P0)>(d, [=](P0 a0){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0)};
      mrb_yield_argv(mrb, v, 1, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1)
template<class R, class P0, class P1>
struct Type<FuncPtr<R(P0, P1)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1)> > d = set_avoid_gc<std::function<R(P0, P1)> >(mrb, v);
    return make_FuncPtr<R(P0, P1)>(d, [=](P0 a0, P1 a1){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 2, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1)
template<class P0, class P1>
struct Type<FuncPtr<void(P0, P1)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1)> > d = set_avoid_gc<std::function<void(P0, P1)> >(mrb, v);
    return make_FuncPtr<void(P0, P1)>(d, [=](P0 a0, P1 a1){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1)};
      mrb_yield_argv(mrb, v, 2, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2)
template<class R, class P0, class P1, class P2>
struct Type<FuncPtr<R(P0, P1, P2)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2)> > d = set_avoid_gc<std::function<R(P0, P1, P2)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2)>(d, [=](P0 a0, P1 a1, P2 a2){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 3, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2)
template<class P0, class P1, class P2>
struct Type<FuncPtr<void(P0, P1, P2)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2)> > d = set_avoid_gc<std::function<void(P0, P1, P2)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2)>(d, [=](P0 a0, P1 a1, P2 a2){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2)};
      mrb_yield_argv(mrb, v, 3, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3)
template<class R, class P0, class P1, class P2, class P3>
struct Type<FuncPtr<R(P0, P1, P2, P3)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 4, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3)
template<class P0, class P1, class P2, class P3>
struct Type<FuncPtr<void(P0, P1, P2, P3)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3)};
      mrb_yield_argv(mrb, v, 4, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4)
template<class R, class P0, class P1, class P2, class P3, class P4>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 5, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4)
template<class P0, class P1, class P2, class P3, class P4>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4)};
      mrb_yield_argv(mrb, v, 5, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4, P5)
template<class R, class P0, class P1, class P2, class P3, class P4, class P5>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4, P5)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4, P5)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4, P5)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4, P5)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4, P5)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 6, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4, P5)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4, P5)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4, P5)
template<class P0, class P1, class P2, class P3, class P4, class P5>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4, P5)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4, P5)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4, P5)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4, P5)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4, P5)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5)};
      mrb_yield_argv(mrb, v, 6, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4, P5)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4, P5)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4, P5, P6)
template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4, P5, P6)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4, P5, P6)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4, P5, P6)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4, P5, P6)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 7, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4, P5, P6)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4, P5, P6)
template<class P0, class P1, class P2, class P3, class P4, class P5, class P6>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4, P5, P6)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4, P5, P6)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4, P5, P6)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4, P5, P6)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6)};
      mrb_yield_argv(mrb, v, 7, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4, P5, P6)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4, P5, P6, P7)
template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 8, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4, P5, P6, P7)
template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7)};
      mrb_yield_argv(mrb, v, 8, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4, P5, P6, P7, P8)
template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 9, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4, P5, P6, P7, P8)
template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8)};
      mrb_yield_argv(mrb, v, 9, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};


// callback R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)
template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
struct Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> > d = set_avoid_gc<std::function<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> >(mrb, v);
    return make_FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8), Type<P9>::ret(mrb, a9)};
      return Type<R>::get(mrb, mrb_yield_argv(mrb, v, 10, a));
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<R(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

// callback void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)
template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
struct Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> > : public TypeFuncBase {
  static int check(mrb_value v) { return mrb_type(v) == MRB_TT_PROC; }
  static FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> get(mrb_state* mrb, mrb_value v) {
    Deleter<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> > d = set_avoid_gc<std::function<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> >(mrb, v);
    return make_FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)>(d, [=](P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9){
      MrubyArenaStore mas(mrb);
      mrb_value a[] = {Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8), Type<P9>::ret(mrb, a9)};
      mrb_yield_argv(mrb, v, 10, a);
    });
  }
  static mrb_value ret(mrb_state* mrb, FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> p) {
      // don't call.
      throw std::runtime_error("don't call Type<FuncPtr<void(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)> >::ret");
      (void)mrb; (void)p; return mrb_nil_value();
  }
};

#undef ARG
#undef CHECK
