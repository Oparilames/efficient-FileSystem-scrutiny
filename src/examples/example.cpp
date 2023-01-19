/*
 * Copyright (C) 2023 S. T. Hoffmann.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3.0 of the License, or (at your option) any later
 * version. See the full text of the license in the base directory of where this
 * source code is published or http://www.gnu.org/copyleft/gpl.html 
 */

#include <iostream>
#include <eFScru/header.hpp>
#include <eFScru/posixlinux.inl>
#include <eFScru/dirStream.inl>
#include <eFScru/FSTree.hpp>

struct argMan {
    inline static const std::string_view skipStr[]{"SKIP_PRINTOUT", "SKIP_PREALLOCATION"};
    
    int ID{0};
    bool skip{false}, alreadyChanged{false};
    
    
    argMan(int i):ID(i){}
    argMan()=delete;
    void activateIfMatch(std::string_view param) {
        if(alreadyChanged) return;
        
        if(!matches(param)) return;
        std::cout << skipStr[ID] << " active.\n";
        skip=true;
        alreadyChanged=true;
    }
    
    
    bool matches(std::string_view param){return param==skipStr[ID];}
    
    struct returnStruct{
        bool allowPrintout{false}, allowPreAllocation{false};
        
        static returnStruct evaluate(int argc, char* argv[]) {
            returnStruct returnValue;
            argMan doPrintout{0}, doPreAllocation{1};
            for(int i=0; i<argc; ++i){
                doPrintout.activateIfMatch(argv[i]);
                doPreAllocation.activateIfMatch(argv[i]);
            }
            if(doPreAllocation.skip) std::cout << "No preallocation!\n";
            else returnValue.allowPreAllocation=true;
            
            if(doPrintout.skip) std::cout << "No printout!\n";            
            else returnValue.allowPrintout=true;
            return returnValue;
        }
    };
};
int main(int argc, char* argv[]) {
    const auto mode{eFScru::FSTree::prescanMode::full}; // full, deepestOnly
    const auto toPrint{eFScru::FSTree::showDirContent::no}; 
    const std::string_view searchPath{"/usr/share/doc"};
    eFScru::FSTree tree(mode);
    
    auto checker{argMan::returnStruct::evaluate(argc,argv)};
    
    if(checker.allowPreAllocation) tree.count(searchPath);
    
    tree.collect(searchPath);
    if(checker.allowPrintout) tree.print(toPrint);
    
}

/*
    g++ -std=c++20 -c -o dirStream.o dirStream.inl  -Wfatal-errors &&
    g++ -std=c++20 -c -o FSTree.o FSTree.cpp  -Wfatal-errors -fPIC &&
    gcc -shared -Wfatal-errors -fPIC -o libeFScru.so FSTree.o
    mkdir -p ./lib ./bin && mv libeFScru.so ./lib/libeFScru.so
    export LD_LIBRARY_PATH=./lib/:$LD_LIBRARY_PATH
    g++ -L./lib -Wall -o ./bin/test libTest.cpp -leFScru && ./bin/test
    g++ -L./lib -Wall -o ./bin/example example.cpp -leFScru && ./bin/example
 */
/*#!/bin/bash

function testIt {
    exe=/home/oparilames/Programming/C++/2023/eFScru/bin/example
    $exe aaa && sleep 3 &&
    $exe aaa bbb && sleep 3 &&
    $exe aaa bbb ccc && sleep 3 &&
    $exe aaa bbb ccc ddd && sleep 3
    $exe aaa bbb ccc ddd eee && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i j  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i j k  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i j k l  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i j k l m  && sleep 10 &&
    $exe aaa bbb ccc ddd eee fff g h i j k l m n
}

testIt

./make.sh && $testIt
*/
