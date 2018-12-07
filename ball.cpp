//#include <sstream>
//#include <string>
//
//#include "ball.h"
//
//using namespace std;
//
//void Ball::gainLife()
//{ lives++;
//  // TODO: Alert the user of the new life.
//}
//
//void Ball::die(Game *game)
//{ if(lives==0)
//    alive=false;
//  else
//    lives--;
//
//  x=startCol;
//  y=startRow;
//  game->reset();
//}
///*
//void Ball::addScore(Game *game, int points)
//{ if(points>=0 || score>=(unsigned)-points)
//    score+=points;
//  else
//  { score=0;
//    die(game);
//  }
//
//  residual+=points;
//
//  while(residual>=3000)
//  { residual-=3000;
//    gainLife();
//  }
//}
////*/
//bool Ball::clock(Game *game)
//{ unsigned oldX=x,
//           oldY=y;
//  bool moved=false;
//
//  // Check whether can do anything
//  if(!alive)
//    return false;
//
//  // Check for anything active where we were.
//  checkin(game);
//  if(jumpstate==7) jumpstate=6;
//
//  // Are we falling?
//  if(!jumpstate && !game->hasClimbable(this, (int)y, (int)x)
//     && !game->hasSolid(this, (int)y+1, (int)x))
//    y++;
//  /*  
//  else
//    // Can we legally carry out the command?
//    if(!ifLegal(game,game->getCommand()))
//      if(!ifLegal(game,dir))
//        ifLegal(game,STOP);
//  */  
//  // Check for anything active where we are.
//  if(oldX!=x || oldY!=y)
//  { checkout(game, oldX!=x && oldY==y, oldX);
//    moved=true;
//  }
//
//  if(jumpstate) jumpstate--;
//
//  return moved;
//}
//
///* Check to see if there is anything active at our location. */
//void Ball::checkin(Game *game)
//{ game->touch(this);
//
//  if(game->hasSpringy(this, y, x))
//  { jumpstate=7;
//    jumpbase=y;
//  } else
//    if(game->hasClimbable(this, y, x) && jumpstate)
//    { jumpstate=0;
//      dir=STOP;
//      game->setCommand(STOP);
//    }
//}
//
///* Check to see if there is anything active at our location. */
//void Ball::checkout(Game *game, bool stepOff, unsigned oldX)
//{ if(stepOff) game->stepOff(this, oldX);
//  game->touch(this);
//
//  if(jumpstate)
//    for(unsigned i=jumpbase; i>y; i--)
//      game->jumpOver(this, (int)i);
//
//  if(game->hasSpringy(this, y, x))
//  { jumpstate=7;
//    jumpbase=y;
//  } else
//    if(game->hasClimbable(this, y, x) && jumpstate)
//    { jumpstate=0;
//      dir=STOP;
//      game->setCommand(STOP);
//    }
//}
