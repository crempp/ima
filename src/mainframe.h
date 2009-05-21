/***************************************************************
 * Name:      mainframe.h
 * Purpose:   Headers for the GUI
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 **************************************************************/
#ifndef MAIN_FRAME_H_INCLUDED
#define MAIN_FRAME_H_INCLUDED

#include "canvas.h"
#include "gui.h"

class imaCanvas;

class mainFrame : public gui_Frame
{
    public:
        mainFrame(wxFrame *frame);
        ~mainFrame();
        imaCanvas *canvas;
    private:
        virtual void OnOpen(wxCommandEvent& event);
        virtual void OnSave(wxCommandEvent& event);
        virtual void OnExit(wxCommandEvent& event);
        virtual void OnShow(wxCommandEvent& event);
        virtual void OnDelete(wxCommandEvent& event);
        virtual void OnLog(wxCommandEvent& event);
        virtual void OnAdd(wxCommandEvent& event);
        virtual void OnSubtract(wxCommandEvent& event);
        virtual void OnMultiply(wxCommandEvent& event);
        virtual void OnDivide(wxCommandEvent& event);
        virtual void OnAnd(wxCommandEvent& event);
        virtual void OnOr(wxCommandEvent& event);

        virtual void OnGaussWindow(wxCommandEvent& event);

        virtual void OnAverage(wxCommandEvent& event);
        virtual void OnOrderStatistic(wxCommandEvent& event);
        virtual void OnGauss(wxCommandEvent& event);
        virtual void OnSharp(wxCommandEvent& event);

        virtual void OnRoberts(wxCommandEvent& event);
        virtual void OnPrewitt(wxCommandEvent& event);
        virtual void OnSobel(wxCommandEvent& event);
        virtual void OnScharr(wxCommandEvent& event);
        virtual void OnLaplace(wxCommandEvent& event);
        virtual void OnKirsch(wxCommandEvent& event);

        virtual void OnGetDevices(wxCommandEvent& event);
        virtual void OnLoadDevice(wxCommandEvent& event);

        void Draw(void);
    DECLARE_EVENT_TABLE()
};

#endif // MAIN_FRAME_H_INCLUDED
