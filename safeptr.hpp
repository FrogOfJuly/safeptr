//
// Created by Kirill on 2018-12-23.
//

#ifndef SAFEPTR_SAFEPTR_H
#define SAFEPTR_SAFEPTR_H

namespace safety {
    template <class T>
    class safeptr {
        char *buffer;
    public:
        safeptr();
    };
}


#endif //SAFEPTR_SAFEPTR_H
