#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <signal.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>

int mfd;
int mouseScroll;
int mouseX;
int mouseY;

int mouseAvailable();
int getMouse();
int initMouse(char * mpath);


int initMouse(char * mpath)
{
        if ((mfd = open(mpath, O_RDONLY)) < 0) 
        {
                return 1;
        }

}

// Returns:
//   0    no mouse event available.
//   1    Left Button up,
//   2    Right Button up
//   3    Centre Button up
//   4    Side Button up
//   5    extra button up
//   125  X movement (and update mouseX value)
//   126  Y movement (and update mouseY value)
//   127  (Reserved for Z Movement)
//   128  scroll wheel move (and updates mouseScroll value)
//   129  Left Button Down
//   130  Right Button Down
//   131  Centre Button Down
//   132  Side Button Down
//   133  extra button Down

//   Add 128 to value for button down event

int getMouse()
{
	int i;
  size_t rb;
  struct input_event ev[64];
  int retval;
  
  retval=0;
  if (mouseAvailable())
  {
    rb=read(mfd,ev,sizeof(struct input_event)*64);
    for (i = 0;  i <  (rb / sizeof(struct input_event)); i++)
    {
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_LEFT)) retval=1+(128*ev[i].value);     //left button
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_RIGHT)) retval=2+(128*ev[i].value);    //right button 
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_MIDDLE)) retval=3+(128*ev[i].value);   //Center button
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_SIDE)) retval=4+(128*ev[i].value);     //Side button
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_EXTRA)) retval=5+(128*ev[i].value);    //Extra button  
      if((ev[i].type==EV_REL) & (ev[i].code==REL_WHEEL)) {                              //Scroll wheel
        mouseScroll=mouseScroll+ev[i].value;
        retval=128;
      }
      if((ev[i].type==EV_REL) & (ev[i].code==REL_X)) {                              //X Movement (Might need to be REL_RX)
        mouseX=mouseX+ev[i].value;
        retval=125;
      }
      if((ev[i].type==EV_REL) & (ev[i].code==REL_Y)) {                              //Y Movement (Might need to be REL_RY)
        mouseY=mouseY+ev[i].value;
        retval=126;
      }

    }
  
  }
 return retval;   
}


int mouseAvailable()  
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(mfd, &fds);
  select(mfd+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(mfd, &fds));
}
