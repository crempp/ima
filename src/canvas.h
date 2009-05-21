/***************************************************************
 * Name:      canvas.h
 * Purpose:   Headers for the canvas
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:
 **************************************************************/
#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include <wx/wx.h>
#include "mainframe.h"

class mainFrame;

class imaCanvas: public wxScrolledWindow{
    public:
        imaCanvas(wxWindow *parent, const wxPoint& pos, const wxSize& size);
        ~imaCanvas(void);
        mainFrame *frm;
        wxBitmap current_image;
        void OnPaint(wxPaintEvent& event);

DECLARE_EVENT_TABLE()
};

#endif // CANVAS_H_INCLUDED
