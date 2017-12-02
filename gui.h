///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __noname__
#define __noname__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* menu_bar;
		wxMenu* m_menuBar;
		wxMenu* m_menu2;
		wxPanel* m_panel1;
		wxStaticText* zoom_label;
		wxSlider* spritesize;
		wxStaticText* speed_label;
		wxSlider* speed;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void onKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void onResize( wxSizeEvent& event ) { event.Skip(); }
		virtual void onAbortGame( wxCommandEvent& event ) { event.Skip(); }
		virtual void onExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoJump( wxCommandEvent& event ) { event.Skip(); }
		virtual void onGoStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void onPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void onSpriteSizeUpdate( wxScrollEvent& event ) { event.Skip(); }
		virtual void onSpeedUpdate( wxScrollEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrameBase();
	
};

#endif //__noname__
