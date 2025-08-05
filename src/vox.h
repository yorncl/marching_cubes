// Temporary file, because my formatter tries to change the order for some
// reason
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#define ERROR -1;
#define OK 0;

class Shader {
  public:
    std::string source;
    GLuint id;
    std::string path;
    GLuint type;

    Shader(GLuint type);
    Shader &from_file(std::string path);
    Shader &compile();

    ~Shader();
};

class ShaderProgram {
  public:
    std::vector<Shader> shaders;
    GLuint id;

    ShaderProgram();
    void attach(Shader &s);
    int link();
    void use();
};

class ShaderCreationException : public std::exception {
  public:
    char *what() { return "Shader creation failed"; }
};

// Basic class to hold vertex data and others
class Mesh {
  public:
    // vertex informations
    std::vector<float> vertices;
    std::vector<float> colors;
};

// Minimal camera class
class Camera {
  public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 left;

    Camera();
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 left);
    glm::mat4 view();
};

// Something that gets drawn
class RenderObject {
  public:
    ShaderProgram prog;
    Mesh m;

    // no transform in this house, we rawdog vectors (for now)
    glm::vec3 pos;

    // Gets rendered from a certain point of view
    virtual void render(Camera &camera) = 0;
};

// Object that holds the voxel data
class VoxelChunk {
  public:
    // dimension of a side, len * len * len voxels
    int len;
    // Center of the cunk, center point of the cube if you will
    glm::vec3 pos;
    std::vector<int> data;

    VoxelChunk(int len);
};

class ScalarField : public RenderObject {
    void render(Camera &camera);
};

class Cube : public RenderObject {
    void render(Camera &camera);
};

class Terrain : public RenderObject {
    void render(Camera &camera);
};

// Top class of the engine, holds EVERYTHING
class Context {
  public:
    GLFWwindow *window;
    Camera camera;
    VoxelChunk chunk;
    std::vector<RenderObject> targets;

    Context(Camera &camera, VoxelChunk &vox);
};

// Renderer API to setup render objects and launch the main loop
Cube setup_cube();
ScalarField setup_field();
class RenderObjectException : public std::exception {
  public:
    char *what() { return "Failed to setup render object"; }
};

void start_renderer(Context &ctx);

// Input API
void setup_input(Context &ctx);
