/*********************************************************************
 * Name:      	main.cpp
 * Purpose:   	Implements simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:    
 * Created:   
 * Copyright: 
 * License:   	wxWidgets license (www.wxwidgets.org)
 * 
 * Notes:		Note that all GUI creation code is implemented in
 * 				gui.cpp source file which is generated by wxFormBuilder.
 *********************************************************************/

#include <exception>
#include <fstream>

#include "game.h"

// initialize the application
IMPLEMENT_APP(MainApp)

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit()
{
	SetTopWindow( new GameDisplay( nullptr ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

GameDisplay::GameDisplay(wxWindow *parent) : MainFrameBase( parent )
{
    #include "willychr.h"
    std::vector<wxBitmap> fontBitmaps;
    this->fontlen=fontlen;
    
    for(int i=0; i<fontlen; i++)
    {
        wxBitmap bitmap(font[i]);
        this->font.push_back(bitmap);
        this->fontScaled.push_back(wxBitmap(bitmap.ConvertToImage().Scale(24,24,wxIMAGE_QUALITY_HIGH)));
    }
    
    for(int c=0; c<256; c++)
        command[c]=STOP;
        
    command[(unsigned char)WXK_LEFT]=command[(unsigned char)WXK_NUMPAD_LEFT]=command[(unsigned char)WXK_NUMPAD4]=command['4']=LEFT;
    command[(unsigned char)WXK_RIGHT]=command[(unsigned char)WXK_NUMPAD_RIGHT]=command[(unsigned char)WXK_NUMPAD6]=command['6']=RIGHT;
    command[(unsigned char)WXK_UP]=command[(unsigned char)WXK_NUMPAD_UP]=command[(unsigned char)WXK_NUMPAD8]=command['8']=UP;
    command[(unsigned char)WXK_DOWN]=command[(unsigned char)WXK_NUMPAD_DOWN]=command[(unsigned char)WXK_NUMPAD2]=command[(unsigned char)WXK_NUMPAD5]=command['2']=command['5']=DOWN;
    command[' ']=JUMP;
    command[27]=QUIT;
    
    try
    {
        game=new Game("../WILLY.DAT", this);
        //setGame(nullptr,40,24);
        needrefresh=true;
        m_panel1->Refresh();
        gameTimer.Start(300);
        m_menuBar->Enable(wxID_ABORT, true);
    } catch(const std::exception &e)
    {
        std::ofstream file("debug.txt"); 
        file << "Oops " << e.what() << std::endl;
        file.close();
        Destroy();
    }
}

GameDisplay::~GameDisplay()
{
}

void GameDisplay::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

void GameDisplay::OnExitClick(wxCommandEvent& event)
{
	Destroy();
}

void GameDisplay::onAbortGame(wxCommandEvent& event)
{
	newCommand=QUIT;
}

void GameDisplay::onKeyDown(wxKeyEvent& event)
{   int key=event.GetKeyCode();
    newCommand=command[(unsigned char)key];
}

void GameDisplay::onPaint( wxPaintEvent& event ) 
{   if(!buffer) return;

    int scale=spritesize->GetValue();
    int max_row, max_column;

    m_panel1->GetSize(&max_column, &max_row);
    max_row/=scale;
    max_column/=scale;

    wxPaintDC draw(m_panel1);
    
    //draw.DrawBitmap(fontScaled[1],3*scale,5*scale);
    
    //if(needrefresh)
    if(true)
    {   if(columns-corner_column<(unsigned)max_column) max_column=columns-corner_column;
        if(rows-corner_row<(unsigned)max_row) max_row=rows-corner_row;
        
        for(int col=0; col<max_column; col++)
            for(int row=0; row<max_row; row++)
            {
                unsigned c=bufferEntry(row+corner_row,col+corner_column);
                if(c<fontlen)
                    draw.DrawBitmap(fontScaled[127],col*scale,row*scale);
                else
                    draw.DrawBitmap(fontScaled[c-fontlen],col*scale,row*scale);
            }
        
        needrefresh=false;
    } else
    {
        for(unsigned a: bufferUpdates)
        {
            unsigned row=a/columns, col=a-row*columns;
            if(col>=corner_column && row>=corner_row && col<corner_column+max_column && row<corner_row+max_row)
            {
                unsigned c=bufferEntry(row,col);
                if(c<fontlen)
                    draw.DrawBitmap(fontScaled[127],col*scale,row*scale);
                else
                    draw.DrawBitmap(fontScaled[c-fontlen],col*scale,row*scale);
            }
        }
    }
        
    bufferUpdates.clear();
}

void GameDisplay::onSpriteSizeUpdate( wxScrollEvent& event )
{   int scale=spritesize->GetValue();

    for(unsigned i=0; i<fontlen; i++)
    {
        this->fontScaled[i]=wxBitmap(font[i].ConvertToImage().Scale(scale,scale,wxIMAGE_QUALITY_HIGH));
    }
    
    corner_column=corner_row=0;
    needrefresh=true;
    m_panel1->Refresh(true);
}

void GameDisplay::onSpeedUpdate( wxScrollEvent& event ) 
{ 
}

/**
* @brief Write the character c at the row and column of the virtual map.
*        The character is stored in a buffer in case the map is scrolled.
* @param row The row of the location, 0 the top row.
* @param column The column of the location, 0 the left column.
* @param c The character to write.
*/
void GameDisplay::writeAt(unsigned row, unsigned column, const chtype c)
{
    int scale=spritesize->GetValue();
    bufferEntry(row,column)=c;
    bufferUpdates.push_back(row*columns+column);
    m_panel1->RefreshRect(wxRect((column-corner_column)*scale,(row-corner_row)*scale,scale,scale),false);
}

/**
* @brief Write the message on the status line at the specified 
*        location. 
* @param row Not used.
* @param column The offset in the status line to write at.
* @param s The message.
*/
void GameDisplay::writeAt(unsigned row, unsigned column, const std::string s)
{
    std::string current=m_statusBar->GetStatusText().ToStdString();
    
    while(current.length()<column+s.length()+1)
        current=current+"                              ";
    
    m_statusBar->SetStatusText(current.substr(0,column)+s+current.substr(column+s.length()));
}

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
void GameDisplay::center(unsigned row, unsigned column,
        unsigned border, unsigned scroll)
{   int scale=spritesize->GetValue();
    int max_row, max_column, move=corner_row+border-row;

    m_panel1->GetSize(&max_column, &max_row);
    max_row/=scale;
    max_column/=scale;

    if(max_row<(int)rows)
    {   if(corner_row && move>0)
        {   move+=scroll-(move-1)%scroll-1;
            if((int)corner_row>move)
                corner_row-=move;
            else
                corner_row=0;

            needrefresh=true;
        } else if(corner_row<rows-max_row&&(move=row-corner_row-max_row+border)>0)
        {   move+=scroll-(move-1)%scroll-1;
            if((int)(corner_row+max_row)-(int)rows>move)
                corner_row+=move;
            else
                corner_row=rows-max_row;
        
            needrefresh=true;
        }
    }

    if(max_column<(int)columns)
    {   if(corner_column && (move=corner_column+border-column)>0)
        {   move+=scroll-(move-1)%scroll-1;
            if((int)corner_column>move)
                corner_column-=move;
            else
                corner_column=0;

            needrefresh=true;
        } else if(corner_column<columns-max_column&&(move=column-corner_column-max_column+border)>0)
        {   move+=scroll-(move-1)%scroll-1;
            if((int)(corner_column+max_column)-(int)columns>move)
                corner_column+=move;
            else
                corner_column=columns-max_column;

            needrefresh=true;
        }
    }
        
    if(needrefresh)
        m_panel1->Refresh();
}
        
/**
* @brief Get the command that was most recently issued by the user.
* @return The current command.
*/
Command GameDisplay::getCommand()
{
    return newCommand;
}

/**
* @brief  Set the next command, as if the user had entered it.
* @param command The new current command.
*/
void GameDisplay::setCommand(Command command)
{
    newCommand=command;
}

void GameDisplay::onTick( wxTimerEvent& event )
{
    int s=-speed->GetValue();
    if(gameTimer.GetInterval()!=s)
        gameTimer.Start(s);
        
    if(!game->clock())
        Destroy();
}