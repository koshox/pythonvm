//
// Created by Kosho on 2020/8/15.
//

#ifndef PYTHONVM_UNIVERSE_HPP
#define PYTHONVM_UNIVERSE_HPP

#include <stdio.h>

class Klass;
class Heap;
class HiInteger;
class HiObject;
class CodeObject;

template <typename T>
class ArrayList;

class Universe {
public:
    static HiObject *HiTrue;
    static HiObject *HiFalse;

    static HiObject *HiNone;

    static ArrayList<Klass*>* klasses;

    static Heap *heap;

public:
    static void genesis();

    static void destory();

};

#endif //PYTHONVM_UNIVERSE_HPP
