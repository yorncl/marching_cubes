#include "vox.h"
#include <GLFW/glfw3.h>
#include <complex>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// The size of a chunk's side, it's a cube, so the total number of voxel would
// be equal to CHUNK_LEN * CHUNK_LEN * CHUNK_LEN
#define CHUNK_LEN 15

void fatal() {
    std::cerr << "Fatal error" << std::endl;
    exit(1);
}

Context prepare_scene() {
    Camera cam = Camera(glm::vec3(0.0, 0.0, -10.0), glm::vec3(0.0, 0.0, 1.0),
                        glm::vec3(0.0, 1.0, 0.0));
    VoxelChunk chunk(CHUNK_LEN);
    Context ctx(cam, chunk);

    ScalarField field;
    
}

void render(GLFWwindow *window) {


    program.use();

    GLuint vertices;
    glGenBuffers(1, &vertices);

    unsigned int colors;
    glGenBuffers(1, &colors);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindVertexArray(VAO);

    auto m = Mesh::gen_cube();

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(float),
                 m.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colors);
    glBufferData(GL_ARRAY_BUFFER, m.colors.size() * sizeof(float),
                 m.colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    int modelLoc = glGetUniformLocation(program.id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    modelLoc = glGetUniformLocation(program.id, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    Context ctx;

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelLoc = glGetUniformLocation(program.id, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                           glm::value_ptr(ctx.camera.view()));

        process_input(ctx);

        program.use();
        // Draw stuff here
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    // Initialise window
    if (!glfwInit())
        return -1;

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "voxel opengl", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Context ctx = prepare_scene();
    ctx.window = window;
    setup_input(ctx);
    start_renderer(ctx);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
