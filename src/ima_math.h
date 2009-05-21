/*******************************************************************************
 * Name:      ima_math.h
 * Purpose:   Header for the image calcuations.
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-11-05
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 * Notes:
 ******************************************************************************/

#ifndef IMA_MATH_H_INCLUDED
#define IMA_MATH_H_INCLUDED

#include "image.h"

bool is_color(wxImage image);

void Average_gray   (imaImage* image, int window, int edge);
void Average_color  (imaImage* image, int window, int edge);
void OS_Median_gray (imaImage* image, int window, int edge);
void OS_Median_color(imaImage* image, int window, int edge);
void OS_Max_gray    (imaImage* image, int window, int edge);
void OS_Max_color   (imaImage* image, int window, int edge);
void OS_Min_gray    (imaImage* image, int window, int edge);
void OS_Min_color   (imaImage* image, int window, int edge);
void Gaussian_gray  (imaImage* image, int window, int edge);
void Gaussian_color (imaImage* image, int window, int edge);

void Sharp(int method);
void Roberts(long thr);
void Prewitt(long thr);
void Sobel(long thr);
void Scharr(long thr);
void Laplace(long thr);
void Kirsch(long thr);
void Canny(void);

void Log_gray     (imaImage* image);
void Log_color    (imaImage* image);
void Add_gray     (imaImage* image_1, imaImage* image_2);
void Add_color    (imaImage* image_1, imaImage* image_2);
void Subtract_gray(imaImage* image_1, imaImage* image_2);
void Subtract_gray(imaImage* image_1, imaImage* image_2);
void And_gray     (imaImage* image_1, imaImage* image_2);
void And_color    (imaImage* image_1, imaImage* image_2);
void Or_gray      (imaImage* image_1, imaImage* image_2);
void Or_color     (imaImage* image_1, imaImage* image_2);

// array functions
unsigned char** imaalloc(int im_w, int im_h);
void imafree(unsigned char** array, int im_w);
int imacpy(unsigned char** dest, unsigned char**src, int im_w, int im_h);
int imaset(unsigned char** src);
void quicksort(int *array, int left, int right);

// Helper functions
unsigned char** convolvle_f_orig(unsigned char** image, float* kernel, int window, int im_w, int im_h);
unsigned char** convolvle_f_mirror(unsigned char** image, float* kernel, int window, int im_w, int im_h);
long int factorial(int n);
void csort(int array[], const int end, const int max, const int min);

#endif // IMA_MATH_H_INCLUDED
