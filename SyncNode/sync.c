#include "contiki.h"
#include "net/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

#define CMD_FAN_ON 	    100
#define CMD_FAN_OFF    	101
#define CMD_HEATER_ON	102
#define CMD_HEATER_OFF	103
#define CMD_LIGHT_ON	104
#define CMD_LIGHT_OFF	105

#define MSG_CMD     200
#define SUCCESS     0
#define FAILURE     -1

char* get_cmd(int cmd_code)
{
    switch (cmd_code) {
    case CMD_FAN_ON:
	return "CMD_FAN_ON";
    case CMD_FAN_OFF:
	return "CMD_FAN_OFF";
    case CMD_HEATER_ON:
	return "CMD_HEATER_ON";
    case CMD_HEATER_OFF:
	return "CMD_HEATER_OFF";
    case CMD_LIGHT_ON:
	return "CMD_LIGHT_ON";
    case CMD_LIGHT_OFF:
	return "CMD_LIGHT_OFF";
    default:
	return "CMD_UNKNOWN";
    }
}
/*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Example unicast");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/

struct unicast_message {
    uint8_t type;
    uint8_t cmd;
    uint16_t value;
    //rimeaddr_t addr;
};

struct unicast_message *msg;
int sec_since_start  = 0;
static void recv_uc(struct unicast_conn *c, const rimeaddr_t * from)
{
    sec_since_start = clock_seconds();
    printf("\n%d - MSG from %d.%d\n", sec_since_start, from->u8[0], from->u8[1]);
    
    msg = (struct unicast_message *)packetbuf_dataptr();  
    packetbuf_clear();  
    //printf("MSG TYPE: %d \tCMD: %s \tVALUE: %d\n", msg->type, get_cmd(msg->cmd), msg->value);
    switch (msg->cmd) {
    case CMD_FAN_ON:
	printf("PG:FAN ON\n");
	break;
    case CMD_FAN_OFF:
	printf("PG:FAN OFF\n");
	break;
    case CMD_HEATER_ON:
	printf("PG:HEAT ON\n");
	break;
    case CMD_HEATER_OFF:
	printf("PG:HEAT OFF\n");
	break;
    case CMD_LIGHT_ON:
	printf("PG:LIGHT ON\n");
	break;
    case CMD_LIGHT_OFF:
	printf("PG:LIGHT OFF\n");
	break;
    default:
	printf("\n%s\n", get_cmd(msg->cmd));
    }

}

void sync_init()
{
    printf("PG:START\n");
}

void set_rime_addr()
{
    rimeaddr_t addr;
    addr.u8[0] = 1;
    addr.u8[1] = 0;
    printf("\nSetting rime addr : %d.%d \n", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    rimeaddr_set_node_addr(&addr);
}
static const struct unicast_callbacks unicast_callbacks = { recv_uc };

static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_unicast_process, ev, data)
{
    PROCESS_EXITHANDLER(unicast_close(&uc);)
	PROCESS_BEGIN();
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND * 120);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    sync_init();
    
    unicast_open(&uc, 146, &unicast_callbacks);
    while (1) {
	printf("\nCURRENT SYNC ADDR: %d.%d", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
	etimer_set(&et, CLOCK_SECOND * 60);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    }
    //Shut down the system
    printf("PG:ENG\n");
    PROCESS_END();
}
