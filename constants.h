#ifndef AGP_GROUP_PROJECT_CONSTANTS_H
#define AGP_GROUP_PROJECT_CONSTANTS_H

#include <glm/glm.hpp>
#include <vector>

#include "rt3d.h"

namespace Constants {
    namespace BaseWindowSize {
        const int width = 800;
        const int height = 600;
    };

    const double degreeToRadian = 0.017453293;

    const glm::vec4 initialLightPosition(-5.0f, 2.0f, 2.0f, 1.0f);
    const glm::vec4 initialCubePosition(-5.0f, 2.0f, 2.0f, 1.0f);
    const unsigned int motionBlurFramesKept = 5;

    namespace LightAttenuation {
        const float constant = 1.0f;
        const float linear = 0.0f;
        const float quadratic = 0.0f;
    };

    const rt3d::lightStruct light0 = {
            {0.4f, 0.4f, 0.4f, 1.0f}, // ambient
            {1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
            {1.0f, 1.0f, 1.0f, 1.0f}, // specular
            {-5.0f, 2.0f, 2.0f, 1.0f}  // position
    };

    const rt3d::materialStruct material0 = {
            {0.2f, 0.4f, 0.2f, 1.0f}, // ambient
            {0.5f, 1.0f, 0.5f, 1.0f}, // diffuse
            {0.0f, 0.1f, 0.0f, 1.0f}, // specular
            2.0f  // shininess
    };
    const rt3d::materialStruct material1 = {
            {0.4f, 0.4f, 1.0f, 1.0f}, // ambient
            {0.8f, 0.8f, 1.0f, 1.0f}, // diffuse
            {0.8f, 0.8f, 0.8f, 1.0f}, // specular
            1.0f  // shininess
    };

    // vertex attributes for a quad that fills the entire screen
    // in Normalized Device Coordinates.
    const GLfloat screenQuadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    const glm::mat4 biasMatrix = {
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    };
};

#endif //AGP_GROUP_PROJECT_CONSTANTS_H
