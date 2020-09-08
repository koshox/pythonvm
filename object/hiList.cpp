//
// Created by Kosho on 2020/8/20.
//

#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

ListKlass *ListKlass::instance = NULL;

ListKlass::ListKlass() {
    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("append"), new FunctionObject(list_append));
    klass_dict->put(new HiString("insert"), new FunctionObject(list_insert));
    klass_dict->put(new HiString("index"), new FunctionObject(list_index));
    klass_dict->put(new HiString("pop"), new FunctionObject(list_pop));
    klass_dict->put(new HiString("remove"), new FunctionObject(list_remove));
    klass_dict->put(new HiString("reverse"), new FunctionObject(list_reverse));
    klass_dict->put(new HiString("sort"), new FunctionObject(list_sort));
    set_klass_dict(klass_dict);

    (new HiTypeObject())->set_own_klass(this);
    set_name(new HiString("list"));
    set_super(ObjectKlass::get_instance());
}

ListKlass *ListKlass::get_instance() {
    if (instance == NULL)
        instance = new ListKlass();

    return instance;
}

HiObject *ListKlass::add(HiObject *x, HiObject *y) {
    HiList *lx = (HiList *) x;
    assert(lx && lx->klass() == (Klass *) this);
    HiList *ly = (HiList *) y;
    assert(ly && ly->klass() == (Klass *) this);

    HiList *z = new HiList();
    for (int i = 0; i < lx->size(); i++) {
        z->inner_list()->set(i, lx->inner_list()->get(i));
    }

    for (int i = 0; i < ly->size(); i++) {
        z->inner_list()->set(i + lx->size(),
                             ly->inner_list()->get(i));
    }

    return z;
}

HiObject *ListKlass::mul(HiObject *x, HiObject *y) {
    HiList *lx = (HiList *) x;
    assert(lx && lx->klass() == (Klass *) this);
    HiInteger *iy = (HiInteger *) y;
    assert(iy && iy->klass() == IntegerKlass::get_instance());

    HiList *z = new HiList();
    for (int i = 0; i < iy->value(); i++) {
        for (int j = 0; j < lx->size(); j++) {
            z->inner_list()->set(i * lx->size() + j,
                                 lx->inner_list()->get(j));
        }
    }

    return z;
}

void ListKlass::print(HiObject *x) {
    HiList *lx = (HiList *) x;
    assert(lx && lx->klass() == (Klass *) this);

    printf("[");

    int size = lx->_inner_list->size();
    if (size >= 1)
        lx->_inner_list->get(0)->print();

    for (int i = 1; i < size; i++) {
        printf(", ");
        lx->_inner_list->get(i)->print();
    }
    printf("]");
}

HiObject *ListKlass::subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass *) this);
    assert(y && y->klass() == (Klass *) IntegerKlass::get_instance());

    HiList *lx = (HiList *) x;
    HiInteger *iy = (HiInteger *) y;

    return lx->inner_list()->get(iy->value());
}

void ListKlass::store_subscr(HiObject *x, HiObject *y, HiObject *z) {
    assert(x && x->klass() == (Klass *) this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiList *lx = (HiList *) x;
    HiInteger *iy = (HiInteger *) y;

    lx->inner_list()->set(iy->value(), z);
}

void ListKlass::del_subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass *) this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiList *lx = (HiList *) x;
    HiInteger *iy = (HiInteger *) y;

    lx->inner_list()->delete_index(iy->value());
}

HiObject *ListKlass::contains(HiObject *x, HiObject *y) {
    HiList *lx = (HiList *) x;
    assert(lx && lx->klass() == (Klass *) this);

    int size = lx->inner_list()->size();
    for (int i = 0; i < size; ++i) {
        if (lx->inner_list()->get(i)->equal(y)) {
            return Universe::HiTrue;
        }
    }

    return Universe::HiFalse;
}

HiObject *ListKlass::less(HiObject *x, HiObject *y) {
    HiList *lx = (HiList *) x;
    assert(lx && lx->klass() == (Klass *) this);

    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::HiTrue;
        } else {
            return Universe::HiFalse;
        }
    }

    HiList *ly = (HiList *) y;
    assert(ly && ly->klass() == (Klass *) this);

    int len = lx->size() < ly->size() ?
              lx->size() : ly->size();

    for (int i = 0; i < len; i++) {
        if (lx->get(i)->less(ly->get(i)) == Universe::HiTrue) {
            {
                return Universe::HiTrue;
            }
        } else if (lx->get(i)->equal(ly->get(i)) != Universe::HiTrue) {
            {
                return Universe::HiFalse;
            }
        }
    }

    if (lx->size() < ly->size()) {
        return Universe::HiTrue;
    }

    return Universe::HiFalse;
}

HiObject *ListKlass::len(HiObject *x) {
    assert(x->klass() == this);
    return new HiInteger(((HiList *) x)->size());
}

HiObject *ListKlass::iter(HiObject *x) {
    assert(x && x->klass() == this);
    return new ListIterator((HiList *) x);
}

HiObject *ListKlass::allocate_instance(ArrayList<HiObject *> *args) {
    if (!args || args->length() == 0) {
        return new HiList();
    } else {
        return NULL;
    }
}

HiList::HiList() {
    set_klass(ListKlass::get_instance());
    _inner_list = new ArrayList<HiObject *>();
}

HiList::HiList(ObjList ol) {
    set_klass(ListKlass::get_instance());
    _inner_list = ol;
}

HiObject *list_append(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    HiObject *var = args->get(1);
    list->append(var);
    return Universe::HiNone;
}

HiObject *list_insert(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    HiObject *index = args->get(1);
    HiObject *var = args->get(2);

    HiInteger *i = (HiInteger *) index;
    assert(i && i->klass() == IntegerKlass::get_instance());

    list->insert(i->value(), var);
    return Universe::HiNone;
}

HiObject *list_index(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    HiObject *target = (HiObject *) (args->get(1));

    assert(list && list->klass() == ListKlass::get_instance());

    for (int i = 0; i < list->size(); ++i) {
        if (list->get(i)->equal(target) == Universe::HiTrue) {
            return new HiInteger(i);
        }
    }

    return NULL;
}

HiObject *list_pop(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());
    return list->pop();
}

HiObject *list_remove(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    HiObject *target = args->get(1);

    assert(list && list->klass() == ListKlass::get_instance());

    for (int i = 0; i < list->size(); ++i) {
        if (list->get(i)->equal(target) == Universe::HiTrue) {
            list->inner_list()->delete_index(i);
        }
    }

    return Universe::HiNone;
}

HiObject *list_reverse(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());

    int i = 0;
    int j = list->size() - 1;
    while (i < j) {
        HiObject *tmp = list->get(i);
        list->set(i, list->get(j));
        list->set(j, tmp);

        i++;
        j--;
    }

    return Universe::HiNone;
}

HiObject *list_sort(ObjList args) {
    HiList *list = (HiList *) (args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());

    // bubble sort
    int size = list->size();
    for (int i = 0; i < size; i++) {
        for (int j = size - 1; j > i; j--) {
            if (list->get(j)->less(list->get(j - 1)) == Universe::HiTrue) {
                HiObject *tmp = list->get(j);
                list->set(j, list->get(j - 1));
                list->set(j - 1, tmp);
            }
        }
    }

    return Universe::HiNone;
}

ListIteratorKlass *ListIteratorKlass::instance = NULL;

ListIteratorKlass *ListIteratorKlass::get_instance() {
    if (instance == NULL)
        instance = new ListIteratorKlass();

    return instance;
}

ListIteratorKlass::ListIteratorKlass() {
    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("next"),
                    new FunctionObject(listiterator_next));
    set_klass_dict(klass_dict);
}

ListIterator::ListIterator(HiList *list) {
    _owner = list;
    _iter_cnt = 0;
    set_klass(ListIteratorKlass::get_instance());
}

HiObject *listiterator_next(ObjList args) {
    ListIterator *iter = (ListIterator *) (args->get(0));

    HiList *alist = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < alist->inner_list()->size()) {
        HiObject *obj = alist->get(iter_cnt);
        iter->inc_cnt();
        return obj;
    } else {
        // TODO : we need Traceback here to mark iteration end
        return NULL;
    }
}