/*********************************************************************
 * Name:      	main.h
 * Purpose:   	Declares simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is declared in
 * 				gui.h source file which is generated by wxFormBuilder.
 *********************************************************************/

#ifndef __wxmain__
#define __wxmain__

#include <memory>
#include <vector>
// main wxWidgets header file
#include <wx/wx.h>
#include <wx/timer.h>
// gui classes generated by wxFormBuilder
#include "gui.h"

class Game;
typedef unsigned char chtype;
enum Command { STOP, LEFT, RIGHT, UP, DOWN, JUMP, DIE, QUIT,
               NUMCOMMANDS};

////////////////////////////////////////////////////////////////////////////////
// application class declaration 
////////////////////////////////////////////////////////////////////////////////

class MainApp : public wxApp
{
	public:
		virtual bool OnInit();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

////////////////////////////////////////////////////////////////////////////////
// main application frame declaration 
////////////////////////////////////////////////////////////////////////////////

class GameDisplay : public MainFrameBase
{
	public:
		GameDisplay( wxWindow *parent );
		virtual ~GameDisplay();
        
         /**
          * @brief Write the character c at the row and column of the virtual map.
          *        The character is stored in a buffer in case the map is scrolled.
          * @param row The row of the location, 0 the top row.
          * @param column The column of the location, 0 the left column.
          * @param c The character to write.
          */
        void writeAt(unsigned row, unsigned column, const chtype c);
        
         /**
          * @brief Write the message on the status line at the specified 
          *        location. 
          * @param row Not used.
          * @param column The offset in the status line to write at.
          * @param s The message.
          */
        void writeAt(unsigned row, unsigned column, const std::string s);
            
        /**
         * @brief If necessary, scroll so that the specified location is displayed.
         * @param row The row of the location in the virtual map that must display.
         * @param column The column of the location in the virtual map that must display.
         * @param border The size of the border on the screen. If the location
         *               is further from the edge than this border, we do not
         *               scroll.
         * @param scroll The unit of scrolling. 1 will give a smooth scroll,
         *               larger numbers will hold the map still longer and jump
         *               instead.
         */
        void center(unsigned row, unsigned column,
                    unsigned border=10, unsigned scroll=1);
                    
        /**
         * @brief Get the command that was most recently issued by the user.
         * @return The current command.
         */
        Command getCommand();

        /**
         * @brief  Set the next command, as if the user had entered it.
         * @param command The new current command.
         */
        void setCommand(Command command);
        
        void clock(Game *game) {}
            
        void setGame(Game *game, unsigned rows, unsigned columns) 
        { 
            this->game=game;
            this->columns=columns;
            this->rows=rows;
            buffer.reset(new chtype[columns*rows]);
        }
        
	protected:
		// protected event handlers
        virtual void onAbortGame( wxCommandEvent& event );
		virtual void OnCloseFrame( wxCloseEvent& event );
		virtual void OnExitClick( wxCommandEvent& event );
        virtual void onGoUp( wxCommandEvent& event ) { newCommand=UP; }
		virtual void onGoDown( wxCommandEvent& event ) { newCommand=DOWN; }
		virtual void onGoLeft( wxCommandEvent& event ) { newCommand=LEFT; }
		virtual void onGoRight( wxCommandEvent& event ) { newCommand=RIGHT; }
		virtual void onGoJump( wxCommandEvent& event ) { newCommand=JUMP; }
		virtual void onGoStop( wxCommandEvent& event ) { newCommand=STOP; }
        virtual void onKeyDown( wxKeyEvent& event );
        virtual void onPaint( wxPaintEvent& event );
        virtual void onResize( wxSizeEvent& event ) { corner_column=corner_row=0; needrefresh=true; m_panel1->Refresh(); }
		virtual void onSpriteSizeUpdate( wxScrollEvent& event );
		virtual void onSpeedUpdate( wxScrollEvent& event );
        void onTick( wxTimerEvent& event );
        
    private:
        chtype &bufferEntry(unsigned row, unsigned col)
        {
            return buffer[row*columns+col];
        }
        
        std::vector<wxBitmap> font, fontScaled;
        unsigned columns=0, rows=0, // Size of the virtual map.
                 corner_row=0, corner_column=0, // Location of the upper left corner of the displayed
                         // part of the map.
                 fontlen=0;
        std::unique_ptr<chtype[]> buffer; // Contents of the full map.
        std::vector<unsigned> bufferUpdates;
        Command command[256], // The mapping from key pressed to Command.
                lastCommand=STOP, newCommand=RIGHT; // The previous and new commands.
        bool done=false, // Flag to tell the input thread to exit.
             needrefresh=false;
        Game *game=nullptr;
        wxTimer gameTimer;
};

#endif //__wxmain__