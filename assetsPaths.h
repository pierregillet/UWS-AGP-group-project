#ifndef AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
#define AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H

namespace assetsPaths {
    const char * const bunnyObject = "../assets/bunny-5000.obj";
    const char * const cubeObject = "../assets/cube.obj";

    const char * const fabricTexture = "../assets/fabric.bmp";
    const char * const mossTexture = "../assets/moss.png";
    const char * const studdedMetalTexture = "../assets/studdedmetal.bmp";
    const char * const transparentWindowTexture = "../assets/transparent_window.png";

    const char * const skyboxTextures[6] = {
            "../assets/town-skybox/Town_bk.bmp", "../assets/town-skybox/Town_ft.bmp",
            "../assets/town-skybox/Town_rt.bmp", "../assets/town-skybox/Town_lf.bmp",
            "../assets/town-skybox/Town_up.bmp", "../assets/town-skybox/Town_dn.bmp"
    };

    struct Shader {
        char * vertex;
        char * fragment;
    };

    Shader blendingShader = {const_cast<char *>("../shaders/blended.vert"),
                             const_cast<char *>("../shaders/blended.frag")};

    Shader cubeMapShader = {const_cast<char *>("../shaders/cubeMap.vert"),
                            const_cast<char *>("../shaders/cubeMap.frag")};

    Shader gouraudShader = {const_cast<char *>("../shaders/gouraud-tex.vert"),
                            const_cast<char *>("../shaders/gouraud-tex.frag")};

    Shader phongShader = {const_cast<char *>("../shaders/phong-tex.vert"),
                             const_cast<char *>("../shaders/phong-tex.frag")};

    Shader texturedShader = {const_cast<char *>("../shaders/textured.vert"),
                             const_cast<char *>("../shaders/textured.frag")};

    Shader toonShader = {const_cast<char *>("../shaders/toon.vert"),
                         const_cast<char *>("../shaders/toon.frag")};

//    Shader reflectionShader = {const_cast<char *>("../shaders/reflection-tex.vert"),
//                               const_cast<char *>("../shaders/reflection-tex.frag")};
};

#endif //AGP_INDIVIDUAL_PROJECT_ASSETSPATHS_H
