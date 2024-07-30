#include <vector>
#include "Shader.h"

namespace lf {
    static Shader normalShader, coloredShader;

    float Shader::NearPlane = 0.1f;
    float Shader::FarPlane = 100.f;
    glm::vec3 Shader::BackgroundColor = glm::vec3(0.f, 0.f, 0.f);

    void Shader::ChangeDepthFunction(int depthFunction) {
        glDepthFunc(depthFunction);
    }

    void Shader::ChangeNearFarPlaneValues(float nearPlane, float farPlane) {
        if (nearPlane <= 0.f || farPlane <= 0.f || farPlane <= nearPlane) {
            std::cerr << "Near and far plane mustn't be set exactly at 0 and far plane must be further away than near plane.\n";
        }
        else {
            Shader::NearPlane = nearPlane;
            Shader::FarPlane = farPlane;
            normalShader.use();
            normalShader.setFloat("near", nearPlane);
            normalShader.setFloat("far", farPlane);
            coloredShader.use();
            coloredShader.setFloat("near", nearPlane);
            coloredShader.setFloat("far", farPlane);
        }
    }

    void Shader::ChangeFogEffect(float steepness, float offset) {
        normalShader.use();
        normalShader.setFloat("steepness", steepness);
        normalShader.setFloat("offset", offset);
        coloredShader.use();
        coloredShader.setFloat("steepness", steepness);
        coloredShader.setFloat("offset", offset);
    }

    void Shader::ChangeBackgroundColor(const glm::vec3& backgroundColor) {
        Shader::BackgroundColor = backgroundColor;
        normalShader.use();
        normalShader.setVec3("backgroundColor", backgroundColor);
        coloredShader.use();
        coloredShader.setVec3("backgroundColor", backgroundColor);
    }

    void Shader::initializeShaders() {
        static bool firstCall = true;
        if (firstCall) {
            normalShader = Shader("shaders/multiLight.vs", "shaders/multyLight.fs");
            coloredShader = Shader("shaders/newColorShader.vs", "shaders/newColorShader.fs");
            normalShader.use();
            normalShader.setFloat("material.shininess", 32.f);
            normalShader.setInt("material.texture_diffuse", 0);
            normalShader.setInt("material.texture_specular", 1);
            normalShader.setInt("numOfDirLights", 0);
            normalShader.setInt("numOfPointLights", 0);
            normalShader.setInt("numOfFlashLights", 0);
            coloredShader.use();
            coloredShader.setInt("numOfDirLights", 0);
            coloredShader.setInt("numOfPointLights", 0);
            coloredShader.setInt("numOfFlashLights", 0);
            firstCall = false;
        }
    }

    Shader& Shader::getNormalShader() {
        return normalShader;
    }

    Shader& Shader::getColoredShader() {
        return coloredShader;
    }

    Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }

    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

}