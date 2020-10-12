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
    enum Status {
        IS_OK,
        IS_BREAK,
        IS_CONTINUE,
        IS_EXCEPTION,
        IS_RETURN,
        IS_YIELD
    };

private:
    ModuleObject *_builtins;
    HiDict *_modules;
    FrameObject *_frame;
    HiObject *_ret_value;

    HiObject *_exception_class;
    HiObject *_pending_exception;
    HiObject *_trace_back;
    Status _int_status;

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

    Status do_raise(HiObject *exc, HiObject *val, HiObject *tb);

    void oops_do(OopClosure *f);
};

#endif //PYTHONVM_INTERPRETER_HPP
