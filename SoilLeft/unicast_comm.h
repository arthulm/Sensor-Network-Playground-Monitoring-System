#define CMD_FAN_ON 	    100
#define CMD_FAN_OFF    	101
#define CMD_HEATER_ON	102
#define CMD_HEATER_OFF	103
#define CMD_LIGHT_ON	104
#define CMD_LIGHT_OFF	105

#define MSG_CMD     200
#define SUCCESS     0
#define FAILURE     -1
#define TRUE    1
#define FALSE   0

#define SYNC_ADDR_P1 192
#define SYNC_ADDR_P2 11
#define MAX_POWER   31


char *get_cmd(int cmd_code)
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


static rimeaddr_t sync_addr;

struct unicast_message {
    uint8_t type;
    uint8_t cmd;
    uint16_t value;
    //rimeaddr_t addr;
};

static struct unicast_conn unicast;
//struct unicast_message msg;

int set_sync_addr()
{
    printf("\nSync addr set to %d.%d\n", SYNC_ADDR_P1, SYNC_ADDR_P2);
    sync_addr.u8[0] = SYNC_ADDR_P1;
    sync_addr.u8[1] = SYNC_ADDR_P2;
    return 0;
}

static void recv_uc(struct unicast_conn *c, const rimeaddr_t * from)
{
    printf("\Packet recv from %d.%d\n", from->u8[0], from->u8[1]);    
}

static const struct unicast_callbacks unicast_callbacks = { recv_uc };
int send_to_sync(struct unicast_message msg)
{      
    printf("\nSend:%s\n", get_cmd(msg.cmd));
    unicast_open(&unicast, 146, &unicast_callbacks);      
    packetbuf_clear();
    packetbuf_set_attr(PACKETBUF_ATTR_RADIO_TXPOWER,MAX_POWER);
    packetbuf_copyfrom(&msg, sizeof(struct unicast_message));
    					
    if (!rimeaddr_cmp(&sync_addr, &rimeaddr_node_addr)) {
	//printf("\nValue %d sent to sync %d.%d \n", value, sync_addr.u8[0], sync_addr.u8[1]);
	unicast_send(&unicast, &(sync_addr));
    }
    packetbuf_clear();
    return 0;
}
