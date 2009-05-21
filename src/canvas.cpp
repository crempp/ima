/***************************************************************
 * Name:      canvas.cpp
 * Purpose:   Implement the canvas and drawing functions
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 **************************************************************/

#include <wx/image.h>
#include "canvas.h"
#include "globals.h"
#include "mainframe.h"

BEGIN_EVENT_TABLE(imaCanvas, wxScrolledWindow)
    EVT_PAINT(imaCanvas::OnPaint)
END_EVENT_TABLE()

//-----imaCanvas Constructor----------------------------------------------------
imaCanvas::imaCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size):
 wxScrolledWindow(parent, wxID_ANY, pos, size)
{
    // Create stuff
}

//-----imaCanvas Destructor-----------------------------------------------------
imaCanvas::~imaCanvas(void)
{
    // Delete stuff
}

//-----OnPaint------------------------------------------------------------------
void imaCanvas::OnPaint(wxPaintEvent& WXUNUSED(event)){
    //wxLogMessage(_T("imaCanvas::OnPaint"));

    wxPaintDC dc(this);
    PrepareDC(dc);

    if (current_image.Ok())
    {
        dc.DrawBitmap(current_image,0,0);
    }
    else
    {
        wxLogError(_T("Couldn't draw image to canvas"));
    }

    //wxLogMessage(_T("-imaCanvas::OnPaint"));
}
