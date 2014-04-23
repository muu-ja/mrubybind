/**
 * mrubybind - Binding library for mruby/C++
 *
 * Usage:
 *   1. Prepare a function which you want to call from mruby:
 *     > int square(int x)  { return x * x; }
 *
 *   2. Create MrubyBind instance:
 *     > MrubyBind b(mirb)
 *
 *   3. Bind a function:
 *     > b.bind("square", square);
 *
 *   4. You can call it from mruby:
 *     > puts square(1111)  #=> 1234321
 *
 *   There are other methods to bind constant/class/instance method in
 *   MrubyBind. Please see the definition of MrubyBind
 *   (the bottom of this file), or README.
 */
#ifndef __MRUBYBIND_H__
#define __MRUBYBIND_H__

#ifndef __cplusplus
#error mrubybind can be used from C++ only.
#endif

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/proc.h"
#include "mruby/variable.h"
//#include "mrubybind_types.h"

#include <iostream>
#include <vector>
