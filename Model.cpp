#include "Model.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model() {

}

Model::~Model() {
    if (scenegraph) {
        delete scenegraph;
    }
}



sgraph::IScenegraph *Model::getScenegraph() {
    return this->scenegraph;
}

util::PolygonMesh<VertexAttrib> Model::setFrustumMesh(glm::mat4 projectionMatrix , glm::mat4 viewMatrix) {
     vector<glm::vec4> positions;


   
    positions.push_back(glm::vec4(-1, -1, -1, 1));
    positions.push_back(glm::vec4(1, -1, -1, 1));
    positions.push_back(glm::vec4(-1, 1, -1, 1));
    positions.push_back(glm::vec4(1, 1, -1, 1));
    positions.push_back(glm::vec4(-1, -1, 1, 1));
    positions.push_back(glm::vec4(1, -1, 1, 1));
    positions.push_back(glm::vec4(-1, 1, 1, 1));
    positions.push_back(glm::vec4(1, 1, 1, 1));
    positions.push_back(glm::vec4(0,0,-20,1));
    positions.push_back(glm::vec4(0,0,1.5,1));


    for (int i = 0; i < 10; i++) {
        positions[i] =    glm::inverse(projectionMatrix * viewMatrix)  * positions[i];
        positions[i] /= positions[i].w;
    }



    

    vector<VertexAttrib> vertexData;
     for (unsigned int i=0;i<positions.size();i++) {
        VertexAttrib v;
        vector<float> data;
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        data.push_back(positions[i].w);
        v.setData("position",data);
        vertexData.push_back(v);
    }
    vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(0);

    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(5);
    indices.push_back(7);
    indices.push_back(7);
    indices.push_back(6);
    indices.push_back(6);
    indices.push_back(4);

    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(7);

    indices.push_back(8);
    indices.push_back(9);

    

   
    util::PolygonMesh<VertexAttrib> mesh;
    mesh.setVertexData(vertexData);
    mesh.setPrimitives(indices);
    mesh.setPrimitiveType(GL_LINES);         
    mesh.setPrimitiveSize(2);  
    return mesh;
}

void Model::setScenegraph(sgraph::IScenegraph *scenegraph) {
    this->scenegraph = scenegraph;

   

}



