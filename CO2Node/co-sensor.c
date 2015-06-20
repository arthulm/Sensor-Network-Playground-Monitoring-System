#include "contiki.h"
#include "ds1000-sensor.h"
#include <stdio.h>
#include "dev/leds.h"
#include "net/rime.h"
#include "unicast_comm.h"
#include "sample_eval.h"

#define SAMPLE_INTERVAL 3
#define SETTLING_TIME 150
#define SELF_ADDR_P1 3
#define SELF_ADDR_P2 0
#define OPTIMAL_VALUE 600



// Just for testing purpose...
static int number_msg_rcvd = 0, number_msg_sent = 0;
//  To denote fan is on or off
static int is_fan_on = TRUE;

PROCESS(co_sensor, "coSensor");
AUTOSTART_PROCESSES(&co_sensor);

int set_self_addr()
{
    rimeaddr_t addr;
    addr.u8[0] = SELF_ADDR_P1;
    addr.u8[1] = SELF_ADDR_P2;
    rimeaddr_set_node_addr(&addr);
    printf("\nSelf addr: %d.%d \n", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    return 0;
}

PROCESS_THREAD(co_sensor, ev, data)
{
    PROCESS_BEGIN();

    static struct unicast_message msg;
    unicast_open(&unicast, 146, &unicast_callbacks);

    //set_self_addr();
    set_sync1_addr();
    set_sync2_addr();

    static struct etimer et;
    static float co_ppm_calculated, sensor_output;
    static int co_ppm;

    // Time for the network to settle.......... 
    etimer_set(&et, SETTLING_TIME * CLOCK_SECOND);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));


    printf("Activating CO, CO2 Sensor...\n");
    
    // Infinite loop for reading and sending data at definite intervals...    
    while (1) {	
	etimer_set(&et, CLOCK_SECOND * SAMPLE_INTERVAL);
	PROCESS_WAIT_UNTIL(etimer_expired(&et));
    SENSORS_ACTIVATE(ds1000_sensor);
	//Read the required data and pass it to sink....
	// use ds1000_sensor.value(SENSOR_TEMP),ds1000_sensor.value(SENSOR_CO)
	// if required.......
	//**Calibration**
	//CO2 concentration(ppm) = V(Raw voltage) * (3000 / 3) - 200
	//ADC (Vref=2.5V); V(Raw voltage) is adc_value/4096*2.5
    sensor_output = 0.0;
	sensor_output = ds1000_sensor.value(SENSOR_CO2);
    //printf("\n\t\t\t\t\t\t\tsout %d\n", sensor_output);
	co_ppm_calculated = (((sensor_output / 4096) * 2.5) * 1000) - 200;
	co_ppm = (int) co_ppm_calculated;   
    //printf("%d", co_ppm); 
    if(co_ppm > 0)
    {
        submitSample(co_ppm);	
    }
    
    }
    
    SENSORS_DEACTIVATE(ds1000_sensor);
    PROCESS_END();
}
