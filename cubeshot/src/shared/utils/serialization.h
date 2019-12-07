#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <algorithm>

class Serialization {
public:
    template <typename T>
    static std::size_t binary_serialize(std::vector<unsigned char>& buf, std::size_t index, const T& variable)
    {
        std::copy_n(reinterpret_cast<const unsigned char*>(&variable), sizeof(T), &buf[index]);
        return index + sizeof(T);
    }
    
    template <typename T>
    static std::size_t binary_deserialize(const std::vector<unsigned char>& buf, std::size_t index, T* variable)
    {
        *variable = *reinterpret_cast<const T*>(&buf[index]);
        return index + sizeof(T);
    }
};

#endif