/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2009 Vlad Vasiliev
 * Copyright (C) 2006-2009 Pavel Fialko
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
#include "weather-simple-popup.h"
#include "weather-utils.h"
/*******************************************************************************/
void
weather_simple_window_popup(GtkWidget *widget, gpointer user_data){
    GtkWidget       *window = NULL,
                    *main_vbox = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if defined OS2009
    window = hildon_stackable_window_new();
#else
    window = hildon_window_new();
#endif
    gtk_window_set_title(GTK_WINDOW(window), "OM Weather");
    main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    gtk_widget_show(window);

    gtk_box_pack_start(GTK_BOX(main_vbox), create_top_buttons_box(), TRUE, TRUE, 0);
    gtk_widget_show_all(main_vbox);
}
/*******************************************************************************/
gchar*
get_next_station_name(const gchar *current_station_name, GtkListStore *user_stations_list){
    GtkTreeIter     iter;
    gboolean        valid,
                    ready = FALSE;
    gchar           *station_name = NULL;
    GtkTreePath     *path;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(!current_station_name)
        return NULL;

    path = gtk_tree_path_new_first();
    valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                    &iter, path);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                            &iter, NAME_COLUMN, &station_name, -1);
        if(ready)
            return station_name;
        else{
            if((current_station_name) && (station_name) &&
                  !strcmp(current_station_name, station_name))
                ready = TRUE;
            g_free(station_name);
            gtk_tree_path_next(path);

            valid = gtk_tree_model_get_iter(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter, path);
            if(!valid)
                valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list),
                                        &iter);
        }
    }
    gtk_tree_path_free(path);
    return NULL;
}
/*******************************************************************************/
GtkWidget*
create_top_buttons_box(void){
    GtkWidget       *buttons_box = NULL,
                    *station_button = NULL,
                    *update_button = NULL;
    gchar           buffer[255],
                    full_filename[2048];
    struct stat     statv;
/* buttons */
    buttons_box = gtk_hbox_new(TRUE, 0);
    /* station button */
    *buffer = 0;
    snprintf(buffer, sizeof(buffer) - 1, "%s: %s",
                _("next"),
                get_next_station_name(app->config->current_station_name,
                                        app->user_stations_list));
    station_button = create_button_with_2_line_text(app->config->current_station_name,
                                                    buffer, 18, 12);
    gtk_widget_set_size_request(station_button, -1, 80);
    /* prepare last update time*/
    if(app->station_data){
        *full_filename = 0;
        snprintf(full_filename, sizeof(full_filename) - 1, "%s/%s.xml",
                    app->config->cache_dir_name, app->config->current_station_id);
        if(stat(full_filename, &statv))
            snprintf(buffer, sizeof(buffer) - 1, "%s%s", _("Last update: "), _("Unknown"));
        else{
            snprintf(buffer, sizeof(buffer) - 1, _("Last update: "));
            strftime(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer) - 1,
                    "%X %x", localtime(&statv.st_mtime));
        }
    }
    else{
        *buffer = 0;
        snprintf(buffer, sizeof(buffer) - 1, "%s%s", _("Last update: "), _("Unknown"));
    }
    /* update button */
    update_button = create_button_with_2_line_text(_("Update"), buffer, 18, 12);
    gtk_widget_set_size_request(update_button, -1, 80);

    gtk_box_pack_start(GTK_BOX(buttons_box), station_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(buttons_box), update_button, TRUE, TRUE, 0);
    return buttons_box;
}
/*******************************************************************************/
