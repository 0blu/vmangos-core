# Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
# Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

set(ZLIB_PUBLIC_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zlib/zconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/zlib/zlib.h
)
set(ZLIB_PRIVATE_HDRS
    src/crc32.h
    src/deflate.h
    src/gzguts.h
    src/inffast.h
    src/inffixed.h
    src/inflate.h
    src/inftrees.h
    src/trees.h
    src/zutil.h
)
set(ZLIB_SRCS
    src/adler32.c
    src/compress.c
    src/crc32.c
    src/deflate.c
    src/gzclose.c
    src/gzlib.c
    src/gzread.c
    src/gzwrite.c
    src/inflate.c
    src/infback.c
    src/inftrees.c
    src/inffast.c
    src/trees.c
    src/uncompr.c
    src/zutil.c
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include/zlib
)

add_library(zlib STATIC
  ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS}
)

SET_TARGET_PROPERTIES (zlib PROPERTIES FOLDER "3rd Party")
