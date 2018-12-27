//
// Created by Kirill on 2018-12-27.
//
#define SEC_MODE
#include <logger.h>

#ifndef SAFEPTR_SAFEBUFFER_H
#define SAFEPTR_SAFEBUFFER_H

namespace safety{
    class safebuffer{
        volatile static const char secret;
        size_t size;
        char* buf;
        int check_canaries();
        int reset_canaries();
    public:
        safebuffer();
        explicit safebuffer(size_t size);
        int get(size_t start, size_t finish, char* buf);
        int set(size_t start, size_t finish, char* buf);
    };
}

#endif //SAFEPTR_SAFEBUFFER_H
