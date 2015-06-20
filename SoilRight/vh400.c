/*
 * Copyright (c) 2012, TU Darmstadt.
 * All rights reserved.
 *
 * vh400.c
 *
 * -----------------------------------------------------------------
 *
 * Author  : Iliya Gurov
 * Created : 2012-08-20
 */


#include "contiki.h"
#include "lib/sensors.h"
#include "dev/sky-sensors.h"
//#include "dev/vh400.h"
#include "vh400.h"

/* SREF_0 is AVCC */
/* SREF_1 is Vref+ */
/* ADC0 == P6.0/A0 == port "under" logo */
/* ADC1 == P6.1/A1 == port "over" logo */
/* ADC2 == P6.2/A2, bottom expansion port */
/* ADC3 == P6.1/A3, bottom expansion port, End Of (ADC-)Sequence */

#define INPUT_CHANNEL      ((1 << INCH_0) | (1 << INCH_1) | \
                            (1 << INCH_2) | (1 << INCH_3))
#define INPUT_REFERENCE     SREF_0
#define EXT_MEM0            ADC12MEM0
#define EXT_MEM1            ADC12MEM1
#define EXT_MEM2            ADC12MEM2
#define EXT_MEM3            ADC12MEM3

const struct sensors_sensor vh400;
/*---------------------------------------------------------------------------*/
static int value(int type)
{
  switch(type) {
  case ADC0:
    return EXT_MEM0;
  case ADC1:
    return EXT_MEM1;
  case ADC2:
    return EXT_MEM2;
  case ADC3:
    return EXT_MEM3;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
static int status(int type)
{
  return sky_sensors_status(INPUT_CHANNEL, type);
}

/*---------------------------------------------------------------------------*/
static int configure(int type, int c)
{
  return sky_sensors_configure(INPUT_CHANNEL, INPUT_REFERENCE, type, c);
}

/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(vh400, "vh400", value, configure, status);
