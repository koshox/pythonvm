//
// Created by Kosho on 2020/8/15.
//

#ifndef PYTHONVM_INTERPRETER_HPP
#define PYTHONVM_INTERPRETER_HPP

#include "code/bytecode.hpp"
#include "code/codeObject.hpp"
#include "module.hpp"

class FrameObject;

class HiDict;

class OopClosure;

class Interpreter {
private:
    ModuleObject *_builtins;
    HiDict *_modules;
    FrameObject *_frame;
    HiObject *_ret_value;

    static Interpreter *_instance;

    Interpreter();

public:
    static Interpreter *get_instance();

    void initialize();

    void run(CodeObject *codes);

    HiDict *run_mod(CodeObject *codes, HiString *mod_name);

    void build_frame(HiObject *pObject, ObjList args, int op_arg);

    void enter_frame(FrameObject *frame);

    void eval_frame();

    void destroy_frame();

    void leave_frame();

    HiObject *call_virtual(HiObject *func, ObjList args);

    void oops_do(OopClosure *f);
};

#endif //PYTHONVM_INTERPRETER_HPP
