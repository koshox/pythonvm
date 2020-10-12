//
// Created by Kosho on 2020/8/16.
//

#ifndef PYTHONVM_FRAMEOBJECT_HPP
#define PYTHONVM_FRAMEOBJECT_HPP

#include <memory/oopClosure.hpp>
#include "object/hiList.hpp"
#include "code/codeObject.hpp"
#include "util/map.hpp"
#include "util/arrayList.hpp"
#include "functionObject.hpp"

class Block {
public:
    unsigned char _type;
    unsigned int _target;
    int _level;

    Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    Block(unsigned char b_type,
          unsigned int b_target,
          int b_level) :
            _type(b_type),
            _target(b_target),
            _level(b_level) {
    }

    Block(const Block &b) {
        _type = b._type;
        _target = b._target;
        _level = b._level;
    }
};

class FrameObject {
public:
    FrameObject(CodeObject *codes);

    FrameObject(FunctionObject *func, ObjList args, int op_arg);

    HiList *_stack;
    ArrayList<Block *> *_loop_stack;

    ArrayList<HiObject *> *_consts;
    ArrayList<HiObject *> *_names;

    HiDict *_locals;
    HiDict *_globals;
    HiList *_fast_locals;
    HiList *_closure;

    FrameObject *_sender;
    CodeObject *_codes;
    int _pc;
    // 标记是否cpp调用python产生的栈帧
    bool _entry_frame;

public:
    void set_sender(FrameObject *x) { _sender = x; }

    FrameObject *sender() { return _sender; }

    void set_pc(int x) { _pc = x; }

    int get_pc() { return _pc; }

    void set_entry_frame(bool x) { _entry_frame = x; }

    bool is_entry_frame() { return _entry_frame; }

    bool is_first_frame() { return _sender == NULL; }

    HiList *stack() { return _stack; }

    ArrayList<Block *> *loop_stack() { return _loop_stack; }

    ArrayList<HiObject *> *consts() { return _consts; }

    ArrayList<HiObject *> *names() { return _names; }

    HiDict *locals() { return _locals; }

    HiDict *globals() { return _globals; }

    HiList *fast_locals() { return _fast_locals; }

    HiList *closure() { return _closure; }

    HiObject *get_cell_from_parameter(int i);

    bool has_more_codes();

    unsigned char get_op_code();

    int get_op_arg();

    HiString *file_name();

    HiString *func_name();

    int lineno();

    void oops_do(OopClosure *f);
};

#endif //PYTHONVM_FRAMEOBJECT_HPP
