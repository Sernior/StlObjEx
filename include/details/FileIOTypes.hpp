/**
 * @file FileIOTypes.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief File I/O types for the FAConverter library.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef FILE_IO_HPP
#define FILE_IO_HPP

namespace FAConverter{
    enum class FileType {
    BEGIN, // not a supported type just to get the first value of the enum
    OBJ,
    STL,
    END // not a supported type just to get the end value of the enum
    };
};

#endif // FILE_IO_HPP
