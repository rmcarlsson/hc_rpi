/*
 * main.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "fwd_ctrl.h"
#include "esbe3_drv.h"
#include "mylog.h"
#include "sig.h"

int version_ma = 1;
int version_mi = 0;
int version_build = 0;

int main (int argc, char *argv[])
{

  FILE* pid_file_f = NULL;

  //ml_open("ha.log");

  signal(SIGTERM, termination_handler);

  ml_log(LOG_NOTICE, "Started. Version %d.%d build %d. Pid is %d",
	 version_ma,
	 version_mi,
	 version_build,
	 (int)(getpid()));


  pid_file_f = fopen("ha_ng.pid", "w");
  if( pid_file_f == NULL )  {
    ml_log(LOG_NOTICE, "Unable to open /var/run/ha_ng.pid, exiting");
    exit (1);
  }
  fprintf(pid_file_f, "%d\n", (int)(getpid()));
  fclose(pid_file_f);

  init_esbe3_drv();
  FwdCtrl_init();

  int tick = 0;
  while(1) {
    tFwdCtrl();
    sleep(60);
    if (tick > (60 * 12))
    {
      FwdCtrl_adjust_target();
      tick = 0;
    }
    tick++;
  }

  ml_close();
  exit(0);
}


