///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

gui_Frame::gui_Frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	this->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	wxBoxSizer* sz_01;
	sz_01 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	pn_canvas = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	pn_canvas->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer16->Add( pn_canvas, 1, wxALIGN_LEFT|wxALL|wxEXPAND, 1 );
	
	wxBoxSizer* sz_06;
	sz_06 = new wxBoxSizer( wxVERTICAL );
	
	nb_controls = new wxNotebook( this, wxID_ANY, wxPoint( -1,-1 ), wxSize( 220,-1 ), 0 );
	nb_controls->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	nb_controls->SetForegroundColour( wxColour( 231, 233, 237 ) );
	nb_controls->SetBackgroundColour( wxColour( 51, 51, 51 ) );
	
	pn_01 = new wxPanel( nb_controls, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pn_01->SetForegroundColour( wxColour( 231, 233, 237 ) );
	pn_01->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	wxBoxSizer* sz_02;
	sz_02 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( pn_01, wxID_ANY, wxT("file") ), wxVERTICAL );
	
	tx_filename = new wxStaticText( pn_01, wxID_ANY, wxT("none"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	tx_filename->Wrap( -1 );
	sbSizer8->Add( tx_filename, 0, wxALL|wxEXPAND, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 3, 0, 0 );
	
	tx_dims = new wxStaticText( pn_01, wxID_ANY, wxT("0x0"), wxDefaultPosition, wxDefaultSize, 0 );
	tx_dims->Wrap( -1 );
	gSizer1->Add( tx_dims, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
	
	tx_type = new wxStaticText( pn_01, wxID_ANY, wxT("type"), wxDefaultPosition, wxDefaultSize, 0 );
	tx_type->Wrap( -1 );
	gSizer1->Add( tx_type, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
	
	tx_depth = new wxStaticText( pn_01, wxID_ANY, wxT("depth"), wxDefaultPosition, wxDefaultSize, 0 );
	tx_depth->Wrap( -1 );
	gSizer1->Add( tx_depth, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1 );
	
	bt_open = new wxButton( pn_01, ID_OPEN, wxT("open"), wxDefaultPosition, wxSize( 60,25 ), 0|wxRAISED_BORDER );
	bt_open->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_open->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer1->Add( bt_open, 0, wxALL, 1 );
	
	bt_save = new wxButton( pn_01, ID_SAVE, wxT("save"), wxDefaultPosition, wxSize( 60,25 ), 0|wxRAISED_BORDER );
	bt_save->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_save->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer1->Add( bt_save, 0, wxALL, 1 );
	
	bt_exit = new wxButton( pn_01, ID_EXIT, wxT("exit"), wxDefaultPosition, wxSize( 60,25 ), 0|wxRAISED_BORDER );
	bt_exit->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_exit->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer1->Add( bt_exit, 0, wxALL, 1 );
	
	sbSizer8->Add( gSizer1, 0, wxEXPAND, 1 );
	
	sz_02->Add( sbSizer8, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( pn_01, wxID_ANY, wxT("parameters") ), wxVERTICAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText26 = new wxStaticText( pn_01, wxID_ANY, wxT("edge method"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText26->Wrap( -1 );
	bSizer23->Add( m_staticText26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	bSizer23->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxString ch_edgeChoices[] = { wxT("black"), wxT("white"), wxT("mirror"), wxT("wrap"), wxT("original pixel") };
	int ch_edgeNChoices = sizeof( ch_edgeChoices ) / sizeof( wxString );
	ch_edge = new wxChoice( pn_01, wxID_ANY, wxDefaultPosition, wxDefaultSize, ch_edgeNChoices, ch_edgeChoices, 0 );
	ch_edge->SetSelection( 2 );
	ch_edge->SetForegroundColour( wxColour( 231, 233, 237 ) );
	ch_edge->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer23->Add( ch_edge, 0, wxALL, 1 );
	
	sbSizer11->Add( bSizer23, 1, wxEXPAND, 5 );
	
	sz_02->Add( sbSizer11, 0, wxEXPAND, 1 );
	
	
	sz_02->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( pn_01, wxID_ANY, wxT("fourier transform") ), wxVERTICAL );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 1, 3, 0, 0 );
	
	bt_fft = new wxButton( pn_01, ID_FFT, wxT("fft"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_fft->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_fft->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer4->Add( bt_fft, 0, wxALL, 1 );
	
	bt_ifft = new wxButton( pn_01, ID_IFFT, wxT("ifft"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_ifft->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_ifft->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer4->Add( bt_ifft, 0, wxALL, 1 );
	
	sbSizer9->Add( gSizer4, 0, wxEXPAND, 1 );
	
	sz_02->Add( sbSizer9, 0, wxEXPAND, 1 );
	
	pn_01->SetSizer( sz_02 );
	pn_01->Layout();
	sz_02->Fit( pn_01 );
	nb_controls->AddPage( pn_01, wxT("image"), true );
	pn_03 = new wxPanel( nb_controls, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pn_03->SetForegroundColour( wxColour( 231, 233, 237 ) );
	pn_03->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	pn_03->Hide();
	
	wxBoxSizer* sz_09;
	sz_09 = new wxBoxSizer( wxVERTICAL );
	
	lb_devices = new wxListBox( pn_03, wxID_ANY, wxDefaultPosition, wxSize( -1,100 ), 0, NULL, wxLB_SINGLE|wxVSCROLL );
	lb_devices->Append( wxT("none") );
	lb_devices->SetForegroundColour( wxColour( 231, 233, 237 ) );
	lb_devices->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sz_09->Add( lb_devices, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sz_10;
	sz_10 = new wxBoxSizer( wxHORIZONTAL );
	
	tx_vidvend = new wxStaticText( pn_03, wxID_ANY, wxT("vendor"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
	tx_vidvend->Wrap( -1 );
	tx_vidvend->SetForegroundColour( wxColour( 231, 233, 237 ) );
	
	sz_10->Add( tx_vidvend, 0, wxALL, 5 );
	
	
	sz_10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	tx_viddev = new wxStaticText( pn_03, wxID_ANY, wxT("device"), wxDefaultPosition, wxSize( 90,-1 ), 0 );
	tx_viddev->Wrap( -1 );
	tx_viddev->SetForegroundColour( wxColour( 231, 233, 237 ) );
	
	sz_10->Add( tx_viddev, 0, wxALL, 5 );
	
	sz_09->Add( sz_10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_getdevices = new wxButton( pn_03, ID_GDEV, wxT("get devices"), wxDefaultPosition, wxSize( 85,25 ), 0 );
	bt_getdevices->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_getdevices->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer18->Add( bt_getdevices, 0, wxALL, 1 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bt_loaddevice = new wxButton( pn_03, ID_LDEV, wxT("load device"), wxDefaultPosition, wxSize( 80,25 ), 0 );
	bt_loaddevice->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_loaddevice->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer18->Add( bt_loaddevice, 0, wxALL, 1 );
	
	sz_09->Add( bSizer18, 0, wxEXPAND, 5 );
	
	wxBoxSizer* sz_11;
	sz_11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sz_12;
	sz_12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText9 = new wxStaticText( pn_03, wxID_ANY, wxT("brightness"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	sz_12->Add( m_staticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	sz_12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sld_brightness = new wxSlider( pn_03, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize( 130,-1 ), wxSL_HORIZONTAL );
	sz_12->Add( sld_brightness, 0, wxALL, 3 );
	
	sz_11->Add( sz_12, 0, wxEXPAND, 0 );
	
	wxBoxSizer* sz_13;
	sz_13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText10 = new wxStaticText( pn_03, wxID_ANY, wxT("hue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	sz_13->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	sz_13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sld_hue = new wxSlider( pn_03, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize( 130,-1 ), wxSL_HORIZONTAL );
	sz_13->Add( sld_hue, 0, wxALIGN_RIGHT|wxALL, 3 );
	
	sz_11->Add( sz_13, 0, wxEXPAND, 0 );
	
	wxBoxSizer* sz_14;
	sz_14 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( pn_03, wxID_ANY, wxT("color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	sz_14->Add( m_staticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	sz_14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sld_color = new wxSlider( pn_03, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize( 130,-1 ), wxSL_HORIZONTAL );
	sz_14->Add( sld_color, 0, wxALL, 3 );
	
	sz_11->Add( sz_14, 0, wxEXPAND, 0 );
	
	wxBoxSizer* sz_15;
	sz_15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( pn_03, wxID_ANY, wxT("contrast"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	sz_15->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	sz_15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sld_contrast = new wxSlider( pn_03, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize( 130,-1 ), wxSL_HORIZONTAL );
	sz_15->Add( sld_contrast, 0, wxALL, 3 );
	
	sz_11->Add( sz_15, 0, wxEXPAND, 0 );
	
	wxBoxSizer* sz_16;
	sz_16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( pn_03, wxID_ANY, wxT("white"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	sz_16->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	sz_16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sld_white = new wxSlider( pn_03, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxSize( 130,-1 ), wxSL_HORIZONTAL );
	sz_16->Add( sld_white, 0, wxALL, 3 );
	
	sz_11->Add( sz_16, 0, wxEXPAND, 0 );
	
	sz_09->Add( sz_11, 1, wxEXPAND, 5 );
	
	pn_03->SetSizer( sz_09 );
	pn_03->Layout();
	sz_09->Fit( pn_03 );
	nb_controls->AddPage( pn_03, wxT("video"), false );
	pn_02 = new wxPanel( nb_controls, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pn_02->SetForegroundColour( wxColour( 231, 233, 237 ) );
	pn_02->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	wxBoxSizer* sz_07;
	sz_07 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sz_08;
	sz_08 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("average") ), wxHORIZONTAL );
	
	bt_average = new wxButton( pn_02, ID_AVG, wxT("smooth"), wxDefaultPosition, wxSize( 50,25 ), 0|wxRAISED_BORDER );
	bt_average->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	bt_average->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_average->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sz_08->Add( bt_average, 0, wxALL, 1 );
	
	
	sz_08->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText14 = new wxStaticText( pn_02, wxID_ANY, wxT("window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	sz_08->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_avg_window = new wxTextCtrl( pn_02, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 25,25 ), 0 );
	txt_avg_window->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_avg_window->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sz_08->Add( txt_avg_window, 0, wxALL, 1 );
	
	sz_07->Add( sz_08, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("order statistic") ), wxHORIZONTAL );
	
	bt_ord = new wxButton( pn_02, ID_ORD, wxT("smooth"), wxDefaultPosition, wxSize( 50,25 ), 0 );
	bt_ord->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_ord->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer4->Add( bt_ord, 0, wxALL, 1 );
	
	
	sbSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxString ch_ordChoices[] = { wxT("median"), wxT("max"), wxT("min") };
	int ch_ordNChoices = sizeof( ch_ordChoices ) / sizeof( wxString );
	ch_ord = new wxChoice( pn_02, wxID_ANY, wxDefaultPosition, wxSize( 70,25 ), ch_ordNChoices, ch_ordChoices, 0 );
	ch_ord->SetSelection( 0 );
	ch_ord->SetForegroundColour( wxColour( 231, 233, 237 ) );
	ch_ord->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer4->Add( ch_ord, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	m_staticText17 = new wxStaticText( pn_02, wxID_ANY, wxT("window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	sbSizer4->Add( m_staticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_ord_window = new wxTextCtrl( pn_02, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 25,25 ), 0 );
	txt_ord_window->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_ord_window->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer4->Add( txt_ord_window, 0, wxALL, 1 );
	
	sz_07->Add( sbSizer4, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sz_17;
	sz_17 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("gaussian") ), wxHORIZONTAL );
	
	bt_gaussian = new wxButton( pn_02, ID_GAUSS, wxT("smooth"), wxDefaultPosition, wxSize( 50,25 ), 0 );
	bt_gaussian->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_gaussian->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sz_17->Add( bt_gaussian, 0, wxALL, 1 );
	
	
	sz_17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText15 = new wxStaticText( pn_02, wxID_ANY, wxT("s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	sz_17->Add( m_staticText15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_gauss_s = new wxTextCtrl( pn_02, wxID_ANY, wxT("0.6"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	txt_gauss_s->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_gauss_s->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	txt_gauss_s->Enable( false );
	
	sz_17->Add( txt_gauss_s, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	
	sz_17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText16 = new wxStaticText( pn_02, wxID_ANY, wxT("window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	sz_17->Add( m_staticText16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_gauss_window = new wxTextCtrl( pn_02, ID_GAUSS_W, wxT("2"), wxDefaultPosition, wxSize( 25,-1 ), 0 );
	txt_gauss_window->SetForegroundColour( wxColour( 255, 255, 255 ) );
	txt_gauss_window->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sz_17->Add( txt_gauss_window, 0, wxALL, 1 );
	
	sz_07->Add( sz_17, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("kuwahara") ), wxHORIZONTAL );
	
	bt_kuwahara = new wxButton( pn_02, ID_KUWAHARA, wxT("smooth"), wxDefaultPosition, wxSize( 50,25 ), 0 );
	bt_kuwahara->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_kuwahara->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer61->Add( bt_kuwahara, 0, wxALL, 1 );
	
	
	sbSizer61->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText24 = new wxStaticText( pn_02, wxID_ANY, wxT("window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	sbSizer61->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_kuwahara_window = new wxTextCtrl( pn_02, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 25,-1 ), 0 );
	txt_kuwahara_window->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_kuwahara_window->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer61->Add( txt_kuwahara_window, 0, wxALL, 1 );
	
	sz_07->Add( sbSizer61, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("symetric nearest neighbor") ), wxHORIZONTAL );
	
	bt_snn = new wxButton( pn_02, ID_SNN, wxT("smooth"), wxDefaultPosition, wxSize( 50,25 ), 0 );
	bt_snn->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_snn->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer7->Add( bt_snn, 0, wxALL, 1 );
	
	
	sbSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText25 = new wxStaticText( pn_02, wxID_ANY, wxT("window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	sbSizer7->Add( m_staticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_snn_window = new wxTextCtrl( pn_02, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 25,-1 ), 0 );
	txt_snn_window->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_snn_window->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer7->Add( txt_snn_window, 0, wxALL, 1 );
	
	sz_07->Add( sbSizer7, 0, wxEXPAND, 1 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( pn_02, wxID_ANY, wxT("sharp") ), wxHORIZONTAL );
	
	bt_sharp = new wxButton( pn_02, ID_SHARP, wxT("sharp"), wxDefaultPosition, wxSize( 50,25 ), 0 );
	bt_sharp->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_sharp->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer6->Add( bt_sharp, 0, wxALL, 1 );
	
	
	sbSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxString ch_sharpChoices[] = { wxT("laplacian") };
	int ch_sharpNChoices = sizeof( ch_sharpChoices ) / sizeof( wxString );
	ch_sharp = new wxChoice( pn_02, wxID_ANY, wxDefaultPosition, wxDefaultSize, ch_sharpNChoices, ch_sharpChoices, 0 );
	ch_sharp->SetSelection( 0 );
	ch_sharp->SetForegroundColour( wxColour( 231, 233, 237 ) );
	ch_sharp->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	ch_sharp->SetMinSize( wxSize( 95,25 ) );
	
	sbSizer6->Add( ch_sharp, 0, wxALL, 1 );
	
	sz_07->Add( sbSizer6, 0, wxEXPAND, 1 );
	
	pn_02->SetSizer( sz_07 );
	pn_02->Layout();
	sz_07->Fit( pn_02 );
	nb_controls->AddPage( pn_02, wxT("smooth/sharp"), false );
	pn_04 = new wxPanel( nb_controls, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pn_04->SetForegroundColour( wxColour( 231, 233, 237 ) );
	pn_04->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( pn_04, wxID_ANY, wxT("gradient") ), wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_robertscross = new wxButton( pn_04, ID_ROBERTS, wxT("roberts cross"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_robertscross->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_robertscross->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer21->Add( bt_robertscross, 0, wxALL, 1 );
	
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText19 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	bSizer21->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_roberts = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_roberts->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_roberts->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer21->Add( txt_roberts, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer21, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_prewitt = new wxButton( pn_04, ID_PREWITT, wxT("prewitt"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_prewitt->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_prewitt->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer22->Add( bt_prewitt, 0, wxALL, 1 );
	
	
	bSizer22->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText20 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	bSizer22->Add( m_staticText20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_prewitt = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_prewitt->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_prewitt->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer22->Add( txt_prewitt, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer22, 0, wxEXPAND, 10 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_sobel = new wxButton( pn_04, ID_SOBEL, wxT("sobel"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_sobel->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_sobel->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer24->Add( bt_sobel, 0, wxALL, 1 );
	
	
	bSizer24->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText201 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	bSizer24->Add( m_staticText201, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_sobel = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_sobel->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_sobel->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer24->Add( txt_sobel, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer24, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_scharr = new wxButton( pn_04, ID_SCHARR, wxT("scharr"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_scharr->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_scharr->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer25->Add( bt_scharr, 0, wxALL, 1 );
	
	
	bSizer25->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText21 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizer25->Add( m_staticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_scharr = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_scharr->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_scharr->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer25->Add( txt_scharr, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer25, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_laplace = new wxButton( pn_04, ID_LAPLACE, wxT("laplace"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_laplace->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_laplace->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer26->Add( bt_laplace, 0, wxALL, 1 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText22 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	bSizer26->Add( m_staticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_laplace = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_laplace->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_laplace->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer26->Add( txt_laplace, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer26, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_kirsch = new wxButton( pn_04, ID_KIRSCH, wxT("kirsch"), wxDefaultPosition, wxSize( 90,25 ), 0 );
	bt_kirsch->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_kirsch->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer27->Add( bt_kirsch, 0, wxALL, 1 );
	
	
	bSizer27->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText23 = new wxStaticText( pn_04, wxID_ANY, wxT("thr"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	bSizer27->Add( m_staticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	txt_kirsch = new wxTextCtrl( pn_04, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 35,-1 ), 0 );
	txt_kirsch->SetForegroundColour( wxColour( 231, 233, 237 ) );
	txt_kirsch->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer27->Add( txt_kirsch, 0, wxALL, 1 );
	
	sbSizer5->Add( bSizer27, 0, wxEXPAND, 1 );
	
	bSizer20->Add( sbSizer5, 0, wxEXPAND, 1 );
	
	pn_04->SetSizer( bSizer20 );
	pn_04->Layout();
	bSizer20->Fit( pn_04 );
	nb_controls->AddPage( pn_04, wxT("gradient"), false );
	
	sz_06->Add( nb_controls, 1, wxALL|wxEXPAND, 1 );
	
	bSizer16->Add( sz_06, 0, wxALIGN_RIGHT|wxEXPAND, 1 );
	
	sz_01->Add( bSizer16, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	txt_log = new wxTextCtrl( this, ID_LOG, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxNO_BORDER );
	txt_log->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	txt_log->SetForegroundColour( wxColour( 64, 204, 28 ) );
	txt_log->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	bSizer19->Add( txt_log, 1, wxALL|wxEXPAND, 0 );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 220,-1 ), wxTAB_TRAVERSAL );
	m_panel6->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	m_panel6->SetForegroundColour( wxColour( 231, 233, 237 ) );
	m_panel6->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel6, wxID_ANY, wxT("set") ), wxHORIZONTAL );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 3, 3, 0, 0 );
	
	bt_show = new wxButton( m_panel6, ID_SHOW, wxT("show"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_show->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_show->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_show, 0, wxALL, 1 );
	
	bt_delete = new wxButton( m_panel6, ID_DELETE, wxT("delete"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_delete->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_delete->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_delete, 0, wxALL, 1 );
	
	bt_log = new wxButton( m_panel6, ID_LOG, wxT("log"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_log->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_log->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_log, 0, wxALL, 1 );
	
	bt_add = new wxButton( m_panel6, ID_ADD, wxT("add"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_add->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_add->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_add, 0, wxALL, 1 );
	
	bt_subtract = new wxButton( m_panel6, ID_SUBTRACT, wxT("subtract"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_subtract->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_subtract->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_subtract, 0, wxALL, 1 );
	
	bt_multiply = new wxButton( m_panel6, ID_MULTIPLY, wxT("multiply"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_multiply->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_multiply->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_multiply, 0, wxALL, 1 );
	
	bt_divide = new wxButton( m_panel6, ID_DIVIDE, wxT("divide"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_divide->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_divide->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_divide, 0, wxALL, 1 );
	
	bt_and = new wxButton( m_panel6, ID_AND, wxT("and"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_and->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_and->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_and, 0, wxALL, 1 );
	
	bt_or = new wxButton( m_panel6, ID_OR, wxT("or"), wxDefaultPosition, wxSize( 60,25 ), 0 );
	bt_or->SetForegroundColour( wxColour( 231, 233, 237 ) );
	bt_or->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	gSizer3->Add( bt_or, 0, wxALL, 1 );
	
	sbSizer10->Add( gSizer3, 1, wxEXPAND, 0 );
	
	lb_images = new wxListBox( m_panel6, wxID_ANY, wxDefaultPosition, wxSize( 220,80 ), 0, NULL, wxLB_SINGLE ); 
	lb_images->SetFont( wxFont( 8, 76, 90, 90, false, wxT("Courier 10 Pitch") ) );
	lb_images->SetForegroundColour( wxColour( 231, 233, 237 ) );
	lb_images->SetBackgroundColour( wxColour( 22, 22, 22 ) );
	
	sbSizer10->Add( lb_images, 0, wxALL, 0 );
	
	m_panel6->SetSizer( sbSizer10 );
	m_panel6->Layout();
	bSizer19->Add( m_panel6, 0, wxEXPAND | wxALL, 0 );
	
	sz_01->Add( bSizer19, 0, wxEXPAND, 1 );
	
	this->SetSizer( sz_01 );
	this->Layout();
}

gui_Frame::~gui_Frame()
{
}
