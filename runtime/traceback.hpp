//
// Created by Kosho on 2020/10/12.
//

#ifndef PYTHONVM_TRACEBACK_HPP
#define PYTHONVM_TRACEBACK_HPP

#include "object/hiObject.hpp"

class HiList;
class HiString;
class FrameObject;

class StackElementKlass : public Klass {
private:
    StackElementKlass() {}

    static StackElementKlass *_instance;

public:
    static StackElementKlass *get_instance();

    virtual void print(HiObject *x);

    virtual size_t size();

    virtual void oops_do(OopClosure *f, HiObject *obj);
};

class StackElement : public HiObject {
    friend StackElementKlass;
private:
    HiString *_file_name;
    HiString *_func_name;
    int _line_no;

public:
    StackElement(HiString *fname, HiString *mname, int lineno);
};

class TracebackKlass : public Klass {
private:
    TracebackKlass() {}

    static TracebackKlass *_instance;

public:
    static TracebackKlass *get_instance();

    virtual void print(HiObject *x);

    virtual size_t size();

    virtual void oops_do(OopClosure *f, HiObject *obj);
};

class Traceback : public HiObject {
    friend class TracebackKlass;

private:
    HiList *_stack_elements;

public:
    Traceback();

    void record_frame(FrameObject *frame);
};

#endif //PYTHONVM_TRACEBACK_HPP
