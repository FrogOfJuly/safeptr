//
// Created by Kirill on 2018-12-27.
//

#include "safebuffer.h"

volatile const char safety::safebuffer::secret = static_cast<const char>(rand()%255);

safety::safebuffer::safebuffer() {
    buf = nullptr;
    size = 0;
}

int safety::safebuffer::reset_canaries() {
    if(buf == nullptr) {
        return 0;
    }
    buf[0] = secret;
    buf[size + 1] = secret;
    return 0;
}


int safety::safebuffer::check_canaries() const {
    if(buf == nullptr) {
        return 0;
    }
    bool suc = true;
    if(buf[0] != secret){
        suc = false;
        F_NEW_LOG_ENTRY("first canary is corrupted");
    }
    if(buf[size + 1] != secret){
        suc = false;
        F_NEW_LOG_ENTRY("second canary is corrupted");
    }
    if(suc)
        return 0;
    return -1;
}

safety::safebuffer::safebuffer(size_t size):safebuffer() {
    if(buf != nullptr or this->size != 0){
        throw std::runtime_error("buffer was already have been initialized");
    }
    buf = new char[size + 2];
    this->size = size;
    reset_canaries();
}

safety::safebuffer::~safebuffer() {
    if(check_canaries() < 0){
        F_NEW_LOG_ENTRY("safe buffer is corrupted");
    }
    delete[] buf;
    size = 0;
}

ssize_t safety::safebuffer::get(size_t start, size_t finish, char *buf) const{
    if(start >= size){
        F_NEW_LOG_ENTRY("start is out of range");
        return -1;
    }
    if(finish > size){
        F_NEW_LOG_ENTRY("finish is out of range");
        return -1;
    }
    if(buf == nullptr){
        F_NEW_LOG_ENTRY("buffer is nullptr");
        return -1;
    }
    while(start != finish){
        *(buf + finish - 1) = *(this->buf - 1 + finish + 1);
        finish--;
    }
    return finish - start;
}

ssize_t safety::safebuffer::set(char* start, char* finish) {
    if(start == nullptr){
        return -1;
    }
    if(finish == nullptr){
        return -1;
    }
    size_t i = 0;
    for(; start != finish; i++){
        if(i >= size){
            return i;
        }
        buf[i + 1] = *start;
        start++;
    }
    return i;
}

bool safety::safebuffer::is_consistant() const {
    return check_canaries() == 0;
}

size_t safety::safebuffer::get_size() const {
    return size;
}


