/*
*   Class that stores all of the shape types, handles rotation of shapes, and stores the current shape and its rotation
*
*   @author Nikodem Kisielewski
*
*/

#include "shapes.h"

// Default constructor
Shapes::Shapes() : currentShape(0)
{
    allShapes[0].append("....XXXX........");
    allShapes[1].append(".....XX..XX.....");
    allShapes[2].append("X..XXX...");
    allShapes[3].append("..XXXX...");
    allShapes[4].append(".XXXX....");
    allShapes[5].append("XX..XX...");
    allShapes[6].append(".X.XXX...");
}

// Returns the current shape as a string
std::string Shapes::getShape()
{
    return allShapes[currentShape];
}

// Returns a shape string based on it's index
std::string Shapes::getShape(int shapeNum)
{
    return allShapes[shapeNum];
}

// Returns the highest possible index of the current shape
int Shapes::maxIndex()
{
    if (currentShape == 0 || currentShape == 1)
        return 4;
    return 3;
}

// Returns the highest possible index of some shape specified
int Shapes::maxIndex(int currentShape)
{
    if (currentShape == 0 || currentShape == 1)
        return 4;
    return 3;
}

// Returns the correct string index based on the current rotation of a shape
int Shapes::rotate(int x, int y, int currentRotation)
{
    if (currentShape == 0 || currentShape == 1)
    {
        switch (currentRotation % 4)
        {
        case 0:
            return (y * 4) + x;
        case 1:
            return 12 + y - (x * 4);
        case 2:
            return 15 - (y * 4) - x;
        case 3:
            return 3 - y + (x * 4);
        }
    }
    else
    {
        switch (currentRotation % 4)
        {
        case 0:
            return (y * 3) + x;
        case 1:
            return 6 + y - (x * 3);
        case 2:
            return 8 - (y * 3) - x;
        case 3:
            return 2 - y + (x * 3);
        }
    }
    return -1;
}