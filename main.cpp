#include <iostream>
#include "safeptr.hpp"

#include <logger.h>

int main() {
    safety::safeptr<int> ptr;
    ptr.move_init(new int);
    return 0;
}