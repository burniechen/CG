// glew 要放比glfw前面
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <GL/freeglut.h>

// OpenGL 數學庫
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 讀圖檔
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <cstdlib>

#include "Shader.h"
#include "Camera.h"

#define PI 3.14159265358979323846

using namespace std;

float lastX, lastY;
bool firstMouse = true;

int triangle_num;

float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
    
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

// Instantiate Camera class
//    Camera camera(glm::vec3(0, 0, 5.0f), glm::vec3(0, 0, -1.0f), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0, 0, 5.0f), glm::radians(0.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

// esc鍵，關閉視窗
void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.speedZ = 1.0f;
    else if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.speedZ = -1.0f;
    else
        camera.speedZ = 0.0f;


    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyMoveY(true);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyMoveY(false);
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyMoveX(true);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyMoveX(false);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    float deltaX, deltaY;
    
    if (firstMouse == true) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    deltaX = xPos - lastX;
    deltaY = yPos - lastY;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(deltaX, deltaY);
}

void mouse_scroll(GLFWwindow* window, double x_offset, double y_offest)
{
    camera.speedZ -= y_offest;
}

unsigned int loadImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSLot)
{
    unsigned int TexBuffer;
    glGenTextures(1, &TexBuffer);
    // 圖片槽位
    glActiveTexture(GL_TEXTURE0 + textureSLot);
    glBindTexture(GL_TEXTURE_2D, TexBuffer);
    
    int img_w, img_h, nrChannel;
    
    stbi_set_flip_vertically_on_load(true); // 上下顛倒
    
    unsigned char *data = stbi_load(filename, &img_w, &img_h, &nrChannel, 0);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img_w, img_h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << filename << endl;
        cout << "load image failed" << endl;
    }
    
    stbi_image_free(data);
    
    return TexBuffer;
}

float GetX(float r, float u, float v)
{
    float x = r * sin(PI * u) * cos(2 * PI * v);
    return x;
}

float GetY(float r, float u, float v)
{
    float y = r * sin(PI * u) * sin(2 * PI * v);
    return y;
}

float GetZ(float r, float u, float v)
{
    float z = r * cos(PI * u);
    return z;
}

void createSphere(float *sphere, int slice, int stack){
    float r = 1.0f;
    float lengthInv = 1.0f / r;
    // slice：經線切分個數
    // stack：緯線切分個數
    
    float stack_step = 1.0f/stack;
    float slice_step = 1.0f/slice;
    
    int size = 8;
    
    int offset = 0;
    for(int u = 0; u < stack; u++){
        for(int v = 0;v < slice; v++){
            // 4個點，形成兩個三角形
            // { vertices * 3，texture * 2，normal * 3 }
            float point1[8] = {
                GetX(r, u * stack_step, v * slice_step),
                GetY(r, u * stack_step, v * slice_step),
                GetZ(r, u * stack_step, v * slice_step),
                (float) v / slice,
                (float) u / stack,
                GetX(r, u * stack_step, v * slice_step) * lengthInv,
                GetY(r, u * stack_step, v * slice_step) * lengthInv,
                GetZ(r, u * stack_step, v * slice_step) * lengthInv
            };
            float point2[8] = {
                GetX(r, (u + 1) * stack_step, v * slice_step),
                GetY(r, (u + 1) * stack_step, v * slice_step),
                GetZ(r, (u + 1) * stack_step, v * slice_step),
                (float) v / slice,
                (float) (u + 1) / stack,
                GetX(r, (u + 1) * stack_step, v * slice_step) * lengthInv,
                GetY(r, (u + 1) * stack_step, v * slice_step) * lengthInv,
                GetZ(r, (u + 1) * stack_step, v * slice_step) * lengthInv
            };
            float point3[8] = {
                GetX(r, (u + 1) * stack_step, (v + 1) * slice_step),
                GetY(r, (u + 1) * stack_step, (v + 1) * slice_step),
                GetZ(r, (u + 1) * stack_step, (v + 1) * slice_step),
                (float) (v + 1) / slice,
                (float) (u + 1) / stack,
                GetX(r, (u + 1) * stack_step, (v + 1) * slice_step) * lengthInv,
                GetY(r, (u + 1) * stack_step, (v + 1) * slice_step) * lengthInv,
                GetZ(r, (u + 1) * stack_step, (v + 1) * slice_step) * lengthInv
            };
            float point4[8] = {
                GetX(r, u * stack_step, (v + 1) * slice_step),
                GetY(r, u * stack_step, (v + 1) * slice_step),
                GetZ(r, u * stack_step, (v + 1) * slice_step),
                (float) (v + 1) / slice,
                (float) u / stack,
                GetX(r, u * stack_step, (v + 1) * slice_step) * lengthInv,
                GetY(r, u * stack_step, (v + 1) * slice_step) * lengthInv,
                GetZ(r, u * stack_step, (v + 1) * slice_step) * lengthInv
            };
            
            memcpy(sphere + offset, point4, size * sizeof(float));
            offset += size;
            memcpy(sphere + offset, point1, size * sizeof(float));
            offset += size;
            memcpy(sphere + offset, point3, size * sizeof(float));
            offset += size;
            
            memcpy(sphere + offset, point3, size * sizeof(float));
            offset += size;
            memcpy(sphere + offset, point1, size * sizeof(float));
            offset += size;
            memcpy(sphere + offset, point2, size * sizeof(float));
            offset += size;
        }
    }
}

void myCube()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    
    // 位置屬性
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(6);
    
    // 紋理屬性
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(8);
    
    // normal
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(9);
    
    triangle_num = 36;
}

void mySphere()
{
    int stack = 180;
    int slice = 360;
    int total_size = 6 * 8 * stack * slice;
    float *sphere_vertices;
    sphere_vertices = (float*) malloc(total_size * sizeof(float));

    createSphere(sphere_vertices, slice, stack);
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, total_size * sizeof(float), sphere_vertices, GL_STATIC_DRAW);
    
    // 位置屬性
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(6);
    
    // 紋理屬性
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(8);
    
    // normal
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(9);
    
    triangle_num = 6 * stack * slice;
    
    free(sphere_vertices);
}

void switchShape(GLFWwindow* window, unsigned int ID)
{
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        mySphere();
        glUniform1i(glGetUniformLocation(ID, "ourTexture"), 1);
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        mySphere();
        glUniform1i(glGetUniformLocation(ID, "ourTexture"), 2);
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        myCube();
        glUniform1i(glGetUniformLocation(ID, "ourTexture"), 3);
    }
}

int main(int argc, char* argv[]) {
    
    glfwInit();
    // 版本號
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

// --------------------------------------------------------------
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll);

// --------------------------------------------------------------
    
    // glew init
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }
    
// --------------------------------------------------------------
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
//    glEnable(GL_CULL_FACE); // 砍面
//    glCullFace(GL_BACK); // 砍背面（順時針），正面（逆時針）
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); // z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清緩衝
    
// --------------------------------------------------------------
    const char* vertexSource = "/users/burnie/Desktop/CG/HW/HW01/HW01/vertexSource.txt";
    const char* fragmentSource = "/users/burnie/Desktop/CG/HW/HW01/HW01/fragmentSource.txt";
    
    Shader* myShader = new Shader(vertexSource, fragmentSource);
    
// --------------------------------------------------------------
    
    const char* earth = "/users/burnie/Desktop/CG/HW/HW01/HW01/img/earth.jpg";
    const char* moon = "/users/burnie/Desktop/CG/HW/HW01/HW01/img/moon.jpg";
    const char* container = "/users/burnie/Desktop/CG/HW/HW01/HW01/img/container.jpg";
    
    unsigned int TexBufferA;
    TexBufferA = loadImageToGPU(earth, GL_RGB, GL_RGB, 1);
    
    unsigned int TexBufferB;
    TexBufferB = loadImageToGPU(moon, GL_RGB, GL_RGB, 2);
    
    unsigned int TexBufferC;
    TexBufferC = loadImageToGPU(container, GL_RGB, GL_RGB, 3);
        
// --------------------------------------------------------------
    
    // Calculate our transformation matrix
    glm::mat4 modelMat(1.0f); // 初始化
    glm::mat4 modelMat2(1.0f);
    modelMat2 = glm::rotate(modelMat2, glm::radians(90.0f), glm::vec3(1.0, 0, 0));
    
    glm::mat4 viewMat(1.0f);
    glm::mat4 projMat(1.0f);
    
    projMat = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window); // 關閉視窗(esc)
        
        modelMat = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        
        // Clear Screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 視窗背景顏色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        viewMat = camera.GetViewMatrix();
        
        // Set Model Matrix
        
        // Set View & Projection Matrices here if you want
        
        // Set Material -> Shader Program
        myShader->use();
        
        // Set Material -> Textures
        
        // Set Material -> Uniform
        
        // Transform
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat * modelMat2));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
        
        // Texture
//        glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
//        glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 3);
        switchShape(window, myShader->ID);
        
        // Lighting
        glUniform3f(glGetUniformLocation(myShader->ID, "objColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPos"), 10.0f, 10.0f, 10.0f);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightColor"), 1.0f, 1.0f, 1.0f);
        
        // Set Model
//        glBindVertexArray(VAO);
        
        // Draw
        glDrawArrays(GL_TRIANGLES, 0, triangle_num);
        
        // Clean up, prepare for next render loop
        glfwPollEvents();
        glfwSwapBuffers(window);
        camera.UpdateCameraPos();
    }
    
    glfwTerminate();
    return 0;
}
