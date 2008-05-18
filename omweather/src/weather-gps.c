/* This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2008 Vlad Vasiliev
 * Copyright (C) 2006-2008 Pavel Fialko
 * 	for the code
 *        
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
	
*/
/*******************************************************************************/
#include "weather-common.h"
#include "weather-gps.h"
#ifdef RELEASE
#undef DEBUGFUNCTIONCALL
#endif
/*******************************************************************************/
#ifdef HILDON

void
get_nearest_station( double lat, double lon, Station *result)
{
    FILE		*fh;
    char		buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    Region_item  region;
    GtkListStore *stations_list = NULL;
    GtkTreeIter  iter;
    gboolean     valid;
    gchar        *station_name = NULL,
                 *station_id0 = NULL;
    double       station_latitude,
                 station_longtitude,
                 distance,
		 min_distance = 40000;


    fh = fopen(REGIONSFILE, "rt");
    if(!fh){
	fprintf(stderr, "\nCan't read file %s: %s", REGIONSFILE,
		strerror(errno));
	return;
    }
    /* Reading region settings */
    while(!feof(fh)){
	memset(buffer, 0, sizeof(buffer));
	fgets(buffer, sizeof(buffer) - 1, fh);
        parse_region_string(buffer,&region);
	/* Checking insiding point in to region */
        if ( lat >= region.minlat && lat <= region.maxlat && lon >= region.minlon && lon <= region.maxlon){
	    
            stations_list = create_items_list(LOCATIONSFILE, region.start,region.end, NULL);
            valid =  gtk_tree_model_get_iter_first(GTK_TREE_MODEL(stations_list), &iter);
            
            while (valid){
                gtk_tree_model_get(GTK_TREE_MODEL(stations_list),
				                &iter, 
                                                0, &station_name,
					        1, &station_id0,
					        2, &station_latitude,
					        3, &station_longtitude,
                		                -1);
		/* Calculating distance */				
		distance = calculate_distance(lat,lon,station_latitude,station_longtitude);

		if (distance<min_distance){

		    /* New minimal distance */
		    min_distance = distance;

		    /* Copying to result */
    		    memset(result->name, 0, sizeof(result->name)+5);
    		    memcpy(result->name, station_name,
		    	    ((sizeof(result->name) - 1) > (int)(strlen(station_name)) ?
			    (int)(strlen(station_name)) : (sizeof(result->name) - 1)));

		    /* Add word (GPS) */
		    if (strlen(result->name)<(sizeof(result->name)-5)){
			result->name[strlen(result->name)] = '(';
			result->name[strlen(result->name)] = 'G';
			result->name[strlen(result->name)] = 'P';
			result->name[strlen(result->name)] = 'S';
			result->name[strlen(result->name)] = ')';
		    }else{
		    	result->name[sizeof(result->name)-5] = '(';
		    	result->name[sizeof(result->name)-4] = 'G';
		    	result->name[sizeof(result->name)-3] = 'P';
		    	result->name[sizeof(result->name)-2] = 'S';
			result->name[sizeof(result->name)-1] = ')';						
		    }
		    
    		    memset(result->id0, 0, sizeof(result->id0));
    		    memcpy(result->id0, station_id0,
		    	    ((sizeof(result->id0) - 1) > (int)(strlen(station_id0)) ?
			    (int)(strlen(station_id0)) : (sizeof(result->id0) - 1)));			    
		    result->latitude = station_latitude;
		    result->longtitude = station_longtitude;
		}
		                
            	valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(stations_list),&iter);
            }
	    
	    /* Clearing station list */
            if(stations_list){
	          gtk_list_store_clear(stations_list);
	          stations_list = NULL;
	    }	    
        }          
    }
    fclose(fh);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
static void
gps_location_changed (LocationGPSDevice *device, gpointer userdata)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!app->config->gps_station)
        return;
    if (device->fix->fields & LOCATION_GPS_DEVICE_LATLONG_SET){
	app->temporary_station_latitude = device->fix->latitude;
	app->temporary_station_longtitude = device->fix->longitude;
    }
}
/*******************************************************************************/ 
void
initial_gps_connect(void)
{ 
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    app->gps_device = g_object_new (LOCATION_TYPE_GPS_DEVICE, NULL);
    app->gps_id_connection = g_signal_connect (app->gps_device, "changed", G_CALLBACK (gps_location_changed), NULL);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif    
}
/*******************************************************************************/
void
deinitial_gps_connect(void)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    g_signal_handler_disconnect (app->gps_device,app->gps_id_connection);
    g_object_unref(app->gps_device);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
#endif
