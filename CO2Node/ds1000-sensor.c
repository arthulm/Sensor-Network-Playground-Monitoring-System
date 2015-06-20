/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: ds1000-sensor.c,v 1.2 2010/08/25 19:34:42 nifi Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne, Marcus Lundén,
 *           Jesper Karlsson
 * Created : 2005-11-01
 * Updated : $Date: 2010/08/25 19:34:42 $
 *           $Revision: 1.2 $
 */


#include "contiki.h"
#include "ds1000-sensor.h"
#include "dev/sky-sensors.h"


#define INPUT_CHANNEL      ((1 << INCH_0) | (1 << INCH_1) | (1 << INCH_2))
//#define INPUT_CHANNEL      ((1 << INCH_4) | (1 << INCH_5))
#define INPUT_REFERENCE     SREF_1

#define CO2_MEM		    ADC12MEM0
#define TEMP_MEM  	  ADC12MEM1
#define CO_MEM		    ADC12MEM2
/*#define CO2_MEM   ADC12MEM4
#define CO_MEM    ADC12MEM5
#define TEMP_MEM  ADC12MEM0*/

// Macro for GIO2 --> P2.3
#define P23_OUT() P2DIR |= BV(3)
#define P23_SEL() P2SEL &= ~BV(3)
#define P23_PULL_UP() P2OUT |= BV(3)
#define P23_PULL_DOWN() P2OUT &= ~BV(3)
// Macros for GIO3 --> P2.6
#define P26_OUT() P2DIR |= BV(6)
#define P26_SEL() P2SEL &= ~BV(6)
#define P26_PULL_UP() P2OUT |= BV(6)
#define P26_PULL_DOWN() P2OUT &= ~BV(6)


const struct sensors_sensor ds1000_sensor;
/*---------------------------------------------------------------------------*/
static int
value(int type)
{
   switch(type) {
	  case SENSOR_CO2:
	    return CO2_MEM;
	  case SENSOR_TEMP:
	    return TEMP_MEM;
	  case SENSOR_CO:
	    return CO_MEM;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static int
status(int type)
{
  return sky_sensors_status(INPUT_CHANNEL, type);
}
/*---------------------------------------------------------------------------*/
static int
configure(int type, int c)
{


  P23_OUT();
  P26_OUT();
  P23_SEL();
  P26_SEL();

  if (c == 1) { // user wants to activate
    P23_PULL_UP();
    P26_PULL_UP();
  } else { // user wants to deactivate
    P23_PULL_DOWN();
    P26_PULL_DOWN();
  }
	
  return sky_sensors_configure(INPUT_CHANNEL, INPUT_REFERENCE, type, c);
}
/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(ds1000_sensor, DS1000_SENSOR, value, configure, status);
