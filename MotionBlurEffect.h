/**
 * This file is part of a group project written in C++ using the SDL2
 * library and OpenGL API.
 *
 * Copyright (C) 2018 Pierre Gillet
 * Copyright (C) 2018 Jamie McKechnie
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AGP_GROUP_PROJECT_MOTIONBLUREFFECT_H
#define AGP_GROUP_PROJECT_MOTIONBLUREFFECT_H

#include <GL/glew.h>
#include <list>

#include "Shader.h"


class MotionBlurEffect {
public:
//    MotionBlurEffect(const GLuint &motionBlurShader,
//                     const unsigned int &framesKept);

private:
    GLuint motionBlurShader;
    unsigned int framesKept;
    std::list<GLuint> bufferedTextures = std::list<GLuint>();

public:
    void setShader(const GLuint &shader);
    const unsigned int getNumberOfFramesKept();
    void setFramesKept(const unsigned int &framesKept);
    void updateBufferedTextures(const GLuint &bufferedTexture);
    const std::list<GLuint> getBufferedFrames();
};

#endif //AGP_GROUP_PROJECT_MOTIONBLUREFFECT_H
