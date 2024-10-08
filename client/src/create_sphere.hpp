#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#import <glm/glm.hpp>
#import "vertex.hpp"

glm::vec3 sphericalToNorma(
    float phi,
    float theta
) {
    return {
        glm::cos(theta) * glm::cos(phi),
        glm::cos(theta) * glm::sin(phi),
        glm::sin(theta)
    };
}

void addPointOnSphere(
    float phi,
    float theta,
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices,
    glm::vec3 offset
) {
    indices.push_back(vertices.size());
    const glm::vec3 coordinates = offset + sphericalToNorma(phi, theta);
    vertices.push_back(
        {coordinates, {1.0f, 0.0f, 0.0f}, {(coordinates.x + 1)/2, (coordinates.y + 1)/2}}
    );
}

void createSphere(
    std::vector<Vertex> &vertices,
    std::vector<uint32_t> &indices,
    glm::vec3 offset
) {
    const int numSphereParts = 12;

    for (int i = 0; i < numSphereParts; i++) {
        for (int j = 0; j < numSphereParts; j++) {
            const float phi = 2 * glm::pi<float>() * i / numSphereParts; 
            const float theta = 2 * glm::pi<float>() * j / numSphereParts;
            const float phiPlus = 2 * glm::pi<float>() * (i + 1) / numSphereParts; 
            const float thetaPlus = 2 * glm::pi<float>() * (j + 1) / numSphereParts;
            const float phiMinus = 2 * glm::pi<float>() * (i - 1) / numSphereParts; 
            const float thetaMinus = 2 * glm::pi<float>() * (j - 1) / numSphereParts;

            addPointOnSphere(phi, theta, vertices, indices, offset);
            addPointOnSphere(phi, thetaPlus, vertices, indices, offset);
            addPointOnSphere(phiPlus, thetaPlus, vertices, indices, offset);

            addPointOnSphere(phi, theta, vertices, indices, offset);
            addPointOnSphere(phiPlus, thetaPlus, vertices, indices, offset);
            addPointOnSphere(phi, thetaPlus, vertices, indices, offset);

            addPointOnSphere(phi, theta, vertices, indices, offset);
            addPointOnSphere(phiPlus, theta, vertices, indices, offset);
            addPointOnSphere(phiPlus, thetaMinus, vertices, indices, offset);
        }
    }
}