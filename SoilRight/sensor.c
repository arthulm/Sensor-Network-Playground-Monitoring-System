#include "vh400.c"
#include "contiki.h"
#include "stdio.h"
#include "dev/leds.h"
#include "net/rime.h"

//#include "act_commands.h"
#include "unicast_comm.h"
#include "sample_eval.h"


#define SAMPLE_DELAY    2
#define MSG_TYPE_DATA_COMMAND 1
#define SELF_ADDR_P1    2
#define SELF_ADDR_P2    0

//Global data declarations



int set_self_addr()
{
    rimeaddr_t addr;
    addr.u8[0] = SELF_ADDR_P1;
    addr.u8[1] = SELF_ADDR_P2;
    rimeaddr_set_node_addr(&addr);
    printf("\nSelf addr: %d.%d \n", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    return 0;
}

int getVWCValue(int adc_value)
{
    double raw_voltage = 0.0;
    float vmc_float = 0.0;
    //raw_voltage = (adc_value/4096.0)*2.5;
    raw_voltage = (adc_value / 1000.0);

    if (raw_voltage > 0.0 && raw_voltage < 1.1) {
	vmc_float = 10.0 * (raw_voltage) - 1.0;
    } else if (raw_voltage > 1.1 && raw_voltage < 1.3) {
	vmc_float = 25.0 * (raw_voltage) - 17.5;
    } else if (raw_voltage > 1.3 && raw_voltage < 1.82) {
	vmc_float = 48.08 * (raw_voltage) - 47.5;
    } else if (raw_voltage > 1.82) {
	vmc_float = (26.32 * (raw_voltage)) - 7.89;
    }
    return (int) vmc_float;
}

PROCESS(moisture_read, "sensor");
AUTOSTART_PROCESSES(&moisture_read);
PROCESS_THREAD(moisture_read, ev, data)
{
    PROCESS_BEGIN();
    static struct etimer et;
    static int VWC;
    static int adc_value;    
    unicast_open(&unicast, 146, &unicast_callbacks);
    
    //set_self_addr();
    set_sync_addr();

    etimer_set(&et, CLOCK_SECOND * 150);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    while (1) {
	configure(SENSORS_ACTIVE, 1);
	status(1);
	adc_value = value(ADC0);	
	VWC = getVWCValue(adc_value);
    submitSample(VWC);
	printf("\tVWC \t%d ", VWC);	
    leds_on(LEDS_RED);
    etimer_set(&et, CLOCK_SECOND * SAMPLE_DELAY/2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    leds_off(LEDS_RED);
    etimer_set(&et, CLOCK_SECOND * SAMPLE_DELAY/2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	configure(SENSORS_ACTIVE, 0);
    }
    PROCESS_END();
}
