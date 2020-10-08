#include <iostream>
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "memory/heap.hpp"

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("vm need a param : filename\n");
        return 0;
    }

    Universe::genesis();
    BufferedInputStream stream(argv[1]);
    BinaryFileParser parser(&stream);
    Universe::main_code = parser.parse();
    // TODO Function default GC有问题
    // Universe::heap->gc();

    Interpreter::get_instance()->run(Universe::main_code);

    return 0;
}