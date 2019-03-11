//
// Created by Kirill on 2019-03-10.
//

#ifndef SAFEPTR_SAFEPTR_H
#define SAFEPTR_SAFEPTR_H

namespace utils {
    class SafePtr {
    protected:
        const static char SafePtr_secret;
        const volatile char first_canary = SafePtr_secret;
        char *ptr; //NO OWNERSHIP
        size_t size;
        const volatile char second_canary = SafePtr_secret;

        bool valid_outside_canaries() const noexcept;
        bool valid_inside_canaries() const noexcept;
        void reset_inside_canaries() noexcept;

    public:
        virtual ~SafePtr();

        SafePtr() noexcept;

        explicit SafePtr(size_t amount);

        SafePtr(const SafePtr &other);

        SafePtr &operator=(const SafePtr &other);

        bool is_valid() const noexcept;

        void erase() noexcept;

    };
}


#endif //SAFEPTR_SAFEPTR_H
