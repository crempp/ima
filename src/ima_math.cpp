/*******************************************************************************
 * Name:      ima_math.cpp
 * Purpose:   Code for the image calcuations.
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-11-05
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 * Notes:
 ******************************************************************************/

#include <sys/time.h>
#include <wx/image.h>
#include <wx/log.h>

#include "ima_math.h"

#define MIRROR_X(u,v,x,y,w) ((u<w)||(v<w)) ? (u+abs(x)) : (u-abs(x))
#define MIRROR_Y(u,v,x,y,w) ((u<w)||(v<w)) ? (v+abs(y)) : (v-abs(y))

//------------------------------------------------------------------------------
//     is_color
//
// Takes a wxImage. Compares each channel (R,G,B). If they're all the same then
// it's a grayscale image (this is how wxImage stores gray images).
//------------------------------------------------------------------------------
bool is_color(wxImage image){
    unsigned char *data = image.GetData();
    for(int j = 0; j < image.GetHeight(); j++){
        for(int i = 0 ; i < image.GetWidth(); i++) {
            if ((*data != *(data+1)) || (*data != *(data+2))){
                wxLogMessage(_T("  NOT EQUAL"));
                return true;
            }
            data += 3;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
//     Average
//
// Takes an imaImage, window radius, and edge method. Averages values in the
// window around each pixel.
// TODO - The averaging kernel has rank 1 so it's seperable. Seperate.
//------------------------------------------------------------------------------

//-----Average_gray-------------------------------------------------------------
void Average_gray(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("Average_gray"));
        printf("Average\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int sum;
    unsigned char** red   = image->GetRed();
    unsigned char ** tmp_array = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp array
    tmp_array = imaalloc(im_w, im_h);

    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        // Averaging
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                sum = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        sum += red[i + k][j + l];
                    }
                }
                tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                //sum += red[i + abs(k)][j + abs(l)];
                                sum += red[MIRROR_X(i,j,k,l,window)][MIRROR_Y(i,j,k,l,window)];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array  [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array  [i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[MIRROR_X(i,j,k,l,window)][MIRROR_Y(i,j,k,l,window)];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[MIRROR_X(i,j,k,l,window)][MIRROR_Y(i,j,k,l,window)];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[MIRROR_X(i,j,k,l,window)][MIRROR_Y(i,j,k,l,window)];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum   = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum += red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_array[i][j] = sum / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }

        // Copy tmp_array to ima_array
        image->SetGray(tmp_array);
        // Clean up
        imafree(tmp_array, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  averaging took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-Average_gray"));
        printf("-Average_gray\n");fflush(stdout);
    #endif
}

//-----Average_color------------------------------------------------------------
void Average_color(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("Average_color"));
        printf("Average_color\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int sum_red, sum_green, sum_blue;
    unsigned char** red   = image->GetRed();
    unsigned char** green = image->GetGreen();
    unsigned char** blue  = image->GetBlue();
    unsigned char ** tmp_red = NULL;
    unsigned char ** tmp_green = NULL;
    unsigned char ** tmp_blue = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_red = imaalloc(im_w, im_h);
    tmp_green = imaalloc(im_w, im_h);
    tmp_blue  = imaalloc(im_w, im_h);

    if ((tmp_red == NULL) || tmp_blue == NULL || tmp_green == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        // Do the averaging
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                sum_red   = 0;
                sum_green = 0;
                sum_blue  = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        sum_red   += red   [i + k][j + l];
                        sum_green += green [i + k][j + l];
                        sum_blue  += blue  [i + k][j + l];
                    }
                }
                tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red[i + abs(k)][j + abs(l)];
                                sum_green += green[i + abs(k)][j + abs(l)];
                                sum_blue  += blue [i + abs(k)][j + abs(l)];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_green += green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_blue  += blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [i - abs(k)][j - abs(l)];
                                sum_green += green[i - abs(k)][j - abs(l)];
                                sum_blue  += blue [i - abs(k)][j - abs(l)];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_green += green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_blue  += blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_red[i][j] = red[i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [i + abs(k)][j + abs(l)];
                                sum_green += green[i + abs(k)][j + abs(l)];
                                sum_blue  += blue [i + abs(k)][j + abs(l)];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_green += green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_blue  += blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_red[i][j]   = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [i - abs(k)][j - abs(l)];
                                sum_green += green[i - abs(k)][j - abs(l)];
                                sum_blue  += blue [i - abs(k)][j - abs(l)];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_WRAP:
                        sum_red   = 0;
                        sum_green = 0;
                        sum_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                sum_red   += red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_green += green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                sum_blue  += blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        tmp_red  [i][j] = sum_red   / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_green[i][j] = sum_green / ((window * 2 + 1) * (window * 2 + 1));
                        tmp_blue [i][j] = sum_blue  / ((window * 2 + 1) * (window * 2 + 1));
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }

        // Copy temp arrays to image
        image->SetColor(tmp_red, tmp_green, tmp_blue);
        // Clean up
        imafree(tmp_red, im_w);
        imafree(tmp_green, im_w);
        imafree(tmp_blue, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  averaging took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-Average_color"));
        printf("-Average_color\n");fflush(stdout);
    #endif
}
//------------------------------------------------------------------------------
//     OrderStatistic
//
// Takes an imaImage, window radius, and edge method. Computes the
// median/max/min in the window around each pixel.
//------------------------------------------------------------------------------

//-----OS_Median_gray-----------------------------------------------------------
void OS_Median_gray(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Median_gray"));
        printf("OS_Median_gray\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int d = (window * 2) + 1;
    int list_size  = ((4 * window * window) + (4 * window) + 1);
    unsigned char** red   = image->GetRed();
    unsigned char ** tmp_array = NULL;
    int middle = (int)floor(list_size/2);

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_array = imaalloc(im_w, im_h);

    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        // Create array for window values
        int* list = (int*)calloc(list_size, sizeof(int));

        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        list[(d * (l + window)) + (k + window)] = red[i + k][j + l];
                    }
                }
                csort(list, list_size, 255, 0);
                tmp_array[i][j] = list[middle];
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[i + abs(k)][j + abs(l)];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[i - abs(k)][j - abs(l)];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[i + abs(k)][j + abs(l)];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[i - abs(k)][j - abs(l)];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list[(d * (l + window)) + (k + window)] = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list, list_size, 255, 0);
                        tmp_array[i][j] = list[middle];
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetGray(tmp_array);
        // Clean up
        imafree(tmp_array, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_median_gray took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Median_gray"));
        printf("-OS_Median_gray\n");fflush(stdout);
    #endif
}

//-----OS_Median_color----------------------------------------------------------
void OS_Median_color(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Median_color"));
        printf("OS_Median_color\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int d = (window * 2) + 1;
    int list_size  = ((4 * window * window) + (4 * window) + 1);
    unsigned char** red   = image->GetRed();
    unsigned char** green = image->GetGreen();
    unsigned char** blue  = image->GetBlue();
    unsigned char ** tmp_red   = NULL;
    unsigned char ** tmp_green = NULL;
    unsigned char ** tmp_blue  = NULL;
    int middle = (int)floor(list_size/2);

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_red   = imaalloc(im_w, im_h);
    tmp_green = imaalloc(im_w, im_h);
    tmp_blue  = imaalloc(im_w, im_h);

    if (tmp_red == NULL || tmp_green == NULL || tmp_blue == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        // Create array for window values
        int* list_red   = (int*)calloc(list_size, sizeof(int));
        int* list_green = (int*)calloc(list_size, sizeof(int));
        int* list_blue  = (int*)calloc(list_size, sizeof(int));

        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        list_red  [(d * (l + window)) + (k + window)] = red  [i + k][j + l];
                        list_green[(d * (l + window)) + (k + window)] = green[i + k][j + l];
                        list_blue [(d * (l + window)) + (k + window)] = blue [i + k][j + l];
                    }
                }
                csort(list_red  , list_size, 255, 0);
                csort(list_green, list_size, 255, 0);
                csort(list_blue , list_size, 255, 0);
                tmp_red  [i][j] = list_red  [middle];
                tmp_green[i][j] = list_green[middle];
                tmp_blue [i][j] = list_blue [middle];
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [i + abs(k)][j + abs(l)];
                                list_green[(d * (l + window)) + (k + window)] = green[i + abs(k)][j + abs(l)];
                                list_blue [(d * (l + window)) + (k + window)] = blue [i + abs(k)][j + abs(l)];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_green[(d * (l + window)) + (k + window)] = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_blue [(d * (l + window)) + (k + window)] = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_ORIG:
                        tmp_red[i][j] = red[i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [i - abs(k)][j - abs(l)];
                                list_green[(d * (l + window)) + (k + window)] = green[i - abs(k)][j - abs(l)];
                                list_blue [(d * (l + window)) + (k + window)] = blue [i - abs(k)][j - abs(l)];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_green[(d * (l + window)) + (k + window)] = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_blue [(d * (l + window)) + (k + window)] = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_ORIG:
                        tmp_red[i][j] = red[i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [i + abs(k)][j + abs(l)];
                                list_green[(d * (l + window)) + (k + window)] = green[i + abs(k)][j + abs(l)];
                                list_blue [(d * (l + window)) + (k + window)] = blue [i + abs(k)][j + abs(l)];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_green[(d * (l + window)) + (k + window)] = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_blue [(d * (l + window)) + (k + window)] = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_ORIG:
                        tmp_red[i][j] = red[i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [i - abs(k)][j - abs(l)];
                                list_green[(d * (l + window)) + (k + window)] = green[i - abs(k)][j - abs(l)];
                                list_blue [(d * (l + window)) + (k + window)] = blue [i - abs(k)][j - abs(l)];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_WRAP:
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                list_red  [(d * (l + window)) + (k + window)] = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_green[(d * (l + window)) + (k + window)] = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                                list_blue [(d * (l + window)) + (k + window)] = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h];
                            }
                        }
                        csort(list_red  , list_size, 255, 0);
                        csort(list_green, list_size, 255, 0);
                        csort(list_blue , list_size, 255, 0);
                        tmp_red  [i][j] = list_red  [middle];
                        tmp_green[i][j] = list_green[middle];
                        tmp_blue [i][j] = list_blue [middle];
                        break;
                    case EDGE_ORIG:
                        tmp_red[i][j] = red[i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red[i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red[i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue[i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetColor(tmp_red, tmp_green, tmp_blue);
        // Clean up
        imafree(tmp_red, im_w);
        imafree(tmp_green, im_w);
        imafree(tmp_blue, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_median_color took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Median_color"));
        printf("-OS_Median_color\n");fflush(stdout);
    #endif
}

//-----OS_Max_gray--------------------------------------------------------------
void OS_Max_gray(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Max_gray"));
        printf("OS_Max_gray\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int max;
    unsigned char** red   = image->GetRed();
    unsigned char ** tmp_array = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_array = imaalloc(im_w, im_h);

    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                max = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        max = red[i + k][j + l] > max ? red[i + k][j + l] : max;
                    }
                }
                tmp_array[i][j] = max;
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[i + abs(k)][j + abs(l)] > max ? red[i + abs(k)][j + abs(l)] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_WRAP:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[i - abs(k)][j - abs(l)] > max ? red[i - abs(k)][j - abs(l)] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_WRAP:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[i + abs(k)][j + abs(l)] > max ? red[i + abs(k)][j + abs(l)] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_WRAP:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[i - abs(k)][j - abs(l)] > max ? red[i - abs(k)][j - abs(l)] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_WRAP:
                        max = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max;
                            }
                        }
                        tmp_array[i][j] = max;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetGray(tmp_array);
        // Clean up
        imafree(tmp_array, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_max_gray took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Max_gray"));
        printf("-OS_Max_gray\n");fflush(stdout);
    #endif
}

//-----OS_Max_color-------------------------------------------------------------
void OS_Max_color(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Max_color"));
        printf("OS_Max_color\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int max_red, max_green, max_blue;
    unsigned char** red   = image->GetRed();
    unsigned char** green = image->GetGreen();
    unsigned char** blue  = image->GetBlue();
    unsigned char ** tmp_red   = NULL;
    unsigned char ** tmp_green = NULL;
    unsigned char ** tmp_blue  = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_red   = imaalloc(im_w, im_h);
    tmp_green = imaalloc(im_w, im_h);
    tmp_blue  = imaalloc(im_w, im_h);

    if (tmp_red == NULL || tmp_green == NULL || tmp_blue == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                max_red   = 0;
                max_green = 0;
                max_blue  = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        max_red   = red  [i + k][j + l] > max_red   ? red  [i + k][j + l] : max_red;
                        max_green = green[i + k][j + l] > max_green ? green[i + k][j + l] : max_green;
                        max_blue  = blue [i + k][j + l] > max_blue  ? blue [i + k][j + l] : max_blue;
                    }
                }
                tmp_red  [i][j] = max_red;
                tmp_green[i][j] = max_green;
                tmp_blue [i][j] = max_blue;
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [i + abs(k)][j + abs(l)] > max_red   ? red  [i + abs(k)][j + abs(l)] : max_red;
                                max_green = green[i + abs(k)][j + abs(l)] > max_green ? green[i + abs(k)][j + abs(l)] : max_green;
                                max_blue  = blue [i + abs(k)][j + abs(l)] > max_blue  ? blue [i + abs(k)][j + abs(l)] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_WRAP:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_red;
                                max_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_green;
                                max_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [i - abs(k)][j - abs(l)] > max_red   ? red  [i - abs(k)][j - abs(l)] : max_red;
                                max_green = green[i - abs(k)][j - abs(l)] > max_green ? green[i - abs(k)][j - abs(l)] : max_green;
                                max_blue  = blue [i - abs(k)][j - abs(l)] > max_blue  ? blue [i - abs(k)][j - abs(l)] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_WRAP:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_red;
                                max_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_green;
                                max_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [i + abs(k)][j + abs(l)] > max_red   ? red  [i + abs(k)][j + abs(l)] : max_red;
                                max_green = green[i + abs(k)][j + abs(l)] > max_green ? green[i + abs(k)][j + abs(l)] : max_green;
                                max_blue  = blue [i + abs(k)][j + abs(l)] > max_blue  ? blue [i + abs(k)][j + abs(l)] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_WRAP:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_red;
                                max_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_green;
                                max_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [i - abs(k)][j - abs(l)] > max_red   ? red  [i - abs(k)][j - abs(l)] : max_red;
                                max_green = green[i - abs(k)][j - abs(l)] > max_green ? green[i - abs(k)][j - abs(l)] : max_green;
                                max_blue  = blue [i - abs(k)][j - abs(l)] > max_blue  ? blue [i - abs(k)][j - abs(l)] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_WRAP:
                        max_red   = 0;
                        max_green = 0;
                        max_blue  = 0;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                max_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_red;
                                max_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_green;
                                max_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] > max_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : max_blue;
                            }
                        }
                        tmp_red  [i][j] = max_red;
                        tmp_green[i][j] = max_green;
                        tmp_blue [i][j] = max_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetColor(tmp_red, tmp_green, tmp_blue);
        // Clean up
        imafree(tmp_red, im_w);
        imafree(tmp_green, im_w);
        imafree(tmp_blue, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_max_color took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Max_color"));
        printf("-OS_Max_color\n");fflush(stdout);
    #endif
}

//-----OS_Min_gray--------------------------------------------------------------
void OS_Min_gray(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Min_gray"));
        printf("OS_Min_gray\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int min;
    unsigned char** red   = image->GetRed();
    unsigned char ** tmp_array = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_array = imaalloc(im_w, im_h);

    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                min = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        min = red[i + k][j + l] > min ? red[i + k][j + l] : min;
                    }
                }
                tmp_array[i][j] = min;
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[i + abs(k)][j + abs(l)] < min ? red[i + abs(k)][j + abs(l)] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_WRAP:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[i - abs(k)][j - abs(l)] < min ? red[i - abs(k)][j - abs(l)] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_WRAP:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[i + abs(k)][j + abs(l)] < min ? red[i + abs(k)][j + abs(l)] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_WRAP:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[i - abs(k)][j - abs(l)] < min ? red[i - abs(k)][j - abs(l)] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_WRAP:
                        min = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min = red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min ? red[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min;
                            }
                        }
                        tmp_array[i][j] = min;
                        break;
                    case EDGE_ORIG:
                        tmp_array[i][j] = red[i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_array[i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_array[i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetGray(tmp_array);
        // Clean up
        imafree(tmp_array, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_min_gray took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Min_gray"));
        printf("-OS_Min_gray\n");fflush(stdout);
    #endif
}

//-----OS_Min_color-------------------------------------------------------------
void OS_Min_color(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("OS_Min_color"));
        printf("OS_Min_color\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    int min_red, min_green, min_blue;
    unsigned char** red   = image->GetRed();
    unsigned char** green = image->GetGreen();
    unsigned char** blue  = image->GetBlue();
    unsigned char ** tmp_red   = NULL;
    unsigned char ** tmp_green = NULL;
    unsigned char ** tmp_blue  = NULL;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_red   = imaalloc(im_w, im_h);
    tmp_green = imaalloc(im_w, im_h);
    tmp_blue  = imaalloc(im_w, im_h);

    if (tmp_red == NULL || tmp_green == NULL || tmp_blue == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                min_red   = 255;
                min_green = 255;
                min_blue  = 255;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        min_red   = red  [i + k][j + l] < min_red   ? red  [i + k][j + l] : min_red;
                        min_green = green[i + k][j + l] < min_green ? green[i + k][j + l] : min_green;
                        min_blue  = blue [i + k][j + l] < min_blue  ? blue [i + k][j + l] : min_blue;
                    }
                }
                tmp_red  [i][j] = min_red;
                tmp_green[i][j] = min_green;
                tmp_blue [i][j] = min_blue;
            }
        }
        // Top edge (UL corner)
        for (int j = 0; j < window; j++){ // rows
            for (int i = 0; i < im_w - window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [i + abs(k)][j + abs(l)] < min_red   ? red  [i + abs(k)][j + abs(l)] : min_red;
                                min_green = green[i + abs(k)][j + abs(l)] < min_green ? green[i + abs(k)][j + abs(l)] : min_green;
                                min_blue  = blue [i + abs(k)][j + abs(l)] < min_blue  ? blue [i + abs(k)][j + abs(l)] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_WRAP:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_red;
                                min_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_green;
                                min_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Bottom edge (LR corner)
        for (int j = (im_h - window); j < im_h; j++){ // rows
            for (int i = 0 + window; i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [i - abs(k)][j - abs(l)] < min_red   ? red  [i - abs(k)][j - abs(l)] : min_red;
                                min_green = green[i - abs(k)][j - abs(l)] < min_green ? green[i - abs(k)][j - abs(l)] : min_green;
                                min_blue  = blue [i - abs(k)][j - abs(l)] < min_blue  ? blue [i - abs(k)][j - abs(l)] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_WRAP:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_red;
                                min_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_green;
                                min_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Left edge (LL corner)
        for (int j = window; j < im_h; j++){ // rows
            for (int i = 0; i < window; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [i + abs(k)][j + abs(l)] < min_red   ? red  [i + abs(k)][j + abs(l)] : min_red;
                                min_green = green[i + abs(k)][j + abs(l)] < min_green ? green[i + abs(k)][j + abs(l)] : min_green;
                                min_blue  = blue [i + abs(k)][j + abs(l)] < min_blue  ? blue [i + abs(k)][j + abs(l)] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_WRAP:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_red;
                                min_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_green;
                                min_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Right edge (UR corner)
        for (int j = 0; j < im_h; j++){ // rows
            for (int i = (im_w - window); i < im_w; i++){ // columns
                switch(edge){
                    case EDGE_MIRROR:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [i - abs(k)][j - abs(l)] < min_red   ? red  [i - abs(k)][j - abs(l)] : min_red;
                                min_green = green[i - abs(k)][j - abs(l)] < min_green ? green[i - abs(k)][j - abs(l)] : min_green;
                                min_blue  = blue [i - abs(k)][j - abs(l)] < min_blue  ? blue [i - abs(k)][j - abs(l)] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_WRAP:
                        min_red   = 255;
                        min_green = 255;
                        min_blue  = 255;
                        for (int l = -window; l <= window; l++){
                            for (int k = -window; k <= window; k++){
                                min_red   = red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_red   ? red  [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_red;
                                min_green = green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_green ? green[(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_green;
                                min_blue  = blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] < min_blue  ? blue [(i + (k+im_w))%im_w][(j + (l+im_h))%im_h] : min_blue;
                            }
                        }
                        tmp_red  [i][j] = min_red;
                        tmp_green[i][j] = min_green;
                        tmp_blue [i][j] = min_blue;
                        break;
                    case EDGE_ORIG:
                        tmp_red  [i][j] = red  [i][j];
                        tmp_green[i][j] = green[i][j];
                        tmp_blue [i][j] = blue [i][j];
                        break;
                    case EDGE_BLACK:
                        tmp_red  [i][j] = 0;
                        tmp_green[i][j] = 0;
                        tmp_blue [i][j] = 0;
                        break;
                    case EDGE_WHITE:
                        tmp_red  [i][j] = 255;
                        tmp_green[i][j] = 255;
                        tmp_blue [i][j] = 255;
                        break;
                }
            }
        }
        // Copy tmp_array to ima_array
        image->SetColor(tmp_red, tmp_green, tmp_blue);
        // Clean up
        imafree(tmp_red, im_w);
        imafree(tmp_green, im_w);
        imafree(tmp_blue, im_w);
    }

    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  os_min_color took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-OS_Min_color"));
        printf("-OS_Min_color\n");fflush(stdout);
    #endif
}

//------------------------------------------------------------------------------
//     Gaussian
//
// Takes an imaImage, window radius, and edge method. Computes the
// median/max/min in the window around each pixel.
//------------------------------------------------------------------------------
void Gaussian_gray(imaImage* image, int window, int edge){
    #ifdef DEBUG
        wxLogMessage(_T("Gaussian_gray"));
        printf("Gaussian_gray\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    unsigned char** red = image->GetRed();
    unsigned char ** tmp_array = NULL;
    float* kernel = NULL;
    float s, a, b;

    // Start timing
    #ifdef linux
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif

    // Create a temp arrays
    tmp_array = imaalloc(im_w, im_h);

    // Create kernel
    s = float(window) / 3;
    a = (1 / (sqrt(2 * PI) * s));
    b = 2 * s * s;
    //printf("    Allocating kernels\n");fflush(stdout);
    kernel = (float*)calloc(window*2+1, sizeof(float));
    //printf("    Creating Kernels\n");fflush(stdout);
    for (int i = -window; i <= window; i++){
        kernel[i + window] = a*pow(E,-((i*i)/b));
        //printf("  kernel=%f\n",kernel[i + window ][j + window]);fflush(stdout);
    }

    if (kernel == NULL)
        wxLogError(_T("Could not allocate memory for kernel"));
    else{


        // Convolve
        //printf("    Convolving tmp1\n");fflush(stdout);
        tmp_array = convolvle_f_orig(red, kernel, window, im_w, im_h);

        // Set gaussed image
        image->SetGray(tmp_array);

        // Clean up
        imafree(tmp_array, im_w);
        free(kernel);
    }



    // End timing
    #ifdef linux
        gettimeofday(&end, NULL);
        wxLogMessage(_T("  gaussian_gray took %ld usecs"),
            ((end.tv_sec * 1000000 + end.tv_usec) -
            (start.tv_sec * 1000000 + start.tv_usec)));
    #endif

    #ifdef DEBUG
        wxLogMessage(_T("-Gaussian_gray"));
        printf("-Gaussian_gray\n");fflush(stdout);
    #endif
}

/*
//-----Sharp--------------------------------------------------------------------
// TODO - Use alternative to abs() scaling
void imaImage::Sharp(int method){
    //wxLogMessage(_T("imaImage::Sharp"));

    int window = 1;
    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{
        switch(method){
            case 0:{ // 1st Derivative
                for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
                    for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                        tmp_array[i][j] = (ima_array[i+1][j]/2)+(ima_array[i][j+1]/2)-(ima_array[i-1][j]/2)-(ima_array[i][j-1]/2);
                    }
                }

                // Copy tmp_array to ima_array
                imacpy(ima_array, tmp_array);
                // Copy values to RGBRGB... format and set it to ima_image
                imaset(tmp_array);
                // Clean up
                imafree(tmp_array);
                break;
            }
            case 1:{ // 2nd Derivative
                for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
                    for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                        //tmp_array[i][j] = (4*ima_array[i][j])-ima_array[i+1][j]-ima_array[i][j+1]-ima_array[i-1][j]-ima_array[i][j-1];
                        tmp_array[i][j] = (9*ima_array[i][j])
                                        -ima_array[i-1][j-1]
                                        -ima_array[i][j-1]
                                        -ima_array[i+1][j-1]
                                        -ima_array[i-1][j]
                                        -ima_array[i+1][j]
                                        -ima_array[i-1][j+1]
                                        -ima_array[i][j+1]
                                        -ima_array[i+1][j+1];
                    }
                }

                // Copy tmp_array to ima_array
                imacpy(ima_array, tmp_array);
                // Copy values to RGBRGB... format and set it to ima_image
                imaset(tmp_array);
                // Clean up
                imafree(tmp_array);
                break;
            }
            default:
                wxLogError(_T("Don't know what to do"));
                imafree(tmp_array);
        }
    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  sharpening took %f seconds"), elapsed);

    //wxLogMessage(_T("-imaImage::Sharp"));
}

void imaImage::Roberts(long thr){
    wxLogMessage(_T("imaImage::Roberts"));

    int window = 1;
    int im_w = GetWidth();
    int im_h = GetHeight();
    int Gx, Gy, val;
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{
        for(int j = 0; j < im_h - window; j++){ // Loop through rows
            for (int i = 0; i < im_w - window; i++){ // Loop through this rows pixels
                Gx = ima_array[i][j] - ima_array[i + 1][j + 1];
                Gy = ima_array[i + 1][j] - ima_array[i][j + 1];
                val = abs(Gx) + abs(Gy);
                if (val >= thr)
                    tmp_array[i][j] = val;
            }
        }
        // Copy tmp_array to ima_array
        imacpy(ima_array, tmp_array);
        // Copy values to RGBRGB... format and set it to ima_image
        imaset(tmp_array);
        // Clean up
        imafree(tmp_array);

        // End timing
        end = clock();
        elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
        wxLogMessage(_T("  roberts took %f seconds"), elapsed);

        wxLogMessage(_T("-imaImage::Roberts"));
    }
}


void imaImage::Prewitt(long thr){
    //wxLogMessage(_T("imaImage::Prewitt"));

    int window = 1;
    int im_w = GetWidth();
    int im_h = GetHeight();
    int Gx, Gy, val;
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                Gx =   ima_array[i + 1][j - 1]
                     + ima_array[i + 1][j]
                     + ima_array[i + 1][j + 1]
                     - ima_array[i - 1][j - 1]
                     - ima_array[i - 1][j]
                     - ima_array[i - 1][j + 1];
                Gy =   ima_array[i - 1][j + 1]
                     + ima_array[i][j + 1]
                     + ima_array[i + 1][j + 1]
                     - ima_array[i - 1][j - 1]
                     - ima_array[i][j - 1]
                     - ima_array[i + 1][j - 1];
                val = abs(Gx) + abs(Gy);
                if (val >= thr)
                    tmp_array[i][j] = val;
            }
        }

        // Copy tmp_array to ima_array
        imacpy(ima_array, tmp_array);
        // Copy values to RGBRGB... format and set it to ima_image
        imaset(tmp_array);
        // Clean up
        imafree(tmp_array);

        // End timing
        end = clock();
        elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
        wxLogMessage(_T("  prewitt took %f seconds"), elapsed);
    }

    wxLogMessage(_T("-imaImage::Prewitt"));
}

void imaImage::Sobel(long thr){
    //wxLogMessage(_T("imaImage::Sobel"));

    int window = 1;
    int im_w = GetWidth();
    int im_h = GetHeight();
    int Gx, Gy, val;
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                Gx =       ima_array[i - 1][j - 1]
                     + 2 * ima_array[i - 1][j]
                     +     ima_array[i - 1][j + 1]
                     -     ima_array[i + 1][j - 1]
                     - 2 * ima_array[i + 1][j]
                     -     ima_array[i + 1][j + 1];
                Gy =       ima_array[i - 1][j - 1]
                     + 2 * ima_array[i][j - 1]
                     +     ima_array[i + 1][j - 1]
                     -     ima_array[i - 1][j + 1]
                     - 2 * ima_array[i][j + 1]
                     -     ima_array[i + 1][j + 1];
                val = abs(Gx) + abs(Gy);
                if (val >= thr)
                    tmp_array[i][j] = val;
            }
        }

        // Copy tmp_array to ima_array
        imacpy(ima_array, tmp_array);
        // Copy values to RGBRGB... format and set it to ima_image
        imaset(tmp_array);
        // Clean up
        imafree(tmp_array);

        // End timing
        end = clock();
        elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
        wxLogMessage(_T("  sobel took %f seconds"), elapsed);
    }

    //wxLogMessage(_T("-imaImage::Sobel"));
}

void imaImage::Scharr(long thr){
    //wxLogMessage(_T("imaImage::Scharr"));

    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{

    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  scharr took %f seconds"), elapsed);

    //wxLogMessage(_T("-imaImage::Scharr"));
}


void imaImage::Laplace(long thr){
    //wxLogMessage(_T("imaImage::Laplace"));

    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{

    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  laplace took %f seconds"), elapsed);

    //wxLogMessage(_T("-imaImage::Laplace"));
}


void imaImage::Kirsch(long thr){
    //wxLogMessage(_T("imaImage::Kirsch"));

    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else{

    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  kirsch took %f seconds"), elapsed);

    //wxLogMessage(_T("-imaImage::Kirsch"));
}

void imaImage::Canny(void){
    wxLogMessage(_T("imaImage::Canny"));

    int window = 2;
    int G_d = window * 2 + 1; // Convolution matrix diameter
    int s = 0.6;
    int im_w = GetWidth();
    int im_h = GetHeight();
    double a = (1 / (2 * PI * s * s));
    double b = 2 * s * s;
    double sum;
    int Gx,Gy, val;
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    // Create direction matrix
    int ** dir_array = imaalloc(im_w, im_h);
    // Create Gaussian Convolution Matrix
    double **G = (double**)calloc(G_d, sizeof(double *));
    for (int j = 0; j < G_d; j++){
        G[j] = (double*)calloc(G_d, sizeof(double));
    }

    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));
    else if (dir_array == NULL)
        wxLogError(_T("Could not allocate memory for dirs_array"));
    else{

        for (int j = -window; j <= window; j++){
            for (int i = -window; i <= window; i++){
                G[i+window][j+window] = double(a*pow(E,-((i*i+j*j)/b)));
            }
        }
        // Do the Gaussing
        for(int j = 0 + window; j < im_h - window; j++){ // Loop through rows
            for (int i = 0 + window; i < im_w - window; i++){ // Loop through this rows pixels
                sum = 0;
                for (int l = -window; l <= window; l++){ // Loop through windows rows
                    for (int k = -window; k <= window; k++){
                        sum += G[k + window][l + window] * ima_array[i + k][j +l];
                    }
                }
                tmp_array[i][j] = (int)sum;

            }
        }
        // Sobel Gradient and gradient directions
        double dir;
        for(int j = 1; j < im_h - 1; j++){ // Loop through rows
            for (int i = 1; i < im_w - 1; i++){ // Loop through this rows pixels
                Gx =       ima_array[i - 1][j - 1]
                     + 2 * ima_array[i - 1][j]
                     +     ima_array[i - 1][j + 1]
                     -     ima_array[i + 1][j - 1]
                     - 2 * ima_array[i + 1][j]
                     -     ima_array[i + 1][j + 1];
                Gy =       ima_array[i - 1][j - 1]
                     + 2 * ima_array[i][j - 1]
                     +     ima_array[i + 1][j - 1]
                     -     ima_array[i - 1][j + 1]
                     - 2 * ima_array[i][j + 1]
                     -     ima_array[i + 1][j + 1];
                val = abs(Gx) + abs(Gy);
                dir = atan(Gy/Gx);
                if ((dir <= 0.392699082) || (dir > 2.748893572))
                    dir_array[i][j] = 0;
                else if ((dir > 0.392699082) || (dir <= 1.178097245))
                    dir_array[i][j] = 45;
                else if ((dir > 1.178097245) || (dir < 1.963495408))
                    dir_array[i][j] = 90;
                else if ((dir > 1.963495408) || (dir < 2.748893572))
                    dir_array[i][j] = 135;
            }
        }


    }

    // Copy tmp_array to ima_array
    imacpy(ima_array, tmp_array);
    // Copy values to RGBRGB... format and set it to ima_image
    imaset(tmp_array);
    // Clean up
    imafree(tmp_array);
    imafree(dir_array);
    for (int i = 0; i < G_d; i++)
        free(G[i]);
    free(G);

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  kirsch took %f seconds"), elapsed);

    wxLogMessage(_T("-imaImage::Canny"));
}
*/

//-----Log----------------------------------------------------------------------
void Log_color(imaImage* image){
    #ifdef DEBUG
        wxLogMessage(_T("Log_color"));
        printf("Log_color\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    clock_t start, end;
    double elapsed;
    unsigned char** red   = image->GetRed();
    unsigned char** green = image->GetGreen();
    unsigned char** blue  = image->GetBlue();
    unsigned char ** tmp_red   = NULL;
    unsigned char ** tmp_green = NULL;
    unsigned char ** tmp_blue  = NULL;

    // Start timing
    start = clock();

    // Create a temp arrays
    tmp_red   = imaalloc(im_w, im_h);
    tmp_green = imaalloc(im_w, im_h);
    tmp_blue  = imaalloc(im_w, im_h);

    if (tmp_red == NULL || tmp_green == NULL || tmp_blue == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0; j < im_h; j++){ // Loop through rows
            for (int i = 0; i < im_w; i++){ // Loop through this rows pixels
                tmp_red  [i][j] = 45*log(red  [i][j]);
                tmp_green[i][j] = 45*log(green[i][j]);
                tmp_blue [i][j] = 45*log(blue [i][j]);
            }
        }
        // Copy tmp_array to ima_array
        image->SetColor(tmp_red, tmp_green, tmp_blue);
        // Clean up
        imafree(tmp_red, im_w);
        imafree(tmp_green, im_w);
        imafree(tmp_blue, im_w);
    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  log color took %f seconds"), elapsed);

    #ifdef DEBUG
        wxLogMessage(_T("-Log_color"));
        printf("-Log\n_color");fflush(stdout);
    #endif
}

void Log_gray(imaImage* image){
    #ifdef DEBUG
        wxLogMessage(_T("Log_gray"));
        printf("Log_gray\n");fflush(stdout);
    #endif

    int im_w = image->GetWidth();
    int im_h = image->GetHeight();
    clock_t start, end;
    double elapsed;
    unsigned char** red   = image->GetRed();
    unsigned char ** tmp_red   = NULL;

    // Start timing
    start = clock();

    // Create a temp arrays
    tmp_red   = imaalloc(im_w, im_h);

    if (tmp_red == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0; j < im_h; j++){ // Loop through rows
            for (int i = 0; i < im_w; i++){ // Loop through this rows pixels
                tmp_red  [i][j] = 45*log(red  [i][j]);
            }
        }
        // Copy tmp_array to ima_array
        image->SetGray(tmp_red);
        // Clean up
        imafree(tmp_red, im_w);
    }

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  log gray took %f seconds"), elapsed);

    #ifdef DEBUG
        wxLogMessage(_T("-Log_gray"));
        printf("-Log_gray\n");fflush(stdout);
    #endif
}

/*
//-----Add----------------------------------------------------------------------
void imaImage::Add(int** im_1, int** im_2){
    wxLogMessage(_T("imaImage::Add"));

    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));

    // Do the averaging
    for(int j = 0; j < im_h; j++){ // Loop through rows
        for (int i = 0; i < im_w; i++){ // Loop through this rows pixels
            tmp_array[i][j] = im_1[i][j] + im_2[i][j];
        }
    }

    // Copy tmp_array to ima_array
    imacpy(ima_array, tmp_array);
    // Copy values to RGBRGB... format and set it to ima_image
    imaset(tmp_array);
    // Clean up
    imafree(tmp_array);

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  Adding took %f seconds"), elapsed);

    wxLogMessage(_T("-imaImage::Add"));
}

//-----Subtract-----------------------------------------------------------------
void imaImage::Subtract(int** im_1, int** im_2){
    wxLogMessage(_T("imaImage::Subtract"));

    int im_w = GetWidth();
    int im_h = GetHeight();
    clock_t start, end;
    double elapsed;

    // Start timing
    start = clock();

    // Create a temp array
    int ** tmp_array = imaalloc(im_w, im_h);
    if (tmp_array == NULL)
        wxLogError(_T("Could not allocate memory for tmp_array"));

    // Do the averaging
    for(int j = 0; j < im_h; j++){ // Loop through rows
        for (int i = 0; i < im_w; i++){ // Loop through this rows pixels
            tmp_array[i][j] = abs(im_1[i][j] - im_2[i][j]);
        }
    }

    // Copy tmp_array to ima_array
    imacpy(ima_array, tmp_array);
    // Copy values to RGBRGB... format and set it to ima_image
    imaset(tmp_array);
    // Clean up
    imafree(tmp_array);

    // End timing
    end = clock();
    elapsed = ((double)(end-start)/CLOCKS_PER_SEC);
    wxLogMessage(_T("  subtracting took %f seconds"), elapsed);

    wxLogMessage(_T("-imaImage::Subtract"));
}

*/

//-----imaalloc-----------------------------------------------------------------
unsigned char** imaalloc(int im_w, int im_h){
    #ifdef DEBUG
        wxLogMessage(_T("imaalloc"));
        printf("imaalloc\n");fflush(stdout);
    #endif

    unsigned char** array = (unsigned char**)calloc(im_w, sizeof(unsigned char *));
    for (int j = 0; j < im_w; j++){
        array[j] = (unsigned char*)calloc(im_h, sizeof(unsigned char));
    }

    #ifdef DEBUG
        wxLogMessage(_T("-imaalloc"));
        printf("-imaalloc\n");fflush(stdout);
    #endif

    return array;
}

//-----imafree------------------------------------------------------------------
void imafree(unsigned char** array, int im_w){
    #ifdef DEBUG
        wxLogMessage(_T("imafree"));
        printf("imafree\n");fflush(stdout);
    #endif

    for (int i = 0; i < im_w; i++)
        free(array[i]);
    free(array);

    #ifdef DEBUG
        wxLogMessage(_T("-imafree"));
        printf("-imafree\n");fflush(stdout);
    #endif
}

//-----imacpy-------------------------------------------------------------------
int imacpy(unsigned char** dest, unsigned char** src, int im_w, int im_h){
    #ifdef DEBUG
        wxLogMessage(_T("imacpy"));
        printf("imacpy\n");fflush(stdout);
    #endif

    for (int j = 0; j < im_h; j++){
        //printf("  j = %d\n", j);fflush(stdout);
        for (int i = 0; i < im_w; i++){
            //printf("  i = %d dest=%d src=%d\n", i, dest[i][j], src[i][j]);fflush(stdout);
            dest[i][j] = src[i][j];
        }
    }
    return 0;

    #ifdef DEBUG
        wxLogMessage(_T("-imacpy"));
        printf("-imacpy\n");fflush(stdout);
    #endif
}

//-----imaset-------------------------------------------------------------------
/*
int imaset(unsigned char** src, int im_w, int im_h){
    unsigned char *set_array = (unsigned char*)malloc(datasize);
    int index;
    for (int j = 0; j < im_h; j++){
        for (int i = 0; i < im_w; i++){
            index = ((j * im_w) + i)*3; // flat RGB array index
            set_array[index]   = src[i][j];
            set_array[index+1] = src[i][j];
            set_array[index+2] = src[i][j];
        }
    }
    ima_image.SetData(set_array,true); // This takes care of freeing set_array
    return 0;
}
*/

//------------------------------------------------------------------------------
//     Convolve
//
//
//------------------------------------------------------------------------------
unsigned char** convolvle_f_orig(unsigned char** image, float* kernel, int window, int im_w, int im_h){
    #ifdef DEBUG
        wxLogMessage(_T("convolve"));
        printf("convolve\n");fflush(stdout);
    #endif

    float** tmp1_array   = NULL;
    unsigned char** tmp2_array   = NULL;
    float sum;

    // Create a temp arrays
    tmp1_array = (float**)calloc(im_w, sizeof(float*));
    for (int j = 0; j < im_w; j++){
        tmp1_array[j] = (float*)calloc(im_h, sizeof(float));
    }
    tmp2_array = imaalloc(im_w, im_h);
    for (int i = 0; i < im_w; i++){
        for (int j = 0; j < im_h; j++){
            tmp1_array[i][j] = float(image[i][j]);
            tmp2_array[i][j] = image[i][j];
        }
    }

    if (tmp1_array == NULL || tmp2_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = window; j < (im_h - window); j++){ // Loop through rows
            for (int i = window; i < (im_w - window); i++){ // Loop through this rows pixels
                sum = 0;
                for(int k = -window; k <= window; k++){
                    sum += (float)(image[i][j+k])*kernel[k+window];
                }
                tmp1_array[i][j] = sum;
            }
        }
        for (int i = window; i < (im_w - window); i++){ // Loop through this rows pixels
            for(int j = window; j < (im_h - window); j++){ // Loop through rows
                sum = 0;
                for(int k = -window; k <= window; k++){
                    sum += (float)(tmp1_array[i+k][j])*kernel[k+window];
                }
                tmp2_array[i][j] = (unsigned char)sum;
            }
        }
    }

    // Clean up
    for (int i = 0; i < im_w; i++)
        free(tmp1_array[i]);
    free(tmp1_array);

    #ifdef DEBUG
        wxLogMessage(_T("-convolve"));
        printf("-convolve\n");fflush(stdout);
    #endif

    return tmp2_array;
}

unsigned char** convolvle_f_mirror(unsigned char** image, float* kernel, int window, int im_w, int im_h){
    #ifdef DEBUG
        wxLogMessage(_T("convolve"));
        printf("convolve\n");fflush(stdout);
    #endif

    float** tmp1_array   = NULL;
    unsigned char** tmp2_array   = NULL;
    float sum;

    // Create a temp arrays
    tmp1_array = (float**)calloc(im_w, sizeof(float*));
    for (int j = 0; j < im_w; j++){
        tmp1_array[j] = (float*)calloc(im_h, sizeof(float));
    }
    tmp2_array = imaalloc(im_w, im_h);
    //for (int i = 0; i < im_w; i++){
    //    for (int j = 0; j < im_h; j++){
    //        tmp1_array[i][j] = float(image[i][j]);
    //        tmp2_array[i][j] = image[i][j];
    //    }
    //}

    if (tmp1_array == NULL || tmp2_array == NULL)
        wxLogError(_T("Could not allocate memory for temporary arrays"));
    else{
        for(int j = 0; j < (im_h); j++){ // Loop through rows
            for (int i = 0; i < im_w; i++){ // Loop through this rows pixels
                sum = 0;
                for(int k = -window; k <= window; k++){
                    sum += (float)(image[i][abs(j+k)])*kernel[k+window];

                    //sum_red   += red  [i - ak)][j - abs(l)];
                }
                tmp1_array[i][j] = sum;
            }
        }
        for (int i = window; i < (im_w - window); i++){ // Loop through this rows pixels
            for(int j = window; j < (im_h - window); j++){ // Loop through rows
                sum = 0;
                for(int k = -window; k <= window; k++){
                    sum += (float)(tmp1_array[i+k][j])*kernel[k+window];
                }
                tmp2_array[i][j] = (unsigned char)sum;
            }
        }
    }

    // Clean up
    for (int i = 0; i < im_w; i++)
        free(tmp1_array[i]);
    free(tmp1_array);

    #ifdef DEBUG
        wxLogMessage(_T("-convolve"));
        printf("-convolve\n");fflush(stdout);
    #endif

    return tmp2_array;
}

// Factorial
long int factorial(int n){
    if (n<=1)
        return(1);
    else
        n=n*factorial(n-1);
    return(n);
 }

// Counting sort
// From http://en.wikibooks.org/wiki/Algorithm_implementation/Sorting/Counting_sort)
// end is the last index + 1
void csort(int array[], const int end, const int max, const int min){
  int i;
  const int range = max-min+1;
  int count[range+1],
      scratch[end];

  for(i=0; i<range+1; i++)
    count[i] = 0;

  /* Set the value of count[i] to the number of
   * elements in array with value i+min-1. */
  for(i=0; i<end; i++) {
    int c = array[i]+1-min;
    count[c]++;
  }

  /* Update count[i] to be the number of
   * elements with value less than i+min. */
  for(i=1; i<range; i++)
    count[i] += count[i-1];

  /* Copy the elements of array into scratch in
   * stable sorted order. */
  for(i=(end-1); i>=0; i--) {
    int c = array[i]-min;
    int s = count[c];
    scratch[s] = array[i];
    /* Increment count so that the next element
     * with the same value as the current element
     * is placed into its own position in scratch. */
    count[c]++;
  }

  for(i=0; i<end; i++)
    array[i] = scratch[i];
}
