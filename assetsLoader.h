#ifndef AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H
#define AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H

#include <GL/glew.h>
#include <string>
#include <vector>


GLuint loadTexture(std::string fileName);

void loadCubeMap(std::vector<std::string> fileName, GLuint *texID);


#endif //AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H
