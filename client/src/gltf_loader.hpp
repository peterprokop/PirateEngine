#pragma once
// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#import <tinygltf/tiny_gltf.h>

using namespace tinygltf;

class GLTFLoader {
public:
    void loadModel(
        std::string filepath,
        std::vector<Vertex> &vertices,
        std::vector<uint32_t> &indices
    ) {
        Model model;
        TinyGLTF loader;
        std::string err;
        std::string warn;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filepath);
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
        auto indicesAccessorIndex = primitive.indices;
        auto indicesAccessor = model.accessors[indicesAccessorIndex];
        
        auto indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
        auto indicesBuffer = model.buffers[indicesBufferView.buffer];
        
        auto verticesAccessor = model.accessors[positionAccessorIndex];
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
            std::cout << " " << value << "\n";

            // if (i > 200) {
            //     break;
            // }
        }

        std::cout << std::endl;
        std::cout << "Vertices: " << std::endl;

        // If byte stride is specified - use it as increment.
        auto increment = (verticesBufferView.byteStride == 0)
            ? (3 * GetComponentSizeInBytes(TINYGLTF_COMPONENT_TYPE_FLOAT))
            : verticesBufferView.byteStride;
        for (int i = 0; i < verticesBufferView.byteLength; i += increment) {
            auto x = *((float *) (verticesBuffer.data.data() + verticesBufferView.byteOffset + i));
            auto y = *((float *) (verticesBuffer.data.data() + verticesBufferView.byteOffset + i + GetComponentSizeInBytes(TINYGLTF_COMPONENT_TYPE_FLOAT)));
            auto z = *((float *) (verticesBuffer.data.data() + verticesBufferView.byteOffset + i + 2 * GetComponentSizeInBytes(TINYGLTF_COMPONENT_TYPE_FLOAT)));
            
            std::cout << " "
            << (x)
            << " "
            << (y)
            << " "
            << (z)
            << ";\n";

            vertices.push_back(
                {{x, y, z}, {1.0f, 0.0f, 0.0f}, {(x + 1)/2, (y + 1)/2}}
            );
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
};
