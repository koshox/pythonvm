//
// Created by Kosho on 2020/8/14.
//

#include <string.h>
#include <stdio.h>

#include "hiString.hpp"
#include "hiInteger.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

StringKlass *StringKlass::instance = NULL;

StringKlass *StringKlass::get_instance() {
    if (instance == NULL)
        instance = new StringKlass();

    return instance;
}

StringKlass::StringKlass() {
}

void StringKlass::initialize() {
    (new HiTypeObject())->set_own_klass(this);

    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));
    set_klass_dict(klass_dict);

    set_name(new HiString("str"));
    set_super(ObjectKlass::get_instance());
    (new HiTypeObject())->set_own_klass(this);
}

HiObject *StringKlass::equal(HiObject *x, HiObject *y) {
    if (x->klass() != y->klass())
        return Universe::HiFalse;

    HiString *sx = (HiString *) x;
    HiString *sy = (HiString *) y;

    assert(sx && sx->klass() == (Klass *) this);
    assert(sy && sy->klass() == (Klass *) this);

    if (sx->length() != sy->length())
        return Universe::HiFalse;

    for (int i = 0; i < sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i])
            return Universe::HiFalse;
    }

    return Universe::HiTrue;
}

HiString::HiString(const char *x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);

    set_klass(StringKlass::get_instance());
}

HiString::HiString(const char *x, const int length) {
    _length = length;
    _value = new char[length];

    // do not use strcpy here, since '\0' is allowed.
    for (int i = 0; i < length; ++i) {
        _value[i] = x[i];
    }

    set_klass(StringKlass::get_instance());
}

HiString::HiString(const int length) {
    _length = length;
    _value = new char[length];
    set_klass(StringKlass::get_instance());
}

void StringKlass::print(HiObject *obj) {
    HiString *str_obj = (HiString *) obj;
    assert(str_obj && str_obj->klass() == (Klass *) this);

    for (int i = 0; i < str_obj->length(); i++) {
        printf("%c", str_obj->value()[i]);
    }
}

HiObject *StringKlass::add(HiObject *x, HiObject *y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == this);

    HiString *sx = (HiString *) x;
    HiString *sy = (HiString *) y;

    HiString *sz = new HiString(sx->length() + sy->length());

    memcpy(sz->_value, sx->_value, sx->length());
    memcpy(sz->_value + sx->length(),
           sy->_value,
           sy->length());

    sz->set(sx->length() + sy->length(), '\0');

    return sz;
}

HiObject *StringKlass::len(HiObject *x) {
    return new HiInteger(((HiString *) x)->length());
}

HiObject *StringKlass::allocate_instance(HiObject *callable, ArrayList<HiObject *> *args) {
    if (!args || args->length() == 0) {
        return new HiString("");
    } else {
        return NULL;
    }
}

HiObject *StringKlass::less(HiObject *x, HiObject *y) {
    HiString *sx = (HiString *) x;
    assert(sx && (sx->klass() == (Klass *) this));

    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0)
            return Universe::HiTrue;
        else
            return Universe::HiFalse;
    }

    HiString *sy = (HiString *) y;
    assert(sy && (sy->klass() == (Klass *) this));

    int len = sx->length() < sy->length() ?
              sx->length() : sy->length();

    for (int i = 0; i < len; i++) {
        if (sx->value()[i] < sy->value()[i])
            return Universe::HiTrue;
        else if (sx->value()[i] > sy->value()[i])
            return Universe::HiFalse;
    }

    if (sx->length() < sy->length()) {
        return Universe::HiTrue;
    }

    return Universe::HiFalse;
}

HiObject *StringKlass::subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass *) this);
    assert(y && y->klass() == (Klass *) IntegerKlass::get_instance());

    HiString *sx = (HiString *) x;
    HiInteger *iy = (HiInteger *) y;

    return new HiString(&(sx->value()[iy->value()]), 1);
}

HiObject *StringKlass::contains(HiObject *x, HiObject *y) {
    HiString *sx = (HiString *) x;
    HiString *sy = (HiString *) y;
    assert(sx && sx->klass() == (Klass *) this);
    assert(sy && sy->klass() == (Klass *) this);

    for (int i = 0; i < sx->length(); ++i) {
        if (sx->length() - i < sy->length()) {
            return Universe::HiFalse;
        }

        for (int j = 0; j < sy->length(); ++j) {
            if (j == sy->length() - 1) {
                return Universe::HiTrue;
            }

            if (sx->value()[i + j] != sy->value()[j]) {
                break;
            }
        }
    }

    return Universe::HiFalse;
}
