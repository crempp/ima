/*******************************************************************************
 * Name:      image.cpp
 * Purpose:   Implements the image class. Stores working image data
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 * Notes:     For grayscale images wxImage has three copies of the image in the
 *            RGBRGBRGB... array.
 ******************************************************************************/

#include <wx/log.h>
//#include "globals.h"
//#include "imaApp.h"
#include "image.h"
#include "ima_math.h"
//#include "mainframe.h"

//------------------------------------------------------------------------------
// imaImage
//------------------------------------------------------------------------------

//-----Constructor--------------------------------------------------------------
imaImage::imaImage(wxImage image){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::imaImage"));
        printf("imaImage::imaImage\n");fflush(stdout);
    #endif

    // Create image
    ima_image = image.Copy();

    // Set properties
    is_color = true;
    name     = wxString();
    type     = wxString();
    ima_w    = ima_image.GetWidth();
    ima_h    = ima_image.GetHeight();

    // Create data arrays

    // Red (gray)
    ima_red = imaalloc(ima_w, ima_h);
    if (ima_red == NULL)
        wxLogError(_T("Could not allocate memory for ima_red"));
    // Fill red array
    for (unsigned int j = 0; j < ima_h; j++){
        for (unsigned int i = 0; i < ima_w; i++){
            ima_red[i][j] = ima_image.GetRed(i,j);
        }
    }

    // We only need the following if the image is color
    if (is_color){
        // Green
        ima_green = imaalloc(ima_w, ima_h);
        if (ima_green == NULL)
            wxLogError(_T("Could not allocate memory for ima_green"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_green[i][j] = ima_image.GetGreen(i,j);
            }
        }
        // Blue
        ima_blue = imaalloc(ima_w, ima_h);
        if (ima_blue == NULL)
            wxLogError(_T("Could not allocate memory for ima_blue"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_blue[i][j] = ima_image.GetBlue(i,j);
            }
        }
    }

    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::imaImage"));
        printf("-imaImage::imaImage\n");fflush(stdout);
    #endif
}

imaImage::imaImage(wxImage image, wxString ext){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::imaImage"));
        printf("imaImage::imaImage\n");fflush(stdout);
    #endif

    // Create image
    ima_image = image.Copy();

    // Set properties
    is_color = true;
    name     = wxString();
    type     = ext;
    ima_w    = ima_image.GetWidth();
    ima_h    = ima_image.GetHeight();

    // Create data arrays

    // Red (gray)
    ima_red = imaalloc(ima_w, ima_h);
    if (ima_red == NULL)
        wxLogError(_T("Could not allocate memory for ima_red"));
    // Fill red array
    for (unsigned int j = 0; j < ima_h; j++){
        for (unsigned int i = 0; i < ima_w; i++){
            ima_red[i][j] = ima_image.GetRed(i,j);
        }
    }

    // We only need the following if the image is color
    if (is_color){
        // Green
        ima_green = imaalloc(ima_w, ima_h);
        if (ima_green == NULL)
            wxLogError(_T("Could not allocate memory for ima_green"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_green[i][j] = ima_image.GetGreen(i,j);
            }
        }
        // Blue
        ima_blue = imaalloc(ima_w, ima_h);
        if (ima_blue == NULL)
            wxLogError(_T("Could not allocate memory for ima_blue"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_blue[i][j] = ima_image.GetBlue(i,j);
            }
        }
    }

    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::imaImage"));
        printf("-imaImage::imaImage\n");fflush(stdout);
    #endif
}

imaImage::imaImage(wxImage image, wxString ext, wxString filename, bool color){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::imaImage"));
        printf("imaImage::imaImage\n");fflush(stdout);
    #endif

    // Create image
    ima_image = image.Copy();

    // Set properties
    is_color = color;
    name     = filename;
    type     = ext;
    ima_w    = ima_image.GetWidth();
    ima_h    = ima_image.GetHeight();

    // Create data arrays

    // Red (gray)
    ima_red = imaalloc(ima_w, ima_h);
    if (ima_red == NULL)
        wxLogError(_T("Could not allocate memory for ima_red"));
    // Fill red array
    for (unsigned int j = 0; j < ima_h; j++){
        for (unsigned int i = 0; i < ima_w; i++){
            ima_red[i][j] = ima_image.GetRed(i,j);
        }
    }

    // We only need the following if the image is color
    if (is_color){
        // Green
        ima_green = imaalloc(ima_w, ima_h);
        if (ima_green == NULL)
            wxLogError(_T("Could not allocate memory for ima_green"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_green[i][j] = ima_image.GetGreen(i,j);
            }
        }
        // Blue
        ima_blue = imaalloc(ima_w, ima_h);
        if (ima_blue == NULL)
            wxLogError(_T("Could not allocate memory for ima_blue"));
        for (unsigned int j = 0; j < ima_h; j++){
            for (unsigned int i = 0; i < ima_w; i++){
                ima_blue[i][j] = ima_image.GetBlue(i,j);
            }
        }
    }

    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::imaImage"));
        printf("-imaImage::imaImage\n");fflush(stdout);
    #endif
}

//-----Destructor---------------------------------------------------------------
imaImage::~imaImage(void){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::~imaImage"));
        printf("imaImage::~imaImage\n");fflush(stdout);
    #endif
    imafree(ima_red, ima_w);
    if (is_color){
        imafree(ima_green, ima_w);
        imafree(ima_blue, ima_w);
    }
    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::~imaImage"));
        printf("-imaImage::~imaImage\n");fflush(stdout);
    #endif
}

//-----GetImage-----------------------------------------------------------------
wxImage imaImage::GetImage(void){
    return ima_image;
}

//-----GetRed-------------------------------------------------------------------
unsigned char** imaImage::GetRed(void){
    return ima_red;
}

//-----GetGreen-----------------------------------------------------------------
unsigned char** imaImage::GetGreen(void){
    return ima_green;
}

//-----GetBlue------------------------------------------------------------------
unsigned char** imaImage::GetBlue(void){
    return ima_blue;
}

//-----GetWidth-----------------------------------------------------------------
int imaImage::GetWidth(void){
    return ima_w;
}

//-----GetHeight----------------------------------------------------------------
int imaImage::GetHeight(void){
    return ima_h;
}

//-----GetType------------------------------------------------------------------
wxString imaImage::GetType(void){
    return type;
}

//-----GetName------------------------------------------------------------------
wxString imaImage::GetName(void){
    return name;
}

//-----SetRed-------------------------------------------------------------------
int imaImage::SetGray(unsigned char ** gray){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::SetGray"));
        printf("imaImage::SetGray\n");fflush(stdout);
    #endif

    int error = -1;
    int index;
    unsigned char *set_array;

    // Check ima_red has been allocated and copy
    if (ima_red == NULL){
         wxLogError(_T("ima_red has not been allocated"));
         return -1;
    }
    error = imacpy(ima_red, gray, ima_w, ima_h);

    // Create a flat RGBRGB... array
    set_array = (unsigned char*)malloc(ima_w * ima_h * 3);
    for (int j = 0; j < ima_h; j++){
        for (int i = 0; i < ima_w; i++){
            index = ((j * ima_w) + i)*3; // flat RGB array index
            set_array[index]   = ima_red[i][j];
            set_array[index+1] = ima_red[i][j];
            set_array[index+2] = ima_red[i][j];
        }
    }
    ima_image.SetData(set_array,true); // This takes care of freeing set_array

    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::SetGray"));
        printf("-imaImage::SetGray\n");fflush(stdout);
    #endif

    return error;
}

//-----SetColor-----------------------------------------------------------------
int imaImage::SetColor(unsigned char ** red, unsigned char ** green, unsigned char ** blue){
    #ifdef DEBUG
        wxLogMessage(_T("imaImage::SetColor"));
        printf("imaImage::SetColor\n");fflush(stdout);
    #endif

    int error = -1;
    int index;
    unsigned char *set_array;

    // Check ima_green has been allocated
    if (ima_red == NULL){
         wxLogError(_T("ima_red has not been allocated"));
         return -1;
    }
    if (ima_green == NULL){
         wxLogError(_T("ima_green has not been allocated"));
         return -1;
    }
    else if (ima_blue == NULL){
         wxLogError(_T("ima_blue has not been allocated"));
         return -1;
    }
    imacpy(ima_red,   red,   ima_w, ima_h);
    imacpy(ima_green, green, ima_w, ima_h);
    imacpy(ima_blue,  blue,  ima_w, ima_h);

    // Create a flat RGBRGB... array
    set_array = (unsigned char*)malloc(ima_w * ima_h * 3);
    for (int j = 0; j < ima_h; j++){
        for (int i = 0; i < ima_w; i++){
            index = ((j * ima_w) + i)*3; // flat RGB array index
            set_array[index]   = ima_red[i][j];
            set_array[index+1] = ima_green[i][j];
            set_array[index+2] = ima_blue[i][j];
        }
    }

    ima_image.SetData(set_array,true); // This takes care of freeing set_array

    #ifdef DEBUG
        wxLogMessage(_T("-imaImage::SetColor"));
        printf("-imaImage::SetColor\n");fflush(stdout);
    #endif

    return error;
}

//-----SetWidth-----------------------------------------------------------------
int imaImage::SetWidth (int im_w){
    ima_w = im_w;
    return 0;
}

//-----SetHeight----------------------------------------------------------------
int imaImage::SetHeight(int im_h){
    ima_h = im_h;
    return 0;
}

//-----SetType------------------------------------------------------------------
int imaImage::SetType(wxString ext){
    type = ext;
    return 0;
}

//-----SetIsColor-----------------------------------------------------------------
int imaImage::SetIsColor(bool color){
    is_color = color;
    return 0;
}

//-----SetName-----------------------------------------------------------------
int imaImage::SetName (wxString imagename){
    name = imagename;
    return 0;
}

//-----HasAlpha-----------------------------------------------------------------
bool imaImage::HasAlpha(void){
    return ima_image.HasAlpha();
}

//-----HasMask------------------------------------------------------------------
bool imaImage::HasMask(void){
    return ima_image.HasMask();
}

//-----IsColor------------------------------------------------------------------
bool imaImage::IsColor(void){
    return is_color;
}


//------------------------------------------------------------------------------
// imaImageList
//------------------------------------------------------------------------------

//-----imaImageList-------------------------------------------------------------
imaImageList::imaImageList(void){
    #ifdef DEBUG
        printf("imaImageList::imaImageList\n");fflush(stdout);
    #endif

    number_of_images = 0;
    // Point the list elements to NULL since nothing is loaded yet
    for (int i = 0; i < MAX_OPEN_IMAGES; i++)
        imaimage_list[i] = NULL;

    #ifdef DEBUG
        printf("-imaImageList::imaImageList\n");fflush(stdout);
    #endif
}

//-----~imaImageList------------------------------------------------------------
imaImageList::~imaImageList(void){
    int i = 0;

    // Delete every imaImage in the list
    while ((imaimage_list[i] != NULL) && (i < MAX_OPEN_IMAGES)){
        delete imaimage_list[i];
        i++;
    }
}

//-----GetImage-----------------------------------------------------------------
imaImage* imaImageList::GetimaImage(unsigned char index){
    #ifdef DEBUG
        wxLogMessage(_T("imaImageList::GetImage"));
        printf("imaImageList::GetImage\n");fflush(stdout);
    #endif

    return imaimage_list[index];

    #ifdef DEBUG
        wxLogMessage(_T("-imaImageList::GetImage"));
        printf("-imaImageList::GetImage\n");fflush(stdout);
    #endif
}

//-----AddImage-----------------------------------------------------------------
// Returns: image index on success
//          1001 if no open slots available
//          1002 if couldn't create imaImage
int imaImageList::AddImage(wxImage image){
    #ifdef DEBUG
        wxLogMessage(_T("imaImageList::AddImage"));
        printf("imaImageList::AddImage\n");fflush(stdout);
    #endif

    int i = 0;

    // Find first open slot in imaimage_list
    while ((imaimage_list[i] != NULL) && (i < MAX_OPEN_IMAGES)){
        i++;
    }
    // Did we hit the end of the list?
    if (i == MAX_OPEN_IMAGES){
        return -1001;  // Too many images open
    }
    else{
        imaimage_list[i] = new imaImage(image);
        if (imaimage_list[i] == NULL){
            return -1002; // Couldn't create imaimage
        }
    }
    printf("  index = %d\n",i);fflush(stdout);
    //current_image = i;

    #ifdef DEBUG
        wxLogMessage(_T("-imaImageList::AddImage"));
        printf("-imaImageList::AddImage\n");fflush(stdout);
    #endif

    return i;
}

//-----CopyImage-----------------------------------------------------------------
// Returns: image index on success
int imaImageList::CopyImage (int index){
    #ifdef DEBUG
        wxLogMessage(_T("imaImageList::CopyImage"));
        printf("imaImageList::CopyImage\n");fflush(stdout);
    #endif

    int new_index;

    // Copy image
    new_index = AddImage(GetimaImage(index)->GetImage());
    printf("  new_index = %d\n", new_index);fflush(stdout);
    // Copy the other properties
    GetimaImage(new_index)->SetIsColor(GetimaImage(index)->IsColor());
    if (GetimaImage(index)->IsColor()){
        GetimaImage(new_index)->SetColor(GetimaImage(index)->GetRed(),
                                         GetimaImage(index)->GetGreen(),
                                         GetimaImage(index)->GetBlue());
    }
    else{
        GetimaImage(new_index)->SetGray(GetimaImage(index)->GetRed());
    }
    GetimaImage(new_index)->SetName(GetimaImage(index)->GetName());
    GetimaImage(new_index)->SetType(GetimaImage(index)->GetType());
    // Width and Height is set in the constructor from the wxImage

    printf("  old_index = %d new_index = %d\n",index, new_index);fflush(stdout);

    #ifdef DEBUG
        wxLogMessage(_T("-imaImageList::CopyImage"));
        printf("-imaImageList::CopyImage\n");fflush(stdout);
    #endif

    return new_index;
}

//-----DeleteImage--------------------------------------------------------------
void imaImageList::DeleteImage(unsigned char index){

}

//-----MoveUp-------------------------------------------------------------------
void imaImageList::MoveUp(unsigned char index){

}

//-----MoveDown-----------------------------------------------------------------
void imaImageList::MoveDown(unsigned char index){

}

//-----SetCurrent-----------------------------------------------------------------
int imaImageList::SetCurrent (int index){
    current_image = index;
    return 0;
}

//-----GetCurrent-----------------------------------------------------------------
int imaImageList::GetCurrent (void){
    return current_image;
}
