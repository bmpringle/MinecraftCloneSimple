#include "Renderer.h"
#include "World.h"
#include <algorithm>
#include <thread>
#include "Blocks.h"
#include <math.h>
#include "EntityVertex.h"

#define WORLDSIZE_CONST 100

Renderer::Renderer() : textureFetcher(TextureFetcher()), textureArrayCreator(TextureArrayCreator()), fontLoader("src/assets/courier.ttf") {
    Blocks::initTextureArrayCreator(&textureArrayCreator);
    textureArrayCreator.generateTextureArray();

    renderSetup();
}

unsigned int Renderer::compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath) {
    unsigned int _shaderProgram;

    std::ifstream filestream = std::ifstream(vertexShaderPath);
    
    std::string vertexShaderSource = "";

    std::string line = "";

    while(std::getline(filestream, line)) {
        vertexShaderSource += line + "\n";
    }

    filestream = std::ifstream(fragmentShaderPath);
    
    std::string fragmentShaderSource = "";

    line = "";

    while(std::getline(filestream, line)) {
        fragmentShaderSource += line + "\n";
    }

    unsigned int vertexShader, fragmentShader;

    _shaderProgram = glCreateProgram();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vss = vertexShaderSource.c_str();
    const char* fss = fragmentShaderSource.c_str();

    glShaderSource(vertexShader, 1, &vss, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fss, NULL);
    glCompileShader(fragmentShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return _shaderProgram;
}

void Renderer::renderSetup() {  
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);  
    //glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    //glEnable(GL_CULL_FACE); 
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    shaderProgram[0] = compileShaderProgramFromFiles("./shaders/basic_shader.vert", "./shaders/basic_shader.frag");
    shaderProgram[1] = compileShaderProgramFromFiles("./shaders/texture_shader.vert", "./shaders/texture_shader.frag");
    shaderProgram[2] = compileShaderProgramFromFiles("./shaders/2d_shader.vert", "./shaders/2d_shader.frag");
    shaderProgram[3] = compileShaderProgramFromFiles("./shaders/world_shader.vert", "./shaders/world_shader.frag");
    shaderProgram[4] = compileShaderProgramFromFiles("./shaders/2d_shader_untextured.vert", "./shaders/2d_shader_untextured.frag");

    shaderProgram[5] = compileShaderProgramFromFiles("./shaders/entity_shader.vert", "./shaders/entity_shader.frag");

    glGenVertexArrays(4, &VAO[0]);  
    glGenBuffers(4, &VBO[0]); 

    //world VAO
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //overlay VAO
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //highlight + entity VAO
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //non-textured overlay vbo
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    setupEntityRenderer();
}

void Renderer::updateWorldVBO(World* world) {
    
    BlockArrayData* data = world->getBlockData();

    std::map<BlockPos, LoadedChunkInfo> lChunksLocations = data->getLoadedChunkLocations();

    std::vector<BlockPos> posToRemove = std::vector<BlockPos>();
    for(std::pair<const BlockPos, RenderChunkBuffer>& buff : renderChunkBuffers) {
        BlockPos loc = buff.first;

        if(lChunksLocations.count(loc) > 0) {
            lChunksLocations.at(loc);
        }else {
            posToRemove.push_back(buff.first);
        }
    }

    for(BlockPos remove : posToRemove) {
        renderChunkBuffers.erase(remove);
    }
    
    for(std::pair<const BlockPos, LoadedChunkInfo>& lchunk : lChunksLocations) {
        if(lchunk.second.update > 0) {
            Chunk* c = data->getChunkWithBlock(lchunk.first);

            BlockPos pos = lchunk.first;
            if(renderChunkBuffers.count(pos) > 0) {
                renderChunkBuffers.at(pos);
                std::vector<BlockData> blockData = c->getBlocksInChunk();
                std::vector<float> vectorWithColors = std::vector<float>();
                updateChunkData(&vectorWithColors, &blockData, &textureArrayCreator);
                renderChunkBuffers.at(pos).setRenderData(vectorWithColors); 
            }else {
                std::vector<float> vectorWithColors = std::vector<float>();
                std::vector<BlockData> blockData = c->getBlocksInChunk();
                updateChunkData(&vectorWithColors, &blockData, &textureArrayCreator);
                renderChunkBuffers[pos] = RenderChunkBuffer(vectorWithColors, pos);
            }      
        }
    }
}

void Renderer::renderFrame(World* world) {

    setUniforms(world, 3);

    glUseProgram(shaderProgram[3]);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayCreator.getGeneratedTextureArray());

    for(std::pair<const BlockPos, RenderChunkBuffer>& cBuffer : renderChunkBuffers) {
        cBuffer.second.renderChunk();
    }
    std::map<BlockPos, LoadedChunkInfo> loadedChunkMap = world->getBlockData()->getLoadedChunkLocations();
    for(std::pair<BlockPos, LoadedChunkInfo> lChunk : loadedChunkMap) {
        std::map<int, std::shared_ptr<Entity>>& entityList = world->getBlockData()->getChunkWithBlock(lChunk.first)->getEntitiesInChunk();
        for(std::pair<int, std::shared_ptr<Entity>> entity : entityList) {
            renderEntity(entity.second, world);
        }
    }
}

void Renderer::renderOverlay(float rectangle[48], std::string texture) {
    glEnable(GL_BLEND);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    float overlay[48] = {0};

    for(int i = 0; i < 48; ++i) {
        overlay[i] = rectangle[i] * ((i % 8 == 1) ? aspectRatio : 1);
    }
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glUseProgram(shaderProgram[2]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[2], "bounds");

    glUniform3f(boundsVec3Location, 1000, 1000, 1000);


    unsigned int TBO = textureFetcher.getOrLoadTexture(texture);

    glBindTexture(GL_TEXTURE_2D, TBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(overlay), &overlay[0], GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glDisable(GL_BLEND);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void Renderer::renderOverlay(float rectangle[48], unsigned int TBO) {
    glEnable(GL_BLEND);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    float overlay[48] = {0};

    for(int i = 0; i < 48; ++i) {
        overlay[i] = rectangle[i] * ((i % 8 == 1) ? aspectRatio : 1);
    }
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glUseProgram(shaderProgram[2]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[2], "bounds");

    glUniform3f(boundsVec3Location, 1000, 1000, 1000);

    glBindTexture(GL_TEXTURE_2D, TBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(overlay), &overlay[0], GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glDisable(GL_BLEND);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

//Frustrum* ptr will be filled with data if you don't pass nullptr
void Renderer::setUniforms(World* world, int programIndex, Frustrum* frustrum) {

    glUseProgram(shaderProgram[programIndex]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[programIndex], "bounds");
    glUniform3f(boundsVec3Location, WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST);

    Pos camera = world->getPlayer()->getCameraPosition();
    
    glm::mat4x4 perspectiveMatrix = Renderer::calculatePerspectiveMatrix(90, aspectRatio, 0.01, 100);

    GLfloat matrixFloat [16] = {0};

    glm::mat3x3 rotationMatrixX = Renderer::calculateXRotationMatrix(world->getPlayer()->getXRotation());
    glm::mat3x3 rotationMatrixY = Renderer::calculateYRotationMatrix(world->getPlayer()->getYRotation());

    glm::mat3x3 rotationMatrix = rotationMatrixY * rotationMatrixX;

    GLfloat rotationMatrixFloat [16] = {0};

    for(int i = 0; i < 3; ++i) {
        rotationMatrixFloat[4 * i] = rotationMatrix[i][0];
        rotationMatrixFloat[4 * i + 1] = rotationMatrix[i][1];
        rotationMatrixFloat[4 * i + 2] = rotationMatrix[i][2];
        rotationMatrixFloat[4 * i + 3] = 0;
    }
    rotationMatrixFloat[4 * 3] = 0;
    rotationMatrixFloat[4 * 3 + 1] = 0;
    rotationMatrixFloat[4 * 3 + 2] = 0;
    rotationMatrixFloat[4 * 3 + 3] = 1;

    glm::mat4x4 rotationMatrix4x4 = glm::mat4x4(rotationMatrix);

    glm::mat4x4 viewMatrix = glm::mat4x4(1.0f);
    viewMatrix[3] = glm::vec4(glm::vec3(-camera.x, -camera.y, -camera.z), 1);
    viewMatrix = rotationMatrix4x4 * viewMatrix;

    glm::mat4x4 combinedMatrix = perspectiveMatrix * viewMatrix;

    for(int i = 0; i < 16; ++i) {
        matrixFloat[i] = combinedMatrix[i/4][i % 4];
    }

    int combinedMatrixLocation = glGetUniformLocation(shaderProgram[programIndex], "combinedMatrix");

    glUniformMatrix4fv(combinedMatrixLocation, 1, GL_FALSE, &matrixFloat[0]);

    /*if(frustrum != nullptr) {
        // Left clipping plane
        p_planes[0].a = comboMatrix._41 + comboMatrix._11;
        p_planes[0].b = comboMatrix._42 + comboMatrix._12;
        p_planes[0].c = comboMatrix._43 + comboMatrix._13;
        p_planes[0].d = comboMatrix._44 + comboMatrix._14;
        // Right clipping plane
        p_planes[1].a = comboMatrix._41 - comboMatrix._11;
        p_planes[1].b = comboMatrix._42 - comboMatrix._12;
        p_planes[1].c = comboMatrix._43 - comboMatrix._13;
        p_planes[1].d = comboMatrix._44 - comboMatrix._14;
        // Top clipping plane
        p_planes[2].a = comboMatrix._41 - comboMatrix._21;
        p_planes[2].b = comboMatrix._42 - comboMatrix._22;
        p_planes[2].c = comboMatrix._43 - comboMatrix._23;
        p_planes[2].d = comboMatrix._44 - comboMatrix._24;
        // Bottom clipping plane
        p_planes[3].a = comboMatrix._41 + comboMatrix._21;
        p_planes[3].b = comboMatrix._42 + comboMatrix._22;
        p_planes[3].c = comboMatrix._43 + comboMatrix._23;
        p_planes[3].d = comboMatrix._44 + comboMatrix._24;
        // Near clipping plane
        p_planes[4].a = comboMatrix._41 + comboMatrix._31;
        p_planes[4].b = comboMatrix._42 + comboMatrix._32;
        p_planes[4].c = comboMatrix._43 + comboMatrix._33;
        p_planes[4].d = comboMatrix._44 + comboMatrix._34;
        // Far clipping plane
        p_planes[5].a = comboMatrix._41 - comboMatrix._31;
        p_planes[5].b = comboMatrix._42 - comboMatrix._32;
        p_planes[5].c = comboMatrix._43 - comboMatrix._33;
        p_planes[5].d = comboMatrix._44 - comboMatrix._34;
    }*/
}

void Renderer::setUniforms(World* world, int programIndex) {
    setUniforms(world, programIndex, nullptr);
}

void Renderer::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    aspectRatio = (float)width/(float)height;

    this->width = width;
    this->height = height;
}

void Renderer::renderBlockInWireframe(World* world, BlockPos pos) {
    BlockData block = world->getBlockData()->getBlockAtPosition(pos);
    RenderedModel model = block.getRenderedModel().toRenderedModel();

    for(int j = 0; j < model.renderedModel.size(); ++j) {
        RenderedTriangle triangle = model.renderedModel[j];
        RenderedPoint pointa = triangle.a;
        RenderedPoint pointb = triangle.b;
        RenderedPoint pointc = triangle.c;
        
        pointa.x -= 0.5;
        pointa.y -= 0.5;
        pointa.z -= 0.5;

        pointb.x -= 0.5;
        pointb.y -= 0.5;
        pointb.z -= 0.5;

        pointc.x -= 0.5;
        pointc.y -= 0.5;
        pointc.z -= 0.5;

        float scale = 1.001;

        pointa.x *= scale;
        pointa.y *= scale;
        pointa.z *= scale;

        pointb.x *= scale;
        pointb.y *= scale;
        pointb.z *= scale;

        pointc.x *= scale;
        pointc.y *= scale;
        pointc.z *= scale;

        pointa.x += 0.5;
        pointa.y += 0.5;
        pointa.z += 0.5;

        pointb.x += 0.5;
        pointb.y += 0.5; 
        pointb.z += 0.5;

        pointc.x += 0.5;
        pointc.y += 0.5;
        pointc.z += 0.5;
        
        pointa.x += pos.x;
        pointa.y += pos.y;
        pointa.z += pos.z;

        pointb.x += pos.x;
        pointb.y += pos.y;
        pointb.z += pos.z;

        pointc.x += pos.x;
        pointc.y += pos.y;
        pointc.z += pos.z;

        model.renderedModel[j] = RenderedTriangle(pointa, pointb, pointc, -1);
    }

    std::vector<float> vectorWithColors = std::vector<float>();

    setUniforms(world, 0);
    setUniforms(world, 1);

    for(RenderedTriangle triangle : model.renderedModel) {
        //point 1 black
        vectorWithColors.push_back(triangle.a.x);
        vectorWithColors.push_back(triangle.a.y);
        vectorWithColors.push_back(triangle.a.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(triangle.a.u);
        vectorWithColors.push_back(triangle.a.v);

        //point 2 black
        vectorWithColors.push_back(triangle.b.x);
        vectorWithColors.push_back(triangle.b.y);
        vectorWithColors.push_back(triangle.b.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(triangle.b.u);
        vectorWithColors.push_back(triangle.b.v);

        //point 3 black
        vectorWithColors.push_back(triangle.c.x);
        vectorWithColors.push_back(triangle.c.y);
        vectorWithColors.push_back(triangle.c.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(triangle.c.u);
        vectorWithColors.push_back(triangle.c.v);
    }

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

    glUseProgram(shaderProgram[0]);

    unsigned int TBO = textureFetcher.getOrLoadTexture("wireframe.png", GL_REPEAT, GL_LINEAR);

    if((int)TBO != -1) {
        glBindTexture(GL_TEXTURE_2D, TBO);
        glUseProgram(shaderProgram[1]);
    }

    glBufferData(GL_ARRAY_BUFFER, vectorWithColors.size() * sizeof(float), vectorWithColors.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vectorWithColors.size() / 8);
}

template<class T>
void Renderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend) {
    for(T item : vectorToAppend) {
        vectorToAppendTo->push_back(item);
    }
}

glm::mat4x4 Renderer::calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
    float n = zNear, r = 0, l = 0, t = 0, b = 0, f = zFar;
    
    float scale = tan(FOV * 0.5 * M_PI / 180) * n;
    r = aspectRatio * scale;
    l = -r; 
    t = scale;
    b = -t;
    
    glm::mat4x4 perspectiveMatrix = glm::mat4x4(1.0f);

    perspectiveMatrix[0][0] = 2*n/(r-l);
    perspectiveMatrix[0][1] = 0;
    perspectiveMatrix[0][2] = 0;
    perspectiveMatrix[0][3] = 0;

    perspectiveMatrix[1][0] = 0;
    perspectiveMatrix[1][1] = 2*n/(t-b);
    perspectiveMatrix[1][2] = 0;
    perspectiveMatrix[1][3] = 0;

    //third column negated beacuse our z in logic is the opposite of opengl, i.e. point away vs pointing in.
    perspectiveMatrix[2][0] = -(r+l)/(r-l);
    perspectiveMatrix[2][1] = -(t+b)/(t-b);
    perspectiveMatrix[2][2] = (f+n)/(f-n);
    perspectiveMatrix[2][3] = 1;

    perspectiveMatrix[3][0] = 0;
    perspectiveMatrix[3][1] = 0;
    perspectiveMatrix[3][2] = -2*f*n/(f-n);
    perspectiveMatrix[3][3] = 0;

    return perspectiveMatrix;
}

glm::mat3x3 Renderer::calculateXRotationMatrix(double xRotation) {
    glm::mat3x3 rotationMatrix = glm::mat3x3();

    double xRads = xRotation * M_PI / 180;

    rotationMatrix[0][0] = cos(xRads);
    rotationMatrix[0][1] = 0;
    rotationMatrix[0][2] = -sin(xRads);

    rotationMatrix[1][0] = 0;
    rotationMatrix[1][1] = 1;
    rotationMatrix[1][2] = 0;

    rotationMatrix[2][0] = sin(xRads);
    rotationMatrix[2][1] = 0;
    rotationMatrix[2][2] = cos(xRads);

    return rotationMatrix;
}

glm::mat3x3 Renderer::calculateYRotationMatrix(double yRotation) {

    glm::mat3x3 rotationMatrix = glm::mat3x3(1.0f);

    double yRads = yRotation * M_PI / 180;

    rotationMatrix[0][0] = 1;
    rotationMatrix[0][1] = 0;
    rotationMatrix[0][2] = 0;

    rotationMatrix[1][0] = 0;
    rotationMatrix[1][1] = cos(yRads);
    rotationMatrix[1][2] = sin(yRads);

    rotationMatrix[2][0] = 0;
    rotationMatrix[2][1] = -sin(yRads);
    rotationMatrix[2][2] = cos(yRads);

    return rotationMatrix;
}

void Renderer::updateChunkData(std::vector<float>* buffer, std::vector<BlockData>* blocksInChunk, TextureArrayCreator* texCreator) {    
    for(int i = 0; i < blocksInChunk->size(); ++i) {
        BlockData blockData = blocksInChunk->at(i);

        BlockRenderedModel model = blockData.getRenderedModel();
        BlockPos pos = blockData.getPos();
    
        for(BlockFace face : model.renderedBlockModel) {
            int texID = texCreator->getTextureLayer(blockData.getTextureName(face.side));

            for(RenderedTriangle triangle : face.triangles) {
                //point 1 red
                buffer->push_back(triangle.a.x + pos.x);
                buffer->push_back(triangle.a.y + pos.y);
                buffer->push_back(triangle.a.z + pos.z);
                buffer->push_back(1);
                buffer->push_back(0);
                buffer->push_back(0);
                buffer->push_back(triangle.a.u);
                buffer->push_back(triangle.a.v);
                buffer->push_back(texID);

                //point 2 green
                buffer->push_back(triangle.b.x + pos.x);
                buffer->push_back(triangle.b.y + pos.y);
                buffer->push_back(triangle.b.z + pos.z);
                buffer->push_back(0);
                buffer->push_back(1);
                buffer->push_back(0);
                buffer->push_back(triangle.b.u);
                buffer->push_back(triangle.b.v);
                buffer->push_back(texID);

                //point 3 blue
                buffer->push_back(triangle.c.x + pos.x);
                buffer->push_back(triangle.c.y + pos.y);
                buffer->push_back(triangle.c.z + pos.z);
                buffer->push_back(0);
                buffer->push_back(0);
                buffer->push_back(1);
                buffer->push_back(triangle.c.u);
                buffer->push_back(triangle.c.v);
                buffer->push_back(texID);
            }
        }
    }
}

int Renderer::getWidth() {
    return width;
}

int Renderer::getHeight() {
    return height;
}

std::array<int, 2> Renderer::overlayDimensions() {
    return {1000, 1000};
}

unsigned int Renderer::textTextureBuffer(std::string text) {
    unsigned char* bitmap = fontLoader.getTextBitmap(text);

    int b_h = fontLoader.getBH();
    int b_w = fontLoader.getBW();

    unsigned char* bitmap4Channel = (unsigned char*)calloc(b_h * b_w * 4, sizeof(unsigned char));

    for(int i = 0; i < b_h * b_w; ++i) {
        unsigned char c = bitmap[i];
        bitmap4Channel[4 * i] = 1;
        bitmap4Channel[4 * i + 1] = 1;
        bitmap4Channel[4 * i + 2] = 1;
        bitmap4Channel[4 * i + 3] = c;
    }
     
    unsigned int TBO;

    glGenTextures(1, &TBO);

    glBindTexture(GL_TEXTURE_2D, TBO); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, b_w, b_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap4Channel);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(0);

    free(bitmap4Channel);
    free(bitmap);

    return TBO;
}

int Renderer::getBitmapHeight() {
    return fontLoader.getBH();
}

int Renderer::getBitmapWidth() {
    return fontLoader.getBW();
}


void Renderer::renderRectangle(float rectangle[36]) {
    glEnable(GL_BLEND);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    float overlay[36] = {0};

    for(int i = 0; i < 36; ++i) {
        overlay[i] = rectangle[i];// * ((i % 6 == 1) ? aspectRatio : 1);
    }
    
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);

    glUseProgram(shaderProgram[4]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[4], "bounds");

    glUniform3f(boundsVec3Location, 1000, 1000, 1000);

    glBufferData(GL_ARRAY_BUFFER, sizeof(overlay), &overlay[0], GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    glDisable(GL_BLEND);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void Renderer::renderEntity(std::shared_ptr<Entity> entity, World* world) {
    std::vector<EntityVertex> model = entity->getRenderedModel();

    setUniforms(world, 5);

    glBindVertexArray(entityVAO);

    glBindBuffer(GL_ARRAY_BUFFER, entityVBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entityEBO);

    glUseProgram(shaderProgram[5]);

    Pos pos = entity->getPos();
    //setup model->world matrix
    glm::mat4x4 modelToWorld = glm::translate(glm::mat4x4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
    //submit model->world matrix
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram[5], "modelToWorldMatrix"), 1, GL_FALSE, &modelToWorld[0][0]);

    unsigned int TBO = textureFetcher.getOrLoadTexture("testblock.png", GL_REPEAT, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, TBO);

    glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(EntityVertex), model.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, entity->getElementBuffer().size() * sizeof(int), entity->getElementBuffer().data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, entity->getElementBuffer().size(), GL_UNSIGNED_INT, 0);
}

void Renderer::setupEntityRenderer() {
    glGenVertexArrays(1, &entityVAO);
    glGenBuffers(1, &entityVBO);

    glBindVertexArray(entityVAO);

    glBindBuffer(GL_ARRAY_BUFFER, entityVBO);  

    glVertexAttribPointer(
        0, 4, GL_FLOAT, GL_FALSE, sizeof(EntityVertex), (void*)0
    ); //glm::vec4 pos

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, sizeof(EntityVertex),
        reinterpret_cast<void *>(offsetof(EntityVertex, uv))
    ); //glm::vec2 uv

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &entityEBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entityEBO);
}