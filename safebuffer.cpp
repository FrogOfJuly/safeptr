//
// Created by Kirill on 2018-12-27.
//

#include "safebuffer.h"

volatile const char safety::safebuffer::secret = static_cast<const char>(rand() % 255);

safety::safebuffer::safebuffer() {
    buf = nullptr;
    size = 0;
}

int safety::safebuffer::reset_canaries() {
    if (buf == nullptr) {
        return 0;
    }
    buf[0] = secret;
    buf[size + 1] = secret;
    return 0;
}


int safety::safebuffer::check_canaries() const noexcept {
    if (buf == nullptr) {
        return 0;
    }
    bool suc = true;
    if (buf[0] != secret) {
        suc = false;
        F_NEW_LOG_ENTRY("first canary is corrupted");
    }
    if (buf[size + 1] != secret) {
        suc = false;
        F_NEW_LOG_ENTRY("second canary is corrupted");
    }
    if (suc)
        return 0;
    return -1;
}

safety::safebuffer::safebuffer(size_t size) : safebuffer() {
    if (buf != nullptr or this->size != 0) {
        throw std::runtime_error("buffer is already initialized");
    }
    buf = new char[size + 2]{0};
    this->size = size;
    this->reset_canaries();
}

safety::safebuffer::~safebuffer() {
    if (check_canaries() < 0) {
        F_NEW_LOG_ENTRY("safe buffer is corrupted");
    }
}

bool safety::safebuffer::is_consistant() const {
    return check_canaries() == 0;
}

size_t safety::safebuffer::get_size() const {
    return size;
}

const char *safety::safebuffer::raw_ptr() const {
    if (check_canaries() != 0) {
        F_NEW_LOG_ENTRY("safe buffer is corrupted");
        return nullptr;
    }
    if (buf == nullptr) {
        return nullptr;
    }
    return buf + 1;
}

safety::safebuffer::safebuffer(const safety::safebuffer &other) : safebuffer() {
    if (other.check_canaries() != 0) {
        throw std::runtime_error("buffer you are coping is corrupted");
    }
    buf = other.buf;
    size = other.size;
}

safety::safebuffer::safebuffer(safety::safebuffer &&other) noexcept {
    if (other.check_canaries() != 0) {
        F_NEW_LOG_ENTRY("something just returned corrupted safe buffer");
    }
    buf = other.buf;
    size = other.size;
    other.buf = nullptr;
    other.size = 0;
}

void safety::safebuffer::erase() {
    if (check_canaries() != 0) {
        F_NEW_LOG_ENTRY("trying to erase corrupted safe buffer");
    }
    delete[] buf;
    size = 0;
}

safety::safebuffer &safety::safebuffer::operator=(const safety::safebuffer &other) {
    buf = other.buf;
    size = other.size;
    return *this;
}


