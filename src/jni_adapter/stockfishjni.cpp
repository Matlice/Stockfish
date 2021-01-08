//
// Created by stefano on 1/8/21.
//
#include <iostream>
#include "interface/interface.h"

int main(void){
    std::cout << *EngineInterface::getInstance() << std::endl;
    return 0;
}