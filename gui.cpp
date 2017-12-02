///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	menu_bar = new wxMenuBar( 0 );
	m_menuBar = new wxMenu();
	wxMenuItem* go_abort;
	go_abort = new wxMenuItem( m_menuBar, wxID_ABORT, wxString( wxT("End current game") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	m_menuBar->Append( go_abort );
	go_abort->Enable( false );
	
	wxMenuItem* go_exit;
	go_exit = new wxMenuItem( m_menuBar, wxID_ANY, wxString( wxT("Exit") ) + wxT('\t') + wxT("ALT+X"), wxEmptyString, wxITEM_NORMAL );
	m_menuBar->Append( go_exit );
	
	menu_bar->Append( m_menuBar, wxT("File") );
	
	m_menu2 = new wxMenu();
	wxMenuItem* go_up;
	go_up = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Up") ) + wxT('\t') + wxT("UP"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_up );
	
	wxMenuItem* go_down;
	go_down = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Down") ) + wxT('\t') + wxT("DOWN"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_down );
	
	wxMenuItem* go_left;
	go_left = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Left") ) + wxT('\t') + wxT("LEFT"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_left );
	
	wxMenuItem* go_right;
	go_right = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Right") ) + wxT('\t') + wxT("RIGHT"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_right );
	
	wxMenuItem* go_jump;
	go_jump = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Jump") ) + wxT('\t') + wxT("SPACE"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_jump );
	
	wxMenuItem* go_stop;
	go_stop = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("Stop") ) + wxT('\t') + wxT("N"), wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( go_stop );
	
	menu_bar->Append( m_menu2, wxT("MyMenu") );
	
	this->SetMenuBar( menu_bar );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer1->Add( m_panel1, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	zoom_label = new wxStaticText( this, wxID_ANY, wxT("Zoom"), wxDefaultPosition, wxDefaultSize, 0 );
	zoom_label->Wrap( -1 );
	bSizer2->Add( zoom_label, 0, wxALL, 5 );
	
	spritesize = new wxSlider( this, wxID_ANY, 24, 8, 32, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer2->Add( spritesize, 0, wxALL, 5 );
	
	speed_label = new wxStaticText( this, wxID_ANY, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	speed_label->Wrap( -1 );
	bSizer2->Add( speed_label, 0, wxALL, 5 );
	
	speed = new wxSlider( this, wxID_ANY, -300, -2000, -100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer2->Add( speed, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, 0, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::onResize ) );
	menu_bar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	this->Connect( go_abort->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onAbortGame ) );
	this->Connect( go_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onExitClick ) );
	this->Connect( go_up->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoUp ) );
	this->Connect( go_down->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoDown ) );
	this->Connect( go_left->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoLeft ) );
	this->Connect( go_right->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoRight ) );
	this->Connect( go_jump->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoJump ) );
	this->Connect( go_stop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoStop ) );
	m_panel1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	m_panel1->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::onPaint ), NULL, this );
	zoom_label->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	spritesize->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	spritesize->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpriteSizeUpdate ), NULL, this );
	speed_label->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	speed->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpeedUpdate ), NULL, this );
	speed->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	speed->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpeedUpdate ), NULL, this );
	m_statusBar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::onResize ) );
	menu_bar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onAbortGame ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onExitClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoUp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoDown ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoLeft ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoRight ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoJump ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::onGoStop ) );
	m_panel1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	m_panel1->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::onPaint ), NULL, this );
	zoom_label->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	spritesize->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	spritesize->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpriteSizeUpdate ), NULL, this );
	speed_label->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	speed->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpeedUpdate ), NULL, this );
	speed->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
	speed->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MainFrameBase::onSpeedUpdate ), NULL, this );
	m_statusBar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( MainFrameBase::onKeyDown ), NULL, this );
}
