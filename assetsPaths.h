#ifndef AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
#define AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H

#include <string>
#include <vector>

namespace assetsPaths {
#ifdef _WIN32
    const std::string rootPath = "./";
#endif
#ifndef _WIN32
    const std::string rootPath = "../";
#endif

    const std::string bunnyObject = rootPath + "assets/bunny-5000.obj";
    const std::string cubeObject = rootPath + "assets/cube.obj";

    const std::string fabricTexture = rootPath + "assets/fabric.bmp";
    const std::string mossTexture = rootPath + "assets/moss.png";
    const std::string studdedMetalTexture = rootPath + "assets/studdedmetal.bmp";
    const std::string transparentWindowTexture = rootPath + "assets/transparent_window.png";

    const std::vector<std::string> skyboxTextures = {
            rootPath + "assets/town-skybox/Town_bk.bmp", rootPath + "assets/town-skybox/Town_ft.bmp",
            rootPath + "assets/town-skybox/Town_rt.bmp", rootPath + "assets/town-skybox/Town_lf.bmp",
            rootPath + "assets/town-skybox/Town_up.bmp", rootPath + "assets/town-skybox/Town_dn.bmp"
    };

    struct Shader {
        const std::string vertex;
        const std::string fragment;
    };

    const Shader blendingShader = {rootPath + "shaders/blended.vert",
                                   rootPath + "shaders/blended.frag"};

    const Shader cubeMapShader = {rootPath + "shaders/cubeMap.vert",
                                  rootPath + "shaders/cubeMap.frag"};

    const Shader gouraudShader = {rootPath + "shaders/gouraud-tex.vert",
                                  rootPath + "shaders/gouraud-tex.frag"};

    const Shader phongShader = {rootPath + "shaders/phong-tex.vert",
                                rootPath + "shaders/phong-tex.frag"};

    const Shader texturedShader = {rootPath + "shaders/textured.vert",
                                   rootPath + "shaders/textured.frag"};

    const Shader toonShader = {rootPath + "shaders/toon.vert",
                               rootPath + "shaders/toon.frag"};

    const Shader motionBlurShader = {rootPath + "shaders/motionBlur.vert",
                                     rootPath + "shaders/motionBlur.frag"};

    const Shader shadowMapShader = {rootPath + "shaders/shadowMap.vert",
                                    rootPath + "shaders/shadowMap.frag"};

    const Shader depthShader = {rootPath + "shaders/depth.vert",
                               rootPath + "shaders/depth.frag"};

    const Shader quadShader = {rootPath + "shaders/passthrough.vert",
                               rootPath + "shaders/passthrough.frag"};
};

#endif //AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
