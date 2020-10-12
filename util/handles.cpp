//
// Created by Kosho on 2020/10/13.
//

#include "util/handles.hpp"
#include "memory/oopClosure.hpp"

#include <stdio.h>

HandleMark *HandleMark::instance = NULL;

HandleMark::HandleMark() {
    _head = 0x0;
}

HandleMark *HandleMark::get_instance() {
    if (!instance)
        instance = new HandleMark();

    return instance;
}

void HandleMark::oops_do(OopClosure *f) {
    Handle *cur = _head;
    while (cur) {
        cur->oops_do(f);
        cur = cur->_next;
    }
}

Handle::Handle(HiObject *t) {
    _value = t;

    _next = HandleMark::get_instance()->head();
    HandleMark::get_instance()->set_head(this);
}

Handle::~Handle() {
    _value = 0x0;

    HandleMark::get_instance()->set_head(_next);
    _next = 0x0;
}

HiObject *Handle::operator->() {
    return _value;
}

void Handle::oops_do(OopClosure *f) {
    f->do_oop(&_value);
}

HiObject *Handle::resolve() {
    return _value;
}
