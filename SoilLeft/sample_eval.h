/********************************* Submit Sample*********************
 * Copy this code for recording the samples and send the command 
 * to sync node based on the threshold value. 
 * *******************************************************************/

#define NORMAL_LEVEL    45	//Your threshold for normal level
#define MAX_SAMPLE      10	//The sample buffer size
#define CONTINUOUS_HIGH_SAMPLES    5
#define CONTINUOUS_LOW_SAMPLES    5

static int samples_store[MAX_SAMPLE] = { 0 };	//Sample store 

//static int avg_store[CONTINUOUS_HIGH_SAMPLES] = { 0 };

static struct unicast_message msg;
static int is_on = FALSE;
int submitSample(int sample)
{
	int i = 0;
    int seconds = 0;
	int sum = 0;
	int average = 0;
	//Slide the window for accomodating the new sample
	for (i = 0; i < MAX_SAMPLE - 1; i++) {
		samples_store[i] = samples_store[i + 1];
	}
	//Store the new sample
	samples_store[i] = sample;
	//Calculate the sample average
	printf("\n");

	for (i = 0; i < MAX_SAMPLE; i++) {
		//printf("%d ", samples_store[i]);
		sum += samples_store[i];
	}
	average = sum / MAX_SAMPLE;
    seconds = clock_seconds();
	printf("%d \t%d \t%d", seconds, sample, average);
	//If average is above normal level then analyse last 5 received samples
	if (average > NORMAL_LEVEL) {
		//Check if the last five sample values were above normal level                  
		int count_high = 0;

		for (i = 5; i < MAX_SAMPLE; i++) {
			if (samples_store[i] > NORMAL_LEVEL) {
				count_high++;
			}
		}
		//printf("count_high: %d ", count_high);
		if (count_high >= CONTINUOUS_HIGH_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_HEATER_ON;
			msg.value = average;
			//send_to_sync(&msg);    //Need to be removed
			if (is_on == FALSE) {
				send_to_sync(msg);
				is_on = TRUE;
			}
		}
	} else if (average < NORMAL_LEVEL) {
		//Check if the last five sample values were below normal level  
		int count_low = 0;
		for (i = 5; i < MAX_SAMPLE; i++) {
			if (samples_store[i] < NORMAL_LEVEL) {
				count_low++;
			}
		}
		//printf(" countlow %d ", count_low);
		if (count_low >= CONTINUOUS_LOW_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_HEATER_OFF;
            msg.value = average;			
			//send_to_sync(&msg);    //Need to be removed
			if (is_on == TRUE) {
				send_to_sync(msg);
				is_on = FALSE;
			}
		}
	}	
	return sum / MAX_SAMPLE;
}
