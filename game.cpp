#include <curses.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <cstring>

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
Game::Game(const char *map, MainFrame display) :
  rows(24), columns(40), startRow(0), startCol(0)
{   list<string> contents;
    ifstream file(map,ios::binary);
    char page[rows*columns], 
         *pos;
    size_t r=0;

    init();

    catalog[0]=new GameElement(127);
    catalog[1]=new GameElement(127);
    
    for(int i=2; i<51; i++)
        catalog[i]=new GameElement(i);
        
    for(int i=51; i<91; i++)
        catalog[i]=new SolidGameElement(i);
        
    for(int i=91; i<128; i++)
        catalog[i]=new GameElement(i);
    
    for(int i=128; i<256; i++)
        catalog[i]=new GameElement(127);

    // Read the level from the file.
    while(file.read(page,rows*columns))
    {   levels.push_back(std:vector<GameElement*>(rows));
        level=new (GameElement *)[]
    
        for(int r=0; r<rows; r++)
        {
            GameElement **line=new (GameElement*)[columns];
            for(int c=0, p=r; c<columns; c++, p+=rows)
                line[c]=catalog[page[p]];
            level->push_back(line);
        }
    }
    
    level=&levels.front();
    
    pos=std::memchr(page,0,24*40); // Look for Willy's Home.
    if(pos!=nullptr)
    { 
        startCol=(pos-page)/24; // If found, update the start location.
        startRow=pos-page-startCol*24;
    }
  

    // Create the display and map.
    for(list<string>::iterator i=contents.begin(); i!=contents.end(); i++)
    {   for(pos=0; pos<i->size(); pos++)
        {   GameElement *e=catalog[(unsigned char)(*i)[pos]];
    
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