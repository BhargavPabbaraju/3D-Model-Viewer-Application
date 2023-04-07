#include "Camera.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;


Camera::Camera() {

}

Camera::~Camera(){

}

void Camera::init(glm::vec3 pos,glm::vec3 target,glm::vec3 up) 
{
    this->pos = pos;
    this->target = target;
    this->up = up;
    xRot = 0.0f;
    yRot = 0.0f;
    zDistance = 0.0f;

    originalPos = pos;
    originalTarget = target;
    originalUp = up;
    moveSpeed = 5.0f;
    turnSpeed = 5.0f;
    
}

void Camera::reset(){
    this->pos = originalPos;
    this->target = originalTarget;
    this->up = originalUp;
    xRot = 0.0f;
    yRot = 0.0f;
    zDistance = 0;
}

glm::mat4 Camera::getCamera(){
    if (xRot==0 && yRot==0){
        return glm::lookAt(pos,target,up);
    }
   
    target.y=yRot;
    target.x=xRot;
    glm::vec3 forward = glm::normalize(pos-target);
    // glm::vec3 right = glm::normalize(glm::cross(up, forward));
    // up = glm::cross(forward, cameraRight);

    return glm::lookAt(pos,target,up)*glm::translate(glm::mat4(1.0),zDistance*forward);

 

    //return glm::lookAt(pos,target,up);
}

void Camera::move(int direction){
    zDistance +=direction*moveSpeed;
    
}


void Camera::turn(char direction){
    switch(direction){
        case 'U':
            yRot+=turnSpeed;
            break;
        case 'D':
            yRot-=turnSpeed;
            break;
        case 'L':
            xRot-=turnSpeed;
            break;
        case 'R':
            xRot+=turnSpeed;
            break;
        
    }
}

