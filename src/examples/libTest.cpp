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

void report() {
    std::cout << "=============\nToDo\n=============\n   std::vector<std::string> should get replaced with something more lightweigt.\n   refine with concepts so only (directorys,devices,… in short: paths;) / string-like datatypes can be used.\n   base on std::vector<pair<inode number,path-string>>\n   POSIX workaround for readdir-symlink-cycles\n   Proper parameter parsing\n   Adapt to POSIX compliance.\n=============\n\n";
}
void help() {
    std::cout << "=============\nHelp\n=============\n   Argument 1 (mandatory)    path to scan\n   Argument 2                l or a for less/all directories to be captured\n   Argument 3                if content of folders should be displayed too (y/N).\n=============\n\n";
}

int main(int argc, char* argv[]) {
    help();
    if(argc<2) {
        std::cout << "Error: Scan path missing!\n" << std::endl;
        return -1;
    }
    
    
    
    report();
    
    
    std::string chosenPath{argv[1]};
    std::string_view chosenPathSv{argv[1]};
    std::cout << "Path: " << chosenPath << "\n";
    
    // system((std::string("tree -dfi ")+chosenPath.data()+" > "+"./tree-output").c_str());
    eFScru::FSTree::prescanMode mode{eFScru::FSTree::prescanMode::deepestOnly};
    eFScru::FSTree::showDirContent showAll{eFScru::FSTree::showDirContent::no};
        
    if(argc>=3) {
        std::string_view method{argv[2]};
        if(method=="l" || method=="L")       mode=eFScru::FSTree::prescanMode::deepestOnly;
        else if (method=="a" || method=="a") mode=eFScru::FSTree::prescanMode::full;
    }
    
    if(argc>=4) {
        std::string_view showContent{argv[3]};
        if(showContent=="y" || showContent=="Y")       showAll=eFScru::FSTree::showDirContent::yes;
        else if (showContent=="n" || showContent=="N") showAll=eFScru::FSTree::showDirContent::no;
    }
    // if(showAll==eFScru::FSTree::showDirContent::no && mode==eFScru::FSTree::prescanMode::deepestOnly) std::cout << "Result should be similar to »(tree -dfi \""<<chosenPath<<"\" > ./tree-output\n";
    
    eFScru::FSTree tree(mode);
    tree.count(chosenPathSv);
    tree.collect(chosenPathSv);
    tree.print(showAll);
    return 0;
}
/*
    mkdir -p ./lib ./build ./bin
    g++ -std=c++20 -c -o build/dirStream.o dirStream.inl  -Wfatal-errors &&                                                                                                                1 ↵
    g++ -std=c++20 -c -o build/FSTree.o FSTree.cpp  -Wfatal-errors -fPIC && 
    gcc -shared -Wfatal-errors -fPIC -o lib/libeFScru.so FSTree.o
    export LD_LIBRARY_PATH=lib/:$LD_LIBRARY_PATH
    rm -fd ./build
    g++ -L./lib/ -Wall -o ./bin/test -leFScru libTest.cpp
    g++ -L./lib/ -Wall -o ./bin/example -leFScru example.cpp
 */
