/*
*   Stores all the colors that will be used for drawing to the window.
*
*   @author Nikodem Kisielewski
*
*/

#pragma once

// Struct to store color codes
typedef struct Color
{
    int r;
    int g;
    int b;
    int a;
} Color;

// Color constructor
inline Color color(int r, int g, int b, int a)
{
    Color result;
    result.r = r;
    result.g = g;
    result.b = b;
    result.a = a;
    return result;
}

const Color white = color(255, 255, 255, 255);
const Color black = color(0, 0, 0, 255);
const Color border = color(128, 128, 128, 255);

const Color shapeColors[7] =
{
    // I color (cyan)
    color(0, 255, 255, 255),

    // Box color (yellow)
    color(255, 255, 0, 255),

    // Backwards L color (blue)
    color(0, 0, 255, 255),

    // L color (orange)
    color(255, 127, 0, 255),

    // Backwards Z color (green)
    color(0, 255, 0, 255),

    // Z color (red)
    color(255, 0, 0, 255),

    // T color (purple)
    color(128, 0, 128, 255),
};

const Color boardColors[10] =
{
    // Empty space
    color(160, 160, 160, 255),

    // I color (cyan)
    color(0, 255, 255, 255),

    // Box color (yellow)
    color(255, 255, 0, 255),

    // Backwards L color (blue)
    color(0, 0, 255, 255),

    // L color (orange)
    color(255, 127, 0, 255),

    // Backwards Z color (green)
    color(0, 255, 0, 255),

    // Z color (red)
    color(255, 0, 0, 255),

    // T color (purple)
    color(128, 0, 128, 255),

    // Border
    color(128, 128, 128, 255),

    // Line
    white
};