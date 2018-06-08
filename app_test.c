// Interrupt test for 20m sec interrupt simulate.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "sim_intr.h"

#define INTERVAL 5
#define MAXCOUNT 100000

void longrun_test_frm_INT(void ) ;

void longrun_test_frm_INT()
{
        int fd;
        ssize_t rc;
        int i = 0 ;

        intfrm_info interrupt_info_frm;
        fd = open ("/dev/intfrm", O_RDONLY);
        if (fd < 0) {
                printf("File Open Error !!! (%d): %m\n", errno);
                return ;
        }
        memset(&interrupt_info_frm, 0, sizeof(interrupt_info_frm));
        while(1)
        {
                i++;
                sleep(INTERVAL);
                rc=read(fd, &interrupt_info_frm, sizeof(interrupt_info_frm));
                if (rc < 0) {
                        printf("File read Error !!! (%d): %m\n", errno);
                        close(fd);
                        return ;
                }
                printf("[%dsec]4.615ms interrupt count : %d \n", i*INTERVAL,
                                interrupt_info_frm.intfrm_run_count) ;
                if (interrupt_info_frm.intfrm_run_count > MAXCOUNT)
                {
                        printf("4.615ms interrupt count is more than %d!!! longrun test done \n", MAXCOUNT) ;
                        printf("TICK is 4.615ms, Calculated TICK is %.3fms \n",
                                        (double)i*(double)INTERVAL/(double)interrupt_info_frm.intfrm_run_count*1000) ;
                        close(fd);
                        return ;
                }

        }

        close(fd);
}

int main (void)
{
       int no = 0 ;

        printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("+ 1)1.25ms 2)4.615ms 3)20ms interrupt test  +\n");
        printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("Input no : ");
        scanf("%d", &no);

        switch(no)
        {
                case 1 : longrun_test_frm_INT() ;
                                 return 0;
                default :
                                 return 0;
        }
}


