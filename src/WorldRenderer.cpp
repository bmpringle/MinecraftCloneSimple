#include "WorldRenderer.h"
#include "World.h"
#include <algorithm>
#include <thread>
#include "Blocks.h"
#include <math.h>

#define WORLDSIZE_CONST 100

WorldRenderer::WorldRenderer() : textureFetcher(TextureFetcher()), textureArrayCreator(TextureArrayCreator()) {
    Blocks::initTextureArrayCreator(&textureArrayCreator);
    textureArrayCreator.generateTextureArray();

    renderSetup();
}

unsigned int WorldRenderer::compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath) {
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

void WorldRenderer::renderSetup() {  
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);  
    //glEnable(GL_CULL_FACE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    shaderProgram[0] = compileShaderProgramFromFiles("./shaders/basic_shader.vert", "./shaders/basic_shader.frag");
    shaderProgram[1] = compileShaderProgramFromFiles("./shaders/texture_shader.vert", "./shaders/texture_shader.frag");
    shaderProgram[2] = compileShaderProgramFromFiles("./shaders/2d_shader.vert", "./shaders/2d_shader.frag");
    shaderProgram[3] = compileShaderProgramFromFiles("./shaders/world_shader.vert", "./shaders/world_shader.frag");

    glGenVertexArrays(3, &VAO[0]);  
    glGenBuffers(3, &VBO[0]); 

    //world VAO
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

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

    //highlight VAO
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void WorldRenderer::updateWorldVBO(World* world) {
    
    BlockArrayData* data = world->getBlockData();

    std::map<BlockPos, LoadedChunkInfo> lChunksLocations = data->getLoadedChunkLocations();

    std::vector<BlockPos> posToRemove = std::vector<BlockPos>();
    for(std::pair<BlockPos, RenderChunkBuffer> buff : renderChunkBuffers) {
        BlockPos loc = buff.first;

        try {
            lChunksLocations.at(loc);
        }catch(std::out_of_range e) {
            posToRemove.push_back(buff.first);
        }
    }

    for(BlockPos remove : posToRemove) {
        renderChunkBuffers.erase(remove);
    }
    
    for(std::pair<BlockPos, LoadedChunkInfo> lchunk : lChunksLocations) {
        if(lchunk.second.update) {
            Chunk* c = data->getChunkWithBlock(lchunk.first);

            BlockPos pos = lchunk.first;
            try {
                RenderChunkBuffer buffer = renderChunkBuffers.at(pos);
                std::vector<BlockData> blockData = c->getBlocksInChunk();
                std::vector<float> vectorWithColors = std::vector<float>();
                updateChunkData(&vectorWithColors, &blockData, &textureArrayCreator);
                renderChunkBuffers.at(pos).setRenderData(vectorWithColors); 
            }catch(std::out_of_range e) {
                std::vector<float> vectorWithColors = std::vector<float>();
                std::vector<BlockData> blockData = c->getBlocksInChunk();
                updateChunkData(&vectorWithColors, &blockData, &textureArrayCreator);
                renderChunkBuffers[pos] = RenderChunkBuffer(vectorWithColors, pos);
            }      
        }
    }
}

void WorldRenderer::renderFrame(World* world) {

    setUniforms(world, 0);
    setUniforms(world, 1);
    setUniforms(world, 3);

    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayCreator.getGeneratedTextureArray());
    glUseProgram(shaderProgram[3]);

    for(std::pair<const BlockPos, RenderChunkBuffer> cBuffer : renderChunkBuffers) {
        cBuffer.second.renderChunk();
    }
}

void WorldRenderer::renderOverlay(float rectangle[48], std::string texture) {
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
}

void WorldRenderer::setUniforms(World* world, int programIndex) {

    glUseProgram(shaderProgram[programIndex]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[programIndex], "bounds");
    glUniform3f(boundsVec3Location, WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST);

    int playerPosLocation = glGetUniformLocation(shaderProgram[programIndex], "playerPos");
    Pos camera = world->getPlayer()->getCameraPosition();

    glUniform3f(playerPosLocation, camera.x, camera.y, camera.z);

    int perspectiveMatrixLocation = glGetUniformLocation(shaderProgram[programIndex], "perspectiveMatrix");
    
    glm::mat4x4 perspectiveMatrix = WorldRenderer::calculatePerspectiveMatrix(90, aspectRatio, 0.1, 100);

    GLfloat matrixFloat [16] = {0};

    for(int i = 0; i < 16; ++i) {
        matrixFloat[i] = perspectiveMatrix[i/4][i % 4];
    }

    glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE, &matrixFloat[0]);

    
    int rotationMatrixXLocation = glGetUniformLocation(shaderProgram[programIndex], "rotationMatrixX");
    int rotationMatrixYLocation = glGetUniformLocation(shaderProgram[programIndex], "rotationMatrixY");

    glm::mat3x3 rotationMatrixX = WorldRenderer::calculateXRotationMatrix(world->getPlayer()->getXRotation());
    glm::mat3x3 rotationMatrixY = WorldRenderer::calculateYRotationMatrix(world->getPlayer()->getYRotation());

    GLfloat rotationMatrixFloat [9] = {0};

    for(int i = 0; i < 9; ++i) {
        rotationMatrixFloat[i] = rotationMatrixX[i/3][i % 3];
    }

    glUniformMatrix3fv(rotationMatrixXLocation, 1, GL_FALSE, &rotationMatrixFloat[0]);
    
    for(int i = 0; i < 9; ++i) {
        rotationMatrixFloat[i] = rotationMatrixY[i/3][i % 3];
    }

    glUniformMatrix3fv(rotationMatrixYLocation, 1, GL_FALSE, &rotationMatrixFloat[0]);
}

void WorldRenderer::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    aspectRatio = (float)width/(float)height;
}

void WorldRenderer::renderBlockInWireframe(World* world, BlockPos pos) {
    BlockData block = world->getBlockData()->getBlockAtPosition(pos);
    RenderedModel model = block.getBlockType()->getRenderedModel().toRenderedModel();

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

    if(TBO != -1) {
        glBindTexture(GL_TEXTURE_2D, TBO);
        glUseProgram(shaderProgram[1]);
    }

    glBufferData(GL_ARRAY_BUFFER, vectorWithColors.size() * sizeof(float), vectorWithColors.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vectorWithColors.size() / 8);
}

template<class T>
void WorldRenderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend) {
    for(T item : vectorToAppend) {
        vectorToAppendTo->push_back(item);
    }
}

glm::mat4x4 WorldRenderer::calculatePerspectiveMatrix(double FOV, double aspectRatio, double zNear, double zFar) {
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

    perspectiveMatrix[2][0] = (r+l)/(r-l);
    perspectiveMatrix[2][1] = (t+b)/(t-b);
    perspectiveMatrix[2][2] = -1*(f+n)/(f-n);
    perspectiveMatrix[2][3] = -1;

    perspectiveMatrix[3][0] = 0;
    perspectiveMatrix[3][1] = 0;
    perspectiveMatrix[3][2] = -2*f*n/(f-n);
    perspectiveMatrix[3][3] = 0;

    return perspectiveMatrix;
}

glm::mat3x3 WorldRenderer::calculateXRotationMatrix(double xRotation) {
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

glm::mat3x3 WorldRenderer::calculateYRotationMatrix(double yRotation) {

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

void WorldRenderer::updateChunkData(std::vector<float>* buffer, std::vector<BlockData>* blocksInChunk, TextureArrayCreator* texCreator) {    
    for(int i = 0; i < blocksInChunk->size(); ++i) {
        BlockData blockData = blocksInChunk->at(i);

        BlockRenderedModel model = blockData.getBlockType()->getRenderedModel();
        BlockPos pos = blockData.getPos();
    
        for(BlockFace face : model.renderedBlockModel) {
            int texID = texCreator->getTextureLayer(blockData.getBlockType()->getTextureName(face.side));

            for(RenderedTriangle triangle : face.triangles) {
                //point 1 red
                buffer->push_back(triangle.a.x);
                buffer->push_back(triangle.a.y);
                buffer->push_back(triangle.a.z);
                buffer->push_back(1);
                buffer->push_back(0);
                buffer->push_back(0);
                buffer->push_back(triangle.a.u);
                buffer->push_back(triangle.a.v);
                buffer->push_back(texID);
                buffer->push_back(pos.x);
                buffer->push_back(pos.y);
                buffer->push_back(pos.z);

                //point 2 green
                buffer->push_back(triangle.b.x);
                buffer->push_back(triangle.b.y);
                buffer->push_back(triangle.b.z);
                buffer->push_back(0);
                buffer->push_back(1);
                buffer->push_back(0);
                buffer->push_back(triangle.b.u);
                buffer->push_back(triangle.b.v);
                buffer->push_back(texID);
                buffer->push_back(pos.x);
                buffer->push_back(pos.y);
                buffer->push_back(pos.z);

                //point 3 blue
                buffer->push_back(triangle.c.x);
                buffer->push_back(triangle.c.y);
                buffer->push_back(triangle.c.z);
                buffer->push_back(0);
                buffer->push_back(0);
                buffer->push_back(1);
                buffer->push_back(triangle.c.u);
                buffer->push_back(triangle.c.v);
                buffer->push_back(texID);
                buffer->push_back(pos.x);
                buffer->push_back(pos.y);
                buffer->push_back(pos.z);
            }
        }
    }
}