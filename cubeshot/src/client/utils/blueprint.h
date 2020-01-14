#ifndef BLUEPRINT_H
#define BLUEPRINT_H

// for keeping track of loaded objects / textures
struct Blueprint {
    unsigned int vao;
    int textureId;
    unsigned int vertexCount;
};

#endif // BLUEPRINT