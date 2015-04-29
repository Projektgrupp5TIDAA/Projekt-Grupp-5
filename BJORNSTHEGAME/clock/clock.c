#include <stdio.h>
#include <time.h>

 
int main ()
{
	unsigned int time_milliseconds = 0;
	unsigned int time_seconds = 0;
	
	unsigned int totaltime = 0, countdown_secs = 0, time_left = 0;
	//checks if seconds has changed
	unsigned int check;
 
	clock_t startTime, counter;
	countdown_secs = 120;

 
    startTime = clock();
    time_left = countdown_secs - time_seconds;		//uppdates timer
    check = time_left - 1;	//flag

	while (time_left>0) 
	{
		counter = clock(); // update timer difference
		time_milliseconds = counter - startTime;
		time_seconds = (time_milliseconds/(CLOCKS_PER_SEC));

		time_left = countdown_secs - time_seconds; // subtract to get difference 
		
		//prints out seconds if time_seconds has changed
		if(time_left==check)
		{
			check = check - 1;
			printf("%d\n", time_left);
		}
	}
 
	return 0;
}