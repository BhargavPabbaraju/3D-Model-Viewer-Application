//! [code]

#include "Box.h"
#define GLM_FORCE_SWIZZLE
#include "BoxSet.h"
#include <fstream>
using namespace std;
#include "Material.h"

int main(int argc,char *argv[]) {
    BoxSet bset;
    ofstream outStream("output.txt");
    util::Material mat;
    mat.setAmbient(1,1,1);
    mat.setDiffuse(1,1,1);
    mat.setSpecular(1,1,1);
    mat.setShininess(10);
    //start with one box
    bset.add(Box(0,0,0,400,100,400));
    
    //Left Side
    bset.add(Box(0,100,0,300,200,100));
    bset.add(Box(0,300,0,200,150,100));
    bset.add(Box(50,450,0,100,50,100));
    bset.add(Box(75,500,25,50,25,50));

    //Left Side protrude
    bset.add(Box(140,120,-70,70,140,70));
    for (int j=0;j<3;j++) {
        bset.difference(Box(140,142+40*j,-70+10,10,20,60));
        bset.difference(Box(140+70-10,142+40*j,-70+10,10,20,60));
    }
    
    for (int j=0;j<3;j++) {
        bset.difference(Box(140+10,142+40*j,-70,50,20,10));    
    }
   
    //:Left side cutout
    bset.difference(Box(0,300,0,70,140,70));
    
    //Right Side
    bset.add(Box(0,100,100,400,200,300));
    bset.difference(Box(100,100,100,225,170,300));

    //Windows 
    for (int j=0;j<3;j++) {
        bset.difference(Box(0,140,120+100*j,20,40,60));
    }
    for (int j=0;j<3;j++) {
        bset.difference(Box(0,140+80,140+100*j,20,40,60));
    }
    
   
    
  

    //write it out to the above file. Using the one-argument version of 
    //this function means it will export the scene graph with each box
    //of a different random color. When we incorporate lighting into the 
    //scene you should regenerate your model using a specific material,
    //by using the two-argument version of this function
    bset.toScenegraph(outStream);

}