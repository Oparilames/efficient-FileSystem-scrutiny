/*
 * Copyright (C) 2023 S. T. Hoffmann.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3.0 of the License, or (at your option) any later
 * version. See the full text of the license in the base directory of where this
 * source code is published or http://www.gnu.org/copyleft/gpl.html 
 */

#ifndef HEADER_EFSCRU_FSTREE_HPP__
#define HEADER_EFSCRU_FSTREE_HPP__

#ifdef __cplusplus
   extern "C" {
#endif

namespace eFScru {
struct FSTree {
    enum class prescanMode{full,deepestOnly};
    
    FSTree()=delete;
    FSTree(FSTree::prescanMode asDefault):defaultMode(asDefault){}
    
    int count(std::string_view inPath);
    int count(std::string_view inPath, prescanMode toCount);
    
    int collect(std::string_view forPath);
    int collect(std::string_view forPath, prescanMode toCollect);
    enum class showDirContent:bool{yes=true,no=false};
    void print(showDirContent what);
    
private:
    enum state {all2do, needsCounting, hasCounted, hasCollected, completedGeneration} mState{all2do}; // for order of commands
    
    bool isIllegalDirEntry(const struct dirent *currentEntry) const;
    
    void correctEntryPathName(const std::string_view& path, std::string& fullEntryName, struct dirent *currentEntry);
    
    int& countNodes(const std::string_view path, int& continueCounterAt);
    int& countLeaves(const std::string_view path, int& continueCounterAt);
    
    void collectNodes(std::string_view path);
    void collectLeaves(std::string_view path);
    
    void showNonDirectoryContentOf(std::string_view path);

    std::vector<std::string> content; // was std::vector<T>
    const prescanMode defaultMode;
};
} // HEADER_EFSCRU_FSTREE_HPP__

#ifdef __cplusplus
   }
#endif

#endif // HEADER_EFSCRU_FSTREE_HPP__
