#ifndef CONSTANTS_H
#define CONSTANTS_H

const float PLAYER_SCALE = 0.8f;

struct Constants {
    static int64_t currentMillis() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
};

#endif // CONSTANTS_H