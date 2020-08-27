//
// Created by Kosho on 2020/8/15.
//

#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "util/map.hpp"
#include "object/hiInteger.hpp"
#include "interpreter.hpp"
#include "../code/bytecode.hpp"
#include "universe.hpp"
#include "functionObject.hpp"
#include "stringTable.hpp"

#define PUSH(x)       _frame->stack()->append((x))
#define POP()         _frame->stack()->pop()
#define TOP()         _frame->stack()->top()
#define STACK_LEVEL() _frame->stack()->size()

#define HI_TRUE       Universe::HiTrue
#define HI_FALSE      Universe::HiFalse

Interpreter *Interpreter::_instance = NULL;

Interpreter *Interpreter::get_instance() {
    if (_instance == NULL) {
        _instance = new Interpreter();
    }

    return _instance;
}

Interpreter::Interpreter() {
    _frame = NULL;

    _builtins = new HiDict();
    _builtins->put(new HiString("True"), Universe::HiTrue);
    _builtins->put(new HiString("False"), Universe::HiFalse);
    _builtins->put(new HiString("None"), Universe::HiNone);

    _builtins->put(new HiString("len"), new FunctionObject(len));
}

void Interpreter::run(CodeObject *codes) {
    _frame = new FrameObject(codes);

    eval_frame();
    destroy_frame();
}

void Interpreter::eval_frame() {
    Block *b;
    FunctionObject *fo;
    ArrayList<HiObject *> *args = NULL;

    HiObject *lhs, *rhs;
    HiObject *v, *w, *u, *attr;

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

            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;

            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;

            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
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
                w = v->get_attr(StringTable::get_instance()->next_str);
                build_frame(w, NULL);

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
                b = _frame->_loop_stack->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                _frame->set_pc(b->_target);
                break;

            case ByteCode::POP_BLOCK:
                b = _frame->_loop_stack->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
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

            case ByteCode::STORE_MAP:
                w = POP();
                u = POP();
                v = TOP();
                ((HiDict*)v)->put(w , u);
                break;

            case ByteCode::LOAD_ATTR:
                v = POP();
                w = _frame->_names->get(op_arg);
                PUSH(v->get_attr(w));
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

                    default:
                        printf("Error: Unrecognized compare op %d\n", op_arg);
                }
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

            case ByteCode::SETUP_LOOP:
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

            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    args = new ArrayList<HiObject *>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                build_frame(POP(), args);

                if (args != NULL) {
                    delete args;
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
                    delete args;
                    args = NULL;
                }

                PUSH(fo);
                break;

            case ByteCode::RETURN_VALUE:
                _ret_value = POP();
                if (_frame->is_first_frame()) {
                    return;
                }

                leave_frame();
                break;

            default:
                printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}

void Interpreter::destroy_frame() {
    FrameObject *temp = _frame;
    _frame = _frame->sender();
    delete temp;
}

void Interpreter::build_frame(HiObject *callable, ObjList args) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject *) callable)->call(args));
    } else if (callable->klass() == MethodKlass::get_instance()) {
        MethodObject *method = (MethodObject *) callable;
        if (!args) {
            args = new ArrayList<HiObject *>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args);
    } else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject *frame = new FrameObject((FunctionObject *) callable, args);
        frame->set_sender(_frame);
        _frame = frame;
    }
}

void Interpreter::leave_frame() {
    destroy_frame();
    PUSH(_ret_value);
}
