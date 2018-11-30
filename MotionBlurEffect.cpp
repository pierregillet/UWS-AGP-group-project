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

#include "MotionBlurEffect.h"

//MotionBlurEffect::MotionBlurEffect(const GLuint &motionBlurShader,
//                                   const unsigned int &framesKept)
//        : framesKept(framesKept), motionBlurShader(motionBlurShader) {
//
//}

const unsigned int MotionBlurEffect::getNumberOfFramesKept() {
    return this->framesKept;
}

void MotionBlurEffect::setFramesKept(const unsigned int &framesKept) {
    this->framesKept = framesKept;
}

void MotionBlurEffect::updateBufferedTextures(const GLuint &bufferedTexture) {
    this->bufferedTextures.pop_front();
    this->bufferedTextures.push_back(bufferedTexture);
}

void MotionBlurEffect::setShader(const GLuint &shader) {
    this->motionBlurShader = shader;
}

const std::list<GLuint> MotionBlurEffect::getBufferedFrames() {
    return this->bufferedTextures;
}
