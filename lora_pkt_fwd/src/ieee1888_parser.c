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
 * ieee1888_parser.c
 *
 *  Created on: Oct 3, 2017
 *      Author: anolp
 *      License: Copyright@2017 EmOne
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ieee1888_parser.h"
#include "ieee1888.h"

#define MAX_POINT_ID_LEN 1024
#define MAX_CONTENT_LEN 65536
#define MAX_TIME_LEN 32
#define POINT_COUNT 26

struct point_value {
  char point_id[MAX_POINT_ID_LEN];
  char time[MAX_TIME_LEN];
  char content[MAX_CONTENT_LEN];
  int  content_valid;
};

struct point_value m_buffer[POINT_COUNT];

//char m_IEEE1888_FROM[]="http://fiap-sandbox.gutp.ic.i.u-tokyo.ac.jp/axis2/services/FIAPStorage";
//char m_IEEE1888_TO[]="http://fiap-dev.gutp.ic.i.u-tokyo.ac.jp/axis2/services/FIAPStorage";

char m_IEEE1888_FROM[]="https://sclora.herokuapp.com/axis2/services/FIAPStorage";
char m_IEEE1888_TO[]="https://sclora.herokuapp.com/axis2/services/FIAPStorage";

void content_valid_clear(void) {
	int i;
	for (i = 0; i < POINT_COUNT; ++i) {
		m_buffer[i].content_valid = 0;
	}
}

void content_valid_set(void) {
	int i;
	for (i = 0; i < POINT_COUNT; ++i) {
		m_buffer[i].content_valid = 1;
	}
}

void ieee1888_client_init(const char* country_str, const char* city_str, gateway_packet_t *gw_pkt){

	int i, idx = 0;
	char site_id_str[255] = { 0 }, tmp[255] = {0};
	const char *site_attr[12] = {"mode", "fw_version", "pressure" , \
			"temperature", "humidity", "liminosity", "voltage", "current", \
			"latitude", "longitude", "altitudegps" ,"altitudebar" };
	const char *panel_attr[14] = {"fw_version", "voltage" , "current", "power", \
				"energy", "charge", "timebase", "temperature", "humidity", \
				"pressure" ,"rain_detected", "rain_lvl", "battery_lvl", "err_code" };
	char* endpoint = "sclora.emone.co.th";
	char country[4] = "th";
	char city[8] = "bkk";

	/* Get current unix time */
	char* str_time=ieee1888_mk_time(time(NULL));

	content_valid_clear();
	//TODO: Parse country city network_id and dev_id variable

	if (country_str != NULL) {
		sprintf(country , "%s", country_str);
	}

	if (city_str != NULL) {
		sprintf(city, "%s", city_str);
	}

	idx += sprintf(&site_id_str[idx],"%s/%s/%s/%16llX/", endpoint, country, city, gw_pkt->NETWORK_ID); //sclora.emone.com/th/bkk/network_id

	//Site properties
	for (i = 0; i < 12; ++i) {
		strncpy(tmp, site_id_str, sizeof tmp);
		if(strcmp(site_attr[i], "mode") == 0) {
			sprintf(m_buffer[i].content, "%d", gw_pkt->MODE);
		} else if (strcmp(site_attr[i], "fw_verison") == 0){
			sprintf(m_buffer[i].content, "%x", gw_pkt->site.fw_version);
		} else if (strcmp(site_attr[i], "pressure") == 0){
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.pressure);
		} else if (strcmp(site_attr[i], "temperature") == 0) {
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.temperature);
		} else if (strcmp(site_attr[i], "humidity") == 0) {
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.humidity);
		} else if (strcmp(site_attr[i], "liminosity") == 0) {
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.luminosity);
		} else if (strcmp(site_attr[i], "voltage") == 0) {
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.voltage);
		} else if (strcmp(site_attr[i], "current") == 0) {
			sprintf(m_buffer[i].content, "%f", gw_pkt->site.current);
		} else if (strcmp(site_attr[i], "latitude") == 0) {
			sprintf(m_buffer[i].content, "%d", gw_pkt->site.latitude);
		} else if (strcmp(site_attr[i], "longitude") == 0) {
			sprintf(m_buffer[i].content, "%d", gw_pkt->site.longitude);
		} else if (strcmp(site_attr[i], "altitudegps") == 0) {
			sprintf(m_buffer[i].content, "%d", gw_pkt->site.altitudeGps);
		} else if (strcmp(site_attr[i], "altitudebar") == 0) {
			sprintf(m_buffer[i].content, "%d", gw_pkt->site.altitudeBar);
		} else {
			//TODO: error handler
		}

		strcpy(m_buffer[i].point_id, strcat(tmp, site_attr[i]));
		strcpy(m_buffer[i].time, str_time);
	}

	//Panel properties 	//Device address
	sprintf(&site_id_str[idx],"panel/%08X/mems/", gw_pkt->DEVICE_ADDRESS); //sclora.emone.com/th/bkk/network_id/panel/dev_id
	idx = i;
	for (i = 0; i < 14; ++i) {
		strncpy(tmp, site_id_str, sizeof tmp);

		if (strcmp(panel_attr[i], "fw_version") == 0) {
			sprintf(m_buffer[idx + i].content, "%x", gw_pkt->panel.fw_version);
		} else if (strcmp(panel_attr[i], "voltage") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.voltage);
		} else if (strcmp(panel_attr[i], "current") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.current);
		} else if (strcmp(panel_attr[i], "power") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.power);
		} else if (strcmp(panel_attr[i], "energy") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.energy);
		} else if (strcmp(panel_attr[i], "charge") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.charge);
		} else if (strcmp(panel_attr[i], "timebase") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.timebase);
		} else if (strcmp(panel_attr[i], "temperature") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.temperature);
		} else if (strcmp(panel_attr[i], "humidity") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.humidity);
		} else if (strcmp(panel_attr[i], "pressure") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.pressure);
		} else if (strcmp(panel_attr[i], "rain_detected") == 0) {
			sprintf(m_buffer[idx + i].content, "%d", gw_pkt->panel.rain_detected);
		} else if (strcmp(panel_attr[i], "rain_lvl") == 0) {
			sprintf(m_buffer[idx + i].content, "%f", gw_pkt->panel.rain_lvl);
		} else if (strcmp(panel_attr[i], "battery_lvl") == 0) {
			sprintf(m_buffer[idx + i].content, "%d", gw_pkt->panel.battery_lvl);
		} else if (strcmp(panel_attr[i], "err_code") == 0) {
			sprintf(m_buffer[idx + i].content, "%08X", gw_pkt->ErrorCode);
		} else {
			//TODO: error handler
		}
		strcpy(m_buffer[idx + i].point_id, strcat(tmp, panel_attr[i]));
		strcpy(m_buffer[idx + i].time, str_time);
	}

#if 0
  strcpy(m_buffer[0].point_id,"http://gutp.jp/Arduino/test-001/Temperature");
  strcpy(m_buffer[1].point_id,"http://gutp.jp/Arduino/test-001/Illuminance");
  strcpy(m_buffer[2].point_id,"http://gutp.jp/Arduino/test-001/DIPSW");
  strcpy(m_buffer[3].point_id,"http://gutp.jp/Arduino/test-001/TGLSW");
#endif
  content_valid_set();
}

/*
int ieee1888_client_fetch_from_server(){

  ieee1888_transport* rq_transport=ieee1888_mk_transport();
  ieee1888_header* rq_header=ieee1888_mk_header();
  ieee1888_query* rq_query=ieee1888_mk_query();
  ieee1888_key* rq_key=ieee1888_mk_key_array(4);

  int i;
  for(i=0;i<POINT_COUNT;i++){
    rq_key[i].id=ieee1888_mk_string(m_buffer[i].point_id);
    rq_key[i].attrName=ieee1888_mk_attrNameType("time");
    rq_key[i].select=ieee1888_mk_selectType("maximum");
  }

  rq_query->id=ieee1888_mk_new_uuid();
  rq_query->type=ieee1888_mk_queryType("storage");
  rq_query->key=rq_key;
  rq_query->n_key=4;
  rq_header->query=rq_query;
  rq_transport->header=rq_header;

  int err;
  ieee1888_transport* rs_transport=ieee1888_client_query(rq_transport,m_IEEE1888_FROM,0,&err);

  // Recycle
  ieee1888_destroy_objects((ieee1888_object*)rq_transport);
  free(rq_transport);

  if(rs_transport==NULL){
    return IEEE1888_FETCH_FAIL;
  }

  ieee1888_header* rs_header=rs_transport->header;
  if(rs_header==NULL){
    ieee1888_destroy_objects((ieee1888_object*)rs_transport);
    free(rs_transport);
    return IEEE1888_FETCH_FAIL;
  }

  if(rs_header->OK==NULL){
    if(rs_header->error==NULL){
      printf("FATAL: neither OK nor error was put in the response.\n");
    }
    ieee1888_destroy_objects((ieee1888_object*)rs_transport);
    free(rs_transport);
    return IEEE1888_FETCH_FAIL;
  }

  ieee1888_body* rs_body=rs_transport->body;
  if(rs_body!=NULL){
    ieee1888_point* rs_point=rs_body->point;
    int rs_n_point=rs_body->n_point;
    if(rs_n_point>0 && rs_point!=NULL){
      int i,k;
      for(i=0;i<rs_n_point;i++){
        for(k=0;k<POINT_COUNT;k++){
          if(strcmp(m_buffer[k].point_id,rs_point[i].id)==0){
	    ieee1888_value* rs_value=rs_point[i].value;
	    int rs_n_value=rs_point[i].n_value;

            if(rs_n_value>0 && rs_value!=NULL){
               ieee1888_value* target_value=&rs_value[rs_n_value-1];
	       strncpy(m_buffer[k].time,target_value->time,MAX_TIME_LEN);
	       strncpy(m_buffer[k].content,target_value->content,MAX_CONTENT_LEN);
	       m_buffer[k].content_valid=1;
	    }
	    break;
	  }
	}
      }
    }
  }
  ieee1888_destroy_objects((ieee1888_object*)rs_transport);
  free(rs_transport);

  return IEEE1888_FETCH_SUCCESS;
}
*/

int ieee1888_client_write_to_server(void){

   ieee1888_transport* rq_transport=ieee1888_mk_transport();
   ieee1888_body* rq_body=ieee1888_mk_body();
   ieee1888_point* rq_point=ieee1888_mk_point_array(POINT_COUNT);

   int i,n;
   for(i=0,n=0;i<POINT_COUNT;i++){
     if(m_buffer[i].content_valid){
       ieee1888_value* rq_value=ieee1888_mk_value();
       rq_value->time=ieee1888_mk_time_from_string(m_buffer[i].time);
       rq_value->content=ieee1888_mk_string(m_buffer[i].content);
       rq_point[n].id=ieee1888_mk_uri(m_buffer[i].point_id);
       rq_point[n].value=rq_value;
       rq_point[n].n_value=1;
       n++;
     }
   }

   rq_body->point=rq_point;
   rq_body->n_point=n;
   rq_transport->body=rq_body;

   // for Debug
   ieee1888_dump_objects((ieee1888_object*)rq_transport);

   int err;
   ieee1888_transport* rs_transport=ieee1888_client_data(rq_transport,m_IEEE1888_TO,0,&err);

   // Recycle
   ieee1888_destroy_objects((ieee1888_object*)rq_transport);
   free(rq_transport);

   if(rs_transport==NULL){
     return IEEE1888_WRITE_FAIL;
   }

   ieee1888_header* rs_header=rs_transport->header;
   if(rs_header==NULL){
     ieee1888_destroy_objects((ieee1888_object*)rs_transport);
     free(rs_transport);
     return IEEE1888_WRITE_FAIL;
   }

   if(rs_header->OK==NULL){
     if(rs_header->error==NULL){
       printf("FATAL: neither OK nor error was put in the response.\n");
     }
     ieee1888_destroy_objects((ieee1888_object*)rs_transport);
     free(rs_transport);
     return IEEE1888_WRITE_FAIL;
   }

   ieee1888_destroy_objects((ieee1888_object*)rs_transport);
   free(rs_transport);

   return IEEE1888_WRITE_SUCCESS;
}
