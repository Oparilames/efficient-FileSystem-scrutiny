/*
 * Copyright (C) 2023 S. T. Hoffmann.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3.0 of the License, or (at your option) any later
 * version. See the full text of the license in the base directory of where this
 * source code is published or http://www.gnu.org/copyleft/gpl.html 
 */

#ifndef HEADER_EFSCRU_POSIXLINUX_INL__
#define HEADER_EFSCRU_POSIXLINUX_INL__

#ifdef __cplusplus
   extern "C" {
#endif

namespace eFScru {
// to verify if DIR has d_type (BDS) or not (POSIX/UNIX)
enum class compliance{POSIX,BDS};
inline static constexpr const compliance direntPOSIX{compliance::POSIX};
inline static constexpr const compliance direntBDS{compliance::BDS};

#ifdef _DIRENT_HAVE_D_TYPE
#define DIRENT_POSIX direntBDS
#else
#define DIRENT_POSIX direntPOSIX
#endif
inline static constexpr const bool direntIsPOSIXConform{(DIRENT_POSIX == compliance::POSIX)};
} // namespace eFScru

#ifdef __cplusplus
   }
#endif

#endif // HEADER_EFSCRU_POSIXLINUX_INL__
