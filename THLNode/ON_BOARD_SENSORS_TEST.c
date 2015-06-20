// include instruction for accessing contiki's libraries
#include "contiki.h"
#include <stdio.h>
#include "lib/sensors.h"
#include "net/rime.h"
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include "dev/sht11.h"
#include "rimeaddr.h"
#include "unicast_comm.h"
#include "sample_eval.h"

#define SAMPLE_INTERVAL 2
#define BEGIN_DELAY     150
#define ONBOARD_SENSORS 3

PROCESS(on_board_sensors_process, "on_board_sensors_test");
AUTOSTART_PROCESSES(&on_board_sensors_process);

float sht11_TemperatureC(int rawData)
{
	uint32_t rawTemp;	// Raw temperature value
	float tempC;		// Temperature derived by formula

	rawTemp = rawData;

	// Conversion coefficients from SHT11 datasheet for temperature
	const float T1 = -39.6;
	const float T2 = 0.01;

	// Convert raw value to degrees Celsius
	tempC = T1 + T2 * rawTemp;
	return tempC;
}

float sht11_Humidity(int temprawdata, int humidityrawdata)
{
	uint32_t rawHumidity;	// Raw humidity value returned from sensor
	float RHlinear;		// Humidity with linear correction applied
	float RHtrue;		// Temperature-corrected humidity
	float tempC;

	rawHumidity = humidityrawdata;
	tempC = sht11_TemperatureC(temprawdata);

	// Conversion coefficients from SHT11 datasheet for humidity
	const float H1 = -4.0;	// for 12 Bit
	const float H2 = 0.0405;	// for 12 Bit
	const float H3 = -0.0000028;	// for 12 Bit
	const float H4 = 0.01;	// for 14 Bit @ 5V
	const float H5 = 0.00008;	// for 14 Bit @ 5V

	RHlinear = H1 + (H2 * rawHumidity) + ((H3 * rawHumidity * rawHumidity));

	// Correct humidity value for current temperature
	RHtrue = ((tempC - 25.0) * (H4 + H5 * rawHumidity)) + RHlinear;
	return RHtrue;
}

PROCESS_THREAD(on_board_sensors_process, ev, data)
{

	PROCESS_BEGIN();
	unicast_open(&unicast, 146, &unicast_callbacks);
	static struct etimer et;
	struct unicast_message msg;
	//uint32_t raw_vals[ONBOARD_SENSORS]; 
	static int raw_vals[ONBOARD_SENSORS];

	uint32_t rawLight, tempC, RHtrue;

	SENSORS_ACTIVATE(light_sensor);
	SENSORS_ACTIVATE(sht11_sensor);
    set_sync1_addr();
    set_sync2_addr();
    
	static int i;
    static int seconds = 0;
    etimer_set(&et, CLOCK_SECOND * BEGIN_DELAY);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    printf("\n\n\nsec \tHum\tlight\ttemp\tconH\tconT\n");
	while (1) {
		etimer_set(&et, CLOCK_SECOND * SAMPLE_INTERVAL);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);

		raw_vals[0] = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
		raw_vals[1] = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
		raw_vals[2] = sht11_sensor.value(SHT11_SENSOR_TEMP);		
        
		//Fetch raw values
		rawLight = raw_vals[1];
		//Fetch corrected values from raw values
		RHtrue = (int)sht11_Humidity(raw_vals[2], raw_vals[0]);
		//printf("\nHumidity corrected %u.%u \n", (int)RHtrue, ((int)RHtrue * 10) % 10);
		tempC = (int)sht11_TemperatureC(raw_vals[2]);
		//printf("\nTemperature corrected %u.%u \n", (int)tempC, ((int)tempC * 10) % 10);	
        //seconds = clock_seconds();	        
        printf("%d\t%d \t%d\t%d\t%u.%u\t%u.%u\n", seconds++, sht11_sensor.value(SHT11_SENSOR_HUMIDITY),
		       light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR), sht11_sensor.value(SHT11_SENSOR_TEMP), 
               (int)RHtrue, (((int)RHtrue * 10) % 10), (int)tempC, ((int)tempC * 10) % 10);
        //submitHumiditySample(raw_vals[0]);
        submitLightSample(raw_vals[1]);
        submitTempSample((int)tempC);
	}

	PROCESS_END();
}
