/***************************************************************
 * Name:      imaApp.cpp
 * Purpose:   Implement the app
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 **************************************************************/

#include <wx/image.h>
#include "imaApp.h"
#include "image.h"
#include "mainframe.h"

// We need global access to these
imaImageList* image_list;

IMPLEMENT_APP(imaApp);

bool imaApp::OnInit()
{
    // Add handlers for image types.
    wxImage::AddHandler(new wxPNGHandler());
    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxGIFHandler());
    wxImage::AddHandler(new wxPCXHandler());
    wxImage::AddHandler(new wxTIFFHandler());
    wxImage::AddHandler(new wxXPMHandler());

    // Initialize image_data
    image_list = new imaImageList();

    // Create the frame
    mainFrame* frm = new mainFrame(0L);
    frm->Show();

    wxLogMessage(_T("Program started"));
    #ifdef DEBUG
        printf("Program started\n");fflush(stdout);
    #endif
    return true;
}

imaApp::~imaApp(void){
    delete image_list;
}
