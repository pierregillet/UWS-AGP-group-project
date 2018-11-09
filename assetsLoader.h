#ifndef AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H
#define AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H

#include <GL/glew.h>


GLuint loadTexture(const char *fileName);

void loadCubeMap(const char * const fileName[6], GLuint *texID);


#endif //AGP_INDIVIDUAL_PROJECT_ASSETSLOADER_H
