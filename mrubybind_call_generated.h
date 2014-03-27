// This file is generated from gen_template.rb
#define ARG(mrb, i)  Type<P##i>::get(mrb, args[i])
#define CHECK(i)  {if(!Type<P##i>::check(mrb, args[i])) return RAISE(i);}
#define RAISE(i)  raise(mrb, i, Type<P##i>::TYPE_NAME, args[i])


    template<class P0>MrubyRef call(std::string name, P0 a0){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 1, Type<P0>::ret(mrb, a0)));
    }


    template<class P0, class P1>MrubyRef call(std::string name, P0 a0, P1 a1){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 2, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1)));
    }


    template<class P0, class P1, class P2>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 3, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2)));
    }


    template<class P0, class P1, class P2, class P3>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 4, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3)));
    }


    template<class P0, class P1, class P2, class P3, class P4>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 5, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4)));
    }


    template<class P0, class P1, class P2, class P3, class P4, class P5>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4); (void)(narg); CHECK(5);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 6, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5)));
    }


    template<class P0, class P1, class P2, class P3, class P4, class P5, class P6>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4); (void)(narg); CHECK(5); (void)(narg); CHECK(6);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 7, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6)));
    }


    template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4); (void)(narg); CHECK(5); (void)(narg); CHECK(6); (void)(narg); CHECK(7);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 8, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7)));
    }


    template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4); (void)(narg); CHECK(5); (void)(narg); CHECK(6); (void)(narg); CHECK(7); (void)(narg); CHECK(8);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 9, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8)));
    }


    template<class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>MrubyRef call(std::string name, P0 a0, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9){
        MrubyArenaStore mas(mrb);
        //(void)(narg); CHECK(0); (void)(narg); CHECK(1); (void)(narg); CHECK(2); (void)(narg); CHECK(3); (void)(narg); CHECK(4); (void)(narg); CHECK(5); (void)(narg); CHECK(6); (void)(narg); CHECK(7); (void)(narg); CHECK(8); (void)(narg); CHECK(9);
        return MrubyRef(mrb, mrb_funcall(mrb, *(this->v.get()), name.c_str(), 10, Type<P0>::ret(mrb, a0), Type<P1>::ret(mrb, a1), Type<P2>::ret(mrb, a2), Type<P3>::ret(mrb, a3), Type<P4>::ret(mrb, a4), Type<P5>::ret(mrb, a5), Type<P6>::ret(mrb, a6), Type<P7>::ret(mrb, a7), Type<P8>::ret(mrb, a8), Type<P9>::ret(mrb, a9)));
    }

#undef ARG
#undef CHECK
