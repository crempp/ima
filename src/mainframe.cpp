/*******************************************************************************
 * Name:      mainframe.cpp
 * Purpose:   Implement the GUI
 * Author:    Chad Rempp (crempp@gmail.com)
 * Created:   2008-10-28
 * Copyright: Chad Rempp (http://www.tetractys.org)
 * License:   GPLv3 copyright 2008 Chad Rempp
 ******************************************************************************/

#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/log.h>
#include "canvas.h"
#include "gui.h"
#include "image.h"
#include "ima_math.h"
#include "mainframe.h"

extern imaImageList* image_list;

//-----mainFrame Constructor----------------------------------------------------
mainFrame::mainFrame(wxFrame *frame) : gui_Frame(frame)
{
    // Constructor
    canvas = new imaCanvas(this->pn_canvas,wxPoint(0,0),wxSize(0,0));

    // Set up logging window
    wxLogTextCtrl *logWindow = new wxLogTextCtrl(txt_log);
    delete wxLog::SetActiveTarget(logWindow);
}

mainFrame::~mainFrame()
{
}

BEGIN_EVENT_TABLE (mainFrame, wxFrame)
	EVT_BUTTON ( ID_EXIT,    mainFrame::OnExit)
	EVT_BUTTON ( ID_OPEN,    mainFrame::OnOpen)
	EVT_BUTTON ( ID_SAVE,    mainFrame::OnSave)
	EVT_BUTTON ( ID_SHOW,    mainFrame::OnShow)
	EVT_BUTTON ( ID_DELETE,  mainFrame::OnDelete)
	EVT_BUTTON ( ID_LOG,     mainFrame::OnLog)
	EVT_BUTTON ( ID_ADD,     mainFrame::OnAdd)
	EVT_BUTTON ( ID_SUBTRACT,mainFrame::OnSubtract)
	EVT_BUTTON ( ID_MULTIPLY,mainFrame::OnMultiply)
	EVT_BUTTON ( ID_DIVIDE,  mainFrame::OnDivide)
	EVT_BUTTON ( ID_AND,     mainFrame::OnAnd)
	EVT_BUTTON ( ID_OR,      mainFrame::OnOr)
	EVT_TEXT   ( ID_GAUSS_W, mainFrame::OnGaussWindow)
	EVT_BUTTON ( ID_LDEV,    mainFrame::OnLoadDevice)
	EVT_BUTTON ( ID_GDEV,    mainFrame::OnGetDevices)
	EVT_BUTTON ( ID_AVG,     mainFrame::OnAverage)
	EVT_BUTTON ( ID_ORD,     mainFrame::OnOrderStatistic)
	EVT_BUTTON ( ID_GAUSS,   mainFrame::OnGauss)
	EVT_BUTTON ( ID_SHARP,   mainFrame::OnSharp)
	EVT_BUTTON ( ID_ROBERTS, mainFrame::OnRoberts)
	EVT_BUTTON ( ID_PREWITT, mainFrame::OnPrewitt)
	EVT_BUTTON ( ID_SOBEL,   mainFrame::OnSobel)
	EVT_BUTTON ( ID_SCHARR,  mainFrame::OnScharr)
	EVT_BUTTON ( ID_LAPLACE, mainFrame::OnLaplace)
	EVT_BUTTON ( ID_KIRSCH,  mainFrame::OnKirsch)
END_EVENT_TABLE()

//-----OnOpen-------------------------------------------------------------------
void mainFrame::OnOpen(wxCommandEvent& event)
{
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnOpen"));
        printf("mainFrame::OnOpen\n");fflush(stdout);
    #endif

    bool error = false;
    wxImage  image;
    wxString fileext;
    wxString filename;
    wxString s_dims;
    wxString s_depth;
    int index = 0;
    int im_w, im_h;
    int pn_w, pn_h;

    // Show the file dialog
    wxFileDialog* openFileDialog =
		new wxFileDialog( this, _("Open file"), _(""), _(""),
                            _("BMP files (*.bmp)|*.bmp;*.BMP|"
                            "PNG files (*.png)|*.png;*.PNG|"
                            "JPEG files (*.jpg)|*.jpg;*.jpeg;*JPG;*.JPEG|"
                            /*"GIF files (*.gif)|*.gif|"
                            "TIFF files (*.tif)|*.tif|"
                            "PCX files (*.pcx)|*.pcx|"*/
                            "All files|*.*"),
                            wxOPEN, wxDefaultPosition);

    if (openFileDialog->ShowModal() == wxID_OK){
        // Get file to open
        wxString filepath = openFileDialog->GetPath();
        // extract the name and extension
        wxFileName::SplitPath(filepath, NULL, &filename, &fileext);

        if ( fileext == _T("bmp") ){
            if (!image.LoadFile(filepath, wxBITMAP_TYPE_BMP)){
                wxLogError(_T("Couldn't open BMP file"));
                error = true;
            }
        }
        else if ( fileext == _T("png") ){
            if (!image.LoadFile(filepath, wxBITMAP_TYPE_PNG)){
                wxLogError(_T("Couldn't open PNG file"));
                error = true;
            }
        }
        else if ( fileext == _T("jpg") || fileext == _T("jpeg") ){
            // Current this guesses the tpye
            if (!image.LoadFile(filepath)){
                wxLogError(_T("Couldn't open JPG file"));
                error = true;
            }
        }
        else if ( fileext == _T("gif") ){
            // Current this guesses the tpye
            if (!image.LoadFile(filepath)){
                wxLogError(_T("Couldn't open GIF file"));
                error = true;
            }
        }
        else if ( fileext == _T("tif") ){
            // Current this guesses the tpye
            if (!image.LoadFile(filepath)){
                wxLogError(_T("Couldn't open TIF file"));
                error = true;
            }
        }
        else if ( fileext == _T("pcx") ){
            // Current this guesses the tpye
            if (!image.LoadFile(filepath)){
                wxLogError(_T("Couldn't open PCX file"));
                error = true;
            }
        }

        // Successfully opened the file, but check dimensions
        pn_canvas->GetSize(&pn_w, &pn_h);
        if (!error && image.GetWidth() <= pn_w && image.GetHeight() < pn_h){
            // Add image to image list
            index = image_list->AddImage(image);
            image_list->SetCurrent(index);
            if (index == E_TOO_MANY_IMAGES){
                wxLogError(_T("Too many images open"));
                return;
            }
            else if (index == E_CANT_CREATE_IMAGELIST){
                wxLogError(_T("Couldn't create imaImage"));
                return;
            }

            // Set info and fill it into the GUI
            im_w = image.GetWidth();
            im_h = image.GetHeight();
            image_list->GetimaImage(index)->SetWidth(im_w);
            image_list->GetimaImage(index)->SetHeight(im_h);
            s_dims  = wxString() << im_w << wxT("x") << im_h;
            tx_dims->SetLabel(s_dims);
            wxLogMessage(s_dims);

            if (is_color(image)){
                image_list->GetimaImage(index)->SetIsColor(true);
                s_depth = wxString() << 24;
                tx_depth->SetLabel(s_depth);
                wxLogMessage(s_depth);
            }
            else{
                image_list->GetimaImage(index)->SetIsColor(false);
                s_depth = wxString() << 8;
                tx_depth->SetLabel(s_depth);
                wxLogMessage(s_depth);
            }
            image_list->GetimaImage(index)->SetName(filename);
            tx_filename->SetLabel(filename);
            image_list->GetimaImage(index)->SetType(fileext);
            tx_type->SetLabel(fileext);

            // Add image to list box and select it
            lb_images->Append(filename);
            lb_images->SetSelection(lb_images->GetCount() - 1);

            // Draw image to canvas
            Draw();
        }
    }
    else{
        wxLogError(_T("Couldn't start file dialog"));
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnOpen"));
        printf("-mainFrame::OnOpen\n");fflush(stdout);
    #endif
}

//-----OnSave-------------------------------------------------------------------
void mainFrame::OnSave(wxCommandEvent& event)
{
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnSave"));
        printf("mainFrame::OnSave\n");fflush(stdout);
    #endif

    wxString fileext;
    wxString filename;
    int index;

    wxFileDialog* saveFileDialog =
            new wxFileDialog( this, _("Save file"), _(""), _(""),
                            _("BMP files (*.bmp)|*.bmp;*.BMP|"
                            "PNG files (*.png)|*.png;*.PNG|"
                            "JPEG files (*.jpg)|*.jpg;*.jpeg;*JPG;*.JPEG|"
                            /*"GIF files (*.gif)|*.gif|"
                            "TIFF files (*.tif)|*.tif|"
                            "PCX files (*.pcx)|*.pcx|"*/
                            "All files|*.*"),
                            wxSAVE, wxDefaultPosition);

    if (saveFileDialog->ShowModal() == wxID_OK){
        // Get file to open
        wxString filepath = saveFileDialog->GetPath();
        // extract the name and extension
        wxFileName::SplitPath(filepath, NULL, &filename, &fileext);
        index = image_list->GetCurrent();

        if ( fileext == _T("bmp") ){
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_BMP)){
                wxLogError(_T("Couldn't save BMP file"));
            }
        }
        else if ( fileext == _T("png") ){
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_PNG)){
                wxLogError(_T("Couldn't save PNG file"));
            }
        }
        else if ( fileext == _T("jpg") || fileext == _T("jpeg") ){
            // Current this guesses the tpye
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_JPEG)){
                wxLogError(_T("Couldn't save JPG file"));
            }
        }
        else if ( fileext == _T("gif") ){
            // Current this guesses the tpye
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_GIF)){
                wxLogError(_T("Couldn't save GIF file"));
            }
        }
        else if ( fileext == _T("tif") ){
            // Current this guesses the tpye
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_TIF)){
                wxLogError(_T("Couldn't save TIF file"));
            }
        }
        else if ( fileext == _T("pcx") ){
            // Current this guesses the tpye
            if (!image_list->GetimaImage(index)->GetImage().SaveFile(filepath, wxBITMAP_TYPE_PCX)){
                wxLogError(_T("Couldn't save PCX file"));
            }
        }
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnSave"));
        printf("-mainFrame::OnSave\n");fflush(stdout);
    #endif
}

//-----OnExit-------------------------------------------------------------------
void mainFrame::OnExit(wxCommandEvent& event)
{
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnExit"));
        printf("mainFrame::OnExit\n");fflush(stdout);
    #endif

    Destroy();

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnExit"));
        printf("-mainFrame::OnExit\n");fflush(stdout);
    #endif
}

//-----OnShow-------------------------------------------------------------------
void mainFrame::OnShow(wxCommandEvent& event){
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnShow"));
        printf("mainFrame::OnShow\n");fflush(stdout);
    #endif

    int index;

    // Get selection
    index = image_list->GetCurrent();

    // Set as current image
    image_list->SetCurrent(index);

    // Draw
    Draw();

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnShow"));
        printf("-mainFrame::OnShow\n");fflush(stdout);
    #endif
}

//-----OnDelete-----------------------------------------------------------------
void mainFrame::OnDelete(wxCommandEvent& event){
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnDelete"));
        printf("mainFrame::OnDelete\n");fflush(stdout);
    #endif

    int index;

    // Get selection
    index = image_list->GetCurrent();

    // Delete it
    image_list->DeleteImage(index);

    // Remove it from the listing
    lb_images->Delete(index);

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnDelete"));
        printf("-mainFrame::OnDelete\n");fflush(stdout);
    #endif
}

//-----OnLog--------------------------------------------------------------------
void mainFrame::OnLog(wxCommandEvent& event){
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnLog"));
        printf("mainFrame::OnLog\n");fflush(stdout);
    #endif

    wxString name;
    int index = lb_images->GetSelection();
    int new_index = image_list->CopyImage(index);

    if (index == E_TOO_MANY_IMAGES || index == E_CANT_CREATE_IMAGELIST){
        wxLogError(_T("Couldn't create a new image"));
    }
    else{
        // Update the image name
        name = image_list->GetimaImage(new_index)->GetName();
        lb_images->Append(name.Append(_T("-log")));
        lb_images->SetSelection(new_index);
        // Do the logging
        if(image_list->GetimaImage(new_index)->IsColor())
            Log_color(image_list->GetimaImage(new_index));
        else
            Log_gray(image_list->GetimaImage(new_index));
        Draw();
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnLog"));
        printf("-mainFrame::OnLog\n");fflush(stdout);
    #endif
}

//-----OnAdd--------------------------------------------------------------------
void mainFrame::OnAdd(wxCommandEvent& event){

}

//-----OnSubtract---------------------------------------------------------------
void mainFrame::OnSubtract(wxCommandEvent& event){

}

//-----OnMultiply---------------------------------------------------------------
void mainFrame::OnMultiply(wxCommandEvent& event){

}

//-----OnDivide-----------------------------------------------------------------
void mainFrame::OnDivide(wxCommandEvent& event){

}

//-----OnAnd--------------------------------------------------------------------
void mainFrame::OnAnd(wxCommandEvent& event){

}

//-----OnOr---------------------------------------------------------------------
void mainFrame::OnOr(wxCommandEvent& event){

}

//-----OnGaussWindow------------------------------------------------------------
void mainFrame::OnGaussWindow(wxCommandEvent& event){
    //wxLogMessage(_T("mainFrame::OnGaussWindow"));
    /*
    long w;
    double new_w;
    wxString temp;

    if (image_data != NULL){
        if (!txt_gauss_window->GetValue().ToLong(&w)){
            wxLogError(_T("Invalid window value, %s"),
                    txt_gauss_window->GetValue().c_str());
        }
        else{
            new_w = (double)w / 3;
            temp << new_w;
            txt_gauss_s->SetValue(temp);
        }
    }
    else{
        wxLogError(_T("Image data doesn't exist"));
    }
    */
    //wxLogMessage(_T("-mainFrame::OnGaussWindow"));
}

//-----OnAverage----------------------------------------------------------------
void mainFrame::OnAverage(wxCommandEvent& event)
{
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnAverage"));
        printf("mainFrame::OnAverage\n");fflush(stdout);
    #endif

    long w;
    int index, new_index;
    wxString name;

    if( true ){//(image_list->GetimaImage()->GetImage() != NULL){
        if (!txt_avg_window->GetValue().ToLong(&w)){
            wxLogError(_T("Invalid s value, %s"), txt_avg_window->GetValue().c_str());
        }
        else{
            // Create new image
            index = lb_images->GetSelection();
            new_index = image_list->CopyImage(index);
            if (index == E_TOO_MANY_IMAGES || index == E_CANT_CREATE_IMAGELIST){
                wxLogError(_T("Couldn't create a new image"));
            }
            else{
                // Update the image name
                name = image_list->GetimaImage(new_index)->GetName();
                lb_images->Append(name.Append(_T("-average")));
                lb_images->SetSelection(new_index);
                // Do the averaging
                if(image_list->GetimaImage(new_index)->IsColor())
                    Average_color(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                else
                    Average_gray(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                Draw();
            }
        }
    }
    else{
        wxLogError(_T("No Image available"));
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnAverage"));
        printf("-mainFrame::OnAverage\n");fflush(stdout);
    #endif
}

//-----OnOrderStatistic---------------------------------------------------------
void mainFrame::OnOrderStatistic(wxCommandEvent& event)
{
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnOrderStatistic"));
        printf("mainFrame::OnOrderStatistic\n");fflush(stdout);
    #endif

    int selection;
    int index, new_index;
    long w;
    wxString name;

    if( true ){//(image_list->GetimaImage()->GetImage() != NULL){
        selection = ch_ord->GetCurrentSelection();
        if (!txt_ord_window->GetValue().ToLong(&w)){
            wxLogError(_T("Invalid s value, %s"), txt_ord_window->GetValue().c_str());
        }
        else{
            // Create new image
            index = lb_images->GetSelection();
            new_index = image_list->CopyImage(index);
            if (index == E_TOO_MANY_IMAGES || index == E_CANT_CREATE_IMAGELIST){
                wxLogError(_T("Couldn't create a new image"));
            }
            else{
                // Update the image name
                switch(selection){
                    case 0: // Median
                        name = image_list->GetimaImage(new_index)->GetName();
                        lb_images->Append(name.Append(_T("-median")));
                        lb_images->SetSelection(new_index);
                        // Do the medianing
                        if(image_list->GetimaImage(new_index)->IsColor())
                            OS_Median_color(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        else
                            OS_Median_gray(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        Draw();
                        break;
                    case 1:
                        name = image_list->GetimaImage(new_index)->GetName();
                        lb_images->Append(name.Append(_T("-max")));
                        lb_images->SetSelection(new_index);
                        // Do the maxing
                        if(image_list->GetimaImage(new_index)->IsColor())
                            OS_Max_color(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        else
                            OS_Max_gray(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        Draw();
                        break;
                    case 2:
                        name = image_list->GetimaImage(new_index)->GetName();
                        lb_images->Append(name.Append(_T("-min")));
                        lb_images->SetSelection(new_index);
                        // Do the maxing
                        if(image_list->GetimaImage(new_index)->IsColor())
                            OS_Min_color(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        else
                            OS_Min_gray(image_list->GetimaImage(new_index),w,ch_edge->GetCurrentSelection());
                        Draw();
                        break;
                }
            }
        }
    }
    else{
        wxLogError(_T("No Image available"));
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::OnOrderStatistic"));
        printf("-mainFrame::OnOrderStatistic\n");fflush(stdout);
    #endif
}

//-----OnGauss------------------------------------------------------------------
// To prevent dimming or brightening s  = w/3
void mainFrame::OnGauss(wxCommandEvent& event){
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnGauss"));
        printf("mainFrame::OnGauss\n");fflush(stdout);
    #endif

    int index, new_index;
    long w;
    wxString name;

    if( true ){//(image_list->GetimaImage()->GetImage() != NULL){
        if (!txt_gauss_window->GetValue().ToLong(&w)){
            wxLogError(_T("Invalid s value, %s"), txt_ord_window->GetValue().c_str());
        }
        else{
            // Create new image
            index = lb_images->GetSelection();
            new_index = image_list->CopyImage(index);
            if (index == E_TOO_MANY_IMAGES || index == E_CANT_CREATE_IMAGELIST){
                wxLogError(_T("Couldn't create a new image"));
            }
            else{
                // Update the image name
                name = image_list->GetimaImage(new_index)->GetName();
                lb_images->Append(name.Append(_T("-gaussian")));
                lb_images->SetSelection(new_index);
                // Do the medianing
                if(image_list->GetimaImage(new_index)->IsColor())
                    Gaussian_gray(image_list->GetimaImage(new_index), w, ch_edge->GetCurrentSelection());
                else
                    Gaussian_gray(image_list->GetimaImage(new_index), w, ch_edge->GetCurrentSelection());
                Draw();
            }
        }
    }
    else{
        wxLogError(_T("No Image available"));
    }

    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::OnGauss"));
        printf("mainFrame::OnGauss\n");fflush(stdout);
    #endif
}

//-----OnSharp----------------------------------------------------------------
void mainFrame::OnSharp(wxCommandEvent& event)
{
    //wxLogMessage(_T("mainFrame::OnSharp"));
    /*
    int selection;

    if(image_data != NULL){
        selection = ch_sharp->GetCurrentSelection();
        image_data->SetUndo();
        image_data->Sharp(selection);
        Draw();
    }
    else{
        wxLogError(_T("No Image available"));
    }
    */
    //wxLogMessage(_T("-mainFrame::OnSharp"));
}

//-----OnRoberts----------------------------------------------------------------
void mainFrame::OnRoberts(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnRoberts"));
    /*
    long t;

    if(image_data != NULL){
        if (!txt_roberts->GetValue().ToLong(&t)){
            wxLogError(_T("Invalid thr value, %s"),
                    txt_roberts->GetValue().c_str());
        }
        else{
            image_data->SetUndo();
            image_data->Roberts(t);
            Draw();SetSelection(int n)


        }
    }
    else{
        wxLogError(_T("No Image available"));
    }
    */
    wxLogMessage(_T("-mainFrame::OnRoberts"));
}

//-----OnPrewitt----------------------------------------------------------------
void mainFrame::OnPrewitt(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnPrewitt"));
    /*
    long t;

    if(image_data != NULL){
        if (!txt_prewitt->GetValue().ToLong(&t)){
            wxLogError(_T("Invalid thr value, %s"),
                    txt_prewitt->GetValue().c_str());
        }
        else{
            image_data->SetUndo();
            image_data->Prewitt(t);
            Draw();
        }
    }
    else{
        wxLogError(_T("No Image available"));
    }
    */
    wxLogMessage(_T("-mainFrame::OnPrewitt"));
}

//-----OnSobel----------------------------------------------------------------
void mainFrame::OnSobel(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnSobel"));
    /*
    long t;

    if(image_data != NULL){
        if (!txt_sobel->GetValue().ToLong(&t)){
            wxLogError(_T("Invalid thr value, %s"),
                    txt_sobel->GetValue().c_str());
        }
        else{
            image_data->SetUndo();
            image_data->Sobel(t);
            Draw();
        }
    }
    else{
        wxLogError(_T("No Image available"));
    }
    */
    wxLogMessage(_T("-mainFrame::OnSobel"));
}

//-----OnScharr----------------------------------------------------------------
void mainFrame::OnScharr(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnScharr"));
    wxLogMessage(_T("-mainFrame::OnScharr"));
}

//-----OnLaplace----------------------------------------------------------------
void mainFrame::OnLaplace(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnLaplace"));
    wxLogMessage(_T("-mainFrame::OnLaplace"));
}

//-----OnKirsch----------------------------------------------------------------
void mainFrame::OnKirsch(wxCommandEvent& event){
    wxLogMessage(_T("mainFrame::OnKirsch"));
    wxLogMessage(_T("-mainFrame::OnKirsch"));
}

//-----OnGetDevices-------------------------------------------------------------
void mainFrame::OnGetDevices(wxCommandEvent& event)
{
    //wxLogMessage(_T("mainFrame::OnGetDevices"));
    wxLogMessage(_T("Video functions have not been implemented yet"));
    //list_devices();
    //lb_devices->Delete(0);
    //for (int i = 0; i < MAX_DEVICES; i++){
        //if (ima_device_list[i].exists == 1){
            //wxString temp = wxString::FromAscii(device_list[i].identifier);
            //wxLogMessage(_T("  A device exists %s"), temp.c_str());
            //lb_devices->InsertItems(1, &temp, i);
        //}
    //}
    //wxLogMessage(_T("-mainFrame::OnGetDevices"));
}

//-----OnLoadDevice-------------------------------------------------------------
void mainFrame::OnLoadDevice(wxCommandEvent& event)
{
    //wxLogMessage(_T("mainFrame::Load Device"));
    wxLogMessage(_T("Video functions have not been implemented yet"));
    //int i = lb_devices->GetSelection();
    //wxLogMessage(_T("  %d is selected"),i);
    //load_device(i);
    //wxLogMessage(_T("Done Loading Device"));
    //wxLogMessage(_T("-mainFrame::Load Device"));
}

//-----Draw-------------------------------------------------------------------
void mainFrame::Draw(void){
    #ifdef DEBUG
        wxLogMessage(_T("mainFrame::Draw"));
        printf("mainFrame::Draw\n");fflush(stdout);
    #endif

    int pn_w, pn_h, im_w, im_h, index;

    // Get current image
    index = lb_images->GetSelection();
    wxLogMessage(_T("%d"),index);
    // Set imaImageList curren_image
    image_list->SetCurrent(index);

    if (image_list->GetimaImage(index) != NULL){
        im_w = image_list->GetimaImage(index)->GetWidth();
        im_h = image_list->GetimaImage(index)->GetHeight();
        pn_canvas->GetSize(&pn_w, &pn_h);
        canvas->SetSize((pn_w - im_w) / 2, (pn_h - im_h) / 2, im_w, im_h);
        canvas->current_image = wxBitmap(image_list->GetimaImage(index)->GetImage());
        canvas->Refresh();
    }
    else{
        wxLogError(_T("Image data not present"));
    }

    #ifdef DEBUG
        wxLogMessage(_T("-mainFrame::Draw"));
        printf("-mainFrame::Draw\n");fflush(stdout);
    #endif
}
