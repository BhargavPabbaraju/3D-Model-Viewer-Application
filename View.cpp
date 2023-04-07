#include "View.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "sgraph/GLScenegraphRenderer.h"
#include "sgraph/LightsGetter.h"
#include "sgraph/TextRenderer.h"
#include "VertexAttrib.h"
#include "Camera.h"


View::View() {

}

View::~View(){

}

void View::init(Callbacks *callbacks,Model& model) 
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->callbacks = callbacks;
    window = glfwCreateWindow(800, 800, "Assignment 4", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
     glfwSetWindowUserPointer(window, (void *)callbacks);

    //using C++ functions as callbacks to a C-style library
    glfwSetKeyCallback(window, 
    [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->onkey(key,scancode,action,mods);
    });

    glfwSetWindowSizeCallback(window, 
    [](GLFWwindow* window, int width,int height)
    {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->reshape(width,height);
    });

    glfwSetMouseButtonCallback(window,
    [](GLFWwindow* window, int button, int action, int mods) {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->mouseClick(button, action, mods);
    });

    glfwSetCursorPosCallback(window,
    [](GLFWwindow* window, double xpos, double ypos) {
        reinterpret_cast<Callbacks*>(glfwGetWindowUserPointer(window))->mouseMoves(xpos, ypos);
    });

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // create the shader program
    program.createProgram(string("shaders/phong-multiple.vert"),
                          string("shaders/phong-multiple.frag"));
    // assuming it got created, get all the shader variables that it uses
    // so we can initialize them at some point
    // enable the shader program
    program.enable();
    shaderLocations = program.getAllShaderVariables();

    
    /* In the mesh, we have some attributes for each vertex. In the shader
     * we have variables for each vertex attribute. We have to provide a mapping
     * between attribute name in the mesh and corresponding shader variable
     name.
     *
     * This will allow us to use PolygonMesh with any shader program, without
     * assuming that the attribute names in the mesh and the names of
     * shader variables will be the same.

       We create such a shader variable -> vertex attribute mapping now
     */
    map<string, string> shaderVarsToVertexAttribs;

    shaderVarsToVertexAttribs["vPosition"] = "position";
    shaderVarsToVertexAttribs["vNormal"] = "normal";

    map<string,util::PolygonMesh<VertexAttrib> > meshes = model.getScenegraph()->getMeshes();
    
    for (typename map<string,util::PolygonMesh<VertexAttrib> >::iterator it=meshes.begin();
           it!=meshes.end();
           it++) {
        util::ObjectInstance * obj = new util::ObjectInstance(it->first);
        obj->initPolygonMesh(shaderLocations,shaderVarsToVertexAttribs,it->second);
        objects[it->first] = obj;
    }

    

    
    glfwGetFramebufferSize(window,&window_width,&window_height);

    //prepare the projection matrix for perspective projection
	projection = glm::perspective(glm::radians(60.0f),(float)window_width/window_height,0.1f,10000.0f);
    glViewport(0, 0, window_width,window_height);

    frames = 0;
    time = glfwGetTime();
    scale_x = 1.0f;
    isRotating = false;
    renderer = new sgraph::GLScenegraphRenderer(modelview,objects,shaderLocations);
    textRenderer = new sgraph::TextRenderer(objects);
    start_x = 0;
    start_y = 0;
    x_rot_angle = 0;
    y_rot_angle = 0;
    camera = 1;

    camera1 = new Camera();
    //camera1->init(glm::vec3(0.0f,680.0f,500.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    camera1->init(glm::vec3(0.0f,0.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    

    camera2 = new Camera();
    camera2->init(glm::vec3(0.0f,600.0f,300.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

    
    camera3 = new Camera();
    camera3->init(glm::vec3(0.0f,50.0f,250.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    
    
    initLights(model);
    initShaderVariables();
    
}

void View::setCamera(int key){
    camera = key;
}

void View::moveCamera3(int direction){
    camera3->move(direction);
}


void View::turnCamera3(char direction){
    camera3->turn(direction); 
}

void View::initShaderVariables() {
  //get input variables that need to be given to the shader program
  for (int i = 0; i < lights.size(); i++)
    {
      LightLocation ll;
      stringstream name;

      name << "light[" << i << "]";
      ll.ambient = shaderLocations.getLocation(name.str() + "" +".ambient");
      ll.diffuse = shaderLocations.getLocation(name.str() + ".diffuse");
      ll.specular = shaderLocations.getLocation(name.str() + ".specular");
      ll.position = shaderLocations.getLocation(name.str() + ".position");
      ll.spotDirection = shaderLocations.getLocation(name.str() + ".spotDirection");
      ll.spotAngle = shaderLocations.getLocation(name.str() + ".spotAngle");
      lightLocations.push_back(ll);
    }
}

void View::initLights(Model& model) {

    modelview.push(glm::mat4(1.0));
    sgraph::LightsGetter* lightGetter = new sgraph::LightsGetter(modelview);
    displayText(model.getScenegraph());
    model.getScenegraph()->getRoot()->accept(lightGetter);
    
    lights = lightGetter->getLights();
    for(util::Light light:lights){
        lightCoordinateSystems.push_back("world");
    }

   
}


void View::setCameraModelViews(){
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f),(float)window_width/window_height,20.0f,50.0f);
  
    
    glm::mat4 camera2_rot = camera2->getCamera() * glm::rotate(glm::mat4(1.0),(float)glfwGetTime(),glm::vec3(0,1,0));
    
    switch(camera){
        case 1:
            modelview.top() = modelview.top() * camera1->getCamera();
            break;
        case 2:
            modelview.top() = modelview.top() *camera2_rot;
            break;
        case 3:
            modelview.top() = modelview.top() *camera3->getCamera();
    }

    map<string, string> shaderVarsToVertexAttribs;

    shaderVarsToVertexAttribs["vPosition"] = "position";
    
    util::PolygonMesh<VertexAttrib> mesh = callbacks->getFrustumMesh(projectionMatrix,camera1->getCamera());
    util::ObjectInstance * obj = new util::ObjectInstance("frustum1");
    obj->initPolygonMesh(shaderLocations,shaderVarsToVertexAttribs,mesh);
    objects["frustum1"] = obj;

    mesh = callbacks->getFrustumMesh(projectionMatrix,camera2_rot);
    obj = new util::ObjectInstance("frustum2");
    obj->initPolygonMesh(shaderLocations,shaderVarsToVertexAttribs,mesh);
    objects["frustum2"] = obj;

    mesh = callbacks->getFrustumMesh(projectionMatrix,camera3->getCamera());
    obj = new util::ObjectInstance("frustum3");
    obj->initPolygonMesh(shaderLocations,shaderVarsToVertexAttribs,mesh);
    objects["frustum3"] = obj;





}

void View::display(sgraph::IScenegraph *scenegraph) {
    
    program.enable();
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    

    modelview.push(glm::mat4(1.0));

   
    
                        
    setCameraModelViews();
    
        modelview.top() = modelview.top() 
                        * glm::rotate(glm::mat4(1.0),glm::radians(y_rot_angle),glm::vec3(0, 1, 0))
                        * glm::rotate(glm::mat4(1.0),glm::radians(x_rot_angle),glm::vec3(1, 0, 0));
                        
    
    
    
    //modelview.push(glm::mat4(1.0));
    glUniformMatrix4fv(shaderLocations.getLocation("modelview"), 1, GL_FALSE, glm::value_ptr(modelview.top()));
    // glUniform4fv(shaderLocations.getLocation("vColor"),1,glm::value_ptr(glm::vec4(0,1,0,1)));
    // objects["frustum1"]->draw();
    // glUniform4fv(shaderLocations.getLocation("vColor"),1,glm::value_ptr(glm::vec4(1,0,1,1)));
    // objects["frustum2"]->draw();
    //  glUniform4fv(shaderLocations.getLocation("vColor"),1,glm::value_ptr(glm::vec4(0,1,1,1)));
    // objects["frustum3"]->draw();
    

    //send projection matrix to GPU    
    glUniformMatrix4fv(shaderLocations.getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    for (int i = 0; i < lights.size(); i++) {
        
        glm::vec4 pos = lights[i].getPosition();
        glm::vec4 dir = lights[i].getSpotDirection();
        
        glm::mat4 lightTransformation;
        
        
        if (lightCoordinateSystems[i]=="world") {
            
            lightTransformation = modelview.top();
            
            
        }
        else if (lightCoordinateSystems[i]=="view") {
            lightTransformation = glm::mat4(1.0);
        }
        // else {
        //     lightTransformation = modelview.top() * model.getTransform(lightCoordinateSystems[i]);
        // }

        
        pos = lightTransformation * pos;
        dir = lightTransformation * dir;
        printf("%f %f %f\n",pos.x,pos.y,pos.z);
        glUniform4fv(lightLocations[i].position, 1, glm::value_ptr(pos));
        glUniform3fv(lightLocations[i].spotDirection, 1,glm::value_ptr(dir));
        
    }

    

    //pass light color properties to shader
    glUniform1i(shaderLocations.getLocation("numLights"),lights.size());
    
    
    //pass light colors to the shader
    for (int i = 0; i < lights.size(); i++) {
        glUniform3fv(lightLocations[i].ambient, 1, glm::value_ptr(lights[i].getAmbient()));
        glUniform3fv(lightLocations[i].diffuse, 1, glm::value_ptr(lights[i].getDiffuse()));
        glUniform3fv(lightLocations[i].specular, 1,glm::value_ptr(lights[i].getSpecular()));
       
        glUniform1f(lightLocations[i].spotAngle, glm::cos(glm::radians(lights[i].getSpotCutoff())));
    }



    

    //draw scene graph here
    scenegraph->getRoot()->accept(renderer);


    

    
    


    
    modelview.pop();
    glFlush();
    program.disable();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    frames++;
    double currenttime = glfwGetTime();
    if ((currenttime-time)>1.0) {
        // printf("Framerate: %2.0f\r",frames/(currenttime-time));
        frames = 0;
        time = currenttime;
    }
    

}

void View::displayText(sgraph::IScenegraph *scenegraph) {

    scenegraph->getRoot()->accept(textRenderer);
    //std::cout<<"ran only once"<<endl;
}

void View::startRotation(int action) {
    isRotating = true;
    double initialXPos, initialYPos;
    glfwGetCursorPos(window, &initialXPos, &initialYPos);
    start_x = initialXPos;
    start_y = initialYPos;
}

void View::suspendRotation(int action) {
    isRotating = false;
}

void View::rotateModel(double xpos, double ypos) {
    if(isRotating) {
        x_rot_angle = x_rot_angle + (ypos - start_y);
        y_rot_angle = y_rot_angle + (xpos - start_x);
        start_x = xpos;
        start_y = ypos;
    }
}

void View::resetView() {
    start_x = 0;
    start_y = 0;
    x_rot_angle = 0;
    y_rot_angle = 0;
    camera3->reset();
}

bool View::shouldWindowClose() {
    return glfwWindowShouldClose(window);
}


void View::closeWindow() {
    for (map<string,util::ObjectInstance *>::iterator it=objects.begin();
           it!=objects.end();
           it++) {
          it->second->cleanup();
          delete it->second;
    } 
    glfwDestroyWindow(window);

    glfwTerminate();
}