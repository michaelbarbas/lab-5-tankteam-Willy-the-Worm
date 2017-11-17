#include <curses.h>
#include <pthread.h>

#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "game.h"
#include "display.h"

using namespace std;

#define BUFFER(y,x) (buffer[((y)*width)+(x)])
void *commandThread(void *arg)
{ GameDisplay *disp=(GameDisplay *)arg;

  for(;;)
    if(!disp->checkInput())
      pthread_exit(NULL);
}

void GameDisplay::setCommand(Command c)
{ pthread_mutex_lock(&inputLock);
  newCommand=c;
  pthread_mutex_unlock(&inputLock);
}

bool GameDisplay::checkInput()
{ chtype input=wgetch(win);

  pthread_mutex_lock(&inputLock);
  if(done) return false;
  newCommand=command[(unsigned char)input];
  //writeAt(0,0,((input>>4)&0xf)+'0');
  //writeAt(0,1,(input&0xf)+'0');
  pthread_mutex_unlock(&inputLock);
  return true;
}

static pthread_mutex_t tempLock=PTHREAD_MUTEX_INITIALIZER;

GameDisplay::GameDisplay(unsigned width, unsigned height)
: width(width), height(height), newCommand(STOP)
{ buffer=new chtype[width*height];
  win=initscr();
  scrollok(win, false);
  curs_set(0);
  cbreak();
  noecho();

  command['D']=command['4']=LEFT;
  command['C']=command['6']=RIGHT;
  command['A']=command['8']=UP;
  command['B']=command['2']=command['5']=DOWN;
  command[' ']=JUMP;
  command[27]=QUIT;

  inputLock=tempLock;
  pthread_mutex_init(&inputLock, NULL);
  pthread_create(&inputThread, NULL, commandThread, this);
}


GameDisplay::~GameDisplay()
{ endwin();
  delwin(win);
  pthread_mutex_lock(&inputLock);
  done=true;
  pthread_mutex_unlock(&inputLock);
  pthread_cancel(inputThread);
  pthread_join(inputThread, NULL);
  pthread_mutex_destroy(&inputLock);
  delete[](buffer);
}

void GameDisplay::writeAt(unsigned row, unsigned column, const chtype c)
{ //cout << "mvwaddch(" << win << ',' << row-y0 << ',' <<  column-x0 << ',' << c << endl;
  BUFFER(row, column)=c;
  mvwaddch(win, row-y0, column-x0, c);
}

void GameDisplay::writeAt(unsigned row, unsigned column, const string s)
{ //cout << "mvwaddch(" << win << ',' << row-y0 << ',' <<  column-x0 << ',' << c << endl;
  mvwaddstr(win, row, column, s.c_str());
  wrefresh(win);
}

void GameDisplay::center(unsigned row, unsigned column,
                         unsigned border, unsigned scroll)
{ int maxY, maxX, move=y0+border-row;
  bool needrefresh=false;

  getmaxyx(win, maxY, maxX);

  if(y0 && move>0)
  { if((int)y0>move)
      y0-=move+scroll-(move-1)%scroll-1;
    else
      y0=0;

    needrefresh=true;
  } else
    if(y0<height-maxY&&(move=row-y0-maxY+border)>0)
    { if((int)(y0+maxY)-(int)height>move)
        y0+=move+scroll-(move-1)%scroll-1;
      else
        y0=height-maxY;
    
      needrefresh=true;
    }

  if(x0 && (move=x0+border-column)>0)
  { if((int)x0>move)
      x0-=move+scroll-(move-1)%scroll-1;
    else
      x0=0;

    needrefresh=true;
  } else
    if(x0<width-maxX&&(move=column-x0-maxX+border)>0)
    { if((int)(x0+maxX)-(int)width>move)
        x0+=move+scroll-(move-1)%scroll-1;
      else
        x0=width-maxX;

      needrefresh=true;
    }

  if(needrefresh)
  { if(width<(unsigned)maxX) maxX=width;
    if(height<(unsigned)maxY) maxY=height;
    
    for(int i=0; i<maxX; i++)
      for(int j=0; j<maxY; j++)
        mvwaddch(win, j, i, BUFFER(j+y0, i+x0));
  }
}

bool GameDisplay::clock(Game *game)
{ wrefresh(win);
  return true;
}

Command GameDisplay::getCommand()
{ Command c;

  pthread_mutex_lock(&inputLock);
  c=newCommand;

  if(newCommand>=JUMP)
    newCommand=lastCommand;

  pthread_mutex_unlock(&inputLock);

  if(newCommand<JUMP)
    lastCommand=c;

  //writeAt(0,2,c+'A');
  return c;
}

#ifdef TESTDISPLAY
int main()
{ GameDisplay *disp=new GameDisplay(40,24);
  string msg="Welcome to Willy!";
  string pause;

  disp->writeAt(10, 10, "Welcome to Willy!");

  cin >> pause;
}
#endif
