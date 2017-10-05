/*
 * Copyright (c) 2013 Hideya Ochiai, the University of Tokyo,  All rights reserved.
 *
 * Permission of redistribution and use in source and binary forms,
 * with or without modification, are granted, free of charge, to any person
 * obtaining the copy of this software under the following conditions:
 *
 *  1. Any copies of this source code must include the above copyright notice,
 *  this permission notice and the following statement without modification
 *  except possible additions of other copyright notices.
 *
 *  2. Redistributions of the binary code must involve the copy of the above
 *  copyright notice, this permission notice and the following statement
 *  in documents and/or materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * ieee1888_parser.h
 *
 *  Created on: Oct 3, 2017
 *      Author: anolp
 *      License: Copyright@2017 EmOne
 */
#include <stdint.h>     /* C99 types */
#include <stdbool.h>    /* bool type */

#ifndef _IEEE1888_CLIENT_H_
#define _IEEE1888_CLIENT_H_

#define IEEE1888_FETCH_SUCCESS 1
#define IEEE1888_FETCH_FAIL 0

#define IEEE1888_WRITE_SUCCESS 1
#define IEEE1888_WRITE_FAIL 0

typedef struct {
	uint32_t fw_version;
	float voltage; //Voltage
	float current; //Current
	float power; //Power
	float energy; //Energy
	float charge; //Charge
	float timebase;
	float temperature;		/* in mbar */
	float humidity;		/* in �C   */
	float pressure;		/* in %    */
	int8_t rain_detected;		/*Detect rain or liquid on sensor*/
	float rain_lvl; 			/*rain level in %*/
	uint8_t battery_lvl;
	float luminosity;	/* in Lux to W/m^2 conversion*/
} panel_t;

typedef struct {
	uint32_t fw_version;
	float pressure;    /* in mbar */
	float temperature; /* in �C   */
	float humidity;    /* in %    */
	float luminosity;	/* in Lux to W/m^2 conversion*/
	float voltage;
	float current;
	int32_t latitude;	/* in degree    */
	int32_t longitude; /* in degree    */
	int16_t altitudeGps;       /* in m */
	int16_t altitudeBar;       /* in m * 10 */
} site_t;

typedef struct {
  uint32_t	NETWORK_ID;
  uint32_t	DEVICE_ADDRESS;
  uint8_t MODE; /* 0:Normal otherwise Diagnostic (1-32)*/
  site_t site;
  panel_t panel;
  uint16_t ErrorCode;
  /** may be added more*/
} gateway_packet_t;

void ieee1888_client_init(const char* country_str, const char* city_str, gateway_packet_t *gw_pkt);
//int ieee1888_client_fetch_from_server(void);
int ieee1888_client_write_to_server(gateway_packet_t *gw_pkt);

#endif /* _IEEE1888_CLIENT_H_ */