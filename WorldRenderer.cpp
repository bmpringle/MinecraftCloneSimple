#include "WorldRenderer.h"
#include "World.h"

#define WORLDSIZE_CONST 100

WorldRenderer::WorldRenderer() {
    renderSetup();
}

unsigned int WorldRenderer::compileShaderProgramFromFiles(std::string vertexShaderPath, std::string fragmentShaderPath) {
    unsigned int shaderProgram;

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

    shaderProgram = glCreateProgram();

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

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void WorldRenderer::renderSetup() {  
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shaderProgram = compileShaderProgramFromFiles("./shaders/vert.txt", "./shaders/frag.txt");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glUseProgram(shaderProgram);

    int boundsVec3Location = glGetUniformLocation(shaderProgram, "bounds");
    glUniform3f(boundsVec3Location, WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST);
}

void WorldRenderer::renderFrame(World* world) {

    AABB playerAABB = world->getPlayer()->getAABB();

    int playerPosLocation = glGetUniformLocation(shaderProgram, "playerPos");
    //add 3*y/4 to y pos because eyes are at 75% of player height, add z length of AABB because eyes are at front, add half of x length of AABB to place at middle of player.
    glUniform3f(playerPosLocation, world->getPlayer()->getPos().x + playerAABB.xSize / 2, world->getPlayer()->getPos().y + playerAABB.ySize * 3.0 / 4.0, world->getPlayer()->getPos().z + playerAABB.zSize);

    BlockArrayData* data = world->getBlockData();
    std::vector<std::shared_ptr<Block>> rawData = data->getRawBlockArray();

    std::vector<RenderedTriangle> trianglesToRender = std::vector<RenderedTriangle>();

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

            model.renderedModel[j] = RenderedTriangle(pointa, pointb, pointc);
        }

        appendVectorWithVector(&trianglesToRender, model.renderedModel);
    }

    std::vector<float> vectorWithColors = std::vector<float>();

    for(RenderedTriangle triangle : trianglesToRender) {
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

        //point 2 green
        vectorWithColors.push_back(point2.x);
        vectorWithColors.push_back(point2.y);
        vectorWithColors.push_back(point2.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(1);
        vectorWithColors.push_back(0);

        //point 3 blue
        vectorWithColors.push_back(point3.x);
        vectorWithColors.push_back(point3.y);
        vectorWithColors.push_back(point3.z);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(0);
        vectorWithColors.push_back(1);
    }

    float vertexAndColorData[vectorWithColors.size()];

    for(int i = 0; i < vectorWithColors.size(); ++i) {
        vertexAndColorData[i] = vectorWithColors[i];
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAndColorData), vertexAndColorData, GL_DYNAMIC_DRAW);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vectorWithColors.size() / 6);
}

template<class T>
void WorldRenderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend) {
    for(T item : vectorToAppend) {
        vectorToAppendTo->push_back(item);
    }
}