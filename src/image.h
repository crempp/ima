/*******************************************************************************
 * Name:      image.h
 * Purpose:   Header for the image class. Stores working image data
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 * Notes:     Color images are three arrays of values 0-255
 *            Grayscale images just use the red array
 ******************************************************************************/
#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <wx/image.h>
#include "globals.h"

//-----imaImage-----------------------------------------------------------------
// This stores the image data for an open image.
class imaImage{
    public:
        imaImage(wxImage image);
        imaImage(wxImage image, wxString ext);
        imaImage(wxImage image, wxString ext, wxString filename, bool color);
        ~imaImage(void);

        wxImage         GetImage (void);
        unsigned char** GetRed   (void);
        unsigned char** GetGreen (void);
        unsigned char** GetBlue  (void);
        int             GetWidth (void);
        int             GetHeight(void);
        wxString        GetType  (void);
        wxString        GetName  (void);
        int             SetGray  (unsigned char ** gray);
        int             SetColor (unsigned char ** red, unsigned char ** green,
                                  unsigned char ** blue);
        int             SetWidth (int im_w);
        int             SetHeight(int im_h);
        int             SetType  (wxString ext);
        int             SetIsColor (bool color);
        int             SetName  (wxString imagename);
        bool            HasAlpha (void);
        bool            HasMask  (void);
        bool            IsColor  (void);

    protected:
        // Data
        wxImage         ima_image;
        unsigned char** ima_red;  // if grayscale use this
        unsigned char** ima_green;
        unsigned char** ima_blue;

        // Properties
        bool         is_color;
        wxString     name;
        wxString     type;
        wxString     last_op;
        unsigned int ima_w;
        unsigned int ima_h;
};

//-----imaImageList-------------------------------------------------------------
// This is a list of the open images.
class imaImageList{
    public:
        imaImageList(void);
        ~imaImageList(void);

        imaImage* GetimaImage(unsigned char index);
        int       GetNumber  (void);
        int       SetCurrent (int index);
        int       GetCurrent (void);
        int       AddImage   (wxImage);
        int       CopyImage  (int index);
        void      DeleteImage(unsigned char index);
        void      MoveUp     (unsigned char index);
        void      MoveDown   (unsigned char index);

    protected:
        imaImage* imaimage_list[ MAX_OPEN_IMAGES ];
        int current_image;
        int number_of_images;
};

#endif // IMAGE_H_INCLUDED
