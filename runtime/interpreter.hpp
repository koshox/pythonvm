//
// Created by Kosho on 2020/8/15.
//

#ifndef PYTHONVM_INTERPRETER_HPP
#define PYTHONVM_INTERPRETER_HPP

#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "frameObject.hpp"

class Interpreter {
private:
    HiDict *_builtins;
    FrameObject *_frame;
    HiObject *_ret_value;

    static Interpreter *_instance;

    Interpreter();

public:
    static Interpreter *get_instance();

    void run(CodeObject *codes);

    void build_frame(HiObject *pObject, ObjList args, int op_arg);

    void eval_frame();

    void destroy_frame();

    void leave_frame();
};

#endif //PYTHONVM_INTERPRETER_HPP
