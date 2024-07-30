#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"
#include <string>
#include "stb_image.h"

namespace lf {
    namespace internal {
        unsigned int TextureFromFile(const char* path, const std::string& directory = ".", bool gamma = false, bool flipBack = false);

        class ModelLoader
        {
        public:
            // model data 
            std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
            std::vector<Mesh>    meshes;
            std::string directory;
            bool gammaCorrection;

            ModelLoader() {}

            // constructor, expects a filepath to a 3D model.
            ModelLoader(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
            {
                loadModel(path);
            }

            // draws the model, and thus all its meshes
            void Draw(Shader& shader)
            {
                for (unsigned int i = 0; i < meshes.size(); i++)
                    meshes[i].Draw(shader);
            }

        private:
            // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
            void loadModel(std::string const& path);

            // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
            void processNode(aiNode* node, const aiScene* scene);

            Mesh processMesh(aiMesh* mesh, const aiScene* scene);

            // checks all material textures of a given type and loads the textures if they're not loaded yet.
            // the required info is returned as a Texture struct.
            std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        };
    }
}