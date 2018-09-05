//Desc: Generates alarms at random times. Allowing user to 
//add alarms by killing current alarm

#include <stdio.h>     
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include "SignalAlarm.h"

//Global variables

int num; 			//Number of alarms used 
time_t t[20];             	// keeps track of the alarm times
int i, j, k;
int count=0;			//Counts the number of alarms


//Function prototypes

int Min(time_t t[]); 
void alrmHandler(int sig); 
void sigUsrHandler(int sig); 

int main (int argc, char *argv[]) 
{

         int err, n= -1, minimum;
         pid_t cpid;
         time_t currentTime;

        //display process id in the beginning

         cpid = getpid();
         printf("PID = %d\n", cpid);
        n = atoi(argv[1]);

        if(n < 1)
        {
                exit(1);
        }

         struct sigaction alrm_act;

         alrm_act.sa_handler = (void(*)(int))alrmHandler;
        
         alrm_act.sa_flags = 0;

        //points to the alrmHandler when the SIGALRM signal is sent

         err = sigaction (SIGALRM, &alrm_act, NULL);
         if (err == -1) 
         {
	         printf ("Error in SIGALRM = %d.\n", err);
        	 exit (2);
         }
        
	 struct sigaction sigusr;
         sigusr.sa_handler = (void(*)(int))sigUsrHandler;
        
         sigusr.sa_flags = 0;

   
        //points to the sigUsrHandler when the SIGUSR1 signal is sent

         err = sigaction (SIGUSR1, &sigusr, NULL);

         if (err == -1) 
         {
 	        printf ("Error in SIGUSR1 = %d.\n", err);
        	 exit (3);
         }

	        printf("\n");
        	 num = n;
         	currentTime = time((time_t*) NULL);

        //Initializing the alarms & printing them in the next for loop 

         printf("time = %ld: initial list = \n", currentTime);

         for (i = 0; i < n; i++) 
         {
	         int m = rand() % MAX_TIME;
        	 t[i] = (time_t) (currentTime + m);
         }

         for (j = 0; j < n; j++) 
         {
 	        printf("(%d, %ld) ", j+1, t[j]);
         }

	        printf("\n");

	        int rem;
        		//Alarm Occurrences

	while (count != num) 
        {
        	 time_t curTime;

	         minimum = Min(t);
        	 curTime = time((time_t*) NULL);
         	 rem = t[minimum] - curTime;
         	 printf("time = %ld: Waiting for alarm %d in %d seconds alarm time = %ld", curTime, minimum+1, rem, t[minimum]); 

         if (t[minimum] == time(NULL)) 
          {
                  printf("\ntime = %ld: The two alarms are at the same time.\n", time(NULL));
                  count++;
                  return;
         }

            err = alarm ((unsigned int) (t[minimum] - time(NULL)));
   
            err = pause();
      
              if(err == -1 && errno != EINTR)
                {
                        printf("error(%d) trying to pause, ",err);
                        printf("errno = %d\n", errno);

                        exit(2);
                }
    } 

    exit (0);

}

//Sorting

int Min(time_t t[]) 
{

         int i;
         int min = 0;

         for (i = 0; i < num; i++ ) 
        {

         if (t[min] > t[i] )
                     min = i;

         }

         return min;
}


//Alarm Handler

void alrmHandler(int sig) 
{
        //catches the ALRM signal here
        printf("\n"); 
        printf("time = %ld: Alarm occured\n", time((time_t*) NULL) );
        t[Min(t)] = time(NULL) + MAX_TIME * 50;
         count++;
}


//SIGUSR1 Handler

void sigUsrHandler(int sig) 
{
         int err;
         time_t alarm;
 
        printf("\ntime = %ld : SIGUSR1 caught\n", time((time_t*) NULL) );

        alarm = (time_t) (time(NULL) + rand( ) % MAX_TIME + 1);
        num++;							//increment as an alarm is created
         
	printf("time = %ld : alarm %d added, alarm time = %ld\n", time(NULL), num, alarm);
         t[num-1] = alarm;

}