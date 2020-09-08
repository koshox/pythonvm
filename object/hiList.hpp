//
// Created by Kosho on 2020/8/20.
//

#ifndef PYTHONVM_HILIST_HPP
#define PYTHONVM_HILIST_HPP

#include "klass.hpp"
#include "object/hiObject.hpp"
#include "util/arrayList.hpp"

class ListKlass : public Klass {
private:
    ListKlass();

    static ListKlass *instance;

public:
    static ListKlass *get_instance();

    virtual HiObject *add(HiObject *x, HiObject *y);
    virtual HiObject *mul(HiObject *x, HiObject *y);

    virtual void print(HiObject *obj);
    virtual HiObject *subscr(HiObject *x, HiObject *y);
    virtual void store_subscr(HiObject *x, HiObject *y, HiObject *z);
    virtual void del_subscr(HiObject *x, HiObject *y);
    virtual HiObject *contains(HiObject *x, HiObject *y);
    virtual HiObject *less(HiObject *x, HiObject *y);
    virtual HiObject *len(HiObject *x);
    virtual HiObject *iter(HiObject* x);

    virtual HiObject *allocate_instance(HiObject* callable, ArrayList<HiObject *> *args);
};

class HiList : public HiObject {
    friend class ListKlass;

private:
    ArrayList<HiObject *> *_inner_list;

public:
    HiList();
    HiList(ObjList ol);
    ArrayList<HiObject*>* inner_list()  { return _inner_list; }

    int size()                          { return _inner_list->size(); }
    void append(HiObject* obj)          { _inner_list->add(obj); }
    void insert(int i, HiObject* o)     { _inner_list->insert(i, o); }
    HiObject* pop()                     { return _inner_list->pop(); }
    HiObject* get(int index)            { return _inner_list->get(index); }
    void      set(int i, HiObject* o)   { _inner_list->set(i, o); }
    HiObject* top()                     { return get(size() - 1); }
};

HiObject *list_append(ObjList args);
HiObject *list_insert(ObjList args);
HiObject *list_index(ObjList args);
HiObject *list_pop(ObjList args);
HiObject *list_remove(ObjList args);
HiObject *list_reverse(ObjList args);
HiObject *list_sort(ObjList args);

class ListIteratorKlass : public Klass {
private:
    static ListIteratorKlass *instance;
    ListIteratorKlass();

public:
    static ListIteratorKlass *get_instance();
};

class ListIterator : public HiObject {
private:
    HiList* _owner;
    int _iter_cnt;

public:
    ListIterator(HiList *owner);

    HiList* owner()        { return _owner; }
    int iter_cnt()         { return _iter_cnt; }
    void inc_cnt()         { _iter_cnt++; }
};

HiObject *listiterator_next(ObjList args);

#endif //PYTHONVM_HILIST_HPP
