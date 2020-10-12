//
// Created by Kosho on 2020/10/13.
//

#ifndef PYTHONVM_HANDLES_HPP
#define PYTHONVM_HANDLES_HPP

class HiObject;

class OopClosure;

class Handle {
    friend class HandleMark;

private:
    HiObject *_value;
    Handle *_next;

public:
    Handle(HiObject *t);

    ~Handle();

    void oops_do(OopClosure *f);

    HiObject *operator->();

    HiObject *operator()() { return _value; }

    HiObject *resolve();
};

class HandleMark {
private:
    static HandleMark *instance;
    Handle *_head;

public:
    HandleMark();

    static HandleMark *get_instance();

    void oops_do(OopClosure *f);

    Handle *head() { return _head; }

    void set_head(Handle *x) { _head = x; }
};

#endif //PYTHONVM_HANDLES_HPP
