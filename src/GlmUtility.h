#ifndef GLMUTILITY_H
#define GLMUTILITY_H
#define GLM_ENABLE_EXPERIMENTAL

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iomanip>
#include <iostream>
#include <random>

namespace glm {
static std::default_random_engine generator;

static glm::vec3 get_random_within(glm::vec3 origin, glm::vec3 size) {
    std::uniform_real_distribution<float> xDistribution(origin.x - size.x,
                                                        origin.x + size.x);
    std::uniform_real_distribution<float> yDistribution(origin.y - size.y,
                                                        origin.y + size.y);
    std::uniform_real_distribution<float> zDistribution(origin.z - size.z,
                                                        origin.z + size.z);
    float x = xDistribution(generator);
    float y = yDistribution(generator);
    float z = zDistribution(generator);

    return glm::vec3(x, y, z);
}

static float get_random_between(glm::vec2 range) {
    std::uniform_real_distribution<float> distribution(range.x, range.y);
    return distribution(generator);
}

static glm::quat get_random_quaternion(std::array<bool, 3> anglesToRotate) {
    std::uniform_real_distribution<float> distribution(0, 360);
    glm::vec3 rotationAngles;

    for (int i = 0; i < 3; i++) {
        if (anglesToRotate[i]) {
            rotationAngles[i] = distribution(generator);
        }
    }

    glm::quat result = glm::quat(rotationAngles);
    return result;
}

static inline glm::vec3 int_rgb_to_float_rgb(glm::vec3 int_rgb) {
    return int_rgb / (255.f);
}

static void print(glm::vec3 vector) {
    std::cout << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
}

static void print(glm::vec4 vector) {
    std::cout << vector.x << ", " << vector.y << ", " << vector.z << ", "
              << vector.w << std::endl;
}

static void print(glm::mat3 matrix) {
    std::cout << "START";
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            std::cout << std::setw(5) << matrix[r][c];
        }
        std::cout << std::endl;
    }
    std::cout << "END";
}

static void print(glm::mat4 matrix) {
    std::cout << "START" << std::endl;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            std::cout << std::setw(5) << matrix[r][c];
        }
        std::cout << std::endl;
    }
    std::cout << "END" << std::endl;
}

static glm::mat4 arrayToMat4(const std::vector<float> &transformArray) {
    glm::mat4 matrix(1.0);
    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {
            matrix[r][c] = transformArray[r * 4 + c];
        }
    }

    return matrix;
}

static std::vector<float> mat4ToArray(const glm::mat4 &matrix) {
    std::vector<float> matrixArray;
    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {
            matrixArray.push_back(matrix[r][c]);
        }
    }
    return matrixArray;
}

static glm::vec3 arrayToVec3(const std::vector<float> &vecArray) {
    glm::vec3 result(1.0);
    for (size_t v = 0; v < 3; v++) {
        result[v] = vecArray[v];
    }
    return result;
}

static std::vector<float> vec3ToArray(const glm::vec3 vec) {
    std::vector<float> vecArray;
    for (size_t v = 0; v < 3; v++) {
        vecArray.push_back(vec[v]);
    }
    return vecArray;
}
} // namespace glm

#endif
