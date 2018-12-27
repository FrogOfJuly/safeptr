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


int safety::safebuffer::check_canaries() {
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
