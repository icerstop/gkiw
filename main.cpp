#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "shader.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"
#include "camera.h"
#include "Model.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

// rozdzielczoœæ okna
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;

// trasnformacje
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;


//wartoœci startowe

//kamera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 3.0f, -25.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);



// wspó³rzêdne œwiate³ punktowych
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,  15.0f,  0.0f),
    glm::vec3(0.0f,  15.0f,  -5.0f),
    glm::vec3(0.0f,  15.0f,  -20.0f),

    glm::vec3(0.0, 15.0,-10.0),
    glm::vec3(0.0, 15.0, -15.0),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // wspó³rzêdne
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.15f, //k_q
    1       // numer œwiat³a
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // wspó³rzêdne
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.15f, //k_q
    2       // numer
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // wspó³rzêdne
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.15f, //k_q
    3       // numer
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // wspó³rzêdne
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.15f, //k_q
    4       // numer
);

PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // wspó³rzêdne
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.15f, //k_q
    5       // numer
);


// timing
float deltaTime = 0.0f;    // czas miêdzy obecn¹ a ostatni¹ klatk¹
float lastFrame = 0.0f;

bool gateOpen = false;
int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    

//£adowanie tekstur
    
    string sandPath = "field.jpg"; //https://www.urbangreenup.eu/imgpub/1883284/900/0/fences.webp
    string brickPath = "fences.jpg"; //https://www.freepik.com/free-photo/background-made-from-bricks_10980125.htm
    string gatePath = "gate.jpg";   //https://www.freepik.com/free-photo/top-view-metal-surface-with-pattern_11684381.htm

    unsigned int sandMap = loadTexture(sandPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int brickMap = loadTexture(brickPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int gateMap = loadTexture(gatePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    Cube sand = Cube(sandMap, sandMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube brick = Cube(brickMap, brickMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube gate = Cube(gateMap, gateMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    

    stbi_set_flip_vertically_on_load(true);
    Shader modelShader("1.model_loading.vs", "1.model_loading.fs");

    Model tree1("pine_tree/scene.gltf");
    Model tree2("pine_tree2/scene.gltf");
    Model tree3("pine_tree3/scene.gltf");

//pêtla renderowania

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> tree1_lim(0.02, 1.0);
    std::uniform_real_distribution<float> tree1_start(0.001, 0.01);
    std::uniform_real_distribution<float> tree2_lim(0.3, 2.0);
    std::uniform_real_distribution<float> tree2_start(0.01, 0.05);
    std::uniform_real_distribution<float> angle_lim(0.0, 360.0);
    std::uniform_int_distribution<int> lev_lim(1, 2);
    
    float s1 = tree1_start(gen);
    float s1_end = tree1_lim(gen);
    int lev = lev_lim(gen);
    float s2 = tree2_start(gen);
    float s2_end = tree2_lim(gen);
    float tree_ang = angle_lim(gen);
    float gate_ang = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        modelShader.use();
        glm::mat4 Mprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 Mview = camera.GetViewMatrix();
        modelShader.setMat4("projection", Mprojection);
        modelShader.setMat4("view", Mview);


        if (lev == 1)
        {
            if (s1 < s1_end)
                s1 += 0.000005;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0f, 0.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(s1, s1, s1));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(tree_ang), glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 modelForTree = translateMatrix * rotateYMatrix * scaleMatrix;

            modelShader.setMat4("model", modelForTree);
            tree1.Draw(modelShader);
        }

        else if (lev == 2)
        {
            if (s2 < s2_end) s2 += 0.00005;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0f, 0.0f)); 
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(s2, s2, s2));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(tree_ang), glm::vec3(0.0, 1.0, 0.0));
            glm::mat4 modelForTree = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;

            modelShader.setMat4("model", modelForTree);
            tree3.Draw(modelShader);
        }


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        // point light 5
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        

        //trawa
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0, 0.0, -40.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(60.0f, 0.1f, 60.0));
        glm::mat4 modelForBase = translateMatrix * scaleMatrix;
        sand.drawCubeWithTexture(lightingShaderWithTexture, modelForBase);

        
        //zywoplot z tylu i po bokach

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(16.0, 7.0, 5.5));
        glm::mat4 modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-16.0, 7.0, 5.5));
        modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.5, 0.0, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5, 7.0, -25));
        modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-16.0, 0.0, 10.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5, 7.0, -25));
        modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);



        //zywoplot z przodu
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, 0.0, -15.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8.0, 7.0, 5.5));
        modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);

  


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 0.0, -15.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-8.0, 7.0, 5.5));
        modelForBrick = translateMatrix * scaleMatrix;
        brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);

 

        //brama
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(gate_ang), glm::vec3(0.0, 1.0, 0.0));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, 0.0, -15.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.5, 6.8, 0.5));
        modelForBrick = translateMatrix * rotateYMatrix * scaleMatrix;
        gate.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);


        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-gate_ang), glm::vec3(0.0, 1.0, 0.0));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 0.0, -15.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 6.8, 0.5));
        modelForBrick = translateMatrix * rotateYMatrix * scaleMatrix;
        gate.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick);


        if (gateOpen)
        {
            if (gate_ang < 80.0f)
                gate_ang += 1.0f;
        }

        else
        {
            if (gate_ang > 0.0f)
                gate_ang -= 1.0f;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    //sterowanie movementem i kamer¹

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if ( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

        camera.ProcessKeyboard(YAW_L, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

        camera.ProcessKeyboard(YAW_R, deltaTime);
    }

//sterowanie bramk¹

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

        if (!gateOpen) gateOpen = true;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {

        if (gateOpen) gateOpen = false;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}