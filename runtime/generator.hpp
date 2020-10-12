//
// Created by Kosho on 2020/10/13.
//

#ifndef PYTHONVM_GENERATOR_HPP
#define PYTHONVM_GENERATOR_HPP

#include "object/hiObject.hpp"
#include "object/klass.hpp"
#include "util/arrayList.hpp"
#include "util/stack.hpp"

class OopClosure;
class FunctionObject;
class CodeObject;
class HiDict;
class HiList;
class FrameObject;
class Interpreter;
class Block;

class GeneratorKlass : public Klass {
private:
    static GeneratorKlass *instance;

    GeneratorKlass();

public:
    static GeneratorKlass *get_instance();

    virtual HiObject *next(HiObject *obj);

    virtual HiObject *iter(HiObject *obj);

    virtual size_t size();

    virtual void oops_do(OopClosure *f, HiObject *obj);
};

class Generator : public HiObject {
    friend class Interpreter;
    friend class FrameObject;
    friend class GeneratorKlass;

private:
    FrameObject *_frame;

public:
    Generator(FunctionObject *func, ArrayList<HiObject *> *args, int arg_cnt);

    FrameObject *frame() {
        return _frame;
    }

    void set_frame(FrameObject *x) {
        _frame = x;
    }
};

HiObject* generator_next(ObjList args);

#endif //PYTHONVM_GENERATOR_HPP
