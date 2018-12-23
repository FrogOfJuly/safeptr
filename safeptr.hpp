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
        const size_t static size;
        char *buffer;
        int check_canaries() const{
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
            buffer = nullptr;
        }

        int copy_init(const T* ptr){
            ASSERT(buffer, nullptr);
            ASSERT(size, 0);
            if(ptr == nullptr){
                return 0;
            }
            buffer = new(std::nothrow) char[size + 2];
            if(buffer == nullptr){
                F_NEW_LOG_ENTRY("can't allocate memory of size: " + std::to_string(safeptr::size));
                return -1;
            }
            buffer[0] = canary_secret;
            buffer[safeptr::size + 1] = canary_secret;
            reinterpret_cast<T*>(buffer + 1) = *ptr;
            return 0;
        }

        int move_init(void* ptr, size_t ptr_size){
            SAFE_CALL(copy_init(ptr, ptr_size));
            delete reinterpret_cast<T*>(ptr);
            return 0;
        }

        bool is_consistant() const{
            return check_canaries() == 0;
        }
    };

    template<class T>
    const char safeptr<T>::canary_secret = (char) (rand());
    template<class T>
    const size_t safeptr<T>::size = sizeof(T);
}


#endif //SAFEPTR_SAFEPTR_H
