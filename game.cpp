#include <curses.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "game.h"
#include "worm.h"

using namespace std;

// Macro to access the contents by row and column
#define LEVEL(r,c) (this->level[(r)*(this->columns)+(c)])

#ifndef BONUSFRAMES
#define BONUSFRAMES 14
#endif

#ifndef FRAMELEN
#define FRAMELEN (1500000000/BONUSFRAMES)
#endif


GameElement::GameElement(const chtype name) : name(name) {}
GameElement::GameElement() : name(0) {}

#define FIXACS(c,code) ((code)?(code):(c))
Game::Game(const char *level) :
  rows(0), columns(0), startRow(0), startCol(0)
{ list<string> contents;
  ifstream file(level);
  string line;
  size_t pos, r=0;

  init();

  for(int i=0; i<256; i++)
    catalog[i]=NULL;

  // Define special characters and the symbols they create
  catalog['-']=new SolidGameElement('-');
  catalog['=']=new SolidGameElement(FIXACS('=',ACS_HLINE));
  catalog['|']=new SolidGameElement(FIXACS('|',ACS_VLINE));
  catalog['<']=new SolidGameElement(FIXACS('+',ACS_ULCORNER));
  catalog['>']=new SolidGameElement(FIXACS('+',ACS_URCORNER));
  catalog['[']=new SolidGameElement(FIXACS('+',ACS_LLCORNER));
  catalog[']']=new SolidGameElement(FIXACS('+',ACS_LRCORNER));
  catalog['.']=new SolidGameElement(FIXACS('-',ACS_TTEE));
  catalog[';']=new SolidGameElement(FIXACS('|',ACS_LTEE));
  catalog[':']=new SolidGameElement(FIXACS('|',ACS_RTEE));
  catalog[',']=new SolidGameElement(FIXACS('-',ACS_BTEE));
  catalog['5']=new SolidGameElement(FIXACS('+',ACS_PLUS));

  // Make background elements where agents will start.
  catalog['&']=new GameElement(' ');

  // For everything else, make a standard non-active element.
  for(int i=0; i<256; i++)
    if(!catalog[i])
      catalog[i]=new GameElement(isgraph(i)?i:' ');

  // Read the level from the file.
  while(getline(file,line))
  { contents.push_back(line);  

    if(line.size()>columns)
      columns=line.size();

    pos=line.find('&'); // Look for Willy's Home.
    if(pos!=string::npos)
    { startRow=rows; // If found, update the start location.
      startCol=pos;
    }

    rows++;
  }

  // Create the display and map.
  display=new GameDisplay(columns, rows);
  this->level=new list<GameElement *>[rows*columns];
  for(list<string>::iterator i=contents.begin(); i!=contents.end(); i++)
  { for(pos=0; pos<i->size(); pos++)
    { GameElement *e=catalog[(unsigned char)(*i)[pos]];
    
      LEVEL(r,pos).push_back(e);
      e->draw(display, r, pos);
    }

    while(pos<columns)
    { GameElement *e=catalog[' '];
    
      LEVEL(r,pos).push_back(e);
      e->draw(display, r, pos++);
    }

    r++;
  }

  // Create willy
  willy=new Worm('&',startRow, startCol);
  agents.push_front(willy);
  LEVEL(startRow, startCol).push_back(willy);
  willy->draw(display);
  display->center(startRow, startCol, 10, 1);

  reset();
  showStatus();
  display->clock(this);
}

Game::~Game()
{ delete[](level);
  delete(display);

  for(int i=0; i<256; i++)
    delete(catalog[i]);

  for(list<GameAgent *>::iterator i=agents.begin();
      i!=agents.end();
      i++)
    delete(*i);
}

void Game::reset()
{ bonus=101*BONUSFRAMES;
}

int Game::getBonus()
{ return (bonus/BONUSFRAMES)*10;
}

bool Game::clock()
{ int y, x;
  
  command=display->getCommand();

  //display->writeAt(rows,0,commandName());

  if(command==QUIT)
    return false;

  if(bonus--==0)
  { y=willy->getRow();
    x=willy->getCol();
    willy->die(this);
    LEVEL(willy->getRow(), willy->getCol()).push_back(willy);
    LEVEL(y, x).remove(willy);
    willy->draw(display);
    LEVEL(y, x).back()->draw(display, y, x);
  }

  for(list<GameAgent *>::iterator i=agents.begin();
      i!=agents.end();
      i++)
  { y=(*i)->getRow();
    x=(*i)->getCol();
    if((*i)->clock(this))
    { LEVEL((*i)->getRow(), (*i)->getCol()).push_back(*i);
      LEVEL(y, x).remove(*i);
      (*i)->draw(display);
      LEVEL(y, x).back()->draw(display, y, x);
    }
  }

  display->center(willy->getRow(), willy->getCol(), 10, 1);
  showStatus();
  display->clock(this);
  return willy->isAlive();
}

void Game::showStatus()
{ ostringstream statline;

  statline << "  Bonus ";
  statline.width(4);
  statline << getBonus();
  statline.width(1);
  statline << "  ";
  statline << willy->getStatus();
  display->writeAt(rows, 0, statline.str());
}

bool Game::hasSolid(GameAgent *agent, int row, int col)
{ ActiveGameElement *a;

  if(!valid(row,col)) return true;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i))
       && a->isSolid())
      return true;

  return false;
}
  
bool Game::hasSpringy(GameAgent *agent, int row, int col)
{ ActiveGameElement *a;

  if(!valid(row,col)) return true;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i))
       && a->isSpringy())
      return true;

  return false;
}
  
bool Game::hasClimbable(GameAgent *agent, int row, int col)
{ ActiveGameElement *a;

  if(!valid(row,col)) return false;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i))
       && a->isClimbable())
      return true;

  return false;
}
  
void Game::stepOff(GameAgent *agent, int col)
{ unsigned row=agent->getRow()+1;
  ActiveGameElement *a;

  if(!valid(row, col)) return;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i)))
      a->stepOff(this, agent);
}
  
void Game::touch(GameAgent *agent)
{ unsigned row=agent->getRow(),
           col=agent->getCol();
  ActiveGameElement *a;

  if(!valid(row, col)) return;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i)))
      a->touch(this, agent);
}
  
void Game::jumpOver(GameAgent *agent, int row)
{ unsigned col=agent->getCol();
  ActiveGameElement *a;

  if(!valid(row, col)) return;

  list<GameElement *> &e=LEVEL(row, col);

  for(list<GameElement *>::iterator i=e.begin(); i!=e.end(); i++)
    if(*i!=agent && (a=dynamic_cast<ActiveGameElement *>(*i)))
      a->jumpOver(this, agent);
}

void Game::start()
{ timespec nextcycle, remain;
  long frame=FRAMELEN, slop=frame/10;
  //ostringstream msg;
  
  // Initialize a timer that can accurately report very small time diffs.
  clock_gettime(CLOCK_MONOTONIC, &nextcycle);

  for(;;)
  { int stat=-1;
  
    //display->writeAt(rows, 10, "clk");

    if(!clock())
      return;

    nextcycle.tv_nsec+=frame;
    remain.tv_nsec=frame;

    //display->writeAt(rows, 10, "adv");
    while(nextcycle.tv_nsec>=999999999)
    { nextcycle.tv_sec++;
      nextcycle.tv_nsec-=1000000000;
    }

    //msg.str("sleep");
    //msg << nextcycle.tv_sec << "." << nextcycle.tv_nsec;
    //display->writeAt(rows, 20, msg.str());
    while(stat<0 && remain.tv_nsec>slop)
      stat=clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                           &nextcycle, &remain);
  }
}

/* Replace a GameElement at a given location.
 * Input:
 *   row, col: location
 *   e: GameElement to replace
 *   name: name of replacement element
 */
void Game::replace(unsigned row, unsigned col, GameElement *old, chtype c)
{ // Find the new element in the catalog
  GameElement *e=catalog[(unsigned char)c];

  // Find the list for this location in the level
  list<GameElement *>& location=LEVEL(row, col);

  // Replace the element in the list.
  std::replace(location.begin(), location.end(), old, e);

  // Update the display
  // TODO: broken! this code is only valid in the constructor....
  e->draw(display, row, col);
}

/*
int main(int argc, char *argv[])
{ Game *game=new Game(argc>1?argv[1]:"willy1.txt");

  game->start();
  delete(game);
}
*/