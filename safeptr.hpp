//
// Created by Kirill on 2018-12-23.
//

#ifndef SAFEPTR_SAFEPTR_H
#define SAFEPTR_SAFEPTR_H
#define SEC_MODE

#include <logger.h>

namespace safety {
    template<class T>
    class safeptr {
        const char static canary_secret;
        size_t size;
        char *buffer;
        int check_canaries() {
            if(buffer == nullptr){
                return 0;
            }
            bool suc = true;
            if (buffer[0] != canary_secret) {
                F_NEW_LOG_ENTRY("first canary is corrupted");
                suc = false;
            }
            if (buffer[size - 1] != canary_secret) {
                F_NEW_LOG_ENTRY("second canary is corrupted");
                suc = false;
            }
            if(suc)
                return 0;
            return -1;
        }

    public:
        ~safeptr() {
            if(check_canaries() < 0){
                FUNC_NEW_LOG_ENTRY("canary check failed");
            }
            delete buffer;
        }

        safeptr() {
            size = 0;
            buffer = nullptr;
        }

        int copy_init(const void * ptr, size_t ptr_size){
            ASSERT(buffer, nullptr);
            ASSERT(size, 0);
            if(ptr == nullptr and ptr_size != 0){
                F_NEW_LOG_ENTRY("cant init safe ptr wiht nullptr with non-zero size");
                return -1;
            }
            if(ptr == nullptr){
                return 0;
            }
            if(ptr_size % sizeof(T) != 0){
                F_NEW_LOG_ENTRY("given size is a product of type size: " + std::to_string(sizeof(T)));
                return -1;
            }
            buffer = new(std::nothrow) char[ptr_size + 2];
            if(buffer == nullptr){
                F_NEW_LOG_ENTRY("can't allocate memory of size: " + std::to_string(ptr_size));
                return -1;
            }
            buffer[0] = canary_secret;
            buffer[ptr_size - 1] = canary_secret;
            size = ptr_size;
            auto copy_ptr = reinterpret_cast<const char*>(ptr);
            std::copy(copy_ptr, copy_ptr + ptr_size, buffer + 1);
            return 0;
        }

        int move_init(void* ptr, size_t ptr_size){
            SAFE_CALL(copy_init(ptr, ptr_size));
            delete reinterpret_cast<T*>(ptr);
            return 0;
        }

        const T&operator[](int idx) const{
            return *reinterpret_cast<T*>(buffer + 1 + sizeof(T)*idx);
        }

         T&operator[](int idx) {
            return *reinterpret_cast<T*>(buffer + 1 + sizeof(T)*idx);
        }
    };

    template<class T>
    const char safeptr<T>::canary_secret = (char) (rand());
}


#endif //SAFEPTR_SAFEPTR_H
