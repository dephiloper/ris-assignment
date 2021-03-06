#ifndef VECTOR3_H
#define VECTOR3_H

#include <nop/structure.h>
#include <glm/glm.hpp>

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    operator glm::vec3() const {
        return glm::vec3(x, y, z);
    }

    friend std::ostream& operator<< (std::ostream& os, const Vector3& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    friend Vector3 operator+ (Vector3 lvec, const Vector3& rvec) {
        return Vector3{ lvec.x + rvec.x, lvec.y + rvec.y, lvec.z + rvec.z };
    }

    friend Vector3 operator- (Vector3 lvec, const Vector3& rvec) {
        return Vector3{ lvec.x - rvec.x, lvec.y - rvec.y, lvec.z - rvec.z };
    }

    Vector3& operator+= (const Vector3& vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    static Vector3 from(glm::vec3 vec) {
        return Vector3{vec.x, vec.y, vec.z};
    }


    NOP_STRUCTURE(Vector3, x, y, z);
};

#endif //VECTOR3_H