#include "Mesh.h"

#include <vector>

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include "assetsPaths.h"

Mesh::Mesh(const std::string &objectPath) {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textureCoordinates;
    std::vector<GLuint> indices;
    rt3d::loadObj(objectPath.c_str(), vertices, normals, textureCoordinates,
                  indices);
    this->meshIndexCount = static_cast<GLuint>(indices.size());

    this->meshObject = rt3d::createMesh(
            static_cast<const GLuint>(vertices.size() / 3), vertices.data(),
            nullptr, normals.data(), textureCoordinates.data(),
            this->meshIndexCount, indices.data()
    );
}

GLuint Mesh::getMeshIndexCount() {
    return this->meshIndexCount;
}

GLuint Mesh::getMeshObject() {
    return this->meshObject;
}
