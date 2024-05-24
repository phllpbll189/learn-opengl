#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath)
        {
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;

            vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
            try 
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } 
            catch (std::ifstream::failure &e)
            {
                cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");


            fragment = glCreateShader(GL_FRAGMENT_SHADER); 
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            checkCompileErrors(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        };
        //use a shader
        void use()
        {
            glUseProgram(ID);
        };

        //set uniforms
        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
        };

        void setInt(const std::string &name, int value) const 
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        };

        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        };

        void setUniformDataf4(char* name, float* data) const
        {
            int vertexColorLocation = glGetUniformLocation(ID, name);
            glUniform4f(vertexColorLocation, data[0], data[1], data[2], data[3]);
        };

        private:
            void checkCompileErrors(unsigned int shader, string type)
            {
                int success;
                char infoLog[1024];
                if (type != "PROGRAM")
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "/n-------------------------------------------------" << endl;
                }
                else 
                {
                    glGetProgramiv(shader, GL_LINK_STATUS, &success);
                    if(!success){
                        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                        cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "/n" << infoLog << "/n------------------------------------------------" << endl; 
                    }
                };
            }
};
#endif