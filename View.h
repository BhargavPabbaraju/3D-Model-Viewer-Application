#ifndef __VIEW_H__
#define __VIEW_H__

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <ShaderProgram.h>
#include "sgraph/SGNodeVisitor.h"
#include "ObjectInstance.h"
#include "PolygonMesh.h"
#include "VertexAttrib.h"
#include "Callbacks.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "sgraph/IScenegraph.h"

#include <stack>
using namespace std;


class View
{
    class LightLocation {
    public:
        int ambient,diffuse,specular,position,spotDirection,spotAngle;
        LightLocation()
        {
        ambient = diffuse = specular = position = -1;
        }

    };

public:
    View();
    ~View();
    void init(Callbacks* callbacks,Model& model);
    void display(sgraph::IScenegraph *scenegraph);
    void displayText(sgraph::IScenegraph *scenegraph);
    bool shouldWindowClose();
    void rotateModel(double xpos, double ypos);
    void startRotation(int action);
    void suspendRotation(int action);
    void resetView();
    void closeWindow();
    void setCamera(int key);
    void turnCamera3(char direction);
    void moveCamera3(int direction);

private: 

    GLFWwindow* window;
    util::ShaderProgram program;
    util::ShaderLocationsVault shaderLocations;
    map<string,util::ObjectInstance *> objects;
    glm::mat4 projection;
    stack<glm::mat4> modelview;
    sgraph::SGNodeVisitor *renderer;
    sgraph::SGNodeVisitor *textRenderer;
    Callbacks *callbacks;
    int frames;
    double time;
    float scale_x;
    bool isRotating;
    int start_x, start_y;
	int window_width,window_height;
    float x_rot_angle, y_rot_angle;
    int camera;
    Camera* camera1;
    Camera* camera2;
    Camera* camera3;
    void setCameraModelViews();
    vector<glm::vec4> getLightPositions(const glm::mat4& transformation);
    void initLights(Model& model);
    void initShaderVariables();
    //the lights in our scene
    vector<util::Light> lights;
    //the shader locations for all lights, for convenience
    vector<LightLocation> lightLocations;
    //either name of object, or world, or view
    vector<string> lightCoordinateSystems;

};

#endif