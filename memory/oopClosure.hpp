//
// Created by Kosho on 2020/9/18.
//

#ifndef PYTHONVM_OOPCLOSURE_HPP
#define PYTHONVM_OOPCLOSURE_HPP

class Space;

class Heap;

class HiObject;

class Klass;

template<typename T>
class ArrayList;

template<typename K, typename V>
class Map;

template<typename T>
class Stack;

class OopClosure {
public:
    virtual void do_oop(HiObject **obj) = 0;

    virtual void do_array_list(ArrayList<Klass *> **alist) = 0;

    virtual void do_array_list(ArrayList<HiObject *> **alist) = 0;

    virtual void do_map(Map<HiObject *, HiObject *> **amap) = 0;

    virtual void do_raw_mem(char **mem, int length) = 0;

    virtual void do_klass(Klass **k) = 0;
};

class ScavengeOopClosure : public OopClosure {
private:
    Space *_from;
    Space *_to;
    Space *_meta;

    Stack<HiObject *> *_oop_stack;

    HiObject *copy_and_push(HiObject *obj);

public:
    ScavengeOopClosure(Space *from, Space *to, Space *meta);

    ~ScavengeOopClosure();

    virtual void do_oop(HiObject **oop);

    virtual void do_array_list(ArrayList<Klass *> **alist);

    virtual void do_array_list(ArrayList<HiObject *> **alist);

    virtual void do_map(Map<HiObject *, HiObject *> **amap);

    virtual void do_raw_mem(char **mem, int length);

    // CAUTION : we do not move Klass, because they locate at MetaSpace.
    virtual void do_klass(Klass **k);

    void scavenge();

    void process_roots();
};

#endif //PYTHONVM_OOPCLOSURE_HPP
