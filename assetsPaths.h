#ifndef AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
#define AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H

#include <string>
#include <vector>

namespace assetsPaths {
    const std::string bunnyObject = "../assets/bunny-5000.obj";
    const std::string cubeObject = "../assets/cube.obj";

    const std::string fabricTexture = "../assets/fabric.bmp";
    const std::string mossTexture = "../assets/moss.png";
    const std::string studdedMetalTexture = "../assets/studdedmetal.bmp";
    const std::string transparentWindowTexture = "../assets/transparent_window.png";

    const std::vector<std::string> skyboxTextures = {
            "../assets/town-skybox/Town_bk.bmp", "../assets/town-skybox/Town_ft.bmp",
            "../assets/town-skybox/Town_rt.bmp", "../assets/town-skybox/Town_lf.bmp",
            "../assets/town-skybox/Town_up.bmp", "../assets/town-skybox/Town_dn.bmp"
    };

    struct Shader {
        const std::string vertex;
        const std::string fragment;
    };

    const Shader blendingShader = {"../shaders/blended.vert",
                                   "../shaders/blended.frag"};

    const Shader cubeMapShader = {"../shaders/cubeMap.vert",
                                  "../shaders/cubeMap.frag"};

    const Shader gouraudShader = {"../shaders/gouraud-tex.vert",
                                  "../shaders/gouraud-tex.frag"};

    const Shader phongShader = {"../shaders/phong-tex.vert",
                                "../shaders/phong-tex.frag"};

    const Shader texturedShader = {"../shaders/textured.vert",
                                  "../shaders/textured.frag"};

    const Shader toonShader = {"../shaders/toon.vert",
                               "../shaders/toon.frag"};

// const Shader reflectionShader = {"../shaders/reflection-tex.vert",
//                                  "../shaders/reflection-tex.frag"};
};

#endif //AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
