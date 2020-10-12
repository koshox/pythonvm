//
// Created by Kosho on 2020/8/15.
//

#include "runtime/frameObject.hpp"
#include "util/map.hpp"
#include "object/hiInteger.hpp"
#include "interpreter.hpp"
#include "code/bytecode.hpp"
#include "universe.hpp"
#include "functionObject.hpp"
#include "stringTable.hpp"
#include "cellObject.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "memory/oopClosure.hpp"
#include "runtime/module.hpp"
#include "traceback.hpp"

#define PUSH(x)       _frame->stack()->append(x)
#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()
#define STACK_LEVEL() _frame->stack()->size()
#define PEEK(x)       _frame->stack()->get((x))

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse

#define ST(x) StringTable::get_instance()->STR(x)
#define STR(x) x##_str

Interpreter *Interpreter::_instance = NULL;

Interpreter *Interpreter::get_instance() {
    if (_instance == NULL) {
        _instance = new Interpreter();
    }

    return _instance;
}

Interpreter::Interpreter() {
    _frame = NULL;

    _int_status = IS_OK;

    // prepare for import builtin, this should be created first
    _builtins = new ModuleObject(new HiDict());
    _builtins->put(new HiString("True"), Universe::HiTrue);
    _builtins->put(new HiString("False"), Universe::HiFalse);
    _builtins->put(new HiString("None"), Universe::HiNone);

    _builtins->put(new HiString("len"), new FunctionObject(len));
    _builtins->put(new HiString("type"), new FunctionObject(type_of));
    _builtins->put(new HiString("isinstance"), new FunctionObject(isinstance));

    _builtins->put(new HiString("int"), IntegerKlass::get_instance()->type_object());
    _builtins->put(new HiString("object"), ObjectKlass::get_instance()->type_object());
    _builtins->put(new HiString("str"), StringKlass::get_instance()->type_object());
    _builtins->put(new HiString("list"), ListKlass::get_instance()->type_object());
    _builtins->put(new HiString("dict"), DictKlass::get_instance()->type_object());
}

void Interpreter::initialize() {
    _builtins->extend(ModuleObject::import_module((HiString *) Universe::HiNone, new HiString("builtin")));

    _modules = new HiDict();
    _modules->put(new HiString("__builtins__"), _builtins);
}

void Interpreter::run(CodeObject *codes) {
    _frame = new FrameObject(codes);

    eval_frame();
    destroy_frame();
}

HiDict *Interpreter::run_mod(CodeObject *codes, HiString *mod_name) {
    FrameObject *frame = new FrameObject(codes);
    frame->set_entry_frame(true);
    frame->locals()->put(ST(name), mod_name);

    enter_frame(frame);
    eval_frame();
    HiDict *result = frame->locals();
    destroy_frame();
    return result;
}

void Interpreter::eval_frame() {
    Block *b;
    FunctionObject *fo;
    ArrayList<HiObject *> *args = NULL;

    HiObject *lhs, *rhs;
    HiObject *v, *w, *u, *attr;
    HiList *stack;

    unsigned char op_code;
    bool has_arg;
    int op_arg;

    while (_frame->has_more_codes()) {
        op_code = _frame->get_op_code();
        bool has_arg = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_arg) {
            op_arg = _frame->get_op_arg();
        }

        switch (op_code) {
            case ByteCode::POP_TOP:
                POP();
                break;

            case ByteCode::ROT_TWO:
                v = POP();
                w = POP();
                PUSH(v);
                PUSH(w);
                break;

            case ByteCode::ROT_THREE:
                v = POP();
                w = POP();
                u = POP();
                PUSH(v);
                PUSH(u);
                PUSH(w);
                break;

            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;

            case ByteCode::DUP_TOP:
                PUSH(TOP());
                break;

            case ByteCode::BINARY_DIVIDE:
            case ByteCode::INPLACE_DIVIDE:
                v = POP();
                w = POP();
                PUSH(w->div(v));
                break;

            case ByteCode::BINARY_MODULO:
            case ByteCode::INPLACE_MODULO:
                v = POP();
                w = POP();
                // TODO
                // PUSH(w->mod(v));
                u = w->mod(v);
                stack = _frame->stack();
                stack->append(u);
                break;

            case ByteCode::BINARY_ADD:
            case ByteCode::INPLACE_ADD:
                v = POP();
                w = POP();
                // PUSH(w->add(v));
                // _frame->stack()->append(w->add(v));
                stack = _frame->stack();
                u = w->add(v);
                stack->append(u);
                break;

            case ByteCode::BINARY_SUBTRACT:
                v = POP();
                w = POP();
                PUSH(w->sub(v));
                break;

            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;

            case ByteCode::STORE_MAP:
                w = POP();
                u = POP();
                v = TOP();
                ((HiDict *) v)->put(w, u);
                break;

            case ByteCode::STORE_SUBSCR:
                u = POP();
                v = POP();
                w = POP();
                v->store_subscr(u, w);
                break;

            case ByteCode::DELETE_SUBSCR:
                v = POP();
                w = POP();
                w->del_subscr(v);
                break;

            case ByteCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;

            case ByteCode::FOR_ITER:
                v = TOP();
                w = v->getattr(StringTable::get_instance()->next_str);
                // TODO
                build_frame(w, NULL, 0);

                if (TOP() == NULL) {
                    _frame->_pc += op_arg;
                    POP();
                }
                break;

            case ByteCode::PRINT_ITEM:
                v = POP();
                v->print();
                break;

            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;

            case ByteCode::BREAK_LOOP:
                _int_status = IS_BREAK;
                break;

            case ByteCode::LOAD_LOCALS:
                PUSH(_frame->locals());
                break;

            case ByteCode::POP_BLOCK:
                b = _frame->_loop_stack->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                break;

            case ByteCode::END_FINALLY:
                // restore state before finally
                v = POP();
                if (((long) v) & 0x1) {
                    _int_status = (Status) (((long) v) >> 1);
                    if (_int_status == IS_RETURN) {
                        _ret_value = POP();
                    } else if (_int_status == IS_CONTINUE) {
                        _frame->_pc = (int) ((long) POP());
                    }
                } else if (v != Universe::HiNone) {
                    _exception_class = v;
                    _pending_exception = POP();
                    _trace_back = POP();
                    _int_status = IS_EXCEPTION;
                }
                break;

            case ByteCode::BUILD_CLASS:
                // locals
                v = POP();
                // supers
                u = POP();
                // name
                w = POP();
                v = Klass::create_klass(v, u, w);
                PUSH(v);
                break;

            case ByteCode::STORE_NAME:
                v = _frame->names()->get(op_arg);
                _frame->locals()->put(v, POP());
                break;

            case ByteCode::UNPACK_SEQUENCE:
                v = POP();
                while (op_arg--) {
                    PUSH(v->subscr(new HiInteger(op_arg)));
                }
                break;

            case ByteCode::STORE_ATTR:
                u = POP();
                v = _frame->names()->get(op_arg);
                w = POP();
                u->setattr(v, w);
                break;

            case ByteCode::DUP_TOPX:
                for (int i = 0; i < op_arg; i++) {
                    int index = STACK_LEVEL() - op_arg;
                    PUSH(PEEK(index));
                }
                break;

            case ByteCode::LOAD_CONST:
                v = _frame->consts()->get(op_arg);
                PUSH(v);
                break;

            case ByteCode::LOAD_NAME:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::BUILD_TUPLE:
            case ByteCode::BUILD_LIST:
                v = new HiList();
                while (op_arg--) {
                    ((HiList *) v)->set(op_arg, POP());
                }

                PUSH(v);
                break;

            case ByteCode::BUILD_MAP:
                v = new HiDict();
                PUSH(v);
                break;

            case ByteCode::LOAD_ATTR:
                v = POP();
                w = _frame->_names->get(op_arg);
                PUSH(v->getattr(w));
                break;

            case ByteCode::COMPARE_OP:
                w = POP();
                v = POP();

                switch (op_arg) {
                    case ByteCode::GREATER:
                        PUSH(v->greater(w));
                        break;
                    case ByteCode::LESS:
                        PUSH(v->less(w));
                        break;

                    case ByteCode::EQUAL:
                        PUSH(v->equal(w));
                        break;

                    case ByteCode::NOT_EQUAL:
                        PUSH(v->not_equal(w));
                        break;

                    case ByteCode::GREATER_EQUAL:
                        PUSH(v->ge(w));
                        break;

                    case ByteCode::IN:
                        PUSH(w->contains(v));
                        break;

                    case ByteCode::LESS_EQUAL:
                        PUSH(v->le(w));
                        break;

                    case ByteCode::IS:
                        if (v == w) {
                            PUSH(HI_TRUE);
                        } else {
                            PUSH(HI_FALSE);
                        }
                        break;

                    case ByteCode::IS_NOT:
                        if (v == w) {
                            PUSH(HI_FALSE);
                        } else {
                            PUSH(HI_TRUE);
                        }
                        break;

                    case ByteCode::EXC_MATCH: {
                        bool found = false;
                        Klass *k = ((HiTypeObject *) v)->own_klass();
                        if (v == w) {
                            found = true;
                        } else {
                            for (int i = 0; i < k->mro()->size(); i++) {
                                if (v->klass()->mro()->get(i) == w) {
                                    found = true;
                                    break;
                                }
                            }
                        }

                        if (found) {
                            PUSH(Universe::HiTrue);
                        } else {
                            PUSH(Universe::HiFalse);
                        }

                        break;
                    }

                    default:
                        printf("Error: Unrecognized compare op %d\n", op_arg);
                }
                break;

            case ByteCode::IMPORT_NAME:
                POP();
                POP();
                v = _frame->names()->get(op_arg);
                w = _modules->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                HiString *cur_code_path;
                if (_frame->_codes->_file_path != NULL) {
                    cur_code_path = _frame->_codes->_file_path;
                } else {
                    cur_code_path = Universe::main_code->_file_path;
                }

                w = ModuleObject::import_module(cur_code_path, v);
                _modules->put(v, w);
                PUSH(w);
                break;

            case ByteCode::IMPORT_FROM:
                v = _frame->names()->get(op_arg);
                w = TOP();
                u = ((ModuleObject *) w)->get(v);
                PUSH(u);
                break;

            case ByteCode::POP_JUMP_IF_FALSE:
                v = POP();
                if (v == Universe::HiFalse)
                    _frame->set_pc(op_arg);
                break;

            case ByteCode::JUMP_FORWARD:
                _frame->set_pc(op_arg + _frame->get_pc());
                break;

            case ByteCode::JUMP_ABSOLUTE:
                _frame->set_pc(op_arg);
                break;

            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                w = _builtins->get(v);
                if (w != Universe::HiNone) {
                    PUSH(w);
                    break;
                }

                PUSH(Universe::HiNone);
                break;

            case ByteCode::CONTINUE_LOOP:
                _int_status = IS_CONTINUE;
                _ret_value = (HiObject *) ((long) op_arg);
                break;

            case ByteCode::SETUP_LOOP:
            case ByteCode::SETUP_EXCEPT:
            case ByteCode::SETUP_FINALLY:
                _frame->loop_stack()->add(new Block(
                        op_code, _frame->get_pc() + op_arg,
                        STACK_LEVEL()));
                break;

            case ByteCode::LOAD_FAST:
                PUSH(_frame->fast_locals()->get(op_arg));
                break;

            case ByteCode::STORE_FAST:
                _frame->fast_locals()->set(op_arg, POP());
                break;

            case ByteCode::RAISE_VARARGS:
                w = v = u = NULL;
                switch (op_arg) {
                    case 3:
                        u = POP();
                    case 2:
                        v = POP();
                    case 1:
                        w = POP();
                        break;
                }

                do_raise(w, v, u);
                break;

            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    int arg_num = op_arg & 0xff;
                    int key_arg_num = op_arg >> 8;
                    int arg_cnt = arg_num + 2 * key_arg_num;
                    args = new ArrayList<HiObject *>(arg_cnt);
                    while (arg_cnt--) {
                        args->set(arg_cnt, POP());
                    }
                }
                build_frame(POP(), args, op_arg);

                if (args != NULL) {
                    args = NULL;
                }
                break;

            case ByteCode::MAKE_FUNCTION:
                v = POP();
                fo = new FunctionObject(v);
                fo->set_globals(_frame->globals());
                if (op_arg > 0) {
                    args = new ArrayList<HiObject *>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }

                fo->set_default(args);

                if (args != NULL) {
                    args = NULL;
                }

                PUSH(fo);
                break;

            case ByteCode::MAKE_CLOSURE:
                v = POP();
                fo = new FunctionObject(v);
                fo->set_closure((HiList *) POP());
                fo->set_globals(_frame->globals());
                if (op_arg > 0) {
                    args = new ArrayList<HiObject *>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                fo->set_default(args);

                if (args != NULL) {
                    args = NULL;
                }

                PUSH(fo);
                break;

            case ByteCode::LOAD_CLOSURE:
                v = _frame->closure()->get(op_arg);
                // 为空说明不是局部变量而是参数
                if (v == NULL) {
                    v = _frame->get_cell_from_parameter(op_arg);
                    _frame->closure()->set(op_arg, v);
                }

                if (v->klass() == CellKlass::get_instance()) {
                    PUSH(v);
                } else {
                    PUSH(new CellObject(_frame->closure(), op_arg));
                }
                break;

            case ByteCode::LOAD_DEREF:
                v = _frame->closure()->get(op_arg);
                if (v->klass() == CellKlass::get_instance()) {
                    v = ((CellObject *) v)->value();
                }
                PUSH(v);
                break;

            case ByteCode::STORE_DEREF:
                _frame->closure()->set(op_arg, POP());
                break;

            case ByteCode::CALL_FUNCTION_VAR:
                v = POP();
                if (op_arg > 0 || (v && ((HiList *) v)->size() > 0)) {
                    int arg_num = op_arg & 0xff;
                    int key_arg_num = op_arg >> 8;
                    int arg_cnt = arg_num + 2 * key_arg_num;
                    args = new ArrayList<HiObject *>();
                    while (arg_cnt--) {
                        args->set(arg_cnt, POP());
                    }

                    int s = ((HiList *) v)->size();
                    for (int i = 0; i < s; i++) {
                        args->add(((HiList *) v)->get(i));
                    }
                    arg_num += s;
                    op_arg = (key_arg_num << 8) | arg_num;
                }

                build_frame(POP(), args, op_arg);

                if (args != NULL) {
                    args = NULL;
                }
                break;

            case ByteCode::RETURN_VALUE:
                _ret_value = POP();
                _int_status = IS_RETURN;
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }

        while (_int_status != IS_OK && _frame->loop_stack()->size() != 0) {
            b = _frame->_loop_stack->get(_frame->_loop_stack->size() - 1);
            if (_int_status == IS_CONTINUE && b->_type == ByteCode::SETUP_LOOP) {
                _frame->_pc = (int) ((long long) _ret_value);
                _int_status = IS_OK;
                break;
            }

            b = _frame->_loop_stack->pop();
            while (STACK_LEVEL() > b->_level) {
                POP();
            }

            if (_int_status == IS_BREAK && b->_type == ByteCode::SETUP_LOOP) {
                _frame->_pc = b->_target;
                _int_status = IS_OK;
            } else if (b->_type == ByteCode::SETUP_FINALLY ||
                       (_int_status == IS_EXCEPTION
                        && b->_type == ByteCode::SETUP_EXCEPT)) {
                if (_int_status == IS_EXCEPTION) {
                    // traceback, value, exception class
                    PUSH(_trace_back);
                    PUSH(_pending_exception);
                    PUSH(_exception_class);

                    _trace_back = NULL;
                    _pending_exception = NULL;
                    _exception_class = NULL;
                } else {
                    if (_int_status == IS_RETURN || _int_status == IS_CONTINUE) {
                        PUSH(_ret_value);
                    }

                    // 用倒数第一位标记一下是直接整数而不是指针
                    PUSH((HiObject *) (((long) _int_status << 1) | 0x1));
                }

                _frame->_pc = b->_target;
                _int_status = IS_OK;
            }
        }

        // has pending exception and no handler found, unwind stack.
        if (_int_status != IS_OK && _frame->_loop_stack->size() == 0) {
            if (_int_status == IS_RETURN) {
                _int_status = IS_OK;
            }

            if (_frame->is_first_frame() ||
                _frame->is_entry_frame()) {
                return;
            }

            leave_frame();
        }
    }
}

void Interpreter::destroy_frame() {
    FrameObject *temp = _frame;
    _frame = _frame->sender();
    delete temp;
}

void Interpreter::build_frame(HiObject *callable, ObjList args, int op_arg) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject *) callable)->call(args));
    } else if (callable->klass() == MethodKlass::get_instance()) {
        MethodObject *method = (MethodObject *) callable;
        if (!args) {
            args = new ArrayList<HiObject *>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, op_arg + 1);
    } else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject *frame = new FrameObject((FunctionObject *) callable, args, op_arg);
        frame->set_sender(_frame);
        _frame = frame;
    } else if (callable->klass() == TypeKlass::get_instance()) {
        HiObject *instance = ((HiTypeObject *) callable)->own_klass()->
                allocate_instance(callable, args);
        PUSH(instance);
    } else {
        // __call__
        HiObject *m = callable->getattr(ST(call));
        if (m != Universe::HiNone) {
            build_frame(m, args, op_arg);
        } else {
            callable->print();
            printf("\nError : can not call a normal object.\n");
        }
    }
}

void Interpreter::leave_frame() {
    destroy_frame();
    PUSH(_ret_value);
}

HiObject *Interpreter::call_virtual(HiObject *func, ObjList args) {
    if (func->klass() == NativeFunctionKlass::get_instance()) {
        // we do not create a virtual frame, but native frame.
        return ((FunctionObject *) func)->call(args);
    } else if (func->klass() == MethodKlass::get_instance()) {
        MethodObject *method = (MethodObject *) func;
        if (!args) {
            args = new ArrayList<HiObject *>(1);
        }
        args->insert(0, method->owner());
        return call_virtual(method->func(), args);
    } else if (MethodObject::is_function(func)) {
        int size = args ? args->size() : 0;
        FrameObject *frame = new FrameObject((FunctionObject *) func, args, size);
        frame->set_entry_frame(true);
        enter_frame(frame);
        eval_frame();
        destroy_frame();
        return _ret_value;
    }

    return Universe::HiNone;
}

void Interpreter::enter_frame(FrameObject *frame) {
    frame->set_sender(_frame);
    _frame = frame;
}

void Interpreter::oops_do(OopClosure *f) {
    f->do_oop((HiObject **) &_builtins);
    f->do_oop((HiObject **) &_modules);
    f->do_oop((HiObject **) &_ret_value);

    if (_frame) {
        _frame->oops_do(f);
    }
}

/**
 * 抛出异常
 *
 * @param exc 异常类型
 * @param val 异常实例
 * @param tb 异常堆栈
 * @return 状态
 */
Interpreter::Status Interpreter::do_raise(HiObject *exc, HiObject *val, HiObject *tb) {
    assert(exc != NULL);

    _int_status = IS_EXCEPTION;

    if (tb == NULL) {
        tb = new Traceback();
    }

    if (val != NULL) {
        _exception_class = exc;
        _pending_exception = val;
        _trace_back = tb;
        return IS_EXCEPTION;
    }

    if (exc->klass() == TypeKlass::get_instance()) {
        _pending_exception = call_virtual(exc, NULL);
        _exception_class = exc;
    } else {
        _pending_exception = exc;
        _exception_class = _pending_exception->klass()->type_object();
    }

    _trace_back = tb;
    return IS_EXCEPTION;
}
