#pragma once
//------------------------------------------------------------------------------------
// file: utils.h
//
// authors:
// l.zheneudamon (primary author)
//		Added Rectangles intersecting 
//		Added Circles and Rectangles intersecting
//		Added CutString and Text Wrap functions
//		Added Animation Loader and unloader
//		Added Animator (Get animation image address)
// timo.duethorn
//		Added Random Number Generator
//		Added is Area Clicked / is Circle Clicked
//		Added are Circles intersecting
//
// brief:
// Contains declaration of functions in utils.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Checks if Mouse is within a Rectangle.
/// </summary>
/// <param name="area_center_x">rect x value</param>
/// <param name="area_center_y">rect y value</param>
/// <param name="area_width">rect width</param>
/// <param name="area_height">rect height</param>
/// <param name="click_x">mouse x</param>
/// <param name="click_y">mouse y</param>
/// <returns>1 if inside rect, else 0</returns>
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

/// <summary>
/// Checks if Mouse is within a Circle.
/// </summary>
/// <param name="circle_center_x">circle x value</param>
/// <param name="circle_center_y">circle y value</param>
/// <param name="diameter">circle diameter</param>
/// <param name="click_x">mouse x</param>
/// <param name="click_y">mouse y</param>
/// <returns>1 if inside circle, else 0</returns>
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

/// <summary>
/// Checks if two Circles are Intersecting.
/// </summary>
/// <param name="c1_x">circle 1 x value</param>
/// <param name="c1_y">circle 1 y value</param>
/// <param name="r1">circle 1 radius value</param>
/// <param name="c2_x">circle 2 x value</param>
/// <param name="c2_y">circle 2 y value</param>
/// <param name="r2">circle 2 radius value</param>
/// <returns>1 if circles are intersecting, else 0</returns>
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

/// <summary>
/// Checks if a Circle and a Rectangle are Intersecting.
/// </summary>
/// <param name="circle_x">circle x value</param>
/// <param name="circle_y">circle y value</param>
/// <param name="circle_diameter">circle radius value</param>
/// <param name="rect_x">rectangle x</param>
/// <param name="rect_y">rectangle y</param>
/// <param name="rect_width">rectangle width</param>
/// <param name="rect_height">rectangle height</param>
/// <returns>1 if circle and rect are intersecting, else 0</returns>
int IsCircleAndRectIntersecting(float circle_x, float circle_y, float circle_diameter, float rect_x, float rect_y, float rect_width, float rect_height);

/// <summary>
/// Checks if two Rectangles are Intersecting.
/// </summary>
/// <param name="r1_x">rectangle 1 x</param>
/// <param name="r1_y">rectangle 1 y</param>
/// <param name="r1_width">rectangle 1 width</param>
/// <param name="r1_height">rectangle 1 height</param>
/// <param name="r2_x">rectangle 2 x</param>
/// <param name="r2_y">rectangle 2 y</param>
/// <param name="r2_width">rectangle 2 width</param>
/// <param name="r2_height">rectangle 2 height</param>
/// <returns>1 if rects are intersecting, else 0</returns>
int AreRectanglesIntersecting(float r1_x, float r1_y, float r1_width, float r1_height, float r2_x, float r2_y, float r2_width, float r2_height);

/// <summary>
/// Generates a random number within a specified range.
/// </summary>
/// <param name="min"> Min value of the randomly generated number </param>
/// <param name="max"> Max value of the randomly generated number </param>
/// <returns> Returns the randomly generated number </returns>
int RandomNumber(int min, int max);

/// <summary>
/// Deletes characters from the string starting from the front.
/// </summary>
/// <param name="string"> The string to be updated </param>
/// <param name="size"> The max size of the string </param>
/// <param name="cut"> The amount of elements to remove </param>
void CutString(char string[], int size, int cut);

/// <summary>
/// Draws Text on the screen with wrapping based on max characters.
/// This function assumes that text is already predefined.
/// </summary>
/// <param name="text"> String to print on screen </param>
/// <param name="x"> X value to draw </param>
/// <param name="y"> Y value of first line to draw </param>
/// <param name="charactersPerLine"> Max number of characters in a line </param>
/// <param name="gapBetweenEachLine"> Space between each line </param>
void DrawTextWithWrap(char text[], float x, float y, int charactersPerLine, float gapBetweenEachLine);

/// <summary>
/// Loads Images using CP_Image_Load.
/// </summary>
/// <param name="imageArray"> Array holding the Images </param>
/// <param name="fileName"> Relative file name of the Images. "Player/player_idle_" </param>
/// <param name="numberOfFrames"> Number of frames the Image has </param>
void AnimationImageLoader(CP_Image imageArray[], char fileName[], int numberOfFrames);

/// <summary>
/// Frees Images using CP_Image_Free.
/// </summary>
/// <param name="imageArray"> Array holding the Images </param>
/// <param name="numberOfImage"> Number of Images </param>
void ImageUnloader(CP_Image imageArray[], int numberOfImage);

/// <summary>
/// Selects the correct frame in animation using game Timer.
/// </summary>
/// <param name="arrayImages"> Array holding the Images </param>
/// <param name="MAX_FRAME_IN_ANIMATION"> ENUM of max number of frames/images </param>
/// <param name="gameTimer"> Timer that increases per frame </param>
/// <returns> Address of frame Image </returns>
CP_Image* GetAnimationImageAddress(CP_Image arrayImages[], int MAX_FRAME_IN_ANIMATION, float gameTimer);
