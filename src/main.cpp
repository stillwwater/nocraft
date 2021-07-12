#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#include "rendering.h"
#include "camera.h"
#include "world.h"

#include "xmath.h"

World *world = nullptr;
bool cursor_enabled = false;

static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (cursor_enabled) return;

    static float last_x = 0.0f;
    static float last_y = 0.0f;
    static bool init = false;
    const float sensitivity = 0.008f;

    auto &camera = world->camera;

    if (!init) {
        last_x = float(xpos);
        last_y = float(ypos);
        init = true;
    }

    float ox = float(xpos) - last_x;
    float oy = last_y - float(ypos);

    ox *= sensitivity;
    oy *= sensitivity;

    camera.rotation.x += ox;
    camera.rotation.y += oy;
    camera.rotation.y = math::clamp(camera.rotation.y,
                                    -math::PIOver2 + math::Epsilon,
                                    math::PIOver2 - math::Epsilon);
    world->camera.update_rotation();

    last_x = float(xpos);
    last_y = float(ypos);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        return;
    }
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(void) {
    const int Width = 800;
    const int Height = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto *window = glfwCreateWindow(Width, Height, "Nocraft", nullptr, nullptr);
    if (window == nullptr) {
        printf("error: Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("error: Failed to initialize GLAD\n");
        return -1;
    }

    glViewport(0, 0, Width, Height);
    glfwSwapInterval(1);

    world = new World{};
    world->load();

    while (!glfwWindowShouldClose(window)) {
        auto &camera = world->camera;

        float velocity = 0.2f;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (cursor_enabled)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_enabled = !cursor_enabled;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.position += camera.front * velocity;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.position -= camera.front * velocity;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.position -= camera.right * velocity;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.position += camera.right * velocity;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.position.y += velocity;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.position.y -= velocity;

        world->player_position = camera.position;
        world->renderer.draw(world);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
