#include "Controller.h"
#include "sgraph/IScenegraph.h"
#include "sgraph/Scenegraph.h"
#include "sgraph/GroupNode.h"
#include "sgraph/LeafNode.h"
#include "sgraph/ScaleTransform.h"
#include "ObjImporter.h"
using namespace sgraph;
#include <iostream>
using namespace std;

#include "sgraph/ScenegraphExporter.h"
#include "sgraph/ScenegraphImporter.h"


Controller::Controller(Model& m,View& v) {
    model = m;
    view = v;

    initScenegraph();
}

void Controller::initScenegraph() {

     
    
    //read in the file of commands
    ifstream inFile("scenegraphmodels/box.txt");
    //ifstream inFile("tryout.txt");
    sgraph::ScenegraphImporter importer;
    

    IScenegraph *scenegraph = importer.parse(inFile);
    //scenegraph->setMeshes(meshes);
    model.setScenegraph(scenegraph);
    cout <<"Scenegraph made" << endl;   

}

Controller::~Controller()
{
    
}

void Controller::run()
{
    // sgraph::IScenegraph * scenegraph = model.getScenegraph();
    // map<string,util::PolygonMesh<VertexAttrib> > meshes = scenegraph->getMeshes();
    

    view.init(this,model);
    //view.displayText(scenegraph);
    while (!view.shouldWindowClose()) {
        view.display(model.getScenegraph());
    }
    view.closeWindow();
    exit(EXIT_SUCCESS);
}

util::PolygonMesh<VertexAttrib> Controller::getFrustumMesh(glm::mat4 projectionMatrix , glm::mat4 viewMatrix){
    return model.setFrustumMesh(projectionMatrix,viewMatrix);
}

void Controller::onkey(int key, int scancode, int action, int mods)
{
    //cout << (char)key << " pressed" << endl;
    switch(key){
        case GLFW_KEY_R:
            view.resetView();
            break;
        case GLFW_KEY_1:
            view.setCamera(1);
            break;
        case GLFW_KEY_2:
            view.setCamera(2);
            break;
        case GLFW_KEY_3:
            view.setCamera(3);
            break;
        case GLFW_KEY_EQUAL:
            if((mods & GLFW_MOD_SHIFT!=0)){
                view.moveCamera3(1);
            }
            break;
        case GLFW_KEY_MINUS:
             view.moveCamera3(-1);
             break;
        case GLFW_KEY_UP:
            view.turnCamera3('U');
            break;
        case GLFW_KEY_DOWN:
            view.turnCamera3('D');
            break;
        case GLFW_KEY_LEFT:
            view.turnCamera3('L');
            break;
        case GLFW_KEY_RIGHT:
            view.turnCamera3('R');
            break;
    }
    
}

void Controller::mouseClick(int button, int action, int mods) {
    // cout << (char)button << "pressed" << endl;
    if(action == GLFW_PRESS) {
        view.startRotation(action);
    } else if(action == GLFW_RELEASE) {
        view.suspendRotation(action);
    }
}

void Controller::mouseMoves(double xpos, double ypos) {
    // cout << "X: " << xpos << "Y: " << ypos << endl;
    view.rotateModel(xpos,ypos);
}

void Controller::reshape(int width, int height) 
{
    cout <<"Window reshaped to width=" << width << " and height=" << height << endl;
    glViewport(0, 0, width, height);
}

void Controller::dispose()
{
    view.closeWindow();
}

void Controller::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}