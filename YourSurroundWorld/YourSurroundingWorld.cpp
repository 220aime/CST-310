/*Aime Serge Tuyishime
CST-310 
JULY 16, 2025*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>

// Vertex structure
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color;
};

// Material structure
struct Material {
    glm::vec3 color = glm::vec3(1.0f);
    float roughness = 0.5f;
    float metalness = 0.0f;
    float opacity = 1.0f;
    bool transparent = false;
};

// Light structure
struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    int type; // 0=directional, 1=point, 2=ambient
};

// Mesh class
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    GLuint VAO, VBO, EBO;
    glm::mat4 transform = glm::mat4(1.0f);
    bool castShadow = true;
    bool receiveShadow = true;

    Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds, const Material& mat)
        : vertices(verts), indices(inds), material(mat) {
        setupMesh();
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Texture coordinate attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        // Color attribute
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
    }
};

// Scene class
class Scene {
public:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Light> lights;
    glm::vec3 backgroundColor = glm::vec3(0.96f, 0.96f, 0.96f);
    bool isNightMode = false;

    void addMesh(std::unique_ptr<Mesh> mesh) {
        meshes.push_back(std::move(mesh));
    }

    void addLight(const Light& light) {
        lights.push_back(light);
    }

    void toggleLighting() {
        isNightMode = !isNightMode;
        if (isNightMode) {
            backgroundColor = glm::vec3(0.1f, 0.1f, 0.44f); // Night blue
            // Reduce ambient and directional light intensity
            for (auto& light : lights) {
                if (light.type == 2) light.intensity = 0.1f; // Ambient
                else if (light.type == 0) light.intensity = 0.3f; // Directional
                else if (light.type == 1) light.intensity = 2.0f; // Point lights more prominent
            }
        }
        else {
            backgroundColor = glm::vec3(0.96f, 0.96f, 0.96f); // Day white
            for (auto& light : lights) {
                if (light.type == 2) light.intensity = 0.3f; // Ambient
                else if (light.type == 0) light.intensity = 1.0f; // Directional
                else if (light.type == 1) light.intensity = 1.2f; // Point lights
            }
        }
    }
};

// Camera class
class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 5.0f, 18.0f);
    glm::vec3 target = glm::vec3(0.0f, 3.0f, -2.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float fov = 75.0f;
    float aspect = 1.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;

    int mode = 1; // 1=overview, 2=close-up, 3=detail
    float radius = 18.0f;
    float theta = M_PI / 3.0f;
    float phi = M_PI / 4.0f;

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
    }

    void updatePosition() {
        glm::vec3 eye;
        switch (mode) {
        case 1: // Overview
            eye = glm::vec3(
                radius * sin(theta) * cos(phi),
                radius * sin(theta) * sin(phi) + 5.0f,
                radius * cos(theta)
            );
            break;
        case 2: // Close-up
            eye = glm::vec3(
                12.0f * sin(theta + 0.3f) * cos(phi),
                12.0f * sin(theta + 0.3f) * sin(phi) + 3.0f,
                12.0f * cos(theta + 0.3f)
            );
            break;
        case 3: // Detail view
            eye = glm::vec3(
                8.0f * sin(theta + 0.5f) * cos(phi + 0.6f),
                8.0f * sin(theta + 0.5f) * sin(phi + 0.6f) + 2.0f,
                8.0f * cos(theta + 0.5f)
            );
            break;
        }
        position = eye;
    }

    void rotate(float deltaX, float deltaY) {
        phi += deltaX * 0.008f;
        theta = glm::clamp(theta + deltaY * 0.008f, 0.1f, (float)M_PI - 0.1f);
        updatePosition();
    }

    void zoom(float delta) {
        float scale = delta > 0 ? 1.05f : 0.95f;
        float newDistance = glm::length(position) * scale;
        if (newDistance > 5.0f && newDistance < 50.0f) {
            position *= scale;
        }
    }
};

// Geometry creation functions
std::unique_ptr<Mesh> createBox(float width, float height, float depth, const Material& material, const glm::vec3& pos = glm::vec3(0.0f)) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float hw = width * 0.5f, hh = height * 0.5f, hd = depth * 0.5f;

    // Define box vertices with normals
    vertices = {
        // Front face
        {{-hw, -hh,  hd}, {0, 0, 1}, {0, 0}, material.color},
        {{ hw, -hh,  hd}, {0, 0, 1}, {1, 0}, material.color},
        {{ hw,  hh,  hd}, {0, 0, 1}, {1, 1}, material.color},
        {{-hw,  hh,  hd}, {0, 0, 1}, {0, 1}, material.color},

        // Back face
        {{-hw, -hh, -hd}, {0, 0, -1}, {1, 0}, material.color},
        {{-hw,  hh, -hd}, {0, 0, -1}, {1, 1}, material.color},
        {{ hw,  hh, -hd}, {0, 0, -1}, {0, 1}, material.color},
        {{ hw, -hh, -hd}, {0, 0, -1}, {0, 0}, material.color},

        // Left face
        {{-hw, -hh, -hd}, {-1, 0, 0}, {0, 0}, material.color},
        {{-hw, -hh,  hd}, {-1, 0, 0}, {1, 0}, material.color},
        {{-hw,  hh,  hd}, {-1, 0, 0}, {1, 1}, material.color},
        {{-hw,  hh, -hd}, {-1, 0, 0}, {0, 1}, material.color},

        // Right face
        {{ hw, -hh, -hd}, {1, 0, 0}, {1, 0}, material.color},
        {{ hw,  hh, -hd}, {1, 0, 0}, {1, 1}, material.color},
        {{ hw,  hh,  hd}, {1, 0, 0}, {0, 1}, material.color},
        {{ hw, -hh,  hd}, {1, 0, 0}, {0, 0}, material.color},

        // Top face
        {{-hw,  hh, -hd}, {0, 1, 0}, {0, 1}, material.color},
        {{-hw,  hh,  hd}, {0, 1, 0}, {0, 0}, material.color},
        {{ hw,  hh,  hd}, {0, 1, 0}, {1, 0}, material.color},
        {{ hw,  hh, -hd}, {0, 1, 0}, {1, 1}, material.color},

        // Bottom face
        {{-hw, -hh, -hd}, {0, -1, 0}, {1, 1}, material.color},
        {{ hw, -hh, -hd}, {0, -1, 0}, {0, 1}, material.color},
        {{ hw, -hh,  hd}, {0, -1, 0}, {0, 0}, material.color},
        {{-hw, -hh,  hd}, {0, -1, 0}, {1, 0}, material.color}
    };

    indices = {
        0,  1,  2,   0,  2,  3,   // Front
        4,  5,  6,   4,  6,  7,   // Back
        8,  9, 10,   8, 10, 11,   // Left
        12, 13, 14,  12, 14, 15,  // Right
        16, 17, 18,  16, 18, 19,  // Top
        20, 21, 22,  20, 22, 23   // Bottom
    };

    auto mesh = std::make_unique<Mesh>(vertices, indices, material);
    mesh->transform = glm::translate(glm::mat4(1.0f), pos);
    return mesh;
}

std::unique_ptr<Mesh> createCylinder(float radius, float height, int segments, const Material& material, const glm::vec3& pos = glm::vec3(0.0f)) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float halfHeight = height * 0.5f;

    // Top center vertex
    vertices.push_back({ {0, halfHeight, 0}, {0, 1, 0}, {0.5f, 0.5f}, material.color });

    // Top circle vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back({ {x, halfHeight, z}, {0, 1, 0}, {0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle)}, material.color });
    }

    // Bottom center vertex
    vertices.push_back({ {0, -halfHeight, 0}, {0, -1, 0}, {0.5f, 0.5f}, material.color });

    // Bottom circle vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back({ {x, -halfHeight, z}, {0, -1, 0}, {0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle)}, material.color });
    }

    // Side vertices
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));

        vertices.push_back({ {x, halfHeight, z}, normal, {(float)i / segments, 1}, material.color });
        vertices.push_back({ {x, -halfHeight, z}, normal, {(float)i / segments, 0}, material.color });
    }

    // Top face indices
    for (int i = 1; i < segments + 1; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // Bottom face indices
    int bottomCenter = segments + 2;
    for (int i = 1; i < segments + 1; ++i) {
        indices.push_back(bottomCenter);
        indices.push_back(bottomCenter + i + 1);
        indices.push_back(bottomCenter + i);
    }

    // Side face indices
    int sideStart = (segments + 2) * 2;
    for (int i = 0; i < segments; ++i) {
        int current = sideStart + i * 2;
        int next = sideStart + ((i + 1) % (segments + 1)) * 2;

        indices.push_back(current);
        indices.push_back(current + 1);
        indices.push_back(next);

        indices.push_back(next);
        indices.push_back(current + 1);
        indices.push_back(next + 1);
    }

    auto mesh = std::make_unique<Mesh>(vertices, indices, material);
    mesh->transform = glm::translate(glm::mat4(1.0f), pos);
    return mesh;
}

// Enhanced furniture creation functions
std::vector<std::unique_ptr<Mesh>> createEnhancedTable(float x, float z) {
    std::vector<std::unique_ptr<Mesh>> meshes;

    // Table top
    Material topMaterial;
    topMaterial.color = glm::vec3(0.82f, 0.71f, 0.55f); // Wood color
    topMaterial.roughness = 0.3f;
    auto top = createBox(2.4f, 0.15f, 2.4f, topMaterial, glm::vec3(x, 3.5f, z));
    meshes.push_back(std::move(top));

    // Pedestal base
    Material pedestalMaterial;
    pedestalMaterial.color = glm::vec3(0.55f, 0.27f, 0.07f);
    pedestalMaterial.roughness = 0.4f;
    pedestalMaterial.metalness = 0.1f;
    auto pedestal = createCylinder(0.2f, 3.4f, 16, pedestalMaterial, glm::vec3(x, 1.75f, z));
    meshes.push_back(std::move(pedestal));

    // Base feet
    Material baseMaterial;
    baseMaterial.color = glm::vec3(0.2f, 0.2f, 0.2f);
    baseMaterial.roughness = 0.2f;
    baseMaterial.metalness = 0.8f;

    for (int i = 0; i < 4; i++) {
        float angle = i * (M_PI / 2.0f);
        float footX = x + 1.6f * cos(angle);
        float footZ = z + 1.6f * sin(angle);

        auto foot = createCylinder(0.12f, 0.08f, 16, baseMaterial, glm::vec3(footX, 0.04f, footZ));
        meshes.push_back(std::move(foot));
    }

    return meshes;
}

std::vector<std::unique_ptr<Mesh>> createEnhancedChair(float x, float z, float rotation = 0) {
    std::vector<std::unique_ptr<Mesh>> meshes;

    // Seat
    Material seatMaterial;
    seatMaterial.color = glm::vec3(0.58f, 0.44f, 0.86f); // Purple
    seatMaterial.roughness = 0.7f;
    auto seat = createBox(1.0f, 0.15f, 1.0f, seatMaterial, glm::vec3(x, 2.5f, z));
    meshes.push_back(std::move(seat));

    // Backrest
    auto back = createBox(1.0f, 1.2f, 0.12f, seatMaterial, glm::vec3(x, 3.3f, z - 0.44f));
    meshes.push_back(std::move(back));

    // Legs
    Material legMaterial;
    legMaterial.color = glm::vec3(0.4f, 0.4f, 0.4f);
    legMaterial.roughness = 0.1f;
    legMaterial.metalness = 0.9f;

    for (int i = 0; i < 4; i++) {
        float x_offset = (i % 2 == 0) ? -0.4f : 0.4f;
        float z_offset = (i < 2) ? -0.4f : 0.4f;
        auto leg = createCylinder(0.04f, 2.5f, 12, legMaterial, glm::vec3(x + x_offset, 1.25f, z + z_offset));
        meshes.push_back(std::move(leg));
    }

    // Apply rotation to all meshes
    if (rotation != 0) {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));
        for (auto& mesh : meshes) {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z));
            glm::mat4 negativeTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-x, 0, -z));
            mesh->transform = translation * rotationMatrix * negativeTranslation * mesh->transform;
        }
    }

    return meshes;
}

std::vector<std::unique_ptr<Mesh>> createOriginalFloor() {
    std::vector<std::unique_ptr<Mesh>> meshes;

    // Main floor tiles
    float tileSize = 1.5f;
    float tileHeight = 0.05f;
    float floorWidth = 30.0f;
    float floorDepth = 15.0f;
    int rows = (int)(floorDepth / tileSize);
    int cols = (int)(floorWidth / tileSize);

    Material lightGrayMaterial;
    lightGrayMaterial.color = glm::vec3(0.8f, 0.8f, 0.8f);

    Material darkGrayMaterial;
    darkGrayMaterial.color = glm::vec3(0.27f, 0.27f, 0.27f);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            bool isLight = (row + col) % 2 == 0;
            Material& material = isLight ? lightGrayMaterial : darkGrayMaterial;

            float x = col * tileSize - floorWidth / 2.0f + tileSize / 2.0f;
            float y = tileHeight / 2.0f;
            float z = row * tileSize - floorDepth / 2.0f + tileSize / 2.0f;

            auto tile = createBox(tileSize, tileHeight, tileSize, material, glm::vec3(x, y, z));
            meshes.push_back(std::move(tile));
        }
    }

    // Side floor (brown tiles)
    Material lightBrownMaterial;
    lightBrownMaterial.color = glm::vec3(0.82f, 0.71f, 0.55f);

    Material darkBrownMaterial;
    darkBrownMaterial.color = glm::vec3(0.55f, 0.27f, 0.07f);

    float sideFloorDepth = 5.0f;
    int sideRows = (int)(sideFloorDepth / tileSize);
    int sideCols = (int)(floorWidth / tileSize);

    for (int row = 0; row < sideRows; row++) {
        for (int col = 0; col < sideCols; col++) {
            bool isLight = (row + col) % 2 == 0;
            Material& material = isLight ? lightBrownMaterial : darkBrownMaterial;

            float x = col * tileSize - sideCols * tileSize / 2.0f + tileSize / 2.0f;
            float y = -0.005f + 0.06f;
            float z = row * tileSize - sideRows * tileSize / 2.0f - 4.5f;

            auto tile = createBox(tileSize, tileHeight, tileSize, material, glm::vec3(x, y, z));
            meshes.push_back(std::move(tile));
        }
    }

    return meshes;
}

std::vector<std::unique_ptr<Mesh>> createOriginalWalls() {
    std::vector<std::unique_ptr<Mesh>> meshes;

    // Back wall
    Material wallMaterial;
    wallMaterial.color = glm::vec3(1.0f, 0.98f, 0.8f); // Light cream
    auto backWall = createBox(30.0f, 15.0f, 0.1f, wallMaterial, glm::vec3(0, 7.5f, -7.5f));
    meshes.push_back(std::move(backWall));

    // Left wall (brick pattern simulation with darker red)
    Material brickMaterial;
    brickMaterial.color = glm::vec3(0.55f, 0.15f, 0.21f); // Dark red chocolate
    auto leftWall = createBox(0.1f, 15.0f, 15.0f, brickMaterial, glm::vec3(15.0f, 7.5f, 0.0f));
    meshes.push_back(std::move(leftWall));

    // Wainscot
    Material wainscotMaterial;
    wainscotMaterial.color = glm::vec3(0.9f, 0.79f, 0.66f);
    auto wainscot = createBox(30.0f, 3.0f, 0.1f, wainscotMaterial, glm::vec3(0, 1.5f, -7.4f));
    meshes.push_back(std::move(wainscot));

    return meshes;
}

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

struct Light {
    int type; // 0=directional, 1=point, 2=ambient
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light lights[10];
uniform int numLights;
uniform vec3 viewPos;
uniform float roughness;
uniform float metalness;
uniform float opacity;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    
    for(int i = 0; i < numLights && i < 10; i++) {
        if(lights[i].type == 2) { // Ambient
            result += lights[i].color * lights[i].intensity;
        }
        else if(lights[i].type == 0) { // Directional
            vec3 lightDir = normalize(-lights[i].position);
            float diff = max(dot(norm, lightDir), 0.0);
            result += lights[i].color * lights[i].intensity * diff;
        }
        else if(lights[i].type == 1) { // Point
            vec3 lightDir = normalize(lights[i].position - FragPos);
            float distance = length(lights[i].position - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            
            result += lights[i].color * lights[i].intensity * (diff + spec * metalness) * attenuation;
        }
    }
    
    FragColor = vec4(result * Color, opacity);
}
)";

// Shader compilation
GLuint compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Global variables
Scene scene;
Camera camera;
GLuint shaderProgram;
int windowWidth = 1200, windowHeight = 800;
bool mousePressed = false;
double lastMouseX, lastMouseY;

// Input callbacks
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        float deltaX = (float)(xpos - lastMouseX);
        float deltaY = (float)(ypos - lastMouseY);
        camera.rotate(deltaX, deltaY);
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.zoom((float)yoffset);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_1:
            camera.mode = 1;
            camera.updatePosition();
            break;
        case GLFW_KEY_2:
            camera.mode = 2;
            camera.updatePosition();
            break;
        case GLFW_KEY_3:
            camera.mode = 3;
            camera.updatePosition();
            break;
        case GLFW_KEY_L:
            scene.toggleLighting();
            break;
        case GLFW_KEY_R:
            camera.theta = M_PI / 3.0f;
            camera.phi = M_PI / 4.0f;
            camera.updatePosition();
            break;
        }
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    camera.aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

// Scene initialization
void initializeScene() {
    // Create floor
    auto floorMeshes = createOriginalFloor();
    for (auto& mesh : floorMeshes) {
        scene.addMesh(std::move(mesh));
    }

    // Create walls
    auto wallMeshes = createOriginalWalls();
    for (auto& mesh : wallMeshes) {
        scene.addMesh(std::move(mesh));
    }

    // Create tables
    auto table1Meshes = createEnhancedTable(-5.0f, -5.0f);
    for (auto& mesh : table1Meshes) {
        scene.addMesh(std::move(mesh));
    }

    auto table2Meshes = createEnhancedTable(3.0f, -5.0f);
    for (auto& mesh : table2Meshes) {
        scene.addMesh(std::move(mesh));
    }

    // Create chairs
    auto chair1Meshes = createEnhancedChair(-6.0f, -5.0f, 1.6f);
    for (auto& mesh : chair1Meshes) {
        scene.addMesh(std::move(mesh));
    }

    auto chair2Meshes = createEnhancedChair(-3.5f, -5.0f, -1.6f);
    for (auto& mesh : chair2Meshes) {
        scene.addMesh(std::move(mesh));
    }

    auto chair3Meshes = createEnhancedChair(-5.0f, -3.5f, M_PI);
    for (auto& mesh : chair3Meshes) {
        scene.addMesh(std::move(mesh));
    }

    // Add lighting
    Light ambientLight = { 2, glm::vec3(0), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f };
    scene.addLight(ambientLight);

    Light mainLight = { 0, glm::vec3(0, -1, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f };
    scene.addLight(mainLight);

    Light fillLight = { 0, glm::vec3(-1, -1, 0.5f), glm::vec3(0.53f, 0.81f, 0.92f), 0.4f };
    scene.addLight(fillLight);

    // Point lights for pendant lamps
    Light pendantLight1 = { 1, glm::vec3(-11, 7.8f, -3), glm::vec3(1.0f, 1.0f, 0.8f), 1.2f };
    scene.addLight(pendantLight1);

    Light pendantLight2 = { 1, glm::vec3(-5, 7.8f, -3), glm::vec3(1.0f, 1.0f, 0.8f), 1.2f };
    scene.addLight(pendantLight2);

    Light pendantLight3 = { 1, glm::vec3(1, 7.8f, -3), glm::vec3(1.0f, 1.0f, 0.8f), 1.2f };
    scene.addLight(pendantLight3);
}

// Render function
void render() {
    glClearColor(scene.backgroundColor.x, scene.backgroundColor.y, scene.backgroundColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Set matrices
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.position));

    // Set lights
    glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), (int)scene.lights.size());
    for (size_t i = 0; i < scene.lights.size() && i < 10; ++i) {
        std::string base = "lights[" + std::to_string(i) + "]";
        glUniform1i(glGetUniformLocation(shaderProgram, (base + ".type").c_str()), scene.lights[i].type);
        glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".position").c_str()), 1, glm::value_ptr(scene.lights[i].position));
        glUniform3fv(glGetUniformLocation(shaderProgram, (base + ".color").c_str()), 1, glm::value_ptr(scene.lights[i].color));
        glUniform1f(glGetUniformLocation(shaderProgram, (base + ".intensity").c_str()), scene.lights[i].intensity);
    }

    // Render meshes
    for (const auto& mesh : scene.meshes) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(mesh->transform));
        glUniform1f(glGetUniformLocation(shaderProgram, "roughness"), mesh->material.roughness);
        glUniform1f(glGetUniformLocation(shaderProgram, "metalness"), mesh->material.metalness);
        glUniform1f(glGetUniformLocation(shaderProgram, "opacity"), mesh->material.opacity);

        mesh->draw();
    }
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

    // Create window
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Enhanced 3D Office Break Room - C++ OpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shader program
    shaderProgram = createShaderProgram();

    // Initialize scene
    camera.aspect = (float)windowWidth / (float)windowHeight;
    camera.updatePosition();
    initializeScene();

    std::cout << "Enhanced 3D Office Break Room loaded successfully!" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "- Mouse: Click and drag to rotate" << std::endl;
    std::cout << "- Mouse wheel: Zoom in/out" << std::endl;
    std::cout << "- Number keys 1-3: Switch camera views" << std::endl;
    std::cout << "- L key: Toggle day/night lighting" << std::endl;
    std::cout << "- R key: Reset camera position" << std::endl;
    std::cout << "- ESC: Exit application" << std::endl;

    // Main loop
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Auto-rotation in overview mode
        if (camera.mode == 1) {
            camera.phi += 0.002f;
            camera.updatePosition();
        }

        glfwPollEvents();
        render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
