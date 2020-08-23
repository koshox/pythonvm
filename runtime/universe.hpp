//
// Created by Kosho on 2020/8/15.
//

#ifndef PYTHONVM_UNIVERSE_HPP
#define PYTHONVM_UNIVERSE_HPP

#include <stdio.h>
#include <object/hiInteger.hpp>

class Klass;
class HiInteger;
class HiObject;
class CodeObject;

class Universe {
public:
    static HiObject *HiTrue;
    static HiObject *HiFalse;

    static HiObject *HiNone;

public:
    static void genesis();

    static void destory();

};

#endif //PYTHONVM_UNIVERSE_HPP
