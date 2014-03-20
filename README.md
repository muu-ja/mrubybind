mrubybind - Binding library for mruby/C++
=========================================

mrubybind automatically creates C function/class-method binder for [mruby](https://github.com/mruby/mruby),
using C++ template partial specialization.

## Usage

### How to use mrubybind in your project

1. Put following source codes into your project.
  * mrubybind.cc
  * mrubybind.h
2. Include "mrubybind.h"
3. Use `MrubyBind` instance to bind C function/class-method to mruby.

## Examples

### Bind C function and call it from mruby

1. C function (Any type you want):

  ```c++
  int square(int x) {
    return x * x;
  }
  ```

2. Bind it using mrubybind `bind` method:

  ```c++
  #include "mrubybind.h"

  void install_square_function(mrb_state* mrb) {
    mrubybind::MrubyBind b(mrb);
    b.bind("square", square);
  }
  ```

  You can throw away `MrubyBind` instance after binding function.

3. Call it from mruby:

  ```ruby
  puts square(1111)  #=> 1234321
  ```

### Bind C++ class and method and create its instance from mruby

1. C++ class:

  ```c++
  class Foo {
  public:
    Foo(int x) : x_(x) {
      cout << "Foo::ctor(" << x << ")" << endl;
    }
    virtual ~Foo() {
      cout << "Foo::dtor()" << endl;
    }
    int bar(int y) {
      return x_ + y;
    }
    static int baz(int z) {
      return z * z;
    }

  private:
    int x_;
  };
  ```

2. Bind C++ class using mrubybind `bind_class`, `bind_instance_method` and
   `bind_static_method` method:

  ```c++
  #include "mrubybind.h"

  // Helper function for constructor.
  Foo* new_foo(int x) {
    return new Foo(x);
  }

  void install_foo_class(mrb_state* mrb) {
    mrubybind::MrubyBind b(mrb);
    b.bind_class("Foo", new_foo);
    b.bind_instance_method("Foo", "bar", &Foo::bar);
    b.bind_static_method("Foo", "baz", &Foo::baz);
  }
  ```

3. Call it from mruby:

  ```ruby
  foo = Foo.new(123)  #=> Foo::ctor(123)
  p foo               #=> #<Foo:0x7fa828803d80>
  p foo.bar(567)      #=> 690
  p Foo.baz(9999)     #=> 99980001
                      #=> Foo::dtor()
  ```

### Bind functions under some module

1. Pass `RClass*` instace for `MrubyBind` constructor:

  ```c++
  void install(mrb_state* mrb) {
    RClass* mod = mrb_define_module(mrb, "YourModule");
    mrubybind::MrubyBind b(mrb, mod);
    b.bind("foo", foo);
  }
  ```

  You can use `YourModule.foo` function from mruby.

### Bind constant

1. Use `bind_const` method:

  ```c++
  void install(mrb_state* mrb) {
    mrubybind::MrubyBind b(mrb);
    b.bind_const("FOO", FOO_VALUE);
  }
  ```

## Supported types
| C type              | mruby type              |
|---------------------|-------------------------|
| int, unsigned int   | Fixnum                  |
| float, double       | Float                   |
| const char*, string | String                  |
| bool                | TrueClass or FalseClass |
| void*               | Object                  |

See [mrubybind.h](https://github.com/ktaobo/mrubybind/blob/master/mrubybind.h).

                            

### Call block from C++ code

1. define C++ function having callback function

        std::string call_block(mrubybind::sp_mrb_func<void(int a0)> f) {
          if(f)
          {
              f.func()(23);
          }
          return "called\n";
        }

2. Bind it using mrubybind:

        #include "mrubybind.h"
        
        void install_square_function(mrb_state* mrb) {
          mrubybind::MrubyBind b(mrb);
          b.bind("call_block", call_block);
        }
        
3. Call it from mruby:

        puts call_block { |a0|
          puts "a0 = #{a0}"
        }

# License

MIT license.
