/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2010 Vlad Vasiliev
 * Copyright (C) 2006-2010 Pavel Fialko
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/
#include "weather-simple-settings.h"
#include "weather-stations.h"
#include "weather-config.h"
#include "weather-settings.h"
#include "weather-utils.h"
#include "weather-home.h"
#include "weather-event.h"

#define BUTTON_RELEASE_EVENT "button-release-event"
#define BUTTON_PRESS_EVENT "button-press-event"

void
free_list(GSList *list){
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
 
    if (list) {
        g_slist_foreach(list, (GFunc)g_free, NULL);
        g_slist_free(list);
    }
}
/*******************************************************************************/
void
widget_custom_styles_save(GtkWidget *window){
   GtkWidget
       *one_row = NULL,
       *one_column = NULL,
       *two_rows = NULL,
       *two_columns = NULL,
       *combination = NULL,
       *visible_items = NULL,
       *font_color = NULL,
       *icon_size = NULL,
       *transparency = NULL,
       *background_color = NULL, 
       *font = NULL;
   PangoFontDescription *pfd = NULL; 
   gint 
        layout_previous_value,
        visbile_items_previous_value,
	icon_size_previous_value;

   one_row = lookup_widget(window, "one_row");
   one_column = lookup_widget(window, "one_column");
   two_rows = lookup_widget(window, "two_rows");
   two_columns = lookup_widget(window, "two_columns");
   combination = lookup_widget(window, "combination");
   visible_items = lookup_widget(window, "visible_items_number");
   font_color = lookup_widget(window, "font_color");
   font = lookup_widget(window, "font");
   icon_size = lookup_widget(window, "icon_size");
   transparency = lookup_widget(window, "transparency");
   background_color = lookup_widget(window, "background_color");
   if (one_row && one_column && two_rows &&  two_columns && combination){
       layout_previous_value = app->config->icons_layout;
       if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_row)))
            app->config->icons_layout = ONE_ROW;
        else
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_column)))
                app->config->icons_layout = ONE_COLUMN;
            else
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(two_rows)))
                    app->config->icons_layout = TWO_ROWS;
                else
                    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(two_columns)))
                        app->config->icons_layout = TWO_COLUMNS;
                    else
                        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(combination)))
                            app->config->icons_layout = COMBINATION;
                        else
                            app->config->icons_layout = ONE_ROW;
    visbile_items_previous_value = app->config->days_to_show;
    if (visible_items)
        app->config->days_to_show = hildon_controlbar_get_value(visible_items) + 1; 

    if (font_color)
        hildon_color_button_get_color(font_color, &(app->config->font_color));

    if (font) {
        if (app->config->font)
            g_free(app->config->font);
        app->config->font = g_strdup((gchar *)
                                     gtk_font_button_get_font_name
                                     (GTK_FONT_BUTTON(font)));
    }

    icon_size_previous_value = app->config->icons_size;
    if (icon_size) {
        if (app->config->icons_size - 1
            != hildon_controlbar_get_value(HILDON_CONTROLBAR(icon_size))) {
            app->config->icons_size =
                hildon_controlbar_get_value(HILDON_CONTROLBAR
                                            (icon_size)) + 1;
	}
    }	
    if (transparency) {
        app->config->alpha_comp =
            hildon_controlbar_get_value(HILDON_CONTROLBAR(transparency));
    }
/* background color */
    if (background_color)
        hildon_color_button_get_color(background_color, &(app->config->background_color));
/* correct days_to show */
    pfd = pango_font_description_from_string(gtk_font_button_get_font_name
		                                         (GTK_FONT_BUTTON(font)));
    if (app->config->icons_layout == ONE_COLUMN){
        if (app->config->icons_size == SMALL  && app->config->days_to_show > 8)
		app->config->days_to_show = 8;
	if (app->config->icons_size == MEDIUM  && app->config->days_to_show > 6)
		app->config->days_to_show = 6;
	if (app->config->icons_size == LARGE  && app->config->days_to_show > 4)
		app->config->days_to_show = 4;
	if (app->config->icons_size == GIANT  && app->config->days_to_show > 3)
		app->config->days_to_show = 3;
	if (app->config->icons_size <= MEDIUM   && app->config->days_to_show > 9 && pango_font_description_get_size(pfd)>8200)
		app->config->days_to_show = 9;
	if (app->config->icons_size <= MEDIUM  && app->config->days_to_show > 8 && pango_font_description_get_size(pfd)>10200)
		app->config->days_to_show = 8;
	if (app->config->icons_size <= MEDIUM  && app->config->days_to_show > 7 && pango_font_description_get_size(pfd)>11200)
		app->config->days_to_show = 7;
	if (app->config->icons_size <= MEDIUM  && app->config->days_to_show > 6 && pango_font_description_get_size(pfd)>12200)
		app->config->days_to_show = 6;
	if (app->config->icons_size <= MEDIUM  && app->config->days_to_show > 5 && pango_font_description_get_size(pfd)>13200)
		app->config->days_to_show = 5;

    }

    if (app->config->icons_layout == TWO_COLUMNS){
        if (app->config->icons_size == LARGE  && app->config->days_to_show > 8)
		app->config->days_to_show = 8;
        if (app->config->icons_size == GIANT && app->config->days_to_show > 6)
		app->config->days_to_show = 6;
    }

    if (app->config->icons_layout == ONE_ROW){
        if (app->config->icons_size == GIANT  && app->config->days_to_show > 5)
        	    app->config->days_to_show = 5;
        if (app->config->icons_size == LARGE  && app->config->days_to_show > 6)
        	    app->config->days_to_show = 6;
	if (app->config->icons_size == MEDIUM && app->config->days_to_show > 8)
        	    app->config->days_to_show = 8;
    } 

/* save settings */
    config_save(app->config);
    if ((layout_previous_value != app->config->icons_layout)||
	(icon_size_previous_value != app->config->icons_size) ||
        (visbile_items_previous_value = app->config->days_to_show))	    
       app->reload = TRUE; 
    /* Send signal for redraw */
    send_dbus_signal (OMWEATHER_SIGNAL_INTERFACE,
                      OMWEATHER_SIGNAL_PATH,
                      OMWEATHER_RELOAD_CONFIG);
   }
}
/*******************************************************************************/
void
widget_styles_save(GtkWidget *window){
   GtkWidget
    *preset_now = NULL,
    *preset_now_plus_two = NULL,
    *preset_now_plus_three_h = NULL,
    *preset_now_plus_three_v = NULL,
    *preset_now_plus_seven = NULL,
    *preset_custom = NULL,
    *selected_icon_set = NULL,
    *mod_button = NULL;

    GSList      *icon_set = NULL, *tmp_set ;
    gint previous_value;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    preset_now = lookup_widget(window, "preset_now");
    preset_now_plus_two = lookup_widget(window, "preset_now_plus_two");
    preset_now_plus_three_v = lookup_widget(window, "preset_now_plus_three_v");
    preset_now_plus_three_h = lookup_widget(window, "preset_now_plus_three_h");
    preset_now_plus_seven = lookup_widget(window, "preset_now_plus_seven");
    preset_custom = lookup_widget(window, "preset_custom");
    mod_button = lookup_widget(window, "mod_button");

    if (preset_now && preset_now_plus_two && preset_now_plus_three_v && 
        preset_now_plus_three_h && preset_now_plus_seven && preset_custom){
        previous_value = app->config->icons_layout;
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now)))
            app->config->icons_layout = PRESET_NOW;
        else
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_two)))
                app->config->icons_layout = PRESET_NOW_PLUS_TWO;
            else
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_three_v)))
                    app->config->icons_layout = PRESET_NOW_PLUS_THREE_V;
                else
                    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_three_h)))
                        app->config->icons_layout = PRESET_NOW_PLUS_THREE_H;
                    else
                        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_now_plus_seven)))
                            app->config->icons_layout = PRESET_NOW_PLUS_SEVEN;
                        else
                            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(preset_custom))){
                                if (app->config->icons_layout == PRESET_NOW ||
                                    app->config->icons_layout == PRESET_NOW_PLUS_TWO ||
                                    app->config->icons_layout == PRESET_NOW_PLUS_THREE_V ||
                                    app->config->icons_layout == PRESET_NOW_PLUS_THREE_H ||
                                    app->config->icons_layout == PRESET_NOW_PLUS_SEVEN)
                                        app->config->icons_layout = ONE_ROW;    
                            }else
                                app->config->icons_layout = PRESET_NOW;
    }
    /* icon set */
    icon_set =
        (GSList *) g_object_get_data(G_OBJECT(window),
                                     "iconsetlist");
    if (icon_set) {
        tmp_set = icon_set;
        while (tmp_set) {
            selected_icon_set =
                lookup_widget(window, (gchar *) tmp_set->data);
            if (selected_icon_set) {
                if (gtk_toggle_button_get_active
                    (GTK_TOGGLE_BUTTON(selected_icon_set))) {
                    if (app->config->icon_set)
                        g_free(app->config->icon_set);
                    app->config->icon_set =
                        g_strdup((gchar *) tmp_set->data);
                    break;
                }
            }
            tmp_set = g_slist_next(tmp_set);
        }
        update_icons_set_base(app->config->icon_set);
        free_icon_set_list(icon_set);

    }
    if (hildon_button_get_value(mod_button)){
        if (app->config->mod)
            g_free(app->config->mod);
        app->config->mod = g_strdup(hildon_button_get_value(mod_button));
        load_style();
    }
/* save settings */
    config_save(app->config);
    if (previous_value != app->config->icons_layout)
       app->reload = TRUE; 
    /* Send signal for redraw */
    send_dbus_signal (OMWEATHER_SIGNAL_INTERFACE,
                      OMWEATHER_SIGNAL_PATH,
                      OMWEATHER_RELOAD_CONFIG);

//    redraw_home_window(FALSE);
}
/*******************************************************************************/
void
check_custom_changed_handler(GtkToggleButton *button, gpointer user_data){
    GtkWidget
    *button_edit_custom = NULL,
    *mod_button = NULL;

    button_edit_custom = lookup_widget(user_data, "button_edit_custom");
    mod_button = lookup_widget(user_data, "mod_button");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))){
        gtk_widget_hide(mod_button);
        gtk_widget_show(button_edit_custom);
    }else{
        gtk_widget_hide(button_edit_custom);
        gtk_widget_show(mod_button);
    }
}
/*******************************************************************************/

#if OS2009
void
animation_button_toggled (HildonCheckButton *button, gpointer user_data)
{
    gboolean active;

    active = hildon_check_button_get_active (button);
    if (active)
        app->config->animation = TRUE;
    else
        app->config->animation = FALSE;
}
#endif
/*******************************************************************************/
void
changed_custom_layout(GtkButton *button, gpointer user_data){

    GtkWidget *vbox                 = NULL,
              *layouts_line         = NULL,
              *items_line           = NULL,
              *font_line            = NULL,
              *icon_size            = NULL,
	      *transparency         = NULL,
	      *background_color     = NULL,
              *window               = NULL,
              *scrolled_window      = NULL;

    gint result;
 
    vbox = gtk_vbox_new(FALSE, 2);
    window = gtk_dialog_new_with_buttons(_("Edit custom layout"), NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    gtk_widget_set_name(window, "widget_edit_custom_layout_window");
#if defined OS2009
    scrolled_window = hildon_pannable_area_new ();
    hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA (scrolled_window), GTK_WIDGET (vbox));
    gtk_widget_set_size_request(scrolled_window, -1, 350);
#endif
    layouts_line = create_layouts_line(window, 40, MEDIUM_MODE);
    items_line = create_visible_items_line(window, SIMPLE_MODE);
    font_line = create_fontsets_line(window, SIMPLE_MODE);
    icon_size = create_icon_size_line(window, SIMPLE_MODE);
    transparency = create_transparency_line(window, SIMPLE_MODE);
    background_color = create_background_color_button(window, SIMPLE_MODE);
    gtk_box_pack_start(GTK_BOX(vbox), layouts_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), items_line, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), font_line, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), icon_size, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), transparency, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), background_color, TRUE, TRUE, 0);
    
#if defined OS2009
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       scrolled_window, TRUE, TRUE, 0);
#else
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       vbox, TRUE, TRUE, 0);
#endif
    gtk_dialog_add_button (GTK_DIALOG(window), _("Save"), GTK_RESPONSE_YES);
    gtk_widget_show_all(window);
    result = gtk_dialog_run(GTK_DIALOG(window));
    if(result == GTK_RESPONSE_YES)
        widget_custom_styles_save(window);
    if(window)
        gtk_widget_destroy(window);



}
/*******************************************************************************/
void
widget_style_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
    GtkWidget *vbox                 = NULL,
              *layouts_line         = NULL,
              *iconsets_line        = NULL,
              *window               = NULL,
              *custom_edit_layout_button = NULL,
              *check_button         = NULL,
              *mod_button           = NULL,
              *preset_custom        = NULL,
              *widget_style_button  = NULL,
              *selector             = NULL;
    gint i = 0;
    GSList    *mod_set = NULL, *tmp = NULL;
    gint result;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    vbox = gtk_vbox_new(TRUE, 2);
    window = gtk_dialog_new_with_buttons(_("Widget style and Iconset"), NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    gtk_widget_set_name(window, "widget_style_window");

#ifdef OS2009
    #ifdef APPLICATION
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    layouts_line = create_layouts_line(window, 40, SIMPLE_MODE);
    iconsets_line = create_iconsets_line(window, 40, SIMPLE_MODE);
    gtk_box_pack_start(GTK_BOX(vbox), layouts_line, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), iconsets_line, TRUE, TRUE, 10);

#if OS2009
    check_button = hildon_check_button_new (HILDON_SIZE_AUTO);
    gtk_button_set_label (GTK_BUTTON (check_button), _("Animation"));
    if (app->config->animation)
        hildon_check_button_set_active(check_button, TRUE);
    else
        hildon_check_button_set_active(check_button, FALSE);
    g_signal_connect (check_button, "toggled", G_CALLBACK (animation_button_toggled), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), check_button, TRUE, TRUE, 10);

    custom_edit_layout_button = gtk_button_new_with_label(_("Edit custom layout")); 
    GLADE_HOOKUP_OBJECT(window, custom_edit_layout_button, "button_edit_custom");
    gtk_widget_set_name(custom_edit_layout_button, "button_edit_custom");
    g_signal_connect(custom_edit_layout_button, "clicked",
                     G_CALLBACK(changed_custom_layout),
                     NULL);

    create_icon_set_list(IMAGES_PATH, &mod_set, "dir"); 
    selector = hildon_touch_selector_new_text ();

    mod_button = hildon_picker_button_new (HILDON_SIZE_AUTO, HILDON_BUTTON_ARRANGEMENT_VERTICAL);
    hildon_picker_button_set_selector (HILDON_PICKER_BUTTON (mod_button),
                                      HILDON_TOUCH_SELECTOR (selector));
    hildon_button_set_title (HILDON_BUTTON (mod_button), _("Modification"));
    GLADE_HOOKUP_OBJECT(window, mod_button, "mod_button");
    gtk_widget_set_name(mod_button, "mod_button");
    tmp = mod_set;

    while (tmp) { 
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), tmp->data);
        if (tmp->data && app->config->mod && !strcmp(tmp->data, app->config->mod))
                hildon_picker_button_set_active(mod_button,i);
        i++;
        tmp = g_slist_next(tmp);
    }

    gtk_box_pack_start(GTK_BOX(layouts_line), mod_button, FALSE,
                       FALSE, 20);

    gtk_box_pack_start(GTK_BOX(layouts_line), custom_edit_layout_button, FALSE,
                       FALSE, 20);
#endif
 
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       vbox, TRUE, TRUE, 0);

    gtk_dialog_add_button (GTK_DIALOG(window), _("Save"), GTK_RESPONSE_YES);
    gtk_widget_show_all(window);

    switch(app->config->icons_layout){
        case PRESET_NOW:
        case PRESET_NOW_PLUS_TWO:
        case PRESET_NOW_PLUS_THREE_V:
        case PRESET_NOW_PLUS_THREE_H:
        case PRESET_NOW_PLUS_SEVEN:
            gtk_widget_hide (custom_edit_layout_button);
            gtk_widget_show (mod_button);
            break;
        default: 
            preset_custom = lookup_widget(window, "preset_custom");
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(preset_custom),
                                     TRUE);
            gtk_widget_hide (mod_button);
            break;
    }
    result = gtk_dialog_run(GTK_DIALOG(window));
    if(result == GTK_RESPONSE_YES)
        widget_styles_save(window);

    if(window)
        gtk_widget_destroy(window);

    widget_style_button = (gpointer)(g_object_get_data(G_OBJECT(button), 
                                                        "widget_style_button"));
    gtk_widget_destroy(widget_style_button);
    create_and_fill_widget_style_box(user_data);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
/*******************************************************************************/
void
highlight_current_item(GtkTreeView *tree_view, GtkListStore *list, gchar *current){
    GtkTreeIter     iter;
    gchar           *name = NULL;
    gboolean        valid;
    GtkTreePath     *path;
    GtkTreeModel    *model;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if (!current || !tree_view)
        return;

    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list),
                &iter);
    while(valid){
        gtk_tree_model_get(GTK_TREE_MODEL(list),
                           &iter,
                           0, &name,
                            -1);
        if(!strcmp(current, name)){
                model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
                path = gtk_tree_model_get_path(model, &iter);
                gtk_tree_view_set_cursor(GTK_TREE_VIEW(tree_view),
                                         path, NULL, FALSE);
                gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW (tree_view), path, NULL, TRUE, 0.5, 0);
                gtk_tree_path_free(path);
                break;
        }
        valid  = gtk_tree_model_iter_next(GTK_TREE_MODEL(list),
                  &iter);
    }
}
/*******************************************************************************/
static void
row_activated_callback(GtkWidget *tree_view, GtkTreePath *path,
                        GtkTreeViewColumn *column, gpointer user_data){
  GtkTreeIter       iter;
  gchar             *name;
  GtkTreeModel      *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));

  gtk_tree_model_get_iter(model, &iter, path);
  gtk_tree_model_get(model, &iter, 0, &name, -1);
  list_changed(NULL, user_data, name);
}
/*******************************************************************************/
#if defined OS2009
/* This code from Modest */
gchar *
picker_print_func (HildonTouchSelector *selector, gpointer userdata)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *text = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    /* Always pick the selected country from the tree view and
       never from the entry */
    model = hildon_touch_selector_get_model (selector, 0);
    if (hildon_touch_selector_get_selected (selector, 0, &iter)) {
        gint column;
        GtkWidget *entry;
        const gchar *entry_text;

        column = hildon_touch_selector_entry_get_text_column (HILDON_TOUCH_SELECTOR_ENTRY (selector));
        gtk_tree_model_get (model, &iter, column, &text, -1);

        entry = GTK_WIDGET (hildon_touch_selector_entry_get_entry (HILDON_TOUCH_SELECTOR_ENTRY (selector)));
        entry_text = hildon_entry_get_text (HILDON_ENTRY (entry));
        if (entry_text != NULL && text != NULL && strcmp (entry_text, text)) {
            hildon_entry_set_text (HILDON_ENTRY (entry), text);
        }
    }
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return text;
}
/*******************************************************************************/
static void
on_picker_value_changed (HildonPickerButton * button, gpointer user_data)
{
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

  list_changed(NULL,  user_data, (gchar*)hildon_button_get_value (HILDON_BUTTON (button)));

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}
#endif
/******************************************************************************/
void
list_changed(GtkTreeSelection *sel,  gpointer user_data, gchar *name){
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar     *control_name = NULL;
  GtkWidget *vbox                 = NULL,
            *label                = NULL,
            *window               = NULL,
            *main_table           = NULL,
            *temp_button          = NULL,
            *country_button       = NULL,
            *region_button        = NULL,
            *station_button       = NULL,
            *button               = NULL;
  gint      id_country;
  gint      id_state;
  gchar     *id_code = NULL;
  GSList    *tmp_list = NULL;
  struct lists_struct     *list = NULL;
  enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
  gint type_button = UNKNOWN;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

#if defined OS2009
    button = user_data;
#else
    button = (GtkWidget*)g_object_get_data(G_OBJECT(user_data), "button");
#endif

    label = (GtkWidget*)g_object_get_data(G_OBJECT(button), "label");
    vbox = (GtkWidget*)g_object_get_data(G_OBJECT(button), "vbox");
    window = (GtkWidget*)g_object_get_data(G_OBJECT(button), "window");
    if (sel && gtk_tree_selection_get_selected(sel,&model, &iter)){
        gtk_tree_model_get(model, &iter, 0, &name, -1);
    }
    list = (struct lists_struct*)g_object_get_data(G_OBJECT(window), "list");
    /* This is a very serious error */
    if(!list)
       return;

    if (name){
#if ! defined OS2009
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
        label = gtk_label_new(name);
        g_object_set_data(G_OBJECT(button), "label", (gpointer)label);
        gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
        gtk_widget_show(label);
#endif
        gtk_widget_show(button);
    }

    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp("country_button", control_name))
        type_button = COUNTRY;
    if(!strcmp("source_button", control_name))
        type_button = SOURCE;
    if(!strcmp("region_button", control_name))
        type_button = STATE;
    if(!strcmp("station_button", control_name))
        type_button = TOWN;
    if (type_button == TOWN){
        g_object_set_data(G_OBJECT(window), "station_name", (gpointer)name);
        id_code = get_station_code(g_object_get_data(G_OBJECT(window), "station_source"),
                              GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_region_id")), name);
        g_object_set_data(G_OBJECT(window), "station_code", (gpointer)id_code);
        /* add to list for free memory */
        main_table = g_object_get_data(G_OBJECT(window), "settings_window_table");
        tmp_list = g_object_get_data(g_object_get_data(G_OBJECT(main_table), "stations_box"), "list_for_free");
        tmp_list = g_slist_append(tmp_list, id_code);
        g_object_set_data(G_OBJECT(g_object_get_data(G_OBJECT(main_table), "stations_box")), "list_for_free", (gpointer)tmp_list);
        gtk_widget_set_sensitive(g_object_get_data(G_OBJECT(window), "save_button"), TRUE);
    }
    if (type_button == STATE){
        id_state = get_state_code(g_object_get_data(G_OBJECT(window), "station_source"), (gpointer)name);
        g_object_set_data(G_OBJECT(button), "station_region_id", GINT_TO_POINTER(id_state));
        g_object_set_data(G_OBJECT(button), "station_region", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_region_id", GINT_TO_POINTER(id_state));
        changed_state_handler(NULL, window);

        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button station */
        station_button = create_button(_("City"),
                                   NULL,
                                   "station_button", "station_name", window,
                                   list->stations_list);
        g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
        gtk_box_pack_start(GTK_BOX(vbox), station_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(station_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(station_button);

#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
   }
    if (type_button == COUNTRY){

        id_country = get_country_code(g_object_get_data(G_OBJECT(window), "station_source"), (gpointer)name);
        g_object_set_data(G_OBJECT(button), "station_country_id", GINT_TO_POINTER(id_country));
        g_object_set_data(G_OBJECT(button), "station_country", (gpointer)name);
        g_object_set_data(G_OBJECT(window), "station_country_id", GINT_TO_POINTER(id_country));
        changed_country_handler(NULL, window);

        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
#if 0        
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button station */
        fprintf(stderr,"list->stations_list %p\n", list->stations_list);
        station_button = create_button(_("City"),
                                   NULL,
                                   "station_button", "station_name", window,
                                   list->stations_list);
        g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
        gtk_box_pack_start(GTK_BOX(vbox), station_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(station_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(station_button);
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button Region */   
 
        region_button = create_button(_("Region"),
                                       NULL,
                                       "region_button", "station_region",
                                       window, list->regions_list);
        g_object_set_data(G_OBJECT(window), "region_button", (gpointer)region_button);
        gtk_box_pack_start(GTK_BOX(vbox), region_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(region_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(region_button);
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
        }
#endif

 
   }
    /* TO DO make all if we will have +1 source */
    if (type_button == SOURCE){

        g_object_set_data(G_OBJECT(window), "station_source", name);
        g_object_set_data(G_OBJECT(button), "station_source", name);
        changed_sources_handler(NULL, window);

        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "country_button");
 
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button Country */
        country_button = create_button(_("Country"),
                                       NULL,
                                       "country_button", "station_country",
                                       window, list->countries_list);
        g_object_set_data(G_OBJECT(window), "country_button", (gpointer)country_button);
        gtk_box_pack_start(GTK_BOX(vbox), country_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(country_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(country_button);
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "region_button");
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button Region */   
 
        region_button = create_button(_("Region"),
                                       NULL,
                                       "region_button", "station_region",
                                       window, list->regions_list);
        g_object_set_data(G_OBJECT(window), "region_button", (gpointer)region_button);
        gtk_box_pack_start(GTK_BOX(vbox), region_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(region_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(region_button);
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
        temp_button = (GtkWidget*)g_object_get_data(G_OBJECT(window), "station_button");
#if defined OS2009
        vbox = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "vbox");
        /* Button station */
        station_button = create_button(_("City"),
                                   NULL,
                                   "station_button", "station_name", window,
                                   list->stations_list);
        g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
        gtk_box_pack_start(GTK_BOX(vbox), station_button, TRUE, TRUE, 0);
        g_object_set_data(G_OBJECT(station_button), "vbox", (gpointer)vbox);
        gtk_widget_destroy(temp_button);
        gtk_widget_show(station_button);
#else
        label = (GtkWidget*)g_object_get_data(G_OBJECT(temp_button), "label");
        if (label){
            gtk_widget_destroy(label);
            label = NULL;
            g_object_set_data(G_OBJECT(temp_button), "label", NULL);
        }
#endif
   }
#if !defined OS2009
    /* Destroy window */
    g_signal_emit_by_name(G_OBJECT(user_data), "close", NULL);
#endif

    user_data = NULL;
    
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
}/*******************************************************************************/
void
clear_station(GtkWidget *window){
    GtkWidget       *dialog_window = NULL,
                    *label = NULL;
    gint result;
    struct lists_struct     *list = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    dialog_window = gtk_dialog_new_with_buttons(_("Warning"), NULL,
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    label = gtk_label_new(_("Are you sure?"));
    set_font(label, NULL, 20);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog_window)->vbox), label);
    gtk_dialog_add_button (GTK_DIALOG (dialog_window), GTK_STOCK_NO, GTK_RESPONSE_NO);
    gtk_dialog_add_button (GTK_DIALOG (dialog_window), GTK_STOCK_YES, GTK_RESPONSE_YES);

    gtk_widget_show_all(dialog_window);

    result = gtk_dialog_run(GTK_DIALOG(dialog_window));
    if (result == GTK_RESPONSE_YES){
        list = (struct lists_struct*)g_object_get_data(G_OBJECT(window), "list");
        /* Is not then this is a very serious error */
        if(list){
            /* clear regions list */
            if(list->regions_list){
                gtk_list_store_clear(list->regions_list);
                g_object_unref(list->regions_list);
                list->regions_list = NULL;
            }
            /* clear countries list */
            if(list->countries_list){
                gtk_list_store_clear(list->countries_list);
                g_object_unref(list->countries_list);
                list->countries_list = NULL;
            }
              /* clear stations list */
            if(list->stations_list){
                gtk_list_store_clear(list->stations_list);
                g_object_unref(list->stations_list);
                list->stations_list = NULL;
            }
        }
        g_object_set_data(G_OBJECT(window), "station_name", g_strdup(_("Unknown")));
        g_object_set_data(G_OBJECT(window), "station_code", g_strdup(" "));
        g_object_set_data(G_OBJECT(window), "station_source", g_strdup(" "));
        gtk_toggle_button_set_active(g_object_get_data(G_OBJECT(window), "manual_button"), TRUE);
        gtk_toggle_button_set_active(g_object_get_data(G_OBJECT(window), "gps_button"), FALSE);
        save_station(window);
    }

#ifdef ENABLE_GPS
    if (check_needing_of_gps_station())
        app->gps_need = TRUE;
    else
        app->gps_need = FALSE;
#endif

    if (dialog_window)
        gtk_widget_destroy(dialog_window);

}
/*******************************************************************************/
void
save_station(GtkWidget *window){
    GtkTreeIter     iter;
    gboolean        valid;
    gboolean        is_gps;
    GtkWidget       *stations_box = NULL,
                    *main_window = NULL,
                    *additional_table = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    if (gtk_toggle_button_get_active(g_object_get_data(G_OBJECT(window), "gps_button")))
        is_gps = TRUE;
    else
        is_gps = FALSE;
/*
    iter = add_station_to_user_list(g_strdup(g_object_get_data(G_OBJECT(window), "station_name")),
                                      g_strdup(g_object_get_data(G_OBJECT(window), "station_code")),
                                      is_gps,
                                      g_strdup(g_object_get_data(G_OBJECT(window), "station_source")),
                                      GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_number")));
*/
    if (is_gps)
        iter = add_station_to_user_list( NULL,
                                    NULL,
                                    is_gps,
                                    g_object_get_data(G_OBJECT(window), "station_source"),
                                    GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_number")));
    else
        iter = add_station_to_user_list(g_object_get_data(G_OBJECT(window), "station_name"),
                                    g_object_get_data(G_OBJECT(window), "station_code"),
                                    is_gps,
                                    g_object_get_data(G_OBJECT(window), "station_source"),
                                    GPOINTER_TO_INT(g_object_get_data(G_OBJECT(window), "station_number")));
    valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list), &iter);
    if (valid){
          delete_station_from_user_list_using_iter(iter);
    }else{
        /* update current station code */
        if (app->config->current_station_id)
            g_free(app->config->current_station_id);
        app->config->current_station_id = g_strdup(g_object_get_data(G_OBJECT(window), "station_code"));
        /* update current station name */
        if (app->config->current_station_name)
            g_free(app->config->current_station_name);
        app->config->current_station_name = g_strdup(g_object_get_data(G_OBJECT(window), "station_name"));
        /* update current station source */
        if(app->config->current_station_source)
            g_free(app->config->current_station_source);
        app->config->current_station_source = g_strdup(g_object_get_data(G_OBJECT(window), "station_source"));
    }
    if (!is_gps) 
        update_weather(TRUE);
    /* Update config file */
    config_save(app->config);

    /* Redraw applet */
//    redraw_home_window(FALSE);
    send_dbus_signal (OMWEATHER_SIGNAL_INTERFACE,
                      OMWEATHER_SIGNAL_PATH,
                      OMWEATHER_RELOAD_CONFIG);

    main_window = g_object_get_data(G_OBJECT(window), "settings_window_table");
    stations_box = (gpointer)(g_object_get_data(G_OBJECT(main_window), "stations_box"));
    additional_table = gtk_widget_get_parent(stations_box); 
    if (stations_box){
        free_list((GSList*)(g_object_get_data(G_OBJECT(stations_box), "list_for_free")));
        gtk_widget_destroy(stations_box);
        stations_box = NULL;
    }
    stations_box = create_and_fill_stations_buttons((GtkWidget*)(g_object_get_data(G_OBJECT(window), "settings_window_table")));
    g_object_set_data(G_OBJECT((g_object_get_data(G_OBJECT(window), "settings_window_table"))), "stations_box", (gpointer)stations_box);
    gtk_widget_show (stations_box);
 
#ifdef OS2009
    
    gtk_table_attach((GtkTable*)additional_table, stations_box,
                                0, 1, 1, 2,
                                 GTK_FILL,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (additional_table);
#else
   gtk_table_attach((GtkTable*)(g_object_get_data(G_OBJECT(window), "settings_window_table")),
                                stations_box, 1, 2, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
#endif
#ifdef ENABLE_GPS
    if (check_needing_of_gps_station()){
        app->gps_need = TRUE;
#ifdef OS2009 
	gps_location_changed(NULL, NULL);
#endif
        add_gps_event(1);
    }else
        app->gps_need = FALSE;
#ifdef OS2009
    /* Run gps daemon */
    if (is_gps){
       if (app->gps_control){
          location_gpsd_control_start(app->gps_control);
          app->gps_was_started = TRUE;
       }
    }
#endif
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
changing_of_location (GtkWidget *button, gpointer user_data){
    gint                    result;
    gchar                   *control_name = NULL,
                            dialog_name[255];
    struct lists_struct     *list = NULL;
    GtkWidget               *window = NULL,
                            *main_table = NULL,
                            *list_view = NULL,
                            *scrolled_window = NULL;
    GtkWidget               *config = GTK_WIDGET(user_data);
    enum { UNKNOWN, SOURCE, COUNTRY, STATE, TOWN };
    gint                    type_button = UNKNOWN;
    GtkTreeSelection        *sel;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    *dialog_name = 0;
    control_name = (gchar*)gtk_widget_get_name(GTK_WIDGET(button));
    if(!strcmp("country_button", control_name)){
        type_button = COUNTRY;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting country"));
    }
    if(!strcmp("source_button", control_name)){
        type_button = SOURCE;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting source"));
    }
    if(!strcmp("region_button", control_name)){
        type_button = STATE;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting region"));
    }
    if(!strcmp("station_button", control_name)){
        type_button = TOWN;
        snprintf(dialog_name, sizeof(dialog_name) - 1, _("Selecting station"));
    }

    list = (struct lists_struct*)g_object_get_data(G_OBJECT(config), "list");
    /* This is a very serious error */
    if(!list)
       return;

    window = gtk_dialog_new_with_buttons(dialog_name, NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    main_table = gtk_table_new(8, 8, FALSE);

#if defined OS2009
    scrolled_window = hildon_pannable_area_new ();
#else
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW
                                        (scrolled_window),
                                        GTK_SHADOW_ETCHED_IN);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
#endif
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window), 620, 280);

     if (type_button == COUNTRY){
        list_view = create_tree_view(list->countries_list);
        highlight_current_item((GtkTreeView*)list_view, list->countries_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_country"));
        gtk_widget_set_name(list_view, "countries_list");
    }
    if (type_button == SOURCE){
        if(list->sources_list)
            list_view = create_tree_view(list->sources_list);
        highlight_current_item((GtkTreeView*)list_view, list->sources_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_source"));
        gtk_widget_set_name(list_view, "sources_list");
    }
    if (type_button == STATE){
        list_view = create_tree_view(list->regions_list);
        highlight_current_item((GtkTreeView*)list_view, list->regions_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_region"));
        gtk_widget_set_name(list_view, "states_list");
    }
    if ((type_button == TOWN)){
        list_view = create_tree_view(list->stations_list);
        highlight_current_item((GtkTreeView*)list_view, list->stations_list, (gchar*)g_object_get_data(G_OBJECT(button), "station_name"));
        gtk_widget_set_name(list_view, "stations_list");
    }

    gtk_container_add((GtkContainer *)scrolled_window, (GtkWidget *)list_view);

    gtk_table_attach_defaults(GTK_TABLE(main_table),
                              scrolled_window, 1, 2, 1, 2);

    g_object_set_data(G_OBJECT(window), "button", (gpointer)button);
    sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list_view));
#if defined OS2009
    g_signal_connect (list_view, "row-activated",
                    G_CALLBACK (row_activated_callback), window);
#else
    g_signal_connect (sel, "changed",G_CALLBACK (list_changed), window);
#endif

    gtk_widget_show_all(main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (window)
        gtk_widget_destroy(window);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
choose_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
  changing_of_location (button, user_data);
}
/*******************************************************************************/
void
units_save(GtkWidget *window){

 GtkWidget
           *celsius = NULL,
           *fahrenheit = NULL,
           *meters = NULL,
           *kilometers = NULL,
           *miles = NULL,
           *sea_miles = NULL,
           *meters_s = NULL,
           *kilometers_h = NULL,
           *miles_h = NULL,
           *beaufort = NULL,
           *pressure_mb = NULL,
           *pressure_inHg = NULL,
           *pressure_mmHg = NULL;
#ifdef DEBUGFUNCTIONCALL
     START_FUNCTION;
#endif
    celsius = lookup_widget(window, "celsius_button");
    fahrenheit = lookup_widget(window, "fahrenheit_button");

    if(celsius && fahrenheit){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(celsius)))
            app->config->temperature_units = CELSIUS;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fahrenheit)))  
            app->config->temperature_units = FAHRENHEIT;
    }

    meters = lookup_widget(window, "meters_button");
    kilometers = lookup_widget(window, "kilometers_button");
    miles = lookup_widget(window, "miles_button");
    sea_miles = lookup_widget(window, "sea_miles_button");

    if(meters && kilometers && miles && sea_miles){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(meters))) 
            app->config->distance_units = METERS;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(kilometers)))
                app->config->distance_units = KILOMETERS;
            else{
                if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(miles)))
                    app->config->distance_units = MILES;
                else
                    app->config->distance_units = SEA_MILES;
            }
        }
    }

    meters_s = lookup_widget(window, "meters_s_button");
    kilometers_h = lookup_widget(window, "kilometers_h_button");
    miles_h = lookup_widget(window, "miles_h_button");
    beaufort = lookup_widget(window, "beaufort_scale_button");

    if(meters_s && kilometers_h && miles_h && beaufort){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(meters_s)))
            app->config->wind_units = METERS_S;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(kilometers_h)))
                app->config->wind_units = KILOMETERS_H;
            else{
                if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(miles_h)))
                    app->config->wind_units = MILES_H;
                else
                    app->config->wind_units = BEAUFORT_SCALE;
            }
        }
    }

    pressure_mb = lookup_widget(window, "pressure_mb_button");
    pressure_inHg = lookup_widget(window, "pressure_inHg_button");
    pressure_mmHg = lookup_widget(window, "pressure_mmHg_button");

    if(pressure_mb && pressure_inHg && pressure_mmHg){
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pressure_mb)))
            app->config->pressure_units = MB;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pressure_inHg)))
                app->config->pressure_units = INCH;
            else
                app->config->pressure_units = MM;
        }
    }

}
/*******************************************************************************/
void
units_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
   gint        result;
    GtkWidget   *window = NULL,
                *main_table = NULL,
                *left_alignment = NULL,
                *vertical1_alignment = NULL,
                *vertical2_alignment = NULL,
                *vertical3_alignment = NULL,
                *label_set = NULL,
                *hbox_temperature = NULL,
                *hbox_distance = NULL,
                *hbox_speed = NULL,
                *hbox_pressure = NULL,
                *celsius_button = NULL,
                *fahrenheit_button = NULL,
                *meters_button = NULL,
                *kilometers_button = NULL,
                *miles_button = NULL,
                *sea_miles_button = NULL,
                *meters_s_button = NULL,
                *kilometers_h_button = NULL,
                *miles_h_button = NULL,
                *beaufort_scale_button = NULL,
                *pressure_mb_button = NULL,
                *pressure_inHg_button = NULL,
                *pressure_mmHg_button = NULL,
                *units_button = NULL;
    GSList      *group_pressure = NULL,
                *group_temperature = NULL,
                *group_distance = NULL,
                *group_speed = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

    window = gtk_dialog_new_with_buttons(_("Units"), NULL, 
                                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    gtk_widget_set_name(window, "units_simple_settings_window");

#ifdef OS2009
    #ifdef APPLICATION
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    main_table = gtk_table_new(8, 8, FALSE);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), main_table, TRUE, TRUE, 0);

    label_set = gtk_label_new(_("Temperature"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 150, -1);

    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );


    hbox_temperature = gtk_hbox_new(TRUE, 0);
    group_temperature = NULL;

    celsius_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(celsius_button), gtk_label_new(_("C")));
    gtk_widget_set_size_request(celsius_button, 174, 50);
    GLADE_HOOKUP_OBJECT(window, celsius_button, "celsius_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(celsius_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_temperature), celsius_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(celsius_button), group_temperature);

    fahrenheit_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(fahrenheit_button), gtk_label_new(_("F")));
    gtk_widget_set_size_request(fahrenheit_button, 174, 50);
    GLADE_HOOKUP_OBJECT(window, fahrenheit_button, "fahrenheit_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(fahrenheit_button), FALSE);
    group_temperature = gtk_radio_button_get_group(GTK_RADIO_BUTTON(celsius_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(fahrenheit_button), group_temperature);
    gtk_box_pack_end (GTK_BOX (hbox_temperature), fahrenheit_button, TRUE, TRUE, 0);

    if(app->config->temperature_units == CELSIUS)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(celsius_button), TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fahrenheit_button), TRUE);

    gtk_table_attach((GtkTable*)main_table, hbox_temperature,
                                     2, 3, 1, 2,
                                     GTK_FILL | GTK_EXPAND,
                                     (GtkAttachOptions)0, 10, 0 );

    vertical1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignment,
                                0, 8, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    label_set = gtk_label_new(_("Distance"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 150, -1);

    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );

    hbox_distance = gtk_hbox_new(TRUE, 0);
    group_distance = NULL;
    gtk_box_pack_start (GTK_BOX (hbox_distance), label_set, TRUE, TRUE, 0);
    meters_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(meters_button), gtk_label_new(_("m")));
    gtk_widget_set_size_request(meters_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, meters_button, "meters_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(meters_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_distance), meters_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(meters_button), group_distance);

    kilometers_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(kilometers_button), gtk_label_new(_("km")));
    gtk_widget_set_size_request(kilometers_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, kilometers_button, "kilometers_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(kilometers_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(meters_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(kilometers_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), kilometers_button, TRUE, TRUE, 0);

    miles_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(miles_button), gtk_label_new(_("mi")));
    gtk_widget_set_size_request(miles_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, miles_button, "miles_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(miles_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(kilometers_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(miles_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), miles_button, TRUE, TRUE, 0);

    sea_miles_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(sea_miles_button), gtk_label_new(_("nmi")));
    gtk_widget_set_size_request(sea_miles_button, 43, 50);
    GLADE_HOOKUP_OBJECT(window, sea_miles_button, "sea_miles_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(sea_miles_button), FALSE);
    group_distance = gtk_radio_button_get_group(GTK_RADIO_BUTTON(miles_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(sea_miles_button), group_distance);
    gtk_box_pack_start (GTK_BOX (hbox_distance), sea_miles_button, TRUE, TRUE, 0);

    if(app->config->distance_units == METERS)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(meters_button), TRUE);
    else{
        if(app->config->distance_units == KILOMETERS)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kilometers_button), TRUE);
        else{
            if(app->config->distance_units == MILES)
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(miles_button), TRUE);
            else
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sea_miles_button), TRUE);
        }
    }

    gtk_table_attach((GtkTable*)main_table, hbox_distance,
                                2, 3, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                               (GtkAttachOptions)0, 10, 0 );

    vertical2_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical2_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignment,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );


    label_set = gtk_label_new(_("Speed"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 150, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );

    hbox_speed = gtk_hbox_new(TRUE, 0);
    group_speed = NULL;
    meters_s_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(meters_s_button), gtk_label_new(_("m/s")));
    gtk_widget_set_size_request(meters_s_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, meters_s_button, "meters_s_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(meters_s_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_speed), meters_s_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(meters_s_button), group_speed);

    kilometers_h_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(kilometers_h_button), gtk_label_new(_("km/h")));
    gtk_widget_set_size_request(kilometers_h_button, 58, 50);
    GLADE_HOOKUP_OBJECT(window, kilometers_h_button, "kilometers_h_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(kilometers_h_button), FALSE);
    group_speed = gtk_radio_button_get_group(GTK_RADIO_BUTTON(meters_s_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(kilometers_h_button), group_speed);
    gtk_box_pack_start (GTK_BOX (hbox_speed), kilometers_h_button, TRUE, TRUE, 0);

    miles_h_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(miles_h_button), gtk_label_new(_("mi/h")));
    gtk_widget_set_size_request(miles_h_button, 58, 25);
    GLADE_HOOKUP_OBJECT(window, miles_h_button, "miles_h_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(miles_h_button), FALSE);
    group_speed = gtk_radio_button_get_group(GTK_RADIO_BUTTON(kilometers_h_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(miles_h_button), group_speed);
    gtk_box_pack_start (GTK_BOX (hbox_speed), miles_h_button, TRUE, TRUE, 0);

    beaufort_scale_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(beaufort_scale_button), gtk_label_new(_("Beauf.")));
    gtk_widget_set_size_request(beaufort_scale_button, 58, 25);
    GLADE_HOOKUP_OBJECT(window, beaufort_scale_button, "beaufort_scale_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(beaufort_scale_button), FALSE);
    group_speed = gtk_radio_button_get_group(GTK_RADIO_BUTTON(miles_h_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(beaufort_scale_button), group_speed);
    gtk_box_pack_start(GTK_BOX (hbox_speed), beaufort_scale_button, TRUE, TRUE, 0);

    if(app->config->wind_units == METERS_S)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(meters_s_button), TRUE);
    else{
        if(app->config->wind_units == KILOMETERS_H)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(kilometers_h_button), TRUE);
        else{
            if(app->config->wind_units == MILES_H)
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(miles_h_button), TRUE);
            else
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(beaufort_scale_button), TRUE);
        }
    }

    gtk_table_attach((GtkTable*)main_table, hbox_speed,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );

    vertical3_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical3_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignment,
                                0, 8, 6, 7,
                               (GtkAttachOptions)0,
                               GTK_FILL |  GTK_SHRINK,
                               0, 0 );

    label_set = gtk_label_new(_("Pressure"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 150, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );

    hbox_pressure = gtk_hbox_new(TRUE, 0);
    group_pressure = NULL;
    pressure_mb_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_mb_button), gtk_label_new(_("mb")));
    gtk_widget_set_size_request(pressure_mb_button, 70, 50);
    GLADE_HOOKUP_OBJECT(window, pressure_mb_button, "pressure_mb_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_mb_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_mb_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_mb_button), group_pressure);

    pressure_inHg_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_inHg_button), gtk_label_new(_("inHg")));
    gtk_widget_set_size_request(pressure_inHg_button, 70, 50);
    GLADE_HOOKUP_OBJECT(window, pressure_inHg_button, "pressure_inHg_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_inHg_button), FALSE);
    group_pressure = gtk_radio_button_get_group(GTK_RADIO_BUTTON(pressure_mb_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_inHg_button), group_pressure);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_inHg_button, TRUE, TRUE, 0);

    pressure_mmHg_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(pressure_mmHg_button), gtk_label_new(_("mmHg")));
    gtk_widget_set_size_request(pressure_mmHg_button, 70, 25);
    GLADE_HOOKUP_OBJECT(window, pressure_mmHg_button, "pressure_mmHg_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(pressure_mmHg_button), FALSE);
    group_pressure = gtk_radio_button_get_group(GTK_RADIO_BUTTON(pressure_inHg_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(pressure_mmHg_button), group_pressure);
    gtk_box_pack_start (GTK_BOX (hbox_pressure), pressure_mmHg_button, TRUE, TRUE, 0);

    if(app->config->pressure_units == MB)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pressure_mb_button), TRUE);
    else{
        if(app->config->pressure_units == INCH)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pressure_inHg_button), TRUE);
        else
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pressure_mmHg_button), TRUE);
    }
    gtk_table_attach((GtkTable*)main_table, hbox_pressure,
                                2, 3, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 10, 0 );

    gtk_dialog_add_button (GTK_DIALOG (window), _("Save"), GTK_RESPONSE_YES);
    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES)
        units_save(window);
    if(window)
        gtk_widget_destroy(window);

    units_button = (gpointer)(g_object_get_data(G_OBJECT(button), "units_button"));
    gtk_widget_destroy(units_button);
    create_and_fill_units_box(user_data);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
extended_save(GtkWidget *window){
    GtkWidget   *zoom1 = NULL,
                *zoom2 = NULL,
                *zoom3 = NULL,
                *zoom4 = NULL,
                *zoom5 = NULL,
                *zoom6 = NULL;

#ifdef DEBUGFUNCTIONCALL
   START_FUNCTION;
#endif

    zoom1 = lookup_widget(window, "zoom1");
    zoom2 = lookup_widget(window, "zoom2");
    zoom3 = lookup_widget(window, "zoom3");
    zoom4 = lookup_widget(window, "zoom4");
    zoom5 = lookup_widget(window, "zoom5");
    zoom6 = lookup_widget(window, "zoom6");

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom1))) 
        app->config->scale_in_popup = 1;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom2))) 
        app->config->scale_in_popup = 2;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom3))) 
        app->config->scale_in_popup = 3;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom4))) 
        app->config->scale_in_popup = 4;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom5))) 
        app->config->scale_in_popup = 5;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(zoom6))) 
        app->config->scale_in_popup = 6;
}

/*******************************************************************************/
void
update_save(GtkWidget *window){
    GtkWidget   *never_update = NULL,
                *one_hour_update = NULL,
                *four_hours_update = NULL,
                *one_day_update = NULL,
                *gsm = NULL,
                *wlan = NULL;

#ifdef DEBUGFUNCTIONCALL
   START_FUNCTION;
#endif

    never_update = lookup_widget(window, "never_button");
    one_hour_update = lookup_widget(window, "one_hour_button");
    four_hours_update = lookup_widget(window, "four_hours_button");
    one_day_update = lookup_widget(window, "one_day_button");

    if(never_update && one_hour_update && four_hours_update && one_day_update){
             if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(never_update))) 
                 app->config->update_interval = 0;
        else{
            if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_hour_update)))
                app->config->update_interval = 60;
            else{
                if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(four_hours_update)))
                    app->config->update_interval = 240;
                else{
                    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(one_day_update)))
                        app->config->update_interval = 1440;
                }
            }
        }
    }

    gsm = lookup_widget(window, "gsm_button");
    wlan = lookup_widget(window, "wlan_button");
    if(gsm && wlan){
#ifdef OS2009
        if(hildon_check_button_get_active(gsm))
            app->config->update_gsm = TRUE;
        if(hildon_check_button_get_active(wlan))
            app->config->update_wlan = TRUE;
        if(!hildon_check_button_get_active(gsm))
            app->config->update_gsm = FALSE;
        if(!hildon_check_button_get_active(wlan))
            app->config->update_wlan = FALSE;
#else
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gsm)))
            app->config->update_gsm = TRUE;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan)))
            app->config->update_wlan = TRUE;
        if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gsm)))
            app->config->update_gsm = FALSE;
        if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wlan)))
            app->config->update_wlan = FALSE;
#endif
    }

    if(app->config->update_interval == 0)
        remove_periodic_event();
    else{
        remove_periodic_event();
        add_periodic_event(time(NULL));
    }
}
/*******************************************************************************/
void
update_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
    gint        result;
    GtkWidget   *window = NULL,
                *main_table = NULL,
                *left_alignment = NULL,
                *right_local_alignment = NULL,
                *vertical1_alignment = NULL,
                *vertical2_alignment = NULL,
                *vertical3_alignment = NULL,
                *hbox_period = NULL,
                *label_set = NULL,
                *never_button = NULL,
                *one_hour_button = NULL,
                *four_hours_button = NULL,
                *one_day_button = NULL,
                *gsm_button = NULL,
                *wlan_button = NULL,
                *update_button = NULL;
    GSList      *group_period = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    window = gtk_dialog_new_with_buttons(Q_("Preference|Update"), NULL,
                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    gtk_widget_set_name(window, "update_simple_settings_window");

#ifdef OS2009
    #ifdef APPLICATION
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    main_table = gtk_table_new(8, 9, FALSE);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), main_table, TRUE, TRUE, 0);

    left_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignment,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );

    label_set = gtk_label_new(_("Period"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 140, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    hbox_period = gtk_hbox_new(TRUE, 0);
    group_period = NULL;

    never_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(never_button), gtk_label_new(_("never")));
    gtk_widget_set_size_request(never_button, 145, 60);
    GLADE_HOOKUP_OBJECT(window, never_button, "never_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(never_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_period), never_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(never_button), group_period);

    one_hour_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(one_hour_button), gtk_label_new(_("1 hour")));
    gtk_widget_set_size_request(one_hour_button, 145, 60);
    GLADE_HOOKUP_OBJECT(window, one_hour_button, "one_hour_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(one_hour_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(never_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(one_hour_button), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_period), one_hour_button, TRUE, TRUE, 0);

    four_hours_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(four_hours_button), gtk_label_new(_("4 hours")));
    gtk_widget_set_size_request(four_hours_button, 145, 60);
    GLADE_HOOKUP_OBJECT(window, four_hours_button, "four_hours_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(four_hours_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(one_hour_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(four_hours_button), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_period), four_hours_button, TRUE, TRUE, 0);

    one_day_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(one_day_button), gtk_label_new(_("1 day")));
    gtk_widget_set_size_request(one_day_button, 145, 60);
    GLADE_HOOKUP_OBJECT(window, one_day_button, "one_day_button");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(one_day_button), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(four_hours_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(one_day_button), group_period);
    gtk_box_pack_end(GTK_BOX(hbox_period), one_day_button, TRUE, TRUE, 0);

    if(app->config->update_interval == 0)
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(never_button), TRUE);
    else{
        if(app->config->update_interval == 60)
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(one_hour_button), TRUE);
        else{
            if(app->config->update_interval == 240)
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(four_hours_button), TRUE);
            else{
                if(app->config->update_interval == 1440)
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(one_day_button), TRUE);
            }
        }
    }

    gtk_table_attach((GtkTable*)main_table, hbox_period,
                                2, 5, 1, 2,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignment,
                                0, 8, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
#ifdef OS2009
//    gsm_button = hildon_check_button_new (HILDON_SIZE_AUTO);
    gsm_button = hildon_check_button_new (HILDON_SIZE_HALFSCREEN_WIDTH | HILDON_SIZE_FINGER_HEIGHT);
    gtk_button_set_label (GTK_BUTTON (gsm_button), _("GSM"));
    if(app->config->update_gsm == TRUE)
        hildon_check_button_set_active(gsm_button, TRUE);
    else
        hildon_check_button_set_active(gsm_button, FALSE);
#else
    label_set = gtk_label_new(_("GSM"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 140, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );
    gsm_button = gtk_check_button_new();
    gtk_widget_set_size_request(gsm_button, 50, 50);
    if(app->config->update_gsm == TRUE)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gsm_button), TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gsm_button), FALSE);
#endif
    GLADE_HOOKUP_OBJECT(window, gsm_button, "gsm_button");

    gtk_table_attach((GtkTable*)main_table, gsm_button,
                                2, 3, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    vertical2_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical2_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignment,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

#ifdef OS2009
    wlan_button = hildon_check_button_new (HILDON_SIZE_HALFSCREEN_WIDTH | HILDON_SIZE_FINGER_HEIGHT);
    gtk_button_set_label (GTK_BUTTON (wlan_button), _("WLAN"));
    if(app->config->update_wlan == TRUE)
        hildon_check_button_set_active(wlan_button, TRUE);
    else
        hildon_check_button_set_active(wlan_button, FALSE);

#else
    label_set = gtk_label_new(_("WLAN"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 140, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    wlan_button = gtk_check_button_new();
    gtk_widget_set_size_request(wlan_button, 50, 50);

    if(app->config->update_wlan == TRUE)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wlan_button), TRUE);
    else
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wlan_button), FALSE);
#endif
    GLADE_HOOKUP_OBJECT(window, wlan_button, "wlan_button");

    gtk_table_attach((GtkTable*)main_table, wlan_button,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    vertical3_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical3_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignment,
                                0, 8, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    right_local_alignment = gtk_alignment_new (0.5, 0.5, 1, 1 );
    gtk_widget_set_size_request(right_local_alignment, 350, -1);
    gtk_table_attach((GtkTable*)main_table, right_local_alignment,
                                3, 8, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );


    label_set = gtk_label_new(_("Use GSM and/or WLAN for periodic updating\n the weather information.\n"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 140, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 3, 7, 8,
                                GTK_FILL |  GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_table_attach((GtkTable*)main_table, gtk_alignment_new (0.5, 0.5, 1, 1),
                                0, 8, 8, 9,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 20 );


    gtk_dialog_add_button (GTK_DIALOG (window), _("Save"), GTK_RESPONSE_YES);
    gtk_widget_show_all(window);

    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if(result == GTK_RESPONSE_YES)
        update_save(window);
    if(window)
        gtk_widget_destroy(window);

    update_button = (gpointer)(g_object_get_data(G_OBJECT(button), "update_button"));
    gtk_widget_destroy(update_button);
    create_and_fill_update_box((gpointer)user_data);

#ifdef DEBUGFUNCTIONCALL
   END_FUNCTION;
#endif
}
/*******************************************************************************/
void
extended_button_handler(GtkWidget *button, GdkEventButton *event, gpointer user_data){
    gint        result;
    GtkWidget   *window = NULL,
                *main_table = NULL,
                *left_alignment = NULL,
                *right_local_alignment = NULL,
                *vertical1_alignment = NULL,
                *vertical2_alignment = NULL,
                *vertical3_alignment = NULL,
                *hbox_zooming = NULL,
                *label_set = NULL,
                *zoom1 = NULL,
                *zoom2 = NULL,
                *zoom3 = NULL,
                *zoom4 = NULL,
                *zoom5 = NULL,
                *zoom6 = NULL,
                *extended_button = NULL;
    GSList      *group_period = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    window = gtk_dialog_new_with_buttons(_("Extended Settings"), NULL,
                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, NULL);
    gtk_widget_set_name(window, "extended_simple_settings_window");

#ifdef OS2009
    #ifdef APPLICATION
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    main_table = gtk_table_new(8, 9, FALSE);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), main_table, TRUE, TRUE, 0);

    left_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignment,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );

    label_set = gtk_label_new(_("Zooming in popup window"));
    set_font(label_set, NULL, 20);
    //gtk_widget_set_size_request(label_set, 240, -1);
    gtk_table_attach((GtkTable*)main_table, label_set,
                                1, 2, 0, 1,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    hbox_zooming = gtk_hbox_new(TRUE, 0);
    group_period = NULL;

    zoom1 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom1), gtk_label_new(("100%")));
    gtk_widget_set_size_request(zoom1, 90, 60);
    GLADE_HOOKUP_OBJECT(window, zoom1, "zoom1");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom1), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom1, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom1), group_period);

    zoom2 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom2), gtk_label_new(("110%")));
    gtk_widget_set_size_request(zoom2, 90, 70);
    GLADE_HOOKUP_OBJECT(window, zoom2, "zoom2");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom2), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zoom1));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom2), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom2, TRUE, TRUE, 0);

    zoom3 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom3), gtk_label_new(("130%")));
    gtk_widget_set_size_request(zoom3, 90, 70);
    GLADE_HOOKUP_OBJECT(window, zoom3, "zoom3");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom3), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zoom2));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom3), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom3, TRUE, TRUE, 0);

    zoom4 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom4), gtk_label_new(("150%")));
    gtk_widget_set_size_request(zoom4, 90, 70);
    GLADE_HOOKUP_OBJECT(window, zoom4, "zoom4");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom4), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zoom3));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom4), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom4, TRUE, TRUE, 0);
 
    zoom5 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom5), gtk_label_new(("180%")));
    gtk_widget_set_size_request(zoom5, 90, 70);
    GLADE_HOOKUP_OBJECT(window, zoom5, "zoom5");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom5), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zoom4));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom5), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom5, TRUE, TRUE, 0);
  
    zoom6 = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(zoom6), gtk_label_new(("200%")));
    gtk_widget_set_size_request(zoom6, 90, 70);
    GLADE_HOOKUP_OBJECT(window, zoom6, "zoom6");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(zoom6), FALSE);
    group_period = gtk_radio_button_get_group(GTK_RADIO_BUTTON(zoom5));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(zoom6), group_period);
    gtk_box_pack_start (GTK_BOX (hbox_zooming), zoom6, TRUE, TRUE, 0);

    switch (app->config->scale_in_popup){
        case 
              1:
        default: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom1), TRUE); break;
        case  2: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom2), TRUE); break;
        case  3: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom3), TRUE); break;
        case  4: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom4), TRUE); break;
        case  5: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom5), TRUE); break;
        case  6: gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(zoom6), TRUE); break;
    }


    gtk_table_attach((GtkTable*)main_table, hbox_zooming,
                                1, 5, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical1_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignment,
                                0, 8, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    vertical3_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_size_request(vertical3_alignment, -1, 80);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignment,
                                0, 8, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    right_local_alignment = gtk_alignment_new (0.5, 0.5, 1, 1 );
    gtk_widget_set_size_request(right_local_alignment, 350, -1);
    gtk_table_attach((GtkTable*)main_table, right_local_alignment,
                                3, 8, 7, 8,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );


    gtk_dialog_add_button (GTK_DIALOG (window), _("Save"), GTK_RESPONSE_YES);
    gtk_widget_show_all(window);

    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if(result == GTK_RESPONSE_YES)
        extended_save(window);
    if(window)
        gtk_widget_destroy(window);

    extended_button = (gpointer)(g_object_get_data(G_OBJECT(button), "extended_button"));
    gtk_widget_destroy(extended_button);
    create_and_fill_extended_box((gpointer)user_data);

#ifdef DEBUGFUNCTIONCALL
   END_FUNCTION;
#endif
}

/*******************************************************************************/

/*******************************************************************************/
GtkWidget*
create_button(gchar* name, gchar* value, gchar* button_name, gchar* parameter_name, GtkWidget* widget, GtkListStore *list ){

    GtkWidget *button = NULL;
    GtkWidget *selector = NULL;
    GtkTreeIter iter;
    gchar *element;
    gboolean    valid;
    gint position, i;
    gchar *new_value = NULL;
#if defined OS2009
    HildonTouchSelectorColumn *column = NULL;
#endif

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    
#if defined OS2009
    button = hildon_picker_button_new (HILDON_SIZE_FINGER_HEIGHT | HILDON_SIZE_AUTO_WIDTH,
                    HILDON_BUTTON_ARRANGEMENT_VERTICAL);
    hildon_button_set_title (HILDON_BUTTON (button), name);
    selector = hildon_touch_selector_entry_new_text();

    i = 0;
    position = -1;
    if (value){
        new_value = g_strdup(value);
        new_value = g_strchomp(new_value);
    }
    valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL (list), &iter);
    while (valid) {
        gtk_tree_model_get(GTK_TREE_MODEL(list),
                                            &iter, 0, &element, -1);
        hildon_touch_selector_append_text (HILDON_TOUCH_SELECTOR (selector), element);
       if (element && new_value &&  !(strcmp(element, new_value))){
             position = i;
        }
        if (element){
            g_free(element);
            element = NULL;
        }
        i++;
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(list), &iter);
    }
    if (new_value)
        g_free(new_value);
    column = hildon_touch_selector_get_column(HILDON_TOUCH_SELECTOR(selector), 0);

    g_object_set (G_OBJECT (column), "text-column", 0, NULL);
    hildon_touch_selector_column_set_text_column (column, 0);
    hildon_picker_button_set_selector (HILDON_PICKER_BUTTON (button),
                                          HILDON_TOUCH_SELECTOR (selector));
    hildon_touch_selector_set_print_func (HILDON_TOUCH_SELECTOR (selector), 
                 (HildonTouchSelectorPrintFunc) picker_print_func);
    g_signal_connect (G_OBJECT (button), "value-changed",
                    G_CALLBACK (on_picker_value_changed), button);
    if (position == -1 && i == 1){
        hildon_touch_selector_set_active(HILDON_TOUCH_SELECTOR(selector), 0, 0);
        value = hildon_button_get_value (HILDON_BUTTON (button));
    }else{
        hildon_touch_selector_set_active(HILDON_TOUCH_SELECTOR(selector), 0, position);
    }

#else
    button = create_button_with_2_line_text(name, value, 18, 12);
    g_signal_connect(G_OBJECT(button), BUTTON_RELEASE_EVENT,
                     G_CALLBACK(choose_button_handler), widget);

#endif
/*
    g_object_set_data(G_OBJECT(button), "vbox", (gpointer)vertical_box);
    g_object_set_data(G_OBJECT(button), "label", (gpointer)label_name);
*/
    g_object_set_data(G_OBJECT(button), "window", (gpointer)widget);
    g_object_set_data(G_OBJECT(button), parameter_name, (gpointer)value);

    gtk_widget_set_name(button, button_name);
#if defined OS2009
    if (position == -1 && i == 1){
        hildon_touch_selector_set_active(HILDON_TOUCH_SELECTOR(selector), 0, 0);
        g_object_set_data(G_OBJECT(button), parameter_name, hildon_touch_selector_get_current_text(HILDON_TOUCH_SELECTOR(selector)));
        g_object_set_data(G_OBJECT(widget), parameter_name, hildon_touch_selector_get_current_text(HILDON_TOUCH_SELECTOR(selector)));
    }
#endif
    gtk_widget_set_size_request(button, 180, 80);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return button;
}
/*******************************************************************************/
void
station_setup_button_handler(GtkWidget *button, GdkEventButton *event,
                                    gpointer user_data){
   struct lists_struct *list = NULL;
    gint result;
    GtkWidget *window               = NULL,
              *hbox                 = NULL,
              *vbox_temp            = NULL,
              *main_table           = NULL,
              *main_table_parent    = NULL,
              *main_label           = NULL,
              *label_set            = NULL,
              *manual_button        = NULL,
              *source_button        = NULL,
              *country_button       = NULL,
              *region_button        = NULL,
              *station_button       = NULL,
              *vertical1_alignment  = NULL,
              *vertical2_alignment  = NULL,
              *left_alignment       = NULL,
              *right_alignment      = NULL,
              *gps_button           = NULL,
              *save_button          = NULL;
    GSList    *group                = NULL;
    gchar     *source               = NULL;
    GSList    *tmp_list             = NULL;
    GtkTreeIter                     iter;
    gboolean                        valid,
                                    gps = FALSE;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif

/* Preparing */

    window = gtk_dialog_new_with_buttons(_("Select location"), NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            NULL);
    gtk_widget_set_name(window, "simple_settings_window");
#ifdef OS2009
    #ifdef APPLICATION
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    /* create sources list from aviable sources */
    list = g_new0(struct lists_struct, 1);
    list->sources_list = app->sources_list;
    list->stations_list = NULL;
    list->countries_list = NULL;
    list->regions_list = NULL;

    g_object_set_data(G_OBJECT(window), "list", list);

    g_object_set_data(G_OBJECT(window), "current_source", (gpointer)app->config->current_source);
    g_object_set_data(G_OBJECT(window), "station_region_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_region_id"));
    g_object_set_data(G_OBJECT(window), "station_region", (gpointer)g_object_get_data(G_OBJECT(button), "station_region"));
    g_object_set_data(G_OBJECT(window), "station_country_id", (gpointer)g_object_get_data(G_OBJECT(button), "station_country_id"));
    g_object_set_data(G_OBJECT(window), "station_country", (gpointer)g_object_get_data(G_OBJECT(button), "station_country"));
    g_object_set_data(G_OBJECT(window), "station_source", (gpointer)g_object_get_data(G_OBJECT(button), "station_source"));
    g_object_set_data(G_OBJECT(window), "station_number", (gpointer)g_object_get_data(G_OBJECT(button), "station_number"));
    g_object_set_data(G_OBJECT(window), "station_name", (gpointer)g_object_get_data(G_OBJECT(button), "station_name"));
    g_object_set_data(G_OBJECT(window), "station_code", (gpointer)g_object_get_data(G_OBJECT(button), "station_code"));
    g_object_set_data(G_OBJECT(window), "settings_window_table", (gpointer)g_object_get_data(G_OBJECT(button), "settings_window_table"));
    g_object_set_data(G_OBJECT(window), "station_box", (gpointer)g_object_get_data(G_OBJECT(button), "station_box"));
    g_object_set_data(G_OBJECT(window), "station_is_gps", (gpointer)g_object_get_data(G_OBJECT(button), "station_is_gps"));

    main_table = gtk_table_new(8, 8, FALSE);

    left_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignment,
                                0, 1, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (left_alignment);

    main_label = gtk_label_new((gchar*)g_object_get_data(G_OBJECT(button), "station_label"));
    set_font(main_label, NULL, 20);
    gtk_widget_show (main_label);
    gtk_table_attach((GtkTable*)main_table,main_label,
                                1, 8, 1, 2,
                                GTK_FILL | GTK_SHRINK | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );
    vertical1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical1_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignment,
                                0, 3, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical1_alignment);

    label_set = gtk_label_new(_("Set"));
    set_font(label_set, NULL, 20);
    gtk_widget_set_size_request(label_set, 60, -1);
    gtk_table_attach((GtkTable*)main_table,label_set,
                                1, 2, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    hbox = gtk_hbox_new(TRUE, 0);

    group = NULL;
    manual_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(manual_button), gtk_label_new(_("Manual")));
    gtk_widget_set_size_request(manual_button, 174, 50);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(manual_button), FALSE);
    gtk_box_pack_start (GTK_BOX (hbox), manual_button, TRUE, TRUE, 0);
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(manual_button), group);

    g_object_set_data(G_OBJECT(window), "manual_button", manual_button);

    gps_button = gtk_radio_button_new(NULL);
    gtk_container_add(GTK_CONTAINER(gps_button), gtk_label_new(_("GPS")));
    gtk_widget_set_size_request(gps_button, 174, 50);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(gps_button), FALSE);
    group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(manual_button));
    gtk_radio_button_set_group(GTK_RADIO_BUTTON(gps_button), group);
    gtk_box_pack_start (GTK_BOX (hbox), gps_button, TRUE, TRUE, 0);
    gtk_widget_show (hbox);
    g_signal_connect(G_OBJECT(manual_button), BUTTON_PRESS_EVENT,
                                        G_CALLBACK(manual_button_handler), window);
    g_signal_connect(G_OBJECT(gps_button), BUTTON_PRESS_EVENT,
                                            G_CALLBACK(gps_button_handler), window);

    gtk_table_attach((GtkTable*)main_table, hbox,
                                2, 5, 3, 4,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    vertical2_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignment, -1, 20);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignment,
                                0, 8, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );

    /* Button Source */
    /* Preparing the sources data */
    source = (gchar*)g_object_get_data(G_OBJECT(button), "station_source");
    if (!source || ((source && !strcmp(source," ")) || 
        (source && !strcmp(source,_("Unknown"))))){
          valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list->sources_list), &iter);
          if(valid){
              /* TO DO Warning memory leak */

              gtk_tree_model_get(GTK_TREE_MODEL(list->sources_list), &iter,
                                                0, &source,
                                                -1);

              g_object_set_data(G_OBJECT(window), "station_source", (gpointer)source);
              main_table_parent = g_object_get_data(G_OBJECT(window), "settings_window_table");
              tmp_list = g_object_get_data(g_object_get_data(G_OBJECT(main_table_parent), "stations_box"), "list_for_free");
              tmp_list = g_slist_append(tmp_list, source);
              g_object_set_data(g_object_get_data(G_OBJECT(main_table_parent), "stations_box"), "list_for_free", (gpointer)tmp_list);
        }
     }

     source_button = create_button(_("Source"), source,
                                   "source_button", "station_source", window, 
                                   list->sources_list);
     g_object_set_data(G_OBJECT(window), "source_button", (gpointer)source_button);
     vbox_temp = gtk_vbox_new(TRUE, 0);
     gtk_box_pack_start(GTK_BOX(vbox_temp), source_button, TRUE, TRUE, 0);
     g_object_set_data(G_OBJECT(source_button), "vbox", (gpointer)vbox_temp);
     gtk_table_attach((GtkTable*)main_table, vbox_temp,
                                2, 3, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );

    changed_sources_handler(NULL, window);
    changed_country_handler(NULL, window);
    changed_state_handler(NULL, window);


    /* Button Country */
    country_button = create_button(_("Country"),
                                   (gchar*)g_object_get_data(G_OBJECT(button), "station_country"),
                                   "country_button", "station_country",
                                   window, list->countries_list);
    g_object_set_data(G_OBJECT(window), "country_button", (gpointer)country_button);
    vbox_temp = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_temp), country_button, TRUE, TRUE, 0);
    g_object_set_data(G_OBJECT(country_button), "vbox", (gpointer)vbox_temp);
    gtk_table_attach((GtkTable*)main_table, vbox_temp,
                                3, 4, 5, 6,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    /* Button region */
    region_button = create_button(_("Region"),
                                  (gchar*)g_object_get_data(G_OBJECT(button), "station_region"),
                                  "region_button", "station_region", window, 
                                  list->regions_list);
    g_object_set_data(G_OBJECT(window), "region_button", (gpointer)region_button);
    vbox_temp = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_temp), region_button, TRUE, TRUE, 0);
    g_object_set_data(G_OBJECT(region_button), "vbox", (gpointer)vbox_temp);
 
    gtk_table_attach((GtkTable*)main_table, vbox_temp,
                                2, 3, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 20, 0 );
    /* Button station */
    station_button = create_button(_("City"),
                                   (gchar*)g_object_get_data(G_OBJECT(button), "station_name"),
                                   "station_button", "station_name", window,
                                   list->stations_list);
    g_object_set_data(G_OBJECT(window), "station_button", (gpointer)station_button);
    vbox_temp = gtk_vbox_new(TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox_temp), station_button, TRUE, TRUE, 0);
    g_object_set_data(G_OBJECT(station_button), "vbox", (gpointer)vbox_temp);
 
    gtk_table_attach((GtkTable*)main_table, vbox_temp,
                                3, 4, 6, 7,
                                GTK_FILL | GTK_EXPAND,
                                (GtkAttachOptions)0, 0, 0 );

    right_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignment,
                                4, 5, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignment);
    /* Preparing for GPS station */
    g_object_set_data(G_OBJECT(window), "gps_button", (gpointer)gps_button);
    if (g_object_get_data(G_OBJECT(button), "station_is_gps")){
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gps_button),
                                     TRUE);

        gtk_widget_set_sensitive(source_button, FALSE);
        gtk_widget_set_sensitive(country_button, FALSE);
        gtk_widget_set_sensitive(region_button, FALSE);
        gtk_widget_set_sensitive(station_button, FALSE);
        g_object_set_data(G_OBJECT(gps_button), "gps", (gpointer)gps_button);
    }else{
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(manual_button),
                                     TRUE);

        /* Look up gps station */
        valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(app->user_stations_list), &iter);
        while(valid){
            gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list), &iter, 2,
                                              &gps, -1);
            if(gps) {
                gtk_widget_set_sensitive(gps_button, FALSE);
                break;
            }

            valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(app->user_stations_list),
                                             &iter);
        }
    }

    gtk_widget_show (main_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_dialog_add_button(GTK_DIALOG(window), Q_("GUI|Clear"), GTK_RESPONSE_NO);
    save_button = gtk_dialog_add_button(GTK_DIALOG(window), _("Save"), GTK_RESPONSE_YES);
    g_object_set_data(G_OBJECT(window), "save_button", (gpointer)save_button);
    gtk_widget_set_sensitive(save_button, FALSE);

    gtk_widget_show_all(window);
    /* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    switch(result){
        case GTK_RESPONSE_YES:
            save_station(window);
        break;
        default:
        case GTK_RESPONSE_OK:
        break;
        case GTK_RESPONSE_NO:
            clear_station(window);
        break;
    }

    if (list->countries_list){
        gtk_list_store_clear(list->countries_list);
        g_object_unref(list->countries_list);
    }
    if (list->stations_list){
        gtk_list_store_clear(list->stations_list);
        g_object_unref(list->stations_list);
    }
    if (list->regions_list){
        gtk_list_store_clear(list->regions_list);
        g_object_unref(list->regions_list);
    }

    /* close database if it open */
    if (list->database)
         close_database(list->database);

    if (list)
        g_free(list);
    if(window)
        gtk_widget_destroy(window);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
 
}
/*******************************************************************************/
void
manual_button_handler(GtkWidget *window, GdkEventButton *event, gpointer user_data){
    GtkWidget   *source_button = NULL,
                *country_button = NULL,
                *region_button = NULL,
                *station_button = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(window), TRUE);

    source_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"source_button");
    gtk_widget_set_sensitive(source_button, TRUE);

    country_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"country_button");
    gtk_widget_set_sensitive(country_button, TRUE);

    region_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"region_button");
    gtk_widget_set_sensitive(region_button, TRUE);

    station_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"station_button");
    gtk_widget_set_sensitive(station_button, TRUE);

    gtk_widget_set_sensitive(g_object_get_data(G_OBJECT(user_data), "save_button"), FALSE);
}
/*******************************************************************************/
void
gps_button_handler(GtkWidget *window, GdkEventButton *event, gpointer user_data){
    GtkWidget       *source_button = NULL,
                    *country_button = NULL,
                    *region_button = NULL,
                    *station_button = NULL,
                    *dialog_window = NULL,
                    *label = NULL;
    GtkTreeIter     iter;
    gboolean        valid = FALSE,
                    gps = FALSE;
    gint            result;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(window), TRUE);
    /* Check this gps_button was activity early */
    {
        source_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"source_button");
        gtk_widget_set_sensitive(source_button, FALSE);

        country_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"country_button");
        gtk_widget_set_sensitive(country_button, FALSE);

        region_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"region_button");
        gtk_widget_set_sensitive(region_button, FALSE);

        station_button = (GtkWidget *)g_object_get_data(G_OBJECT(user_data),"station_button");
        gtk_widget_set_sensitive(station_button, FALSE);

        gtk_widget_set_sensitive(g_object_get_data(G_OBJECT(user_data), "save_button"), TRUE);
    }
}
/*******************************************************************************/
GtkWidget*
create_station_button(gint station_number, gchar* station_name_s, gchar *station_code_s, gchar *station_source_s,
                      gint country_id, gchar *station_country_s, gint region_id, gchar *station_region_s,
                      gboolean is_gps){
    GtkWidget       *button = NULL;
    char            buffer[512];
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    snprintf(buffer, sizeof(buffer) - 1, _("Station %i"), station_number + 1);
    button = create_button_with_2_line_text(buffer, station_name_s, 18, 12);
    /* Set widht of button */
    gtk_widget_set_size_request(button, 135, -1);
    g_object_set_data(G_OBJECT(button), "station_number", (gpointer)station_number);
    g_object_set_data(G_OBJECT(button), "station_name", (gpointer)station_name_s);
    g_object_set_data(G_OBJECT(button), "station_code", (gpointer)station_code_s);
    g_object_set_data(G_OBJECT(button), "station_source", (gpointer)station_source_s);
    g_object_set_data(G_OBJECT(button), "station_country", (gpointer)station_country_s);
    g_object_set_data(G_OBJECT(button), "station_region", (gpointer)station_region_s);
    g_object_set_data(G_OBJECT(button), "station_country_id", GINT_TO_POINTER(country_id));
    g_object_set_data(G_OBJECT(button), "station_region_id", GINT_TO_POINTER(region_id));
    if (is_gps)
        g_object_set_data(G_OBJECT(button), "station_is_gps", (gpointer)1);
    else
        g_object_set_data(G_OBJECT(button), "station_is_gps", (gpointer)0);

    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(station_setup_button_handler),
                     (gpointer)button);

#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif
    return button;
}
/*******************************************************************************/
GtkWidget*
create_and_fill_stations_buttons(GtkWidget *main_table){
    GtkWidget       *box = NULL,
                    *station = NULL;
    gboolean        valid = FALSE,
                    valid2 = FALSE,
                    is_gps;
    GtkTreeIter     iter;
    GtkTreeIter     iter2;
    gchar           *station_name = NULL,
                    *station_code = NULL,
                    *station_source = NULL,
                    *station_country = NULL,
                    *station_region = NULL;
    gint            station_country_id = 0,
                    station_region_id = 0;
    GtkListStore    *allinformation_list = NULL;
    gint            station_number = 0;
    GSList          *tmp_list = NULL;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    box = gtk_hbox_new(TRUE, 7);
    valid =
        gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                      (app->user_stations_list), &iter);
    while(valid){
       gtk_tree_model_get(GTK_TREE_MODEL(app->user_stations_list),
                           &iter,
                           0, &station_name,
                           1, &station_code,
                           2, &is_gps,
                           3, &station_source, -1);
        allinformation_list = get_all_information_about_station(station_source, station_code);
        if (allinformation_list)
            valid2 = gtk_tree_model_get_iter_first(GTK_TREE_MODEL
                                              (allinformation_list), &iter2);
        else
            valid2 = FALSE;

        if (station_name)
            tmp_list = g_slist_append(tmp_list, station_name);
        if (station_code)
            tmp_list = g_slist_append(tmp_list, station_code);
        if (station_source)
            tmp_list = g_slist_append(tmp_list, station_source);

        if(valid2){
            gtk_tree_model_get(GTK_TREE_MODEL(allinformation_list),
                                   &iter2,
                                   0, &station_country,
                                   1, &station_region,
                                   2, &station_country_id,
                                   3, &station_region_id,
                                   -1);
            if (station_country)
                tmp_list = g_slist_append(tmp_list, station_country);
            if (station_region)
                tmp_list = g_slist_append(tmp_list, station_region);
        }
        if (!station_country){
                station_country = g_strdup(_("Unknown"));
                tmp_list = g_slist_append(tmp_list, station_country);
        }
        if (!station_region){
                station_region = g_strdup(_("Unknown"));
                tmp_list = g_slist_append(tmp_list, station_region);
        }
        station = create_station_button(station_number,  station_name, station_code, station_source, 
                                        station_country_id, station_country, station_region_id, 
                                        station_region, is_gps);
        g_object_set_data(G_OBJECT(station), "settings_window_table", (gpointer)main_table);
        g_object_set_data(G_OBJECT(station), "station_box", (gpointer)box);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        valid =
            gtk_tree_model_iter_next(GTK_TREE_MODEL
                                     (app->user_stations_list), &iter);
        station_number++;
        if (allinformation_list){
            gtk_list_store_clear(allinformation_list);
            g_object_unref(allinformation_list);
            allinformation_list = NULL;
        }   
       /* Only *four* station for simple mode */
       /*
        if(station_number > 3)
            break;
       */
    }
    /* Added nil station_button */
    while(station_number < 4){
        station = create_station_button(station_number, _("Unknown"), 
                                        NULL, app->config->current_source, -1,
                                        _("Unknown"), -1, _("Unknown"), FALSE);
        g_object_set_data(G_OBJECT(station), "settings_window_table", (gpointer)main_table);
        g_object_set_data(G_OBJECT(station), "station_box", (gpointer)box);
        gtk_box_pack_start(GTK_BOX(box), station, TRUE, TRUE, 0);
        station_number++;
    }
    if (allinformation_list){
        gtk_list_store_clear(allinformation_list);
        g_object_unref(allinformation_list);
    }

    g_object_set_data(G_OBJECT(box), "list_for_free", (gpointer)tmp_list);
    return box;
}
/*******************************************************************************/
void
create_and_fill_units_box(GtkWidget *main_table){
  GtkWidget         *units_button = NULL;
  gchar             *units_string = NULL;
  gchar             *temp_string = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    /* temperature */
    if(app->config->temperature_units == CELSIUS)
        units_string = g_strdup("C");
    else
        units_string = g_strdup("F");
    /* distance units */
    if(app->config->distance_units == METERS){
        temp_string = units_string;
        units_string = g_strjoin(", ", temp_string, _("m"), NULL);
        g_free(temp_string);
    }
    else{
            if(app->config->distance_units == KILOMETERS){
                temp_string = units_string;
                units_string = g_strjoin(", ", temp_string, _("km"), NULL);
                g_free(temp_string);
            }
            else{
                if(app->config->distance_units == MILES){
                    temp_string = units_string;
                    units_string = g_strjoin(", ", temp_string, _("mi"), NULL);
                    g_free(temp_string);
                }
                else{
                    temp_string = units_string;
                    units_string = g_strjoin(", ", temp_string, _("nmi"), NULL);
                    g_free(temp_string);
                }
            }
        }

    /* wind units */
    if(app->config->wind_units == METERS_S){
        temp_string = units_string;
        units_string = g_strjoin(", ", temp_string, _("m/s"), NULL);
        g_free(temp_string);
    }
    else{
        if(app->config->wind_units == KILOMETERS_H){
            temp_string = units_string;
            units_string = g_strjoin(", ", temp_string, _("km/h"), NULL);
            g_free(temp_string);
        }
        else{
            if(app->config->wind_units == MILES_H){
                temp_string = units_string;
                units_string = g_strjoin(", ", temp_string, _("mi/h"), NULL);
                g_free(temp_string);
            }
            else{
                temp_string = units_string;
                units_string = g_strjoin(", ", temp_string, _("Beaufort scale"), NULL);
                g_free(temp_string);
            }
        }
    }

    /* pressure */
    if(app->config->pressure_units == MB){
        temp_string = units_string;
        units_string = g_strjoin(", ", temp_string, _("mb"), NULL);
        g_free(temp_string);
    }
    else{
        if(app->config->pressure_units == INCH){
            temp_string = units_string;
            units_string = g_strjoin(", ", temp_string, _("inch"), NULL);
            g_free(temp_string);
        }
        else{
            temp_string = units_string;
            units_string = g_strjoin(", ", temp_string, _("mm"), NULL);
            g_free(temp_string);
        }
    }
    /* TO DO !!!!!! check memory leak in units_string */
    units_button = create_button_with_2_line_text(_("Units"), units_string, 18, 12);
    g_free(units_string);

    gtk_widget_set_size_request(units_button, 490, 65);
    gtk_table_attach((GtkTable*)main_table, units_button,
                                1, 2, 3, 4, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (units_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(units_button), "settings_window_table", (gpointer)main_table);
    g_object_set_data(G_OBJECT(units_button), "units_button", (gpointer)units_button);

    g_signal_connect(G_OBJECT(units_button), BUTTON_RELEASE_EVENT,
                              G_CALLBACK(units_button_handler), (gpointer)main_table);

}
/*******************************************************************************/
void
create_and_fill_extended_box(GtkWidget *main_table){
    GtkWidget       *update_button = NULL;
    gchar           *output_string = NULL;
    gchar           *temp_string = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    temp_string = g_strdup(_("Zoom in popup window ")); 
    switch (app->config->scale_in_popup){
        case 
              1:
        default: output_string = g_strjoin(temp_string, "100%", NULL); break;
        case  2: output_string = g_strjoin(temp_string, "110%", NULL); break;
        case  3: output_string = g_strjoin(temp_string, "130%", NULL); break;
        case  4: output_string = g_strjoin(temp_string, "150%", NULL); break;
        case  5: output_string = g_strjoin(temp_string, "180%", NULL); break;
        case  6: output_string = g_strjoin("   ", temp_string, "200%", NULL); break;
    }

    g_free(temp_string);
    update_button = create_button_with_2_line_text(_("Extended Settings"), output_string, 18, 12);
    g_free(output_string);

    gtk_widget_set_size_request(update_button, 490, 65);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 9, 10, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (update_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(update_button), "settings_window_table", (gpointer)main_table);
    g_object_set_data(G_OBJECT(update_button), "extended_button", (gpointer)update_button);
    g_signal_connect(G_OBJECT(update_button), BUTTON_RELEASE_EVENT,
                                 G_CALLBACK(extended_button_handler), (gpointer)main_table);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/

void
create_and_fill_update_box(GtkWidget *main_table){
    GtkWidget       *update_button = NULL;
    gchar           *update_string = NULL;
    gchar           *temp_string = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    if(app->config->update_interval == 0)
        update_string = g_strdup(_("Never"));
    else{
        if(app->config->update_interval == 60)
            update_string = g_strdup(_("1 hour"));
        else{
            if(app->config->update_interval == 240)
                update_string = g_strdup(_("4 hours"));
            else{
                if(app->config->update_interval == 1440)
                    update_string = g_strdup(_("1 day"));
            }
        }
    }

    if(app->config->update_gsm && app->config->update_wlan){
        temp_string = update_string;
        update_string = g_strjoin(", ", temp_string, _("GSM+WLAN"), NULL);
        g_free(temp_string);
    }
    else{
        if(app->config->update_gsm){
            temp_string = update_string;
            update_string = g_strjoin(", ", temp_string, _("GSM"), NULL);
            g_free(temp_string);
        }
        else{
            if(app->config->update_wlan){
                temp_string = update_string;
                update_string = g_strjoin(", ", update_string, _("WLAN"), NULL);
                g_free(temp_string);
            }
        }
    }

    update_button = create_button_with_2_line_text(Q_("Preference|Update"), update_string, 18, 12);
    g_free(update_string);

    gtk_widget_set_size_request(update_button, 490, 65);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 7, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (update_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(update_button), "settings_window_table", (gpointer)main_table);
    g_object_set_data(G_OBJECT(update_button), "update_button", (gpointer)update_button);
    g_signal_connect(G_OBJECT(update_button), BUTTON_RELEASE_EVENT,
                                 G_CALLBACK(update_button_handler), (gpointer)main_table);
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
void
create_and_fill_widget_style_box(GtkWidget *main_table){
    GtkWidget
#if !defined OS2009
                    *widget_style_description = NULL,
                    *widget_style_label = NULL,
                    *widget_style_hbox = NULL,
                    *widget_style_vbox = NULL,
#endif
                    *widget_style_icon = NULL,
                    *widget_style_button = NULL;

    GdkPixbuf       *icon_buffer = NULL;
    gchar           buffer[256],
                    *widget_style_string = NULL;
#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    switch(app->config->icons_layout){
        case PRESET_NOW_PLUS_SEVEN: 
                                widget_style_string = g_strdup_printf("%s(%s)",_("Now + 7 days vert."),app->config->mod);
                                break;
        case PRESET_NOW_PLUS_TWO: 
                                widget_style_string = g_strdup_printf("%s(%s)",_("Now + 2 days vert."),app->config->mod);
                                break;
        case PRESET_NOW_PLUS_THREE_V: 
                                widget_style_string = g_strdup_printf("%s(%s)",_("Now + 3 days vert."),app->config->mod);
                                break;  
        case PRESET_NOW:
                                widget_style_string = g_strdup_printf("%s(%s)",_("Now"),app->config->mod);
                                break;
        case PRESET_NOW_PLUS_THREE_H: 
                                widget_style_string = g_strdup_printf("%s(%s)",_("Now + 3 days hor."),app->config->mod);
                                break;
        case ONE_ROW: 
        case ONE_COLUMN: 
        case TWO_ROWS:
        case TWO_COLUMNS:
        case COMBINATION: 
                                widget_style_string = g_strdup(_("Custom"));
                                break;
    }

    /*Icon image*/
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer) - 1, "%s%s/44a.png", ICONS_PATH,
                      (gchar *) (app->config->icon_set));
    if (access(buffer, R_OK) != 0 || !app->config->animation)
        snprintf(buffer, sizeof(buffer) - 1, "%s%s/44.png", ICONS_PATH,
                          (gchar *) (app->config->icon_set));
    icon_buffer = gdk_pixbuf_new_from_file_at_size(buffer, 60,
                                                  60, NULL);
    if (icon_buffer) {
        widget_style_icon = gtk_image_new_from_pixbuf(icon_buffer);
        g_object_unref(G_OBJECT(icon_buffer));
    }
#if defined OS2009
    widget_style_button = create_button_with_2_line_text(_("Widget style"), widget_style_string, 18, 12);
    if (widget_style_string)
        g_free(widget_style_string);
    hildon_button_set_image (HILDON_BUTTON (widget_style_button), widget_style_icon);
    hildon_button_set_image_position (HILDON_BUTTON (widget_style_button), GTK_POS_RIGHT);
#else

    widget_style_button = gtk_button_new ();
    widget_style_label = gtk_label_new(_("Widget style"));

    set_font(widget_style_label, NULL, 12);
/*    gtk_widget_set_size_request(widget_style_label, 380, -1);*/
    gtk_widget_show (widget_style_label);

    widget_style_description = gtk_label_new (widget_style_string);
    set_font(widget_style_description, NULL, 18);
/*    gtk_widget_set_size_request(widget_style_description, 380, -1);*/

     gtk_widget_show(widget_style_description);

      widget_style_vbox = gtk_vbox_new(TRUE, 2);
      gtk_widget_show(widget_style_vbox);

      widget_style_hbox = gtk_hbox_new(FALSE, 0);
      gtk_widget_show(widget_style_hbox);

      gtk_widget_show (widget_style_icon);

    gtk_box_pack_start(GTK_BOX(widget_style_vbox), widget_style_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widget_style_vbox), widget_style_description, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(widget_style_hbox), widget_style_vbox, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(widget_style_hbox), widget_style_icon, TRUE, TRUE, 0);
    gtk_container_add (GTK_CONTAINER (widget_style_button), widget_style_hbox);

#endif
    gtk_widget_set_size_request(widget_style_button, 490, 65);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    gtk_widget_show (widget_style_button);
    gtk_widget_show (main_table);
    g_object_set_data(G_OBJECT(widget_style_button), "settings_window_table",
                                        (gpointer)main_table);
    g_object_set_data(G_OBJECT(widget_style_button), "widget_style_button", 
                                        (gpointer)widget_style_button);
    
    g_signal_connect(G_OBJECT(widget_style_button), BUTTON_RELEASE_EVENT,
                            G_CALLBACK(widget_style_setup_button_handler),
                           (gpointer)main_table);
}
/*******************************************************************************/
void
weather_simple_window_settings(gpointer user_data){
  GtkWidget
          *window               = NULL,
          *main_table           = NULL,
          *additional_table     = NULL, 
          *stations_box         = NULL,
          *left_alignment       = NULL,
          *right_alignment      = NULL,
          *vertical0_alignment  = NULL,
          *vertical1_alignment  = NULL,
          *vertical2_alignment  = NULL,
          *vertical3_alignment  = NULL,
          *vertical4_alignment  = NULL,
          *stations_area        = NULL;
  gint    result;

#ifdef DEBUGFUNCTIONCALL
    START_FUNCTION;
#endif
    window = gtk_dialog_new();
#ifdef OS2009
    #ifdef APPLICATION
        gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->main_view));
    #else
        if (app->popup_window)
            gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(app->popup_window));
    #endif
#endif

    app->settings_window = window;
    gtk_window_set_title(GTK_WINDOW(window), _("OMWeather Settings"));
    init_landscape(window);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    main_table = gtk_table_new(4,11, FALSE);
    stations_box = create_and_fill_stations_buttons(main_table);
    g_object_set_data(G_OBJECT(main_table), "stations_box", (gpointer)stations_box);
    additional_table = gtk_table_new(1,3, FALSE);

#ifdef OS2009
    
    stations_area = hildon_pannable_area_new();
    g_object_set (stations_area, "mov_mode", HILDON_MOVEMENT_MODE_HORIZ,
                  "vscrollbar_policy", GTK_POLICY_NEVER, NULL);
    gtk_table_attach((GtkTable*)additional_table, stations_box,
                                0, 1, 1, 2,
                                 GTK_FILL,
                                (GtkAttachOptions)0, 0, 0 );

    g_object_set_data(G_OBJECT(main_table), "stations_box", (gpointer)stations_box);
    hildon_pannable_area_add_with_viewport (HILDON_PANNABLE_AREA (stations_area), additional_table);
    gtk_widget_show (stations_area);
    gtk_widget_show (additional_table);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       stations_area, TRUE, TRUE, 0);
    gtk_widget_show (stations_box);
    gtk_widget_set_size_request(stations_area, -1, 90);
#else
    left_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(left_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, left_alignment,
                                0, 1, 0, 8,
                                 GTK_FILL,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (left_alignment);

    vertical0_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical0_alignment, -1, 15);
    gtk_table_attach((GtkTable*)main_table, vertical0_alignment,
                                0, 3, 0, 1,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical0_alignment);

    gtk_table_attach((GtkTable*)main_table,stations_box,
                                1, 2, 1, 2, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (stations_box);

    vertical1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical1_alignment, -1, 15);
    gtk_table_attach((GtkTable*)main_table, vertical1_alignment,
                                0, 3, 2, 3,
                                (GtkAttachOptions)0,
                                GTK_FILL |  GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical1_alignment);

#endif
    create_and_fill_units_box(main_table);

    vertical2_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical2_alignment, -1, 15);
    gtk_table_attach((GtkTable*)main_table, vertical2_alignment,
                                0, 3, 4, 5,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show(vertical2_alignment);

    /*widget_style_button = gtk_button_new_with_label (_("Widget style"));
    gtk_widget_set_size_request(widget_style_button, 490, 70);
    gtk_widget_show (widget_style_button);
    gtk_table_attach((GtkTable*)main_table, widget_style_button,
                                1, 2, 5, 6, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );
    g_signal_connect(G_OBJECT(widget_style_button), BUTTON_RELEASE_EVENT,
                   G_CALLBACK(widget_style_setup_button_handler),
                   (gpointer)widget_style_button);*/

    create_and_fill_widget_style_box(main_table);

    vertical3_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical3_alignment, -1, 15);
    gtk_table_attach((GtkTable*)main_table, vertical3_alignment,
                                0, 6, 6, 7,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical3_alignment);

    create_and_fill_update_box(main_table);


/*    update_button = gtk_button_new_with_label (_("Update"));
    gtk_widget_set_size_request(update_button, 490, 70);
    gtk_widget_show (update_button);
    gtk_table_attach((GtkTable*)main_table, update_button,
                                1, 2, 7, 8, (GtkAttachOptions)0,
                                (GtkAttachOptions)0, 0, 0 );

    g_signal_connect(G_OBJECT(update_button), BUTTON_RELEASE_EVENT,
                           G_CALLBACK(update_button_handler), (gpointer)main_table);
*/
    vertical4_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(vertical4_alignment, -1, 15);
    gtk_table_attach((GtkTable*)main_table, vertical4_alignment,
                                0, 6, 8, 9,
                                (GtkAttachOptions)0,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                0, 0 );
    gtk_widget_show (vertical4_alignment);

    create_and_fill_extended_box(main_table);

    right_alignment = gtk_alignment_new (0.5, 0.5, 1, 1  );
    gtk_widget_set_size_request(right_alignment, 5, -1);
    gtk_table_attach((GtkTable*)main_table, right_alignment,
                                4, 5, 0, 8,
                                GTK_FILL | GTK_EXPAND | GTK_SHRINK,
                                (GtkAttachOptions)0, 0, 0 );
    gtk_widget_show (right_alignment);

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       main_table, TRUE, TRUE, 0);
    gtk_dialog_add_button (GTK_DIALOG (window), _("Save"), GTK_RESPONSE_YES);

    gtk_widget_show(main_table);

    gtk_widget_show(window);
/* start dialog window */
    result = gtk_dialog_run(GTK_DIALOG(window));
    if (result == GTK_RESPONSE_YES){
        /* Save config file */
        config_save(app->config);
        /* Update information about connection to Internet */
        check_current_connection();
    }

    /* free memory for stations data */
    if (g_object_get_data(G_OBJECT(main_table), "stations_box")){
        free_list((GSList*)(g_object_get_data(g_object_get_data(G_OBJECT(main_table), "stations_box"), "list_for_free")));
    }

    if (window)
        gtk_widget_destroy(window);
    app->settings_window = NULL;
#ifndef APPLICATION   
    if (app->reload)
        reload_omw_plugin();
#else
/* Send signal for reload omweather plugin */
        send_dbus_signal (OMWEATHER_SIGNAL_INTERFACE,
                          OMWEATHER_SIGNAL_PATH,
                          OMWEATHER_RELOAD_PLUGIN);
#endif
#ifdef DEBUGFUNCTIONCALL
    END_FUNCTION;
#endif

}
/*******************************************************************************/
