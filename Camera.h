#ifndef __CAMERA_H__
#define __CAMERA_H__


#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexAttrib.h"


#include <stack>
using namespace std;


class Camera
{
public:
    Camera();
    ~Camera();
    void init(glm::vec3 pos,glm::vec3 target,glm::vec3 up);
    void reset();
    void turn(char direction);
    void move(int direction);
    glm::mat4 getCamera();

private: 

   glm::vec3 pos;
   glm::vec3 target;
   glm::vec3 up;
   float xRot;
   float yRot;
   float zDistance;
   glm::vec3 originalPos;
   glm::vec3 originalTarget;
   glm::vec3 originalUp;
   float turnSpeed;
   float moveSpeed;
};

#endif