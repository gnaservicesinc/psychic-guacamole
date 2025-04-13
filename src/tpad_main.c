/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2014, 2015 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_main.c , is part of tpad.
 *
 *   tpad is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *   tpad is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with tpad.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
////////////////////////////////////////////////////////////////////////
#include "tpad_headers.h"
extern GtkSourceBuffer *mBuff;
extern gboolean save_locked;
extern GtkWidget *window;
extern gchar* origfile;
extern GtkSourceView *view;
extern gchar path_temp_file[];
extern int setModified;
extern int madetemp;
extern gboolean disable_file_watch;
extern int tpad_wach_thread_set;
extern int spell_state;
extern int run_count_update;
extern unsigned int gbid;
// Using libspelling instead of gtkspell
extern SpellingChecker* spell_checker;
extern SpellingTextBufferAdapter* spell_adapter;
////////////////////////////////////////////////////////////////////////
GtkWidget *window=NULL;
GtkSourceView *view=NULL;
G_LOCK_DEFINE (mBuff);
GtkSourceBuffer *mBuff;
int spell_state=2;
GtkSpinButton *xSpinButton,*ySpinButton,*uSpinButton;
unsigned int gbid=0;
gchar *content;
gchar* origfile;
gboolean save_locked;
int tpad_wach_thread_set=0;
int setModified=0;
int madetemp=0;
int run_count_update=1;
gboolean disable_file_watch = FALSE;
gpointer current_data = NULL;

////////////////////////////////////////////////////////////////////////
/* TPAD_MAIN  */
////////////////////////////////////////////////////////////////////////
static GtkApplication *app = NULL;

// Callback for GtkApplication activate signal
static void app_activate(GtkApplication *app, gpointer user_data) {
	char* fchar = (char*)user_data;

	// Set up the application actions for the menu system
	tpad_setup_actions(app);

	// Initialize file path tracking
	tpad_fp_init();

	// Create the main window and UI
	window = tpad_new_ui();
	gtk_window_set_application(GTK_WINDOW(window), app);
	gtk_widget_set_visible(window, TRUE);

	// Initialize the editor
	new_file();
	show_file(g_strdup(fchar ? fchar : ""));
	if(cfg_spell()) toggle_spelling();
}

extern int tpad_main(char* fchar)
{
	////////////////////////////////////////////////////////////////////////
	/* GTK INIT */
	////////////////////////////////////////////////////////////////////////
	// In GTK4, we use GtkApplication
	int status;

	// Create a new GtkApplication
	// Use G_APPLICATION_DEFAULT_FLAGS instead of deprecated G_APPLICATION_FLAGS_NONE
	app = gtk_application_new("com.gnaservicesinc.tpad", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), fchar);

	// Initialize configuration
	config_setup();
	//[FIXME] Unity integration needed.
	#ifdef HAVE_LIBUNITY
	//ui_unity_init();
	#endif
	printf("\nOpening file:\t%s.\ntpad License and Copyright Notice:\n\nCopyright (C) 2014, 2015, 2016, 2017, 2018 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>\n\ntpad is free software: you can redistribute it and/or modify it\nunder the terms of the GNU General Public License as published by the\nFree Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\ntpad is distributed in the hope that it will be useful, but\nWITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\nSee the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License along\nwith this program.  If not, see <http://www.gnu.org/licenses/>.\n", fchar);
	////////////////////////////////////////////////////////////////////////
	/* GTK MAIN */
	////////////////////////////////////////////////////////////////////////
	// Run the GtkApplication main loop
	status = g_application_run(G_APPLICATION(app), 0, NULL);

	// Clean up
	g_object_unref(app);

	////////////////////////////////////////////////////////////////////////
	/* MAIN RETURN */
	////////////////////////////////////////////////////////////////////////
	return status;
	////////////////////////////////////////////////////////////////////////
}