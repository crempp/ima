///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listbox.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_OPEN 1000
#define ID_SAVE 1001
#define ID_EXIT 1002
#define ID_FFT 1003
#define ID_IFFT 1004
#define ID_GDEV 1005
#define ID_LDEV 1006
#define ID_AVG 1007
#define ID_ORD 1008
#define ID_GAUSS 1009
#define ID_GAUSS_W 1010
#define ID_KUWAHARA 1011
#define ID_SNN 1012
#define ID_SHARP 1013
#define ID_ROBERTS 1014
#define ID_PREWITT 1015
#define ID_SOBEL 1016
#define ID_SCHARR 1017
#define ID_LAPLACE 1018
#define ID_KIRSCH 1019
#define ID_LOG 1020
#define ID_SHOW 1021
#define ID_DELETE 1022
#define ID_ADD 1023
#define ID_SUBTRACT 1024
#define ID_MULTIPLY 1025
#define ID_DIVIDE 1026
#define ID_AND 1027
#define ID_OR 1028

///////////////////////////////////////////////////////////////////////////////
/// Class gui_Frame
///////////////////////////////////////////////////////////////////////////////
class gui_Frame : public wxFrame 
{
	private:
	
	protected:
		wxNotebook* nb_controls;
		wxPanel* pn_01;
		wxStaticText* tx_filename;
		wxStaticText* tx_dims;
		wxStaticText* tx_type;
		wxStaticText* tx_depth;
		wxButton* bt_open;
		wxButton* bt_save;
		wxButton* bt_exit;
		wxStaticText* m_staticText26;
		
		wxChoice* ch_edge;
		
		wxButton* bt_fft;
		wxButton* bt_ifft;
		wxPanel* pn_03;
		wxListBox* lb_devices;
		wxStaticText* tx_vidvend;
		
		wxStaticText* tx_viddev;
		wxButton* bt_getdevices;
		
		wxButton* bt_loaddevice;
		wxStaticText* m_staticText9;
		
		wxSlider* sld_brightness;
		wxStaticText* m_staticText10;
		
		wxSlider* sld_hue;
		wxStaticText* m_staticText11;
		
		wxSlider* sld_color;
		wxStaticText* m_staticText12;
		
		wxSlider* sld_contrast;
		wxStaticText* m_staticText13;
		
		wxSlider* sld_white;
		wxPanel* pn_02;
		wxButton* bt_average;
		
		wxStaticText* m_staticText14;
		wxTextCtrl* txt_avg_window;
		wxButton* bt_ord;
		
		wxChoice* ch_ord;
		wxStaticText* m_staticText17;
		wxTextCtrl* txt_ord_window;
		wxButton* bt_gaussian;
		
		wxStaticText* m_staticText15;
		wxTextCtrl* txt_gauss_s;
		
		wxStaticText* m_staticText16;
		wxTextCtrl* txt_gauss_window;
		wxButton* bt_kuwahara;
		
		wxStaticText* m_staticText24;
		wxTextCtrl* txt_kuwahara_window;
		wxButton* bt_snn;
		
		wxStaticText* m_staticText25;
		wxTextCtrl* txt_snn_window;
		wxButton* bt_sharp;
		
		wxChoice* ch_sharp;
		wxPanel* pn_04;
		wxButton* bt_robertscross;
		
		wxStaticText* m_staticText19;
		wxTextCtrl* txt_roberts;
		wxButton* bt_prewitt;
		
		wxStaticText* m_staticText20;
		wxTextCtrl* txt_prewitt;
		wxButton* bt_sobel;
		
		wxStaticText* m_staticText201;
		wxTextCtrl* txt_sobel;
		wxButton* bt_scharr;
		
		wxStaticText* m_staticText21;
		wxTextCtrl* txt_scharr;
		wxButton* bt_laplace;
		
		wxStaticText* m_staticText22;
		wxTextCtrl* txt_laplace;
		wxButton* bt_kirsch;
		
		wxStaticText* m_staticText23;
		wxTextCtrl* txt_kirsch;
		wxPanel* m_panel6;
		wxButton* bt_show;
		wxButton* bt_delete;
		wxButton* bt_log;
		wxButton* bt_add;
		wxButton* bt_subtract;
		wxButton* bt_multiply;
		wxButton* bt_divide;
		wxButton* bt_and;
		wxButton* bt_or;
		wxListBox* lb_images;
	
	public:
		wxPanel* pn_canvas;
		wxTextCtrl* txt_log;
		gui_Frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("IMA"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1080,640 ), long style = wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~gui_Frame();
	
};

#endif //__gui__
