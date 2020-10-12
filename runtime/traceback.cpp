//
// Created by Kosho on 2020/10/12.
//

#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "runtime/traceback.hpp"
#include "runtime/frameObject.hpp"
#include "memory/oopClosure.hpp"

StackElementKlass *StackElementKlass::_instance = NULL;
TracebackKlass *TracebackKlass::_instance = NULL;

StackElementKlass *StackElementKlass::get_instance() {
    if (_instance == NULL) {
        _instance = new StackElementKlass();
    }

    return _instance;
}

void StackElementKlass::print(HiObject *x) {
    StackElement *xse = (StackElement *) x;
    printf("  File \"");
    xse->_file_name->print();
    printf("\", line %d,", xse->_line_no);
    printf(" in ");
    xse->_func_name->print();
    printf("\n");
}

size_t StackElementKlass::size() {
    return sizeof(StackElement);
}

StackElement::StackElement(HiString *file_name,
                           HiString *func_name,
                           int line_no) :
        _file_name(file_name),
        _func_name(func_name),
        _line_no(line_no) {
    set_klass(StackElementKlass::get_instance());
}

TracebackKlass *TracebackKlass::get_instance() {
    if (_instance == NULL) {
        _instance = new TracebackKlass();
    }

    return _instance;
}

void TracebackKlass::print(HiObject *x) {
    Traceback *tbx = (Traceback *) x;

    printf("Traceback (most recent call last):\n");
    for (int i = tbx->_stack_elements->size() - 1; i >= 0; i--) {
        tbx->_stack_elements->get(i)->print();
    }
}

void TracebackKlass::oops_do(OopClosure *f, HiObject *obj) {
    f->do_oop((HiObject **) &(((Traceback *) obj)->_stack_elements));
}

void StackElementKlass::oops_do(OopClosure *f, HiObject *obj) {
    f->do_oop((HiObject **) &(((StackElement *) obj)->_file_name));
    f->do_oop((HiObject **) &(((StackElement *) obj)->_func_name));
}

size_t TracebackKlass::size() {
    return sizeof(Traceback);
}

Traceback::Traceback() {
    _stack_elements = new HiList();
    set_klass(TracebackKlass::get_instance());
}

void Traceback::record_frame(FrameObject *frame) {
    // _stack_elements->append(
    //         new StackElement(
    //                 frame->file_name(),
    //                 frame->func_name(),
    //                 frame->lineno()));
}

