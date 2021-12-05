#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  
using namespace std;

class Shader {
    public:
        unsigned int id;
    
        Shader(const char* vertexPath, const char* fragmentPath);
        
        void use();
        void setBool(const std::string &name, bool value) const;  
        void setInt(const std::string &name, int value) const;   
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, float xpos, float ypos);
};
  
#endif //SHADER_H