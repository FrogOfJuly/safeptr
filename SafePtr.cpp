//
// Created by Kirill on 2019-03-10.
//
#include <cstdlib>
#include "SafePtr.h"
#include "logger.h"

const char utils::SafePtr::SafePtr_secret = (char) (rand() % 255);

utils::SafePtr::~SafePtr() {
    ptr = nullptr;
    size = 0;
}

utils::SafePtr::SafePtr() noexcept {
    ptr = nullptr;
    size = 0;
}

bool utils::SafePtr::valid_outside_canaries() const noexcept {
    bool valid = true;
    if (first_canary != SafePtr_secret) {
        valid = false;
        F_NEW_LOG_ENTRY("first outside canary is invalid");
    }
    if (second_canary != SafePtr_secret) {
        valid = false;
        F_NEW_LOG_ENTRY("second outside canary is invalid");
    }
    return valid;
}

utils::SafePtr::SafePtr(const utils::SafePtr &other) : SafePtr() {
    bool valid_op = true;
    if (not valid_inside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("inside canaries of the left operand are invalid");
    }
    if (not valid_outside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("outside canaries of the left operand are invalid");
    }
    if (not other.valid_inside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("inside canaries of the right operand are invalid");
    }
    if (not other.valid_outside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("outside canaries of the right operand are invalid");
    }
    if (not valid_op) {
        F_NEW_LOG_ENTRY("throwing runtime error");
        throw std::runtime_error("assignment operation failed due to invalid arguments. See log for deteiles");
    }
    ptr = other.ptr;
    size = other.size;
}

utils::SafePtr &utils::SafePtr::operator=(const utils::SafePtr &other) {
    bool valid_op = true;
    if (not valid_inside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("inside canaries of the left operand are invalid");
    }
    if (not valid_outside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("outside canaries of the left operand are invalid");
    }
    if (not other.valid_inside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("inside canaries of the right operand are invalid");
    }
    if (not other.valid_outside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("outside canaries of the right operand are invalid");
    }
    if (not valid_op) {
        F_NEW_LOG_ENTRY("throwing runtime error");
        throw std::runtime_error("assignment operation failed due to invalid arguments. See log for details");
    }
    ptr = other.ptr;
    size = other.size;
    return *this;
}

void utils::SafePtr::erase() noexcept {
    bool valid_op = true;
    if (not valid_inside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("inside canaries of the left operand are invalid");
    }
    if (not valid_outside_canaries()) {
        valid_op = false;
        F_NEW_LOG_ENTRY("outside canaries of the left operand are invalid");
    }
    delete[] ptr;
    size = 0;
}

bool utils::SafePtr::valid_inside_canaries() const noexcept {
    bool valid = true;
    if (ptr == nullptr) {
        return true;
    }
    if (ptr[0] != SafePtr_secret) {
        F_NEW_LOG_ENTRY("first inside canary is invalid");
        valid = false;
    }
    if (ptr[size + 1] != SafePtr_secret) {
        F_NEW_LOG_ENTRY("second inside canary is invalid");
        valid = false;
    }
    return valid;
}

bool utils::SafePtr::is_valid() const noexcept {
    return valid_outside_canaries() and valid_inside_canaries();
}

void utils::SafePtr::reset_inside_canaries() noexcept {
    if (ptr == nullptr)
        return;
    ptr[0] = SafePtr_secret;
    ptr[size + 1] = SafePtr_secret;
}


utils::SafePtr::SafePtr(size_t amount) : SafePtr() {
    ptr = new char[amount + 2]{0}; //can throw
    //--------------------
    size = amount;
    reset_inside_canaries(); //can't throw
}

