/*
 * Copyright (C) 2023 S. T. Hoffmann.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3.0 of the License, or (at your option) any later
 * version. See the full text of the license in the base directory of where this
 * source code is published or http://www.gnu.org/copyleft/gpl.html 
 */

#include <eFScru/header.hpp>
#include <iostream>
#include <eFScru/dirStream.inl>
#include <eFScru/FSTree.hpp>
#include <iterator> // for std::ostream_iterator
#include <algorithm> // for std::ranges::copy depending on lib support

namespace eFScru {

int FSTree::count(std::string_view inPath)
{
    return count(inPath,defaultMode);
}

// to notice: toCount is not overwriting defaultMode.
int FSTree::count(std::string_view inPath, FSTree::prescanMode toCount) {
    if(mState != FSTree::state::needsCounting && mState != FSTree::state::all2do) {std::cout << __LINE__ << "\n"; return -1;}
    
    mState = FSTree::state::needsCounting;
    
    int retVal{0};
    #ifdef __DEBUGMODE_ON
    std::cout << "Counting »" << inPath << "«.\n";
    #endif
    
    if(toCount == FSTree::prescanMode::full)
        content.reserve(countNodes(inPath,retVal)+1);
    
    if(toCount == FSTree::prescanMode::deepestOnly)
        content.reserve(countLeaves(inPath,retVal)+1);
    #ifdef __DEBUGMODE_ON
    std::cout << "Memory reserved for " << content.capacity() << " entries.\n";
    #endif
    mState = FSTree::state::hasCounted;
    return retVal;
}



int FSTree::collect(std::string_view inPath)
{
    return collect(inPath,defaultMode);
}

int FSTree::collect(std::string_view inPath, FSTree::prescanMode toCollect) {
    if(mState != FSTree::state::hasCounted)  {std::cout << __LINE__ << "Warning: space for vector wasn't allocated beforehand.\n"; } // used to do return -1;
    
    #ifdef __DEBUGMODE_ON
    std::cout << "Collecting »" << inPath << "«.\n";
    #endif
    
    if(toCollect == FSTree::prescanMode::full)
        collectNodes(inPath);
        
    if(toCollect == FSTree::prescanMode::deepestOnly)
        collectLeaves(inPath);
        
    mState = FSTree::state::hasCollected;
    
    if(content.empty()) {std::cout << __LINE__ << "\n"; return -1;}
    
    content.emplace_back(inPath.data());
    
    mState = FSTree::state::completedGeneration;
    #ifdef __DEBUGMODE_ON
    std::cout << "Filled: " << content.size() << "/" << content.capacity() << "\n";
    #endif
    return content.size();
    
}

void FSTree::print(showDirContent what) {
    if(mState != FSTree::state::completedGeneration ){ std::cout << __LINE__ << "\n"; return; }
    if(what==showDirContent::yes)
        for(auto&  entry : content) {
            std::cout << "[D] " << entry << "\n";
            showNonDirectoryContentOf(entry);
        }
    else {
        for(auto&  entry : content) 
            std::cout << "[D] " << entry << "\n";
        }
}



/// private methods
inline bool FSTree::isIllegalDirEntry(const struct dirent *currentEntry) const {
    if(strcmp(currentEntry->d_name, "..") == (int)NULL) return true;
    if(strcmp(currentEntry->d_name, ".") == (int)NULL) return true;
        
    // Non-POSIX method to escape symbolic link loops.
    if(!direntIsPOSIXConform)
        if( currentEntry->d_type != DT_DIR ) return true;
        
    return false;
}


inline void FSTree::correctEntryPathName(const std::string_view& path, std::string& fullEntryName, struct dirent *currentEntry) {
    if(path[path.size()-1]!='/')
        fullEntryName=std::string{path.data()}+"/"+currentEntry->d_name;
    else
        fullEntryName=std::string{path.data()}+currentEntry->d_name;
}


void FSTree::collectNodes(std::string_view forPath){
    dirStream dStream{forPath};
    if(dStream.status == NULL) return;
    
    struct dirent *currentEntry;
    std::string fullEntryName;
    
    while((currentEntry=readdir(dStream.status))!=NULL) {
        if(isIllegalDirEntry(currentEntry)) continue;
        
        correctEntryPathName(forPath, fullEntryName, currentEntry);
        
        dirStream tmpStr{fullEntryName};
        if(tmpStr.status!=NULL)
            collectNodes(content.emplace_back(fullEntryName));
    }
    return;
 }

int& FSTree::countNodes(const std::string_view path, int& continueCounterAt) {
    dirStream dStream{path};
    if(dStream.status == NULL) return continueCounterAt;
    
    struct dirent *currentEntry;
    std::string fullEntryName;
    
    while((currentEntry=readdir(dStream.status))!=NULL) {
        if(isIllegalDirEntry(currentEntry)) continue;
        
        correctEntryPathName(path, fullEntryName, currentEntry);
        
        dirStream tmpStr{fullEntryName};
        if(tmpStr.status!=NULL)
            countNodes(fullEntryName, ++continueCounterAt);
    }
    return continueCounterAt;
}

int& FSTree::countLeaves(std::string_view path, int& continueCounterAt) {
    dirStream dStream{path};
    if(dStream.status == NULL) return continueCounterAt;
    
    struct dirent *currentEntry;
    std::string fullEntryName;
        int dirsInThisOne{0};
        while((currentEntry=readdir(dStream.status))!=NULL) {
            if(isIllegalDirEntry(currentEntry)) continue;
            
            correctEntryPathName(path, fullEntryName, currentEntry);
            
            dirStream tmpStr{fullEntryName};
            if(tmpStr.status!=NULL) {
                countLeaves(fullEntryName, continueCounterAt);
                ++dirsInThisOne;
            }
        }
    if(dirsInThisOne==0) {
        ++continueCounterAt;
    }
    return continueCounterAt;
}

void FSTree::collectLeaves(std::string_view forPath) {
    dirStream dStream{forPath};
    if(dStream.status == NULL) return;
    
    struct dirent *currentEntry;
    std::string fullEntryName;
    int dirsInThisOne{0};
    while((currentEntry=readdir(dStream.status))!=NULL) {
        if(isIllegalDirEntry(currentEntry)) continue;
            
        correctEntryPathName(forPath, fullEntryName, currentEntry);
        
        dirStream tmpStr{fullEntryName};
        if(tmpStr.status!=NULL) {
            collectLeaves(fullEntryName);
            ++dirsInThisOne;
        }
    }
    if(dirsInThisOne==0) {
        content.emplace_back(forPath.data());
    }
    return;
}


void FSTree::showNonDirectoryContentOf(std::string_view path) {
    dirStream dStream{path};
    if(dStream.status == NULL) return;
    
    std::string fullEntryName;
    struct dirent *currentEntry;
    int cntr{0};
    while((currentEntry=readdir(dStream.status))!=NULL) {
        // Non-POSIX method to escape symbolic link loops.
        if(!direntIsPOSIXConform)
            if( currentEntry->d_type == DT_DIR ) continue;
        
        correctEntryPathName(path, fullEntryName, currentEntry);
        std::cout << "      »" << fullEntryName << "« (not browsable)" << "\n";
    }
}

} // namespace eFScru
