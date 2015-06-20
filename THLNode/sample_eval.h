/********************************* Submit Sample*********************
 * Copy this code for recording the samples and send the command 
 * to sync node based on the threshold value. 
 * *******************************************************************/

#define NORMAL_LIGHT_LEVEL      140	//Your threshold for normal level
#define NORMAL_HUMIDITY_LEVEL   580
#define NORMAL_TEMP_LEVEL       28

#define MAX_SAMPLE      5	//The sample buffer size
#define CONTINUOUS_HIGH_SAMPLES    5
#define CONTINUOUS_LOW_SAMPLES    5

static int light_samples_store[MAX_SAMPLE] = { 0 };	//Sample store 

//static int avg_store[CONTINUOUS_HIGH_SAMPLES] = { 0 };

static struct unicast_message msg;
static int is_light_on = FALSE;
static int sample_count = 0;
int submitLightSample(int sample)
{
	int i = 0;
    int seconds = 0;
	int sum = 0;
	int average = 0;
	//Slide the window for accomodating the new sample
	for (i = 0; i < MAX_SAMPLE - 1; i++) {
		light_samples_store[i] = light_samples_store[i + 1];
	}
	//Store the new sample
	light_samples_store[i] = sample;
	//Calculate the sample average
    sample_count++;
    if(sample_count < MAX_SAMPLE){
        //printf("\nNot enough light samples\n");
        return 0;
    }
	//printf("\n");

	for (i = 0; i < MAX_SAMPLE; i++) {
		//printf("%d ", light_samples_store[i]);
		sum += light_samples_store[i];
	}
	average = sum / MAX_SAMPLE;
    
    //seconds = clock_seconds();
	//printf("%d \t%d \t%d", seconds, sample, average);
    //printf("\navg light: %d\n", average);
	//If average is above normal level then analyse last 5 received samples
	if (average > NORMAL_LIGHT_LEVEL) {
		//Check if the last five sample values were above normal level                  
		int count_high = 0;

		for (i = 0; i < MAX_SAMPLE; i++) {
			if (light_samples_store[i] > NORMAL_LIGHT_LEVEL) {
				count_high++;
			}
		}
		//printf("count_high: %d ", count_high);
		if (count_high >= CONTINUOUS_HIGH_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_LIGHT_OFF;
			msg.value = average;
			//send_to_sync(&msg);    //Need to be removed
			if (is_light_on == TRUE) {
				send_to_sync1(msg);
                send_to_sync2(msg);
				is_light_on = FALSE;
			}
		}
	} else if (average < NORMAL_LIGHT_LEVEL) {
		//Check if the last five sample values were below normal level  
		int count_low = 0;
		for (i = 0; i < MAX_SAMPLE; i++) {
			if (light_samples_store[i] < NORMAL_LIGHT_LEVEL) {
				count_low++;
			}
		}
		//printf(" countlow %d ", count_low);
		if (count_low >= CONTINUOUS_LOW_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_LIGHT_ON;
            msg.value = average;			
			//send_to_sync(msg);    //Need to be removed
			if (is_light_on == FALSE) {
                send_to_sync1(msg);
                send_to_sync2(msg);
				is_light_on = TRUE;
			}
		}
	}	
	return sum / MAX_SAMPLE;
}


//TEMPERATURE 

static int temp_samples_store[MAX_SAMPLE] = { 0 };	//Sample store 

//static int avg_store[CONTINUOUS_HIGH_SAMPLES] = { 0 };

//static struct unicast_message msg;
static int is_fan_on = FALSE;
int submitTempSample(int sample)
{
	int i = 0;
    int seconds = 0;
	int sum = 0;
	int average = 0;
	//Slide the window for accomodating the new sample
	for (i = 0; i < MAX_SAMPLE - 1; i++) {
		temp_samples_store[i] = temp_samples_store[i + 1];
	}
	//Store the new sample
	temp_samples_store[i] = sample;
	//Calculate the sample average
	//printf("\n");
    if(sample_count < MAX_SAMPLE){
        //printf("\nNot enough temp samples\n");
        return 0;
    }
	for (i = 0; i < MAX_SAMPLE; i++) {
		//printf("%d ", samples_store[i]);
		sum += temp_samples_store[i];
	}
	average = sum / MAX_SAMPLE;
    //printf("\navg temp: %d", average);
    //seconds = clock_seconds();
	//printf("%d \t%d \t%d", seconds, sample, average);
	//If average is above normal level then analyse last 5 received samples
    //printf("\nTemp avg: %d\n", average);
	if (average > NORMAL_TEMP_LEVEL) {
		//Check if the last five sample values were above normal level  
        //printf("\nTemp High\n");
		int count_high = 0;

		for (i = 0; i < MAX_SAMPLE; i++) {
			if (temp_samples_store[i] > NORMAL_TEMP_LEVEL) {
				count_high++;
			}
		}
		//printf("count_high: %d ", count_high);
		if (count_high >= CONTINUOUS_HIGH_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_FAN_ON;
			msg.value = average;
			//send_to_sync(&msg);    //Need to be removed
			if (is_fan_on == FALSE) {                
				send_to_sync1(msg);
                send_to_sync2(msg);
				is_fan_on = TRUE;
			}
		}
	} else if (average < NORMAL_TEMP_LEVEL) {
        //printf("\nTemp Low\n");
		//Check if the last five sample values were below normal level  
		int count_low = 0;
		for (i = 0; i < MAX_SAMPLE; i++) {
			if (temp_samples_store[i] < NORMAL_TEMP_LEVEL) {
				count_low++;
			}
		}
		//printf(" countlow %d ", count_low);
		if (count_low >= CONTINUOUS_LOW_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_FAN_OFF;
            msg.value = average;			
			//send_to_sync(msg);    //Need to be removed
			if (is_fan_on == TRUE) {                
				send_to_sync1(msg);
                send_to_sync2(msg);
				is_fan_on = FALSE;
			}
		}
	}	
	return sum / MAX_SAMPLE;
}

//HUMIDITY
static int humidity_samples_store[MAX_SAMPLE] = { 0 };	//Sample store 

//static int avg_store[CONTINUOUS_HIGH_SAMPLES] = { 0 };

//static struct unicast_message msg;
//static int is_light_on = FALSE;
int submitHumiditySample(int sample)
{
	int i = 0;
    int seconds = 0;
	int sum = 0;
	int average = 0;
	//Slide the window for accomodating the new sample
	for (i = 0; i < MAX_SAMPLE - 1; i++) {
		humidity_samples_store[i] = humidity_samples_store[i + 1];
	}
	//Store the new sample
	humidity_samples_store[i] = sample;
	//Calculate the sample average
	//printf("\n");

	for (i = 0; i < MAX_SAMPLE; i++) {
		//printf("%d ", samples_store[i]);
		sum += humidity_samples_store[i];
	}
	average = sum / MAX_SAMPLE;
    //printf("\t%d\n", average);
    //seconds = clock_seconds();
	//printf("%d \t%d \t%d", seconds, sample, average);
	//If average is above normal level then analyse last 5 received samples
    //printf("\nHumidity avg: %d\n", average);
	if (average > NORMAL_HUMIDITY_LEVEL) {
        //printf("\nHumidity High\n");
		//Check if the last five sample values were above normal level                  
		int count_high = 0;

		for (i = 0; i < MAX_SAMPLE; i++) {
			if (humidity_samples_store[i] > NORMAL_HUMIDITY_LEVEL) {
				count_high++;
			}
		}
		//printf("count_high: %d ", count_high);
		if (count_high >= CONTINUOUS_HIGH_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_FAN_ON;
			msg.value = average;
			//send_to_sync(msg);    //Need to be removed
			if (is_fan_on == FALSE) {
				send_to_sync1(msg);
                send_to_sync2(msg);
				is_fan_on = TRUE;
			}
		}
	} else if (average < NORMAL_HUMIDITY_LEVEL) {
         //printf("\nHumidity Low\n");
		//Check if the last five sample values were below normal level  
		int count_low = 0;
		for (i = 0; i < MAX_SAMPLE; i++) {
			if (humidity_samples_store[i] < NORMAL_HUMIDITY_LEVEL) {
				count_low++;
			}
		}
		//printf(" countlow %d ", count_low);
		if (count_low >= CONTINUOUS_LOW_SAMPLES) {
			msg.type = MSG_CMD;
			msg.cmd = CMD_FAN_OFF;
            msg.value = average;			
			//send_to_sync(msg);    //Need to be removed
			if (is_fan_on == TRUE) {
                send_to_sync1(msg);
                send_to_sync2(msg);
				is_fan_on = FALSE;
			}
		}
	}	
	return sum / MAX_SAMPLE;
}

