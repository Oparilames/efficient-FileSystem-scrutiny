/*
 * Copyright (C) 2023 S. T. Hoffmann.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3.0 of the License, or (at your option) any later
 * version. See the full text of the license in the base directory of where this
 * source code is published or http://www.gnu.org/copyleft/gpl.html 
 */

#ifndef HEADER_EFSCRU_DIRSTREAM_INL__
#define HEADER_EFSCRU_DIRSTREAM_INL__

#ifdef __cplusplus
   extern "C" {
#endif

namespace eFScru {
struct dirStream{DIR* status;std::string_view absolutepath;
    dirStream(std::string_view path):absolutepath(path){
        status=opendir(absolutepath.data());
    }
    inline void close() const {
        if(status!=NULL) closedir(status);
    }
    
    ~dirStream() {
        close();
    }
};

dirStream openDirStream(std::string_view path){
    return dirStream(path);
}
} // namespace eFScru

#ifdef __cplusplus
   }
#endif

#endif // HEADER_EFSCRU_DIRSTREAM_HPP__
