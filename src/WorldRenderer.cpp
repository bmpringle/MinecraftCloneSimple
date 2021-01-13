#include "WorldRenderer.h"
#include "World.h"
#include "RenderHelper.h"

#define WORLDSIZE_CONST 100

WorldRenderer::WorldRenderer() : textureFetcher(TextureFetcher()) {
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
    glEnable(GL_CULL_FACE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shaderProgram[0] = compileShaderProgramFromFiles("./shaders/basic_shader.vert", "./shaders/basic_shader.frag");
    shaderProgram[1] = compileShaderProgramFromFiles("./shaders/texture_shader.vert", "./shaders/texture_shader.frag");
    shaderProgram[2] = compileShaderProgramFromFiles("./shaders/2d_shader.vert", "./shaders/2d_shader.frag");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void WorldRenderer::renderFrame(World* world) {

    setUniforms(world, 0);
    setUniforms(world, 1);


    BlockArrayData* data = world->getBlockData();

    std::vector<std::shared_ptr<Block>> rawData = std::vector<std::shared_ptr<Block>>();
    
    int renderDistance = world->getChunkRenderDistance();
    std::array<int, 3> size = Chunk::getChunkSize();


    for(float x = -(float)renderDistance/2.0; x < (float)renderDistance/2.0; ++x) {
        for(float z = -(float)renderDistance/2.0; z < (float)renderDistance/2.0; ++z) {
            Pos playerPos = world->getPlayer()->getPos();
            BlockPos playerBlock = BlockPos((int)playerPos.x + size[0] * x, 0, (int)playerPos.z + size[2] * z);
            Chunk chunk = data->getChunkWithBlock(playerBlock);

            for(int i = 0; i < chunk.getBlocksInChunk().size(); ++i) {
                std::shared_ptr<Block> block = chunk.getBlocksInChunk().at(i);
                rawData.push_back(block);
            }
        }
    }

    for(int i = 0; i < rawData.size(); ++i) {
        std::shared_ptr<Block> block = rawData.at(i);
        RenderedModel model = block->getRenderedModel();
        BlockPos pos = block->getPos();

        for(int j = 0; j < model.renderedModel.size(); ++j) {
            RenderedTriangle triangle = model.renderedModel[j];
            RenderedPoint pointa = triangle.a;
            RenderedPoint pointb = triangle.b;
            RenderedPoint pointc = triangle.c;

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

        for(RenderedTriangle triangle : model.renderedModel) {
            RenderedPoint point1 = triangle.a;
            RenderedPoint point2 = triangle.b;
            RenderedPoint point3 = triangle.c;

            //point 1 red
            vectorWithColors.push_back(point1.x);
            vectorWithColors.push_back(point1.y);
            vectorWithColors.push_back(point1.z);
            vectorWithColors.push_back(1);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(point1.u);
            vectorWithColors.push_back(point1.v);

            //point 2 green
            vectorWithColors.push_back(point2.x);
            vectorWithColors.push_back(point2.y);
            vectorWithColors.push_back(point2.z);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(1);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(point2.u);
            vectorWithColors.push_back(point2.v);

            //point 3 blue
            vectorWithColors.push_back(point3.x);
            vectorWithColors.push_back(point3.y);
            vectorWithColors.push_back(point3.z);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(0);
            vectorWithColors.push_back(1);
            vectorWithColors.push_back(point3.u);
            vectorWithColors.push_back(point3.v);
        }

        glBindVertexArray(VAO);

        glUseProgram(shaderProgram[0]);

        unsigned int TBO = textureFetcher.getOrLoadTexture(block->getTextureName());

        if(TBO != -1) {
            glBindTexture(GL_TEXTURE_2D, TBO);
            glUseProgram(shaderProgram[1]);
        }

        glBufferData(GL_ARRAY_BUFFER, vectorWithColors.size() * sizeof(float), vectorWithColors.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, vectorWithColors.size() / 6);
    }


    //renders player model. keep commented until model can be rotated with camera
    /*RenderedModel model = world->getPlayer()->getRenderedModel();
    Pos pos = world->getPlayer()->getPos();

    for(int j = 0; j < model.renderedModel.size(); ++j) {
        RenderedTriangle triangle = model.renderedModel[j];
        RenderedPoint pointa = triangle.a;
        RenderedPoint pointb = triangle.b;
        RenderedPoint pointc = triangle.c;

        pointa.x += pos.x;
        pointa.y += pos.y;
        pointa.z += pos.z;

        pointb.x += pos.x;
        pointb.y += pos.y;
        pointb.z += pos.z;

        pointc.x += pos.x;
        pointc.y += pos.y;
        pointc.z += pos.z;

        model.renderedModel[j] = RenderedTriangle(pointa, pointb, pointc);
    }

    appendVectorWithVector(&trianglesToRender, model.renderedModel);*/
}

void WorldRenderer::renderOverlay(float rectangle[48], std::string texture) {
    float overlay[48] = {0};

    for(int i = 0; i < 48; ++i) {
        overlay[i] = rectangle[i] * ((i % 8 == 1) ? aspectRatio : 1);
    }
    
    glBindVertexArray(VAO);

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

    AABB playerAABB = world->getPlayer()->getAABB();

    int playerPosLocation = glGetUniformLocation(shaderProgram[programIndex], "playerPos");
    Pos camera = world->getPlayer()->getCameraPosition();

    glUniform3f(playerPosLocation, camera.x, camera.y, camera.z);

    int perspectiveMatrixLocation = glGetUniformLocation(shaderProgram[programIndex], "perspectiveMatrix");
    
    matrix_float4x4 perspectiveMatrix = calculatePerspectiveMatrix(90, aspectRatio, 0.1, 100);

    GLfloat matrixFloat [16] = {0};

    for(int i = 0; i < 16; ++i) {
        matrixFloat[i] = perspectiveMatrix.columns[i/4][i % 4];
    }

    glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE, &matrixFloat[0]);

    
    int rotationMatrixXLocation = glGetUniformLocation(shaderProgram[programIndex], "rotationMatrixX");
    int rotationMatrixYLocation = glGetUniformLocation(shaderProgram[programIndex], "rotationMatrixY");

    matrix_float3x3 rotationMatrixX = calculateXRotationMatrix(world->getPlayer()->getXRotation());
    matrix_float3x3 rotationMatrixY = calculateYRotationMatrix(world->getPlayer()->getYRotation());

    GLfloat rotationMatrixFloat [9] = {0};

    for(int i = 0; i < 9; ++i) {
        rotationMatrixFloat[i] = rotationMatrixX.columns[i/3][i % 3];
    }

    glUniformMatrix3fv(rotationMatrixXLocation, 1, GL_FALSE, &rotationMatrixFloat[0]);
    
    for(int i = 0; i < 9; ++i) {
        rotationMatrixFloat[i] = rotationMatrixY.columns[i/3][i % 3];
    }

    glUniformMatrix3fv(rotationMatrixYLocation, 1, GL_FALSE, &rotationMatrixFloat[0]);
}

void WorldRenderer::updateAspectRatio(GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    aspectRatio = (float)width/(float)height;
}

void WorldRenderer::renderBlockInWireframe(World* world, BlockPos pos) {
    std::shared_ptr<Block> block = world->getBlockData()->getBlockAtPosition(pos);
    RenderedModel model = block->getRenderedModel();

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
        RenderedPoint point1 = triangle.a;
        RenderedPoint point2 = triangle.b;
        RenderedPoint point3 = triangle.c;

        //point 1 black
        vectorWithColors.push_back(point1.x);
        vectorWithColors.push_back(point1.y);
        vectorWithColors.push_back(point1.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(point1.u);
        vectorWithColors.push_back(point1.v);

        //point 2 black
        vectorWithColors.push_back(point2.x);
        vectorWithColors.push_back(point2.y);
        vectorWithColors.push_back(point2.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(point2.u);
        vectorWithColors.push_back(point2.v);

        //point 3 black
        vectorWithColors.push_back(point3.x);
        vectorWithColors.push_back(point3.y);
        vectorWithColors.push_back(point3.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(point3.u);
        vectorWithColors.push_back(point3.v);
    }

    glBindVertexArray(VAO);

    glUseProgram(shaderProgram[0]);

    unsigned int TBO = textureFetcher.getOrLoadTexture("wireframe.png", GL_REPEAT, GL_LINEAR);

    if(TBO != -1) {
        glBindTexture(GL_TEXTURE_2D, TBO);
        glUseProgram(shaderProgram[1]);
    }

    glBufferData(GL_ARRAY_BUFFER, vectorWithColors.size() * sizeof(float), vectorWithColors.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, vectorWithColors.size() / 6);
}