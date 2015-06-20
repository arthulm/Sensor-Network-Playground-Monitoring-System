CONTIKI_PROJECT = ON_BOARD_SENSORS_TEST
all: $(CONTIKI_PROJECT)

TARGET = xm1000
DEFINES=NETSTACK_MAC=csma_driver,\
	NETSTACK_RDC=contikimac_driver,\
	RF_CHANNEL=26

#CONTIKI = your-path-to-contiki
ifndef CONTIKI
  CONTIKI = /home/user/contiki-2.6/
endif

include $(CONTIKI)/Makefile.include
