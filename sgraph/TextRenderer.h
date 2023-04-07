#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

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
#include <stack>
#include <iostream>
using namespace std;

namespace sgraph {
    /**
     * This visitor implements a text rendering of the scene graph
     * 
     */
    class TextRenderer: public SGNodeVisitor {
        public:
        /**
         * @brief Construct a new TextRenderer object
         * 
         * @param os the map of ObjectInstance objects
         * @param shaderLocations the shader locations for the program used to render
         */
        TextRenderer(map<string,util::ObjectInstance *>& os) 
            : objects(os) {
         
        }

        /**
         * @brief Recur to the children for drawing
         * 
         * @param groupNode 
         */
        void visitGroupNode(GroupNode *groupNode) {
            if(tabs==0){
                cout<<groupNode->getName()<<endl;
            }
            else{
                printTabs(groupNode->getName());
            }
            tabs++;
            for (int i=0;i<groupNode->getChildren().size();i=i+1) {
                
                groupNode->getChildren()[i]->accept(this);
                

            }
            tabs--;
        }

        /**
         * @brief Draw the instance for the leaf, after passing the 
         * modelview and color to the shader
         * 
         * @param leafNode 
         */
        void visitLeafNode(LeafNode *leafNode) {
            printTabs(leafNode->getName());
            //tabs--;
        }

        /**
         * @brief Multiply the transform to the modelview and recur to child
         * 
         * @param transformNode 
         */
        void visitTransformNode(TransformNode * transformNode) {
            printTabs(transformNode->getName());
            tabs++;
            if (transformNode->getChildren().size()>0) {
                
                transformNode->getChildren()[0]->accept(this);
                
            }
            tabs--;
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

        private:
    
        map<string,util::ObjectInstance *> objects;
        int tabs=0;
        void printTabs(string name){
            for(int i=0;i<tabs;i++){
                cout<<"  ";
            }
            cout<<"- "<<name<<endl;
            
            
        }

   };
}

#endif