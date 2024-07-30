#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Camera.h"

namespace lf {
    class Shader
    {        
    public:

        // initialization...
        static void initializeShaders();
        static Shader& getNormalShader();
        static Shader& getColoredShader();
        static float nearPlane, farPlane;


        // additional configuration...
        /*GL_ALWAYS	The depth test always passes.
        GL_NEVER	The depth test never passes.
        GL_LESS	Passes if the fragment's depth value is less than the stored depth value.
        GL_EQUAL	Passes if the fragment's depth value is equal to the stored depth value.
        GL_LEQUAL	Passes if the fragment's depth value is less than or equal to the stored depth value.
        GL_GREATER	Passes if the fragment's depth value is greater than the stored depth value.
        GL_NOTEQUAL	Passes if the fragment's depth value is not equal to the stored depth value.
        GL_GEQUAL	Passes if the fragment's depth value is greater than or equal to the stored depth value.*/
        static void ChangeDepthFunction(int depthFunction);
        static void ChangeNearFarPlaneValues(float nearPlane, float farPlane);
        // offset is a distance at which fog will start taking more effect
        // steepness is a value that coresponds to for growth per distance
        static void ChangeFogEffect(float steepness, float offset);


        // main shader functionality...
        unsigned int ID;
        Shader() {}
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        virtual ~Shader() {}
        void use() {
            glUseProgram(ID);
        }
        void setBool(const std::string& name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string& name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setVec2(const std::string& name, const glm::vec2& value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        void setVec3(const std::string& name, const glm::vec3& value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        void setVec4(const std::string& name, const glm::vec4& value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        void setMat2(const std::string& name, const glm::mat2& mat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat3(const std::string& name, const glm::mat3& mat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat4(const std::string& name, const glm::mat4& mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
    private:
        void checkCompileErrors(GLuint shader, std::string type);
    };
}

#endif