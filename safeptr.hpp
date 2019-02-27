#ifndef SAFEPTR
#define SAFEPTR

#include "safebuffer.h"

namespace safety {
    template<typename T>
    class safeptr : public safebuffer {
    public:
        explicit safeptr(size_t el_num) : safebuffer(el_num * sizeof(T)) {}

        safeptr() : safebuffer(sizeof(T)) {}

        T &operator*() {
            return *dynamic_cast<T *>(this->raw_ptr());
        }

        const T &operator*() const {
            return *dynamic_cast<T *>(this->raw_ptr());
        }

        safeptr(safeptr<T> &&other) noexcept : safebuffer(dynamic_cast<safebuffer>(other)) {}

        safeptr(const safeptr<T> &other) : safebuffer(dynamic_cast<safebuffer>(other)) {}

        T &operator[](size_t idx) {
            if (idx * sizeof(T) >= this->get_size()) {
                F_NEW_LOG_ENTRY("out of bounds");
                throw std::runtime_error("out of bounds error, idx = " + std::to_string(idx) + ", while size = " +
                                         std::to_string(get_size()));
            }
            return *dynamic_cast<T *>(raw_ptr() + idx * sizeof(T));
        }

        const T &operator[](size_t idx) const {
            if (idx * sizeof(T) >= this->get_size()) {
                F_NEW_LOG_ENTRY("out of bounds");
                throw std::runtime_error("out of bounds error, idx = " + std::to_string(idx) + ", while size = " +
                                         std::to_string(get_size()));
            }
            return *dynamic_cast<T *>(raw_ptr() + idx * sizeof(T));
        }
    };
}

#endif