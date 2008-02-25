// Mouse support library 

#define _NAIVE_DOS_REGS // make libary work with gcc
#include <dos.h>

int next_move = 0; // for Jerry's diagonal kludge


#define MOUSE_INT	0x33

static int MouseON = 0;

int mouse_init(void)
{
   union REGS	regs;
   
   /* intialise mouse */
   regs.x.ax = 0;
   int86(MOUSE_INT, &regs, &regs);
   
   /* mouse not present */
   if (regs.x.ax == 0)
      return -1;
   
   MouseON = 1;
   
   /* display mouse */
//   regs.x.ax = 1;
//   int86(MOUSE_INT, &regs, &regs);
   
   return 0;
}

      
int mouse_move(void)
{
   union REGS	regs;
   int dir = 0;
   
   if (!MouseON)
      return 0;

   // now the first part of the diagonal kludge
   if (next_move)
   {
       // next_move acts as a 1 event storage system.
       dir = next_move;
       next_move = 0;

       // and since we don't want to reset the mouse position
       //  here, just return from the routine.
       // if we were to reset the mouse position, (end of this function)
       //  then we could lose movement that happens between the last
       //  call and the next call to mouse_move()
       return dir;
   }
   
   /* get mouse position */
   regs.x.ax = 3;
   int86(MOUSE_INT, &regs, &regs);
   
   /* find mouse direction */
   if (regs.x.cx > 328)
      dir = 1; // no reason to or these anymore...
   
   if (regs.x.cx < 312)
      dir = 4;
   
   // now the second part of the diagonal kludge
   if (dir)
   {
       // there was a horizontal movement, so store the vertical
       //  for the next time mouse_move() is called
       if (regs.x.dx > 104)
	  next_move = 2;
       
       if (regs.x.dx < 96)
	  next_move = 8;
   } else {
       // there was no horizontal movement, so just return the
       //  vertical movment right away...
       if (regs.x.dx > 104)
	  dir = 2;
       
       if (regs.x.dx < 96)
	  dir = 8;
   }

   if (dir != 0)
   {
      /* reset mouse position */
      regs.x.ax = 4;
      regs.x.cx = 320;
      regs.x.dx = 100;
      int86(MOUSE_INT, &regs, &regs);
   }
   
   return dir;
}


int mouse_butt(void)
{
   union REGS	regs;

   if (!MouseON)
      return 0;
   
   /* get button info */
   regs.x.ax = 3;
   int86(MOUSE_INT, &regs, &regs);
   
   return regs.x.bx;
}

void wait_for_button_release(void)
{
    int initbutt;
    int currbutt;
    union REGS	regs;

    if (!MouseON)
	return;

    // first get the state
    regs.x.ax = 3;
    int86(MOUSE_INT, &regs, &regs);
    initbutt=regs.x.bx;
    currbutt=initbutt;

    while (currbutt & initbutt) {
	regs.x.ax = 3;
	int86(MOUSE_INT, &regs, &regs);
	currbutt=regs.x.bx;
    }
}
