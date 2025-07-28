/*Aime Serge Tuyishime
Computer Graphics | CST-310
Project 8 : Simple Demo Scene | Exproring Universe
July 27, 2025 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Window dimensions
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;

// Vertex shader source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec3 LightColor;
out vec3 ObjectColor;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    LightPos = lightPos;
    LightColor = lightColor;
    ObjectColor = objectColor;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec3 LightColor;
in vec3 ObjectColor;

uniform vec3 viewPos;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * LightColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;
    
    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColor;
    
    vec3 result = (ambient + diffuse + specular) * ObjectColor;
    FragColor = vec4(result, 1.0);
}
)";

// Point shader for stars and particles
const char* pointVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;

out vec3 vertexColor;

void main()
{
    vertexColor = aColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_PointSize = pointSize;
}
)";

const char* pointFragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 vertexColor;

void main()
{
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5)
        discard;
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Sphere generation
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class Sphere {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    
    Sphere(float radius, int sectorCount, int stackCount) {
        generateSphere(radius, sectorCount, stackCount);
        setupBuffers();
    }
    
    ~Sphere() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    
private:
    void generateSphere(float radius, int sectorCount, int stackCount) {
        float x, y, z, xy;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;
        
        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;
        
        for(int i = 0; i <= stackCount; ++i) {
            stackAngle = M_PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);
            
            for(int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;
                
                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                
                Vertex vertex;
                vertex.position = glm::vec3(x, y, z);
                vertex.normal = glm::vec3(nx, ny, nz);
                vertices.push_back(vertex);
            }
        }
        
        // Generate indices
        int k1, k2;
        for(int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;
            
            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if(i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                
                if(i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
    
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
public:
    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// Planet class
class Planet {
public:
    glm::vec3 position;
    glm::vec3 color;
    float size;
    float orbitRadius;
    float orbitSpeed;
    float orbitAngle;
    float rotationSpeed;
    float rotation;
    bool isDynamic;
    float sizePhase;
    float originalSize;
    
    Planet(glm::vec3 pos, glm::vec3 col, float s, float orbRadius, float orbSpeed, bool dynamic = false) 
        : position(pos), color(col), size(s), originalSize(s), orbitRadius(orbRadius), 
          orbitSpeed(orbSpeed), orbitAngle(0.0f), rotationSpeed(0.01f), rotation(0.0f),
          isDynamic(dynamic), sizePhase(0.0f) {}
    
    void update(float deltaTime, glm::vec3 sunPosition) {
        orbitAngle += orbitSpeed * deltaTime;
        rotation += rotationSpeed * deltaTime;
        
        // Calculate orbital position
        position.x = sunPosition.x + cos(orbitAngle) * orbitRadius;
        position.z = sunPosition.z + sin(orbitAngle) * orbitRadius;
        position.y = sunPosition.y + sin(orbitAngle * 0.1f) * 30.0f; // Oblique orbit
        
        // Dynamic size changes
        if (isDynamic) {
            sizePhase += 0.05f * deltaTime * 60.0f; // 60 FPS equivalent
            size = originalSize * (0.8f + sin(sizePhase) * 0.4f);
        }
    }
};

// Star particle system
class StarField {
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    unsigned int VAO, VBO, colorVBO;
    
    StarField(int count) {
        generateStars(count);
        setupBuffers();
    }
    
    ~StarField() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &colorVBO);
    }
    
private:
    void generateStars(int count) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> posDis(-3000.0f, 3000.0f);
        std::uniform_real_distribution<float> colorDis(0.5f, 1.0f);
        
        for (int i = 0; i < count; ++i) {
            positions.push_back(glm::vec3(posDis(gen), posDis(gen), posDis(gen)));
            
            float hue = colorDis(gen);
            colors.push_back(glm::vec3(hue, hue * 0.8f, hue * 0.9f));
        }
    }
    
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &colorVBO);
        
        glBindVertexArray(VAO);
        
        // Position buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Color buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
    
public:
    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, positions.size());
        glBindVertexArray(0);
    }
};

// Space flash effect
class SpaceFlash {
public:
    glm::vec3 position;
    glm::vec3 color;
    float size;
    float maxSize;
    float age;
    float maxAge;
    bool active;
    
    SpaceFlash() : active(false) {}
    
    void create(glm::vec3 pos, glm::vec3 col, float maxS, float duration) {
        position = pos;
        color = col;
        maxSize = maxS;
        maxAge = duration;
        age = 0.0f;
        size = 0.0f;
        active = true;
    }
    
    void update(float deltaTime) {
        if (!active) return;
        
        age += deltaTime * 60.0f; // 60 FPS equivalent
        
        float ageRatio = age / maxAge;
        
        if (ageRatio < 0.2f) {
            // Expansion phase
            size = (ageRatio / 0.2f) * maxSize;
        } else if (ageRatio < 0.5f) {
            // Peak phase
            size = maxSize;
        } else {
            // Fade phase
            float fadeRatio = (ageRatio - 0.5f) / 0.5f;
            size = maxSize * (1.0f - fadeRatio * 0.3f);
        }
        
        if (age >= maxAge) {
            active = false;
        }
    }
};

// Shader compilation helper
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return shader;
}

// Shader program creation
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project 8: Exploring the Universe", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0f);
    
    // Create shaders
    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    unsigned int pointShaderProgram = createShaderProgram(pointVertexShaderSource, pointFragmentShaderSource);
    
    // Create sphere for planets and sun
    Sphere sphere(1.0f, 36, 18);
    
    // Create starfield
    StarField starField(5000);
    
    // Create planets
    std::vector<Planet> planets;
    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    
    // Add planets with realistic data
    planets.emplace_back(glm::vec3(0), glm::vec3(0.6f, 0.4f, 0.2f), 6.0f, 100.0f, 0.025f);  // Mercury
    planets.emplace_back(glm::vec3(0), glm::vec3(1.0f, 0.6f, 0.0f), 9.0f, 140.0f, 0.020f);  // Venus
    planets.emplace_back(glm::vec3(0), glm::vec3(0.0f, 0.4f, 0.8f), 10.0f, 175.0f, 0.018f); // Earth
    planets.emplace_back(glm::vec3(0), glm::vec3(1.0f, 0.4f, 0.2f), 8.0f, 210.0f, 0.015f);  // Mars
    planets.emplace_back(glm::vec3(0), glm::vec3(0.8f, 0.4f, 0.1f), 25.0f, 280.0f, 0.010f); // Jupiter
    planets.emplace_back(glm::vec3(0), glm::vec3(1.0f, 0.8f, 0.0f), 22.0f, 350.0f, 0.008f); // Saturn
    planets.emplace_back(glm::vec3(0), glm::vec3(0.3f, 0.8f, 0.9f), 16.0f, 420.0f, 0.006f); // Uranus
    planets.emplace_back(glm::vec3(0), glm::vec3(0.2f, 0.4f, 0.9f), 15.0f, 480.0f, 0.004f); // Neptune
    
    // Add some dynamic exoplanets
    planets.emplace_back(glm::vec3(0), glm::vec3(0.6f, 0.2f, 0.8f), 19.0f, 550.0f, 0.007f, true); // Dynamic
    planets.emplace_back(glm::vec3(0), glm::vec3(1.0f, 0.1f, 0.6f), 21.0f, 650.0f, 0.005f, true); // Dynamic
    
    // Space flashes
    std::vector<SpaceFlash> spaceFlashes(10);
    float flashTimer = 0.0f;
    float nextFlashTime = 50.0f;
    
    // Camera variables
    float cameraTime = 0.0f;
    float cameraSpeed = 0.005f;
    int cameraMode = 0;
    float cameraTransition = 0.0f;
    
    // Timing
    float lastFrame = 0.0f;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDis(-1500.0f, 1500.0f);
    std::uniform_int_distribution<int> colorDis(0, 5);
    
    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        cameraTime += cameraSpeed;
        cameraTransition += deltaTime;
        
        // Change camera mode every 15 seconds
        if (cameraTransition > 15.0f) {
            cameraMode = (cameraMode + 1) % 4;
            cameraTransition = 0.0f;
        }
        
        // Update planets
        for (auto& planet : planets) {
            planet.update(deltaTime, sunPosition);
        }
        
        // Update space flashes
        flashTimer += deltaTime * 60.0f;
        if (flashTimer >= nextFlashTime) {
            // Create new flash
            for (auto& flash : spaceFlashes) {
                if (!flash.active) {
                    glm::vec3 flashColors[] = {
                        glm::vec3(1.0f, 1.0f, 1.0f),  // White
                        glm::vec3(1.0f, 0.3f, 0.3f),  // Red
                        glm::vec3(0.3f, 0.3f, 1.0f),  // Blue
                        glm::vec3(1.0f, 0.7f, 0.0f),  // Orange
                        glm::vec3(1.0f, 0.0f, 1.0f),  // Magenta
                        glm::vec3(0.0f, 1.0f, 1.0f)   // Cyan
                    };
                    
                    glm::vec3 flashPos(posDis(gen), posDis(gen) * 0.5f, posDis(gen));
                    glm::vec3 flashColor = flashColors[colorDis(gen)];
                    flash.create(flashPos, flashColor, 15.0f + (rand() % 20), 30.0f + (rand() % 30));
                    break;
                }
            }
            flashTimer = 0.0f;
            nextFlashTime = 30.0f + (rand() % 90); // 30-120 frames
        }
        
        for (auto& flash : spaceFlashes) {
            flash.update(deltaTime);
        }
        
        // Clear screen
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Camera positioning based on mode
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        
        switch (cameraMode) {
            case 0: { // Exploration mode
                float radius = 400.0f + sin(cameraTime * 0.3f) * 200.0f;
                cameraPos = glm::vec3(
                    sin(cameraTime) * radius + cos(cameraTime * 0.4f) * 300.0f,
                    sin(cameraTime * 0.6f) * 150.0f + cos(cameraTime * 0.8f) * 80.0f,
                    cos(cameraTime) * radius + sin(cameraTime * 0.5f) * 400.0f
                );
                cameraTarget = glm::vec3(
                    sin(cameraTime * 0.2f) * 50.0f,
                    cos(cameraTime * 0.3f) * 30.0f,
                    sin(cameraTime * 0.4f) * 100.0f
                );
                break;
            }
            case 1: { // Sun focus
                float sunDistance = 150.0f + sin(cameraTime) * 50.0f;
                float sunAngle = cameraTime * 0.8f;
                cameraPos = glm::vec3(
                    cos(sunAngle) * sunDistance,
                    sin(sunAngle * 0.3f) * 100.0f,
                    sin(sunAngle) * sunDistance
                );
                cameraTarget = sunPosition;
                break;
            }
            case 2: { // Planet chase
                if (!planets.empty()) {
                    Planet& targetPlanet = planets[0]; // Chase first planet
                    float planetDistance = 80.0f + sin(cameraTime * 2.0f) * 30.0f;
                    float chaseAngle = cameraTime * 1.5f;
                    cameraPos = targetPlanet.position + glm::vec3(
                        cos(chaseAngle) * planetDistance,
                        sin(chaseAngle * 0.5f) * 50.0f,
                        sin(chaseAngle) * planetDistance
                    );
                    cameraTarget = targetPlanet.position;
                }
                break;
            }
            case 3: { // Wide overview
                cameraPos = glm::vec3(0.0f, 500.0f, 800.0f + sin(cameraTime * 0.2f) * 200.0f);
                cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                break;
            }
        }
        
        // Set up matrices
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), 
                                              (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 
                                              0.1f, 10000.0f);
        
        // Draw starfield
        glUseProgram(pointShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(pointShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(pointShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(glGetUniformLocation(pointShaderProgram, "pointSize"), 2.0f);
        starField.draw();
        
        // Draw 3D objects
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(sunPosition));
        glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 0.8f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        
        // Draw sun
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, sunPosition);
        model = glm::scale(model, glm::vec3(40.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.8f, 0.0f);
        sphere.draw();
        
        // Draw planets
        for (const auto& planet : planets) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, planet.position);
            model = glm::rotate(model, planet.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(planet.size));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(planet.color));
            sphere.draw();
        }
        
        // Draw space flashes
        for (const auto& flash : spaceFlashes) {
            if (flash.active) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, flash.position);
                model = glm::scale(model, glm::vec3(flash.size));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(flash.color));
                sphere.draw();
            }
        }
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteProgram(pointShaderProgram);
    glfwTerminate();
    return 0;
}
