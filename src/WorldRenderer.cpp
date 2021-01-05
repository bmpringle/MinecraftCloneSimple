#include "WorldRenderer.h"
#include "World.h"

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

    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shaderProgram[0] = compileShaderProgramFromFiles("./shaders/basic_shader.vert", "./shaders/basic_shader.frag");
    shaderProgram[1] = compileShaderProgramFromFiles("./shaders/texture_shader.vert", "./shaders/texture_shader.frag");

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
    std::vector<std::shared_ptr<Block>> rawData = data->getRawBlockArray();
    
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

        float vertexAndColorData[vectorWithColors.size()];

        for(int i = 0; i < vectorWithColors.size(); ++i) {
            vertexAndColorData[i] = vectorWithColors[i];
        }

        glBindVertexArray(VAO);

        glUseProgram(shaderProgram[0]);

        unsigned int TBO = textureFetcher.getOrLoadTexture(block->getTextureName());

        if(TBO != -1) {
            glBindTexture(GL_TEXTURE_2D, TBO);
            glUseProgram(shaderProgram[1]);
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAndColorData), vertexAndColorData, GL_DYNAMIC_DRAW);

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

template<class T>
void WorldRenderer::appendVectorWithVector(std::vector<T>* vectorToAppendTo, std::vector<T> vectorToAppend) {
    for(T item : vectorToAppend) {
        vectorToAppendTo->push_back(item);
    }
}

matrix_float4x4 WorldRenderer::calculatePerspectiveMatrix(double FOV, double zNear, double zFar) {
    float n = zNear, r = 0, l = 0, t = 0, b = 0, f = zFar;
    
    float scale = tan(FOV * 0.5 * M_PI / 180) * n;
    r = 1 * scale;
    l = -r; 
    t = scale;
    b = -t;
    
    matrix_float4x4 perspectiveMatrix = matrix_identity_float4x4;

    simd_float4 column1 = simd_float4();
    column1[0] = 2*n/(r-l);
    column1[1] = 0;
    column1[2] = 0;
    column1[3] = 0;

    simd_float4 column2 = simd_float4();
    column2[0] = 0;
    column2[1] = 2*n/(t-b);
    column2[2] = 0;
    column2[3] = 0;

    simd_float4 column3 = simd_float4();
    column3[0] = (r+l)/(r-l);
    column3[1] = (t+b)/(t-b);
    column3[2] = -1*(f+n)/(f-n);
    column3[3] = -1;

    simd_float4 column4 = simd_float4();
    column4[0] = 0;
    column4[1] = 0;
    column4[2] = -2*f*n/(f-n);
    column4[3] = 0;

    perspectiveMatrix.columns[0] = column1;
    perspectiveMatrix.columns[1] = column2;
    perspectiveMatrix.columns[2] = column3;
    perspectiveMatrix.columns[3] = column4;
    return perspectiveMatrix;
}

matrix_float3x3 multiplyTwo3x3Matrices(matrix_float3x3 m1, matrix_float3x3 m2) {
    matrix_float3x3 result = matrix_float3x3();
    simd_float3 col1 = simd_float3();
    simd_float3 col2 = simd_float3();
    simd_float3 col3 = simd_float3();

    col1[0] = m1.columns[0][0] * m2.columns[0][0] + m1.columns[1][0] * m2.columns[0][1] + m1.columns[2][0] * m2.columns[0][2];
    col1[1] = m1.columns[0][1] * m2.columns[0][0] + m1.columns[1][1] * m2.columns[0][1] + m1.columns[2][1] * m2.columns[0][2];
    col1[2] = m1.columns[0][2] * m2.columns[0][0] + m1.columns[1][2] * m2.columns[0][1] + m1.columns[2][2] * m2.columns[0][2];
    
    col2[0] = m1.columns[0][0] * m2.columns[1][0] + m1.columns[1][0] * m2.columns[1][1] + m1.columns[2][0] * m2.columns[1][2];
    col2[1] = m1.columns[0][1] * m2.columns[1][0] + m1.columns[1][1] * m2.columns[1][1] + m1.columns[2][1] * m2.columns[1][2];
    col2[2] = m1.columns[0][2] * m2.columns[1][0] + m1.columns[1][2] * m2.columns[1][1] + m1.columns[2][2] * m2.columns[1][2];
    
    col3[0] = m1.columns[0][0] * m2.columns[2][0] + m1.columns[1][0] * m2.columns[2][1] + m1.columns[2][0] * m2.columns[2][2];
    col3[1] = m1.columns[0][1] * m2.columns[2][0] + m1.columns[1][1] * m2.columns[2][1] + m1.columns[2][1] * m2.columns[2][2];
    col3[2] = m1.columns[0][2] * m2.columns[2][0] + m1.columns[1][2] * m2.columns[2][1] + m1.columns[2][2] * m2.columns[2][2];
    
    result.columns[0] = col1;
    result.columns[1] = col2;
    result.columns[2] = col3;
    return result;
}

matrix_float3x3 addTwo3x3Matrices(matrix_float3x3 m1, matrix_float3x3 m2) {
    matrix_float3x3 result = matrix_float3x3();
    simd_float3 col1 = simd_float3();
    simd_float3 col2 = simd_float3();
    simd_float3 col3 = simd_float3();

    col1[0] = m1.columns[0][0] + m2.columns[0][0];
    col1[1] = m1.columns[0][1] * m2.columns[0][1];
    col1[2] = m1.columns[0][2] * m2.columns[0][2];

    col2[0] = m1.columns[1][0] + m2.columns[1][0];
    col2[1] = m1.columns[1][1] * m2.columns[1][1];
    col2[2] = m1.columns[1][2] * m2.columns[1][2];    

    col3[0] = m1.columns[2][0] + m2.columns[2][0];
    col3[1] = m1.columns[2][1] * m2.columns[2][1];
    col3[2] = m1.columns[2][2] * m2.columns[2][2]; 
    
    result.columns[0] = col1;
    result.columns[1] = col2;
    result.columns[2] = col3;
    return result;
}

matrix_float3x3 WorldRenderer::calculateXRotationMatrix(double xRotation) {
    matrix_float3x3 rotationMatrix = matrix_float3x3();

    double xRads = xRotation * M_PI / 180;

    simd_float3 column1 = simd_float3();
    column1[0] = cos(xRads);
    column1[1] = 0;
    column1[2] = -sin(xRads);

    simd_float3 column2 = simd_float3();
    column2[0] = 0;
    column2[1] = 1;
    column2[2] = 0;

    simd_float3 column3 = simd_float3();
    column3[0] = sin(xRads);
    column3[1] = 0;
    column3[2] = cos(xRads);

    rotationMatrix.columns[0] = column1;
    rotationMatrix.columns[1] = column2;
    rotationMatrix.columns[2] = column3;

    return rotationMatrix;
}

matrix_float3x3 WorldRenderer::calculateYRotationMatrix(double yRotation) {

    matrix_float3x3 rotationMatrix = matrix_float3x3();

    double yRads = yRotation * M_PI / 180;

    simd_float3 column1 = simd_float3();
    column1[0] = 1;
    column1[1] = 0;
    column1[2] = 0;

    simd_float3 column2 = simd_float3();
    column2[0] = 0;
    column2[1] = cos(yRads);
    column2[2] = sin(yRads);

    simd_float3 column3 = simd_float3();
    column3[0] = 0;
    column3[1] = -sin(yRads);
    column3[2] = cos(yRads);

    rotationMatrix.columns[0] = column1;
    rotationMatrix.columns[1] = column2;
    rotationMatrix.columns[2] = column3;

    return rotationMatrix;
}

void WorldRenderer::setUniforms(World* world, int programIndex) {

    glUseProgram(shaderProgram[programIndex]);

    int boundsVec3Location = glGetUniformLocation(shaderProgram[programIndex], "bounds");
    glUniform3f(boundsVec3Location, WORLDSIZE_CONST, WORLDSIZE_CONST, WORLDSIZE_CONST);

    AABB playerAABB = world->getPlayer()->getAABB();

    int playerPosLocation = glGetUniformLocation(shaderProgram[programIndex], "playerPos");
    //add 3*y/4 to y pos because eyes are at 75% of player height, add z length of AABB because eyes are at front, add half of x length of AABB to place at middle of player.
    glUniform3f(playerPosLocation, world->getPlayer()->getPos().x + playerAABB.xSize / 2, world->getPlayer()->getPos().y + playerAABB.ySize * 3.0 / 4.0, world->getPlayer()->getPos().z + playerAABB.zSize);

    int perspectiveMatrixLocation = glGetUniformLocation(shaderProgram[programIndex], "perspectiveMatrix");
    
    matrix_float4x4 perspectiveMatrix = calculatePerspectiveMatrix(90, 0.0001, 100);

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