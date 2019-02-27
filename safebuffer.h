//
// Created by Kirill on 2018-12-27.
//
#define SEC_MODE

#include <logger.h>

#ifndef SAFEPTR_SAFEBUFFER_H
#define SAFEPTR_SAFEBUFFER_H

namespace safety {
    class safebuffer {
        volatile static const char secret;
        size_t size;
        char *buf;

        int check_canaries() const noexcept;

        int reset_canaries();

    public:
        ~safebuffer();

        safebuffer();

        explicit safebuffer(size_t size);

        safebuffer(const safebuffer &other);

        safebuffer(safebuffer &&other) noexcept;

        const char *raw_ptr() const;

        bool is_consistant() const;

        size_t get_size() const;

        void erase();

        safebuffer&operator=(const safebuffer& other);
    };
}

#endif //SAFEPTR_SAFEBUFFER_H
