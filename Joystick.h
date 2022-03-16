#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <signal.h>
#include <linux/joystick.h>

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
//   1 .. 32   	Button 1-32 up
//   128  	Axis Movement (and updates joyX value)
//   129 .. 161 Button 1-32 Down

//   Add 128 to value for button down event

int getJoystick()
{
  int i;
  size_t rb;
  struct input_event ev[64];
  int retval;

  retval=0;
  if (joystickAvailable())
  {
    rb=read(jfd,ev,sizeof(struct input_event) * 64);
    for (i = 0;  i <  (rb / sizeof(struct input_event)); i++)
    {
//      if(js[i].type==JS_EVENT_BUTTON) retval=js[i].number+1+(128*js[i].value);
      if(ev[i].type==EV_ABS) {                              //X Movement
//        printf("Axis num [%d]: %d\n", js[i].number, js[i].value);
//        joyX=ev[i].value;
        if ( ev[i].code == ABS_X ) {
	  joyX=(int)(50+(ev[i].value/655));
          retval=128;
	}
        if ( ev[i].code == ABS_Y ) {
          joyY=(int)(50+(ev[i].value/655));
          retval=127;
        }
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
