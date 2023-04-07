#ifndef _LIGHTSGETTER_H_
#define _LIGHTSGETTER_H_

#include "SGNodeVisitor.h"
#include "GroupNode.h"
#include "LeafNode.h"
#include "TransformNode.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "TranslateTransform.h"
#include <ShaderProgram.h>
#include <ShaderLocationsVault.h>
#include "ObjectInstance.h"
#include "Light.h"
#include <stack>
#include <iostream>
using namespace std;

namespace sgraph {
    /**
     * This visitor implements a text rendering of the scene graph
     * 
     */
    class LightsGetter: public SGNodeVisitor {
        public:
       
       LightsGetter(stack<glm::mat4>& mv) 
            : modelview(mv)
         {
            
           
        }

        /**
         * @brief Recur to the children for drawing
         * 
         * @param groupNode 
         */
        void visitGroupNode(GroupNode *groupNode) {
            processTempLights(groupNode->getLights());
            
            for (int i=0;i<groupNode->getChildren().size();i=i+1) {
                
                groupNode->getChildren()[i]->accept(this);
                

            }
            
        }

        /**
         * @brief Draw the instance for the leaf, after passing the 
         * modelview and color to the shader
         * 
         * @param leafNode 
         */
        void visitLeafNode(LeafNode *leafNode) {
           
            processTempLights(leafNode->getLights());
            

        }

        /**
         * @brief Multiply the transform to the modelview and recur to child
         * 
         * @param transformNode 
         */
        void visitTransformNode(TransformNode * transformNode) {
            processTempLights(transformNode->getLights());

            modelview.push(modelview.top());
            modelview.top() = modelview.top() * transformNode->getTransform();
            if (transformNode->getChildren().size()>0) {
                transformNode->getChildren()[0]->accept(this);
            }
            modelview.pop();
        
        }

        /**
         * @brief For this visitor, only the transformation matrix is required.
         * Thus there is nothing special to be done for each type of transformation.
         * We delegate to visitTransformNode above
         * 
         * @param scaleNode 
         */
        void visitScaleTransform(ScaleTransform *scaleNode) {
            visitTransformNode(scaleNode);
        }

        /**
         * @brief For this visitor, only the transformation matrix is required.
         * Thus there is nothing special to be done for each type of transformation.
         * We delegate to visitTransformNode above
         * 
         * @param translateNode 
         */
        void visitTranslateTransform(TranslateTransform *translateNode) {
            visitTransformNode(translateNode);
        }

        void visitRotateTransform(RotateTransform *rotateNode) {
            visitTransformNode(rotateNode);
        }

        vector<util::Light> getLights(){
            return lights;
        }

        private:
        void processTempLights(vector<util::Light> tempLights){
            for(int i=0;i<tempLights.size();i++){
                tempLights[i].setPosition(modelview.top() * tempLights[i].getPosition());
                glm::vec3 direction = modelview.top() * tempLights[i].getSpotDirection();
                tempLights[i].setSpotDirection(direction.x,direction.y,direction.z);
                lights.push_back(tempLights[i]);
            }
        }
        
        vector<util::Light> lights;
        stack<glm::mat4>& modelview;    
        

   };
}

#endif