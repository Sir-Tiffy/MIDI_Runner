#pragma once

#ifdef WIN32
	#include <Windows.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <ctime>

#define PI 3.1415926535
#define print(msg) std::cout<<msg<<std::endl


using namespace std; //ha ha ha!

#include "Colours.h"
#include "CheckGLError.h"