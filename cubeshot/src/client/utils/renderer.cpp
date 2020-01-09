#include "renderer.h"

void Renderer::init() {
    assetsDir = std::filesystem::current_path().string() + "/assets/";
    shaderDir = std::filesystem::current_path().string() + "/src/client/shaders/";

    // load shaders
    if (gameShader.load(shaderDir + "game.vert", shaderDir + "game.frag") != 0)
        std::cout << "Error loading shaders" << std::endl;

    if (uiShader.load(shaderDir + "ui.vert", shaderDir + "ui.frag") != 0)
        std::cout << "Error loading shaders" << std::endl;

    std::vector<float> cubeVertices = {
        // front face
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,

        // back
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // right
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f,

        // left    
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,

        // bottom
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.5f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,

        // top
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    auto tileVertices = cubeVertices;
    for (int i = 0; i < tileVertices.size(); i++)
        if (i % 5 == 3 || i % 5 == 4)
            tileVertices[i] *= TILE_SIZE;

    std::vector<float> crossVertices =  {
		-0.002f,  0.04f,
		-0.002f, -0.04f,
		 0.002f, -0.04f,
		 0.002f, -0.04f,
		-0.002f,  0.04f,
		 0.002f,  0.04f,

		-0.03f,  0.002f,
		-0.03f, -0.002f,
		 0.03f, -0.002f,
		 0.03f, -0.002f,
		-0.03f,  0.002f,
		 0.03f,  0.002f
    };

    unsigned int vao = loadObject(tileVertices, 3, false, true);
    int textureId = loadTexture(vao, assetsDir + "cube0_texture.png", false);
    blueprints.insert(std::pair(TILE, Blueprint {vao, textureId, (unsigned int)(tileVertices.size() / 3)}));

    vao = loadObject(cubeVertices, 3, false, true);
    textureId = loadTexture(vao, assetsDir + "cube0_texture.png", false);
    blueprints.insert(std::pair(OBSTACLE, Blueprint {vao, textureId, (unsigned int)(cubeVertices.size() / 3)}));

    textureId = loadTexture(vao, assetsDir + "minion.jpg", false);
    blueprints.insert(std::pair(PLAYER, Blueprint {vao, textureId, (unsigned int)(cubeVertices.size() / 3)}));

    vao = loadObject(crossVertices, 2, false, false);
    blueprints.insert(std::pair(CROSS, Blueprint {vao, -1, (unsigned int)(crossVertices.size() / 2)}));
}

void Renderer::render(const Camera &camera) {
    // render crossair
    auto blueprint = blueprints.at(CROSS);
    uiShader.use();
    glBindVertexArray(blueprint.vao);
    uiShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, blueprint.vertexCount);
    glBindVertexArray(0);

    gameShader.use();
    gameShader.setFloat("tileSize", TILE_SIZE);
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    gameShader.setMat4("projection", projection);
    glm::mat4 view = camera.getViewMatrix();
    gameShader.setMat4("view", view);
}

void Renderer::render(const World &world, const std::string &localPlayerId) {
    // render tiles
    auto blueprint = blueprints.at(TILE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blueprint.textureId);

    glBindVertexArray(blueprint.vao);
    for (auto const& tile : world.tiles) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Vector3::toGlm(tile.position));
        model = glm::scale(model, Vector3::toGlm(tile.scale));
        unsigned int modelLoc = glGetUniformLocation(gameShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, blueprint.vertexCount);
    }

    glBindVertexArray(0);

    // render obstacles
    blueprint = blueprints.at(OBSTACLE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blueprint.textureId);
    
    glBindVertexArray(blueprint.vao);
    for (auto const& tile : world.tiles) {
        for (auto const& obstacle : tile.obstacles) {
            glm::vec3 globalPos{ tile.position.x + obstacle.position.x, obstacle.position.y, tile.position.z + obstacle.position.z };
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, globalPos);
            unsigned int modelLoc = glGetUniformLocation(gameShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, blueprint.vertexCount);
        }
    }

    glBindVertexArray(0);

    // render player
    blueprint = blueprints.at(PLAYER);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blueprint.textureId);
    glBindVertexArray(blueprint.vao);

    for (auto const& [id, p] : world.players)
    {
        if (id == localPlayerId)
            continue;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Vector3::toGlm(p.position));
        glm::vec3 target = Vector3::toGlm(p.front);
        glm::vec3 r = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target);
        model *= glm::inverse(glm::lookAt(glm::vec3(0), target, glm::vec3(0, 1, 0)));
        auto hitPointMatrix = model;
        model = glm::scale(model, glm::vec3(PLAYER_SCALE));

        unsigned int modelLoc = glGetUniformLocation(gameShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, blueprint.vertexCount);

        for (auto const& hitPoint : p.hitPoints) {
            auto hitDirection = Vector3::toGlm(hitPoint);
            auto hitModel = glm::translate(hitPointMatrix, hitDirection);
            hitModel = glm::scale(hitModel, glm::vec3(0.1));
            unsigned int modelLoc = glGetUniformLocation(gameShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(hitModel));
            glDrawArrays(GL_TRIANGLES, 0, blueprint.vertexCount);
        }
    }
}

unsigned int Renderer::loadObject(std::vector<float> vertices, unsigned short dimensions, bool hasColor, bool hasTexture) {
    unsigned int index = 0;
    unsigned int stride = dimensions;
    unsigned int offset = 0;

    if (hasColor)
        stride += 3;
    if (hasTexture)
        stride += 2;

    unsigned int VBO, VAO; // create unique id
    glGenBuffers(1, &VBO); // create buffer object to corresponding id
    glGenVertexArrays(1, &VAO); // create vertex array object   

    // 1. bind buffers and vertex array object
    // -------------------------------------------------------------------
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind created buffer to buffer type GL_ARRAY_BUFFER (type of vertex buffer)

    // after binding, every buffer calls on GL_ARRAY_BUFFER will be used to configure VBO

    // 2. copy vertices array in a buffer for opengl to use
    // copies vertex data into buffer memory
    // 0: type of buffer we want copy data into
    // 1: size of the data
    // 2: the data
    // 3: specifies how the graphics card should manage the given data
    // 3.1: GL_STATIC_DRAW (data does not change or very rarely)
    // 3.2: GL_DYNAMIC_DRAW (data changes a lot)
    // 3.3: GL_STREAM_DRAW (data> changes on every draw) -> 3.2 & 3.3 data is placed in memory for faster drawing
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. set vertex attribute pointers (need explanation)
    glVertexAttribPointer(index, dimensions, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    offset += dimensions;

    if (hasColor) { // rgb = 3
        index++;
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(index);
        offset += 3;
    }

    if (hasTexture) { // uv = 2
        index++;
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(index);
        offset += 2;
    }

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

unsigned int Renderer::loadTexture(unsigned int vao, const std::string& texturePath, bool alphaChannel) {
    GLenum format = alphaChannel ? GL_RGBA : GL_RGB;
    
    glBindVertexArray(vao);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // glTexImage2D parameters
        /* The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
         * The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
         * The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
         * The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
         * The next argument should always be 0 (some legacy stuff).
         * The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
         * The last argument is the actual image data.
         */
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load texture - path: " << texturePath << std::endl;
    }

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // free image resources
    stbi_image_free(data);

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    return texture;
}
