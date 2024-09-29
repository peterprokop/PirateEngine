#pragma once
// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#import <tinygltf/tiny_gltf.h>

using namespace tinygltf;


std::vector<Vertex> getVertices(
    Model const& model,
    Accessor const& accessor,
    int texCoordAccessorIndex
) {
    std::cout << "texCoordAccessorIndex: " << texCoordAccessorIndex << ";\n";

    std::vector<Vertex> vertices;
    const BufferView bufferView = model.bufferViews[accessor.bufferView];
    const Buffer buffer = model.buffers[bufferView.buffer];

    // Texture coordinates
    Accessor texCoordAccessor;
    BufferView texCoordBufferView;
    Buffer texCoordBuffer;
    const bool hasTexCoordAccessor = (texCoordAccessorIndex != -1);
    if (hasTexCoordAccessor) {
        texCoordAccessor = model.accessors[texCoordAccessorIndex];        
        texCoordBufferView = model.bufferViews[texCoordAccessor.bufferView];
        texCoordBuffer = model.buffers[texCoordBufferView.buffer];
    }

    const int componentType = accessor.componentType;
    const int type = accessor.type;
    const size_t byteStride = bufferView.byteStride;

    // If byte stride is specified - use it as increment.
    int increment = (byteStride == 0)
        ? (GetNumComponentsInType(type) * GetComponentSizeInBytes(componentType))
        : byteStride;

    const int texCoordComponentType = texCoordAccessor.componentType;
    const int texCoordType = texCoordAccessor.type;
    const size_t texCoordByteStride = texCoordBufferView.byteStride;

    // If byte stride is specified - use it as increment.
    int texCoordIncrement = (texCoordByteStride == 0)
        ? (GetNumComponentsInType(texCoordType) * GetComponentSizeInBytes(texCoordComponentType))
        : texCoordByteStride;

    int j = 0;
    for (int i = 0; i < bufferView.byteLength; i += increment) {
        float x = *((float *) (buffer.data.data() + bufferView.byteOffset + i));
        float y = *((float *) (buffer.data.data() + bufferView.byteOffset + i + GetComponentSizeInBytes(componentType)));
        float z = *((float *) (buffer.data.data() + bufferView.byteOffset + i + 2 * GetComponentSizeInBytes(componentType)));
        
        // std::cout << " " << x << " " << y << " " << z << ";\n";
        float u = (x + 1)/2;
        float v = (y + 1)/2;
        if (hasTexCoordAccessor && (j < texCoordBufferView.byteLength)) {
            u = *((float *) (texCoordBuffer.data.data() + texCoordBufferView.byteOffset + j));
            v = *((float *) (texCoordBuffer.data.data() + texCoordBufferView.byteOffset + j + GetComponentSizeInBytes(texCoordComponentType)));
            j += texCoordIncrement;
        }

        std::cout << "u: " << u << "v: " << v << ";\n";

        vertices.push_back(
            {{x, y, z}, {1.0f, 0.0f, 0.0f}, {u, v}}
        );
    }

    return vertices;
}

class GLTFLoader {
public:
    void loadModel(
        std::string folderPath,
        std::string filename,
        std::vector<Vertex> &vertices,
        std::vector<uint32_t> &indices,
        std::string &textureFilePath
    ) {
        Model model;
        TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, folderPath + filename);
        // for binary glTF(.glb)
        //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]);

        if (!warn.empty()) {
            printf("Warn: %s\n", warn.c_str());
        }

        if (!err.empty()) {
            printf("Err: %s\n", err.c_str());
        }

        if (!ret) {
            printf("Failed to parse glTF\n");
        }

        #ifndef NDEBUG
        printf("Successfully loaded model:\n");
        printf("accessors %lu\n", model.accessors.size());
        printf("animations %lu\n", model.animations.size());
        printf("buffers %lu\n", model.buffers.size());
        printf("bufferViews %lu\n", model.bufferViews.size());
        printf("materials %lu\n", model.materials.size());
        printf("meshes %lu\n", model.meshes.size());
        printf("nodes %lu\n", model.nodes.size());
        printf("textures %lu\n", model.textures.size());
        printf("images %lu\n", model.images.size());
        printf("skins %lu\n", model.skins.size());
        printf("samplers %lu\n", model.samplers.size());
        printf("cameras %lu\n", model.cameras.size());
        printf("scenes %lu\n", model.scenes.size());
        printf("lights %lu\n", model.lights.size());
        printf("audioEmitters %lu\n", model.audioEmitters.size());
        printf("audioSources %lu\n", model.audioSources.size());

        printAccessors(model.accessors);
        printMeshes(model.meshes);
        #endif
        
        auto primitive = model.meshes[0].primitives[0];
        auto positionAccessorIndex = primitive.attributes["POSITION"];

        int texCoordAccessorIndex;
        try {
            texCoordAccessorIndex = primitive.attributes.at("TEXCOORD_0");
        } catch (const std::out_of_range&) {
            texCoordAccessorIndex = -1;
        }
        
        auto indicesAccessorIndex = primitive.indices;
        Accessor indicesAccessor = model.accessors[indicesAccessorIndex];
        
        auto indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
        auto indicesBuffer = model.buffers[indicesBufferView.buffer];
        
        Accessor verticesAccessor = model.accessors[positionAccessorIndex];

        auto verticesBufferView = model.bufferViews[verticesAccessor.bufferView];
        auto verticesBuffer = model.buffers[verticesBufferView.buffer];

        std::cout << "positionAccessorIndex: " << positionAccessorIndex << std::endl;

        // TINYGLTF_TYPE_SCALAR
        std::cout << "indicesAccessor.type: " << indicesAccessor.type << std::endl;

        // 65 (TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        std::cout << "indicesAccessor.componentType: " << indicesAccessor.componentType << std::endl;

        // 14016
        std::cout << "indicesAccessor.count: " << indicesAccessor.count << std::endl;

        // TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER
        std::cout << "indicesBufferView.target: " << indicesBufferView.target << std::endl;

        // 0
        std::cout << "indicesBufferView.byteStride: " << indicesBufferView.byteStride << std::endl;

        // 3 (TINYGLTF_TYPE_VEC3)
        std::cout << "verticesAccessor.type: " << verticesAccessor.type << std::endl;

        // TINYGLTF_COMPONENT_TYPE_FLOAT
        std::cout << "verticesAccessor.componentType: " << verticesAccessor.componentType << std::endl;
        
        // 3273
        std::cout << "verticesAccessor.count: " << verticesAccessor.count << std::endl;

        // TINYGLTF_TARGET_ARRAY_BUFFER
        std::cout << "verticesBufferView.target: " << verticesBufferView.target << std::endl;

        // 12
        std::cout << "verticesBufferView.byteStride: " << verticesBufferView.byteStride << std::endl;

        std::cout << "Indices: " << std::endl;
        for (int i = 0; i < indicesBufferView.byteLength; i += 2) {
            auto value = *((unsigned short int *) (indicesBuffer.data.data() + indicesBufferView.byteOffset + i));
            indices.push_back(value);
            // std::cout << " " << value << "\n";

            // if (i > 200) {
            //     break;
            // }
        }

        std::vector<Vertex> modelVertices = getVertices(model, verticesAccessor, texCoordAccessorIndex);
        vertices.insert(vertices.end(), modelVertices.begin(), modelVertices.end());

        if (model.images.size() > 0) {
            // TODO: handle multiple images
            textureFilePath = folderPath + model.images[0].uri;
            printImage(model.images[0]);
            std::cout << "textureFilePath: " << textureFilePath << std::endl;
        }        
    }

    void printMeshes(std::vector<Mesh> const& meshes) {
        for (auto const& mesh : meshes) {
            std::cout << "Mesh name: " << mesh.name  << std::endl;
            std::cout << "mesh.primitives: " << mesh.primitives.size()  << std::endl;
            std::cout << "mesh.weights: " << mesh.weights.size()  << std::endl;
            // std::cout << "mesh:extensions: " << mesh.extensions  << std::endl;
            // std::cout << "mesh.extras: " << mesh.extras << std::endl;
            std::cout << "mesh.extras_json_string: " << mesh.extras_json_string  << std::endl;
            std::cout << "mesh.extensions_json_string: " << mesh.extensions_json_string  << std::endl;
            std::cout << std::endl;

            printPrimitives(mesh.primitives);
        }
    }

    void printPrimitives(std::vector<Primitive> const& primitives) {
        for (auto const& primitive : primitives) {
            std::cout << "Attributes: " << std::endl;
            for (auto const& [key, val] : primitive.attributes)
            {
                std::cout << key
                        << ':'  
                        << val
                        << std::endl;
            }
            // std::cout << "Attributes: " << primitive.attributes  << std::endl;
            std::cout << "material: " << primitive.material  << std::endl;
            std::cout << "indices: " << primitive.indices  << std::endl;
            std::cout << "mode: " << primitive.mode  << std::endl;
            std::cout << "targets: " << primitive.targets.size()  << std::endl;
            std::cout << std::endl;
        }
    }

    void printAccessors(std::vector<Accessor> const& accessors) {
        std::cout << "=== Accessors ===" << std::endl;
        for (auto const& accessor : accessors) {
            // std::cout << "Accessor name: " << accessor.name  << std::endl;
            std::cout << "bufferView: " << accessor.bufferView  << std::endl;
            std::cout << "count: " << accessor.count  << std::endl;
            std::cout << std::endl;
        }
    }

    void printImage(Image const& image) {
        std::cout << "Image name: " << image.name  << std::endl;
        std::cout << "mimeType: " << image.mimeType  << std::endl;
        std::cout << "uri: " << image.uri  << std::endl;
        std::cout << "width: " << image.width  << std::endl;
        std::cout << "height: " << image.height  << std::endl;
        std::cout << "bits: " << image.bits  << std::endl;
        std::cout << "pixel_type: " << image.pixel_type  << std::endl;
        std::cout << "component: " << image.component  << std::endl;
        std::cout << "image: " << image.image  << std::endl;
        std::cout << "bufferView: " << image.bufferView  << std::endl;
    }
};
