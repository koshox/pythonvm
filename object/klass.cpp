//
// Created by Kosho on 2020/8/16.
//

#include "klass.hpp"
#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"

Klass::Klass() {
    _name = NULL;
}

int Klass::compare_klass(Klass *x, Klass *y) {
    if (x == y) {
        return 0;
    }

    if (x == IntegerKlass::get_instance()) {
        return -1;
    } else if (y == IntegerKlass::get_instance()) {
        return 1;
    }

    if (x->name()->less(y->name()) == (HiObject *) Universe::HiTrue) {
        return -1;
    } else {
        return 1;
    }
}
