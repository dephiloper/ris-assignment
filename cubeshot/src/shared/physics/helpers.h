#ifndef HELPERS_H
#define HELPERS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <chrono>

static constexpr glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

static int64_t currentMillis() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static glm::vec3 truncate(const glm::vec3& vec, float maxValue) {
    return glm::length(vec) > maxValue ? glm::normalize(vec) * maxValue : vec;
}

static float frand() {
     return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static float loopValue(float value, float min, float max) {
    float range = max - min;
    float mod = fmod(value - min, range);
    if (mod < 0) mod += range;

    return min + mod;
}

static int loopValue(int value, int min, int max) {
    int range = max - min + 1;
    int mod = (value - min) % range;
    if (mod < 0) mod += range;

    return min + mod;
}

#endif // HELPERS_H