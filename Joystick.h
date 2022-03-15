#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
//#include <sys/select.h>
//#include <signal.h>
#include <linux/joystick.h>
#include <linux/input-event-codes.h>

int jfd;		// Joystick File Descriptor
int joyX;
int joyY;
int joyZ;

int joystickAvailable();
int getJoystick();
int initJoystick(char * mpath);


int initJoystick(char * jpath)
{
        if ((jfd = open(jpath, O_RDONLY)) < 0) 
        {
                return 1;
        }

}

// Returns:
//   0    no joystick event available.
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

int getJoystick()
{
  int i;
  size_t rb;
  struct js_event ev[64];
  int retval;
  
  retval=0;
  if (joystickAvailable())
  {
    rb=read(jfd,ev,sizeof(struct js_event)*64);
    for (i = 0;  i <  (rb / sizeof(struct js_event)); i++)
    {
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_TRIGGER)) retval=1+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_THUMB)) retval=2+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_THUMB2)) retval=3+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_TOP)) retval=4+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_TOP2)) retval=5+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_PINKIE)) retval=6+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE)) retval=7+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE2)) retval=8+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE3)) retval=9+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE4)) retval=10+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE5)) retval=11+(128*ev[i].value);
      if((ev[i].type==EV_KEY) & (ev[i].code==BTN_BASE6)) retval=12+(128*ev[i].value);
      if((ev[i].type==EV_REL) & (ev[i].code==REL_X)) {                              //X Movement
        joyX=joyX+ev[i].value;
        retval=128;
      }
      if((ev[i].type==EV_REL) & (ev[i].code==REL_Y)) {                              //Y Movement
        joyY=joyY+ev[i].value;
        retval=127;
      }
      if((ev[i].type==EV_REL) & (ev[i].code==REL_Z)) {                              //Z Movement
        joyZ=joyZ+ev[i].value;
        retval=126;
      }

    }
  
  }
 return retval;   
}


int joystickAvailable()
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(jfd, &fds);
  select(jfd+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(jfd, &fds));
}
