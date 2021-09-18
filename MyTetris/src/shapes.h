/*
*   Class that stores all of the shape types, handles rotation of shapes, and stores the current shape and its rotation
*
*   @author Nikodem Kisielewski
*
*/

#pragma once

#include <string>

class Shapes
{
private:
    std::string allShapes[7];
public:
    int currentShape;
    Shapes();
    std::string getShape();
    std::string getShape(int shapeNum);
    int maxIndex();
    int maxIndex(int currentShape);
    int rotate(int x, int y, int currentRotation);
};