#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Config.h"
  
using namespace std;

class Texture {
    public:
        unsigned int id[1];
        unsigned int width, height;
    
        Texture(unsigned int width, unsigned int height);
};
  
#endif //TEXTURE_H