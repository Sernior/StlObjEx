/**
 * @file Model.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief Model class definition.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef MODEL_HPP
#define MODEL_HPP

#include "FileIOTypes.hpp"
#include <string>

namespace FAConverter {

template<FileType T>
requires (T > FileType::BEGIN && T < FileType::END)
class Model {
public:
    Model(){
        static_assert(T == FileType::OBJ, "Model currently unsupported");
    };

    void read(const std::string& filename);
    template<FileType U>
    void write(const std::string& filename) const;
};

} // namespace FAConverter

#include "ModelOBJ.hpp"

#endif // MODEL_HPP