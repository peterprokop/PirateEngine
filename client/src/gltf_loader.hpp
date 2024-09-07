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
    void loadModel(std::string filepath) {
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
        #endif
        
    }
};
