#include <iostream>

#include <logger.h>
#include "safebuffer.h"

int main() {
    const size_t size = 10;
    auto buf = safety::safebuffer(size);
    if(not buf.is_consistant()){
        std::cout<<processor::log<<std::endl;
        return 0;
    }
    std::cout<<buf.get_size()<<std::endl;
    std::cout<<"buf is consistant after constructor"<<std::endl;
    auto tmp = new char[size]{7};
    std::cout<<"tmp buffer:";
    for(int i = 0; i < size; i++){
        std::cout<<+tmp[i];
    }
    std::cout<<std::endl;
    if(buf.get(0, size, tmp) < 0){
        std::cout<<processor::log<<std::endl;
        return 0;
    }
    std::cout<<"tmp buffer after get:";
    for(int i = 0; i < size; i++){
        std::cout<<+tmp[i];
    }
    std::cout<<std::endl;
    std::cout<<"get is ok"<<std::endl;
    for(int i = 0; i < size; i++){
        tmp[i] = static_cast<char>(i);
    }
    std::cout<<"setting buf with tmp"<<std::endl;
    if(buf.set(tmp, tmp + size) < 0){
        std::cout<<processor::log<<std::endl;
        return 0;
    }
    std::cout<<"set is ok"<<std::endl;
    std::cout<<"resetting tmp with zeros"<<std::endl;
    for(int i = 0; i < size; i++){
        tmp[i] = 0;
    }
    if(buf.get(0, size, tmp) < 0){
        std::cout<<processor::log<<std::endl;
        return 0;
    }
    std::cout<<"tmp buffer after gett:";
    for(int i = 0; i < size; i++){
        std::cout<<+tmp[i];
    }
    std::cout<<std::endl;
    return 0;
}