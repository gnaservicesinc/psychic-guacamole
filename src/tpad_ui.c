/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2013, 2014, 2025 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_ui.c , is part of tpad.
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
 *********************************************************************************/

#include "tpad_headers.h"

extern GtkSourceBuffer *mBuff;
extern GtkSourceView *view;
extern GtkWidget *window;
extern GtkWidget *mainbox;
extern GtkWidget *menubar;
extern GtkWidget *toolbar;
extern GtkWidget *scrolled_window;
extern gchar *content;
extern gchar path_temp_file[];
extern int setModified;
extern int madetemp;
extern gboolean disable_file_watch;
extern int tpad_wach_thread_set;
extern int spell_state;
extern int run_count_update;
// Using libspelling instead of gtkspell
extern SpellingChecker* spell_checker;
extern SpellingTextBufferAdapter* spell_adapter;
extern GdkDisplay* Display;
extern GtkWidget *spelling;
extern GtkWidget *Notebook;
GtkWidget *spelling;
GtkWidget *newfile;
GtkWidget *open_btn; // Renamed from 'open' to avoid conflict with C library function
GtkWidget *save;
GtkWidget *saveas;
GtkWidget *quit;
GtkWidget *reload;
GtkWidget *file;
GtkWidget *hhash;
GtkWidget *settings;
GtkWidget *help;
GtkWidget *selectfont;
GtkWidget *linenumber;
GtkWidget *lang_chk;
GtkWidget *ShowFullPath;
GtkWidget *UIPrefs;
GtkWidget *DocStats;
GtkWidget *keepAbove;
GtkWidget *lineWp;
GtkWidget *checkOnOpen;
#ifdef HAVE_LIBMAGIC
GtkWidget *UseMagic;
#endif
#ifdef AUTO_TAB_TOGGLE
GtkWidget *AutoTab;
#endif

GtkWidget * tpad_new_ui() {
	////////////////////////////////////////////////////////////////////////
	// Get information on the display for use with config.
	////////////////////////////////////////////////////////////////////////
	Display = gdk_display_get_default();
	GdkMonitor *monitor = gdk_display_get_primary_monitor(Display);
	GdkRectangle workarea;
	gdk_monitor_get_workarea(monitor, &workarea);
	cfg_set_screen_width(workarea.width);
	cfg_set_screen_height(workarea.height);
	////////////////////////////////////////////////////////////////////////
	// Main Window Setup
	////////////////////////////////////////////////////////////////////////
	// Create a new window using GTK4 API
	window = gtk_window_new();
	gtk_window_set_title(GTK_WINDOW(window), "Tpad");
	gtk_window_set_default_size(GTK_WINDOW(window), cfg_wWidth(), cfg_wHeight());

	// Set the window icon
	gtk_window_set_icon_name(GTK_WINDOW(window), "tpad");

	mainbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	////////////////////////////////////////////////////////////////////////
	// Create a menu bar using GtkBox and GtkMenuButton for GTK4
	////////////////////////////////////////////////////////////////////////
	menubar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

	// Create a GMenu for the file menu
	GMenu *file_menu_model = g_menu_new();
	GMenuItem *new_item = g_menu_item_new(_NEW_FILE_MNEMONIC, "app.new");
	GMenuItem *open_item = g_menu_item_new(_OPEN_FILE_MNEMONIC, "app.open");
	GMenuItem *save_item = g_menu_item_new(_SAVE_FILE_MNEMONIC, "app.save");
	GMenuItem *saveas_item = g_menu_item_new(_SAVE_FILE_AS_MNEMONIC, "app.saveas");
	GMenuItem *reload_item = g_menu_item_new(_RELOAD_MNEMONIC, "app.reload");
	GMenuItem *quit_item = g_menu_item_new(_QUIT_MNEMONIC, "app.quit");

	g_menu_append_item(file_menu_model, new_item);
	g_menu_append_item(file_menu_model, open_item);
	g_menu_append_item(file_menu_model, save_item);
	g_menu_append_item(file_menu_model, saveas_item);
	g_menu_append_section(file_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(file_menu_model, reload_item);
	g_menu_append_section(file_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(file_menu_model, quit_item);

	// Create a menu button for the file menu
	file = gtk_menu_button_new();
	// In GTK4, we use a label widget
	GtkWidget *file_label = gtk_label_new(_FILE_MENU);
	gtk_menu_button_set_child(GTK_MENU_BUTTON(file), file_label);
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(file), G_MENU_MODEL(file_menu_model));

	// Store references to menu items for compatibility with the old code
	newfile = gtk_button_new_with_label(_NEW_FILE_MNEMONIC);
	open_btn = gtk_button_new_with_label(_OPEN_FILE_MNEMONIC);
	save = gtk_button_new_with_label(_SAVE_FILE_MNEMONIC);
	saveas = gtk_button_new_with_label(_SAVE_FILE_AS_MNEMONIC);
	quit = gtk_button_new_with_label(_QUIT_MNEMONIC);
	reload = gtk_button_new_with_label(_RELOAD_MNEMONIC);

	// Add the file menu button to the menubar
	gtk_box_append(GTK_BOX(menubar), file);
	////////////////////////////////////////////////////////////////////////
	// Setup Hash Menu using GMenu for GTK4
	////////////////////////////////////////////////////////////////////////
	// Create a GMenu for the hash menu
	GMenu *hash_menu_model = g_menu_new();
	GMenuItem *h512_item = g_menu_item_new("SHA512", "app.hash512");
	GMenuItem *h256_item = g_menu_item_new("SHA256", "app.hash256");
	GMenuItem *hmd5_item = g_menu_item_new("MD5", "app.hashmd5");
	GMenuItem *base64_item = g_menu_item_new("Base64", "app.base64");
	GMenuItem *fbase64_item = g_menu_item_new("From Base64", "app.fbase64");
	GMenuItem *h512f_item = g_menu_item_new("SHA512 File", "app.hash512file");
	GMenuItem *h256f_item = g_menu_item_new("SHA256 File", "app.hash256file");
	GMenuItem *hmd5f_item = g_menu_item_new("MD5 File", "app.hashmd5file");
	GMenuItem *base64f_item = g_menu_item_new("Base64 File", "app.base64file");
	GMenuItem *fbase64f_item = g_menu_item_new("From Base64 File", "app.fbase64file");

	g_menu_append_item(hash_menu_model, h512_item);
	g_menu_append_item(hash_menu_model, h256_item);
	g_menu_append_item(hash_menu_model, hmd5_item);
	g_menu_append_section(hash_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(hash_menu_model, base64_item);
	g_menu_append_item(hash_menu_model, fbase64_item);
	g_menu_append_section(hash_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(hash_menu_model, h512f_item);
	g_menu_append_item(hash_menu_model, h256f_item);
	g_menu_append_item(hash_menu_model, hmd5f_item);
	g_menu_append_section(hash_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(hash_menu_model, base64f_item);
	g_menu_append_item(hash_menu_model, fbase64f_item);

	// Create a menu button for the hash menu
	hhash = gtk_menu_button_new();
	GtkWidget *hash_label = gtk_label_new(_HASH_MENU);
	gtk_menu_button_set_child(GTK_MENU_BUTTON(hhash), hash_label);
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(hhash), G_MENU_MODEL(hash_menu_model));

	// Add the hash menu button to the menubar
	gtk_box_append(GTK_BOX(menubar), hhash);
	////////////////////////////////////////////////////////////////////////
	// Setup Settings Menu using GMenu for GTK4
	////////////////////////////////////////////////////////////////////////
	// Create a GMenu for the settings menu
	GMenu *settings_menu_model = g_menu_new();
	GMenuItem *font_item = g_menu_item_new(_SELECT_FONT_MNEMONIC, "app.font");
	GMenuItem *line_numbers_item = g_menu_item_new(_LINE_NUMBER, "app.linenumbers");
	GMenuItem *language_item = g_menu_item_new(_LANGUAGE, "app.language");
	GMenuItem *spelling_item = g_menu_item_new(_SPELLING, "app.spelling");
	GMenuItem *full_path_item = g_menu_item_new(_SHOW_FULL_PATH, "app.fullpath");
	GMenuItem *prefs_item = g_menu_item_new(_PREF_MNEMONIC, "app.preferences");
	GMenuItem *stats_item = g_menu_item_new(_WORD_COUNT, "app.stats");
	GMenuItem *keep_above_item = g_menu_item_new(_KEEP_WIND_ABOVE, "app.keepabove");
	GMenuItem *line_wrap_item = g_menu_item_new(_LINE_WP, "app.linewrap");
	GMenuItem *check_open_item = g_menu_item_new(_CHECK_ON_OPEN, "app.checkonopen");
#ifdef HAVE_LIBMAGIC
	GMenuItem *use_magic_item = g_menu_item_new(_SETTINGS_USE_MAGIC, "app.usemagic");
#endif
#ifdef AUTO_TAB_TOGGLE
	GMenuItem *auto_tab_item = g_menu_item_new(_AUTOTAB, "app.autotab");
#endif

	g_menu_append_item(settings_menu_model, font_item);
	g_menu_append_section(settings_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(settings_menu_model, line_numbers_item);
	g_menu_append_item(settings_menu_model, language_item);
#ifdef AUTO_TAB_TOGGLE
	g_menu_append_item(settings_menu_model, auto_tab_item);
#endif
	g_menu_append_item(settings_menu_model, spelling_item);
	g_menu_append_item(settings_menu_model, full_path_item);
	g_menu_append_section(settings_menu_model, NULL, G_MENU_MODEL(g_menu_new())); // separator
	g_menu_append_item(settings_menu_model, prefs_item);
	g_menu_append_item(settings_menu_model, stats_item);
	g_menu_append_item(settings_menu_model, keep_above_item);
	g_menu_append_item(settings_menu_model, line_wrap_item);
	g_menu_append_item(settings_menu_model, check_open_item);
#ifdef HAVE_LIBMAGIC
	g_menu_append_item(settings_menu_model, use_magic_item);
#endif

	// Create a menu button for the settings menu
	settings = gtk_menu_button_new();
	GtkWidget *settings_label = gtk_label_new(_SETTINGS_MENU);
	gtk_menu_button_set_child(GTK_MENU_BUTTON(settings), settings_label);
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(settings), G_MENU_MODEL(settings_menu_model));

	// Store references to menu items for compatibility with the old code
	selectfont = gtk_button_new_with_label(_SELECT_FONT_MNEMONIC);
	linenumber = gtk_check_button_new_with_label(_LINE_NUMBER);
	lang_chk = gtk_check_button_new_with_label(_LANGUAGE);
#ifdef AUTO_TAB_TOGGLE
	AutoTab = gtk_check_button_new_with_label(_AUTOTAB);
#endif
	spelling = gtk_check_button_new_with_label(_SPELLING);
	ShowFullPath = gtk_check_button_new_with_label(_SHOW_FULL_PATH);
	UIPrefs = gtk_button_new_with_label(_PREF_MNEMONIC);
	DocStats = gtk_button_new_with_label(_WORD_COUNT);
	keepAbove = gtk_check_button_new_with_label(_KEEP_WIND_ABOVE);
	lineWp = gtk_check_button_new_with_label(_LINE_WP);
	checkOnOpen = gtk_check_button_new_with_label(_CHECK_ON_OPEN);
#ifdef HAVE_LIBMAGIC
	UseMagic = gtk_check_button_new_with_label(_SETTINGS_USE_MAGIC);
#endif

	// Set items active state in settings menu to match config state
	gtk_check_button_set_active(GTK_CHECK_BUTTON(lineWp), cfg_line_wrap());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(ShowFullPath), cfg_show_full_path());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(linenumber), cfg_line());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(spelling), cfg_spell());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(lang_chk), cfg_lang());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(keepAbove), cfg_keep_above());
	gtk_check_button_set_active(GTK_CHECK_BUTTON(checkOnOpen), cfg_check_on_open());
#ifdef HAVE_LIBMAGIC
	gtk_check_button_set_active(GTK_CHECK_BUTTON(UseMagic), cfg_use_magic());
#endif
#ifdef AUTO_TAB_TOGGLE
	gtk_check_button_set_active(GTK_CHECK_BUTTON(AutoTab), cfg_auto_tab());
#endif

	// Add the settings menu button to the menubar
	gtk_box_append(GTK_BOX(menubar), settings);
	////////////////////////////////////////////////////////////////////////
	// Setup Help Menu using GMenu for GTK4
	////////////////////////////////////////////////////////////////////////
	// Create a GMenu for the help menu
	GMenu *help_menu_model = g_menu_new();
	GMenuItem *about_item = g_menu_item_new("About", "app.about");

	g_menu_append_item(help_menu_model, about_item);

	// Create a menu button for the help menu
	help = gtk_menu_button_new();
	GtkWidget *help_label = gtk_label_new(_HELP_MENU);
	gtk_menu_button_set_child(GTK_MENU_BUTTON(help), help_label);
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(help), G_MENU_MODEL(help_menu_model));

	// Add the help menu button to the menubar
	gtk_box_append(GTK_BOX(menubar), help);

	// Add the menubar to the main box
	gtk_box_append(GTK_BOX(mainbox), menubar);
	////////////////////////////////////////////////////////////////////////
	// Set up toolbar using GtkBox and GtkButton for GTK4
	////////////////////////////////////////////////////////////////////////
	toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_widget_set_margin_start(toolbar, 5);
	gtk_widget_set_margin_end(toolbar, 5);
	gtk_widget_set_margin_top(toolbar, 5);
	gtk_widget_set_margin_bottom(toolbar, 5);

	// Create toolbar buttons - in GTK4 we use GtkButton instead of GtkToolItem
	GtkWidget *toolnew_btn = gtk_button_new_with_label(_NEW_TOOLBAR);
	GtkWidget *toolopen_btn = gtk_button_new_with_label(_OPEN_TOOLBAR);
	GtkWidget *toolsave_btn = gtk_button_new_with_label(_SAVE_TOOLBAR);
	GtkWidget *toolsaveas_btn = gtk_button_new_with_label(_SAVE_AS_TOOLBAR);
	GtkWidget *toolreload_btn = gtk_button_new_with_label(_TOOLRELOAD_FILE);
	GtkWidget *toolundo_btn = gtk_button_new_with_label(_UNDO_TOOLBAR);
	GtkWidget *toolredo_btn = gtk_button_new_with_label(_REDO_TOOLBAR);
	GtkWidget *toolquit_btn = gtk_button_new_with_label(_QUIT_TOOLBAR);

	// Add buttons to toolbar
	gtk_box_append(GTK_BOX(toolbar), toolnew_btn);
	gtk_box_append(GTK_BOX(toolbar), toolopen_btn);
	gtk_box_append(GTK_BOX(toolbar), toolsave_btn);
	gtk_box_append(GTK_BOX(toolbar), toolsaveas_btn);

	// Add separator
	GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_append(GTK_BOX(toolbar), separator1);

	gtk_box_append(GTK_BOX(toolbar), toolreload_btn);

	// Add separator
	GtkWidget *separator2 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_append(GTK_BOX(toolbar), separator2);

	gtk_box_append(GTK_BOX(toolbar), toolundo_btn);
	gtk_box_append(GTK_BOX(toolbar), toolredo_btn);

	// Add separator
	GtkWidget *separator3 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_append(GTK_BOX(toolbar), separator3);

	gtk_box_append(GTK_BOX(toolbar), toolquit_btn);

	////////////////////////////////////////////////////////////////////////
	// Add the tool bar to the box main box
	////////////////////////////////////////////////////////////////////////
	gtk_box_append(GTK_BOX(mainbox), toolbar);
	////////////////////////////////////////////////////////////////////////
	// Set up GTKSourceView with a buffer
	////////////////////////////////////////////////////////////////////////
	// Create a new GTKSourceBuffer and GTKSourceView for GTK4
	mBuff = gtk_source_buffer_new(NULL);

	view = GTK_SOURCE_VIEW(gtk_source_view_new_with_buffer(mBuff));
	// Set line numbers based on config
	gtk_source_view_set_show_line_numbers(view, cfg_line());
	tpad_set_font("DejaVu Sans Mono 12");
	// Set smart home/end behavior
	gtk_source_view_set_smart_backspace(view, TRUE);
	gtk_source_view_set_smart_home_end(view, GTK_SOURCE_SMART_HOME_END_BEFORE);

	g_signal_connect(mBuff, "insert_text", G_CALLBACK(on_past), NULL);
	g_signal_connect(window, "delete_event", G_CALLBACK(quit_program), NULL);

	////////////////////////////////////////////////////////////////////////
	// Put our source view into a scrolled window and added it to box mainbox
	////////////////////////////////////////////////////////////////////////
	scrolled_window = gtk_scrolled_window_new();
	gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), GTK_WIDGET(view));
	gtk_widget_set_vexpand(scrolled_window, TRUE);
	gtk_widget_set_hexpand(scrolled_window, TRUE);
	gtk_box_append(GTK_BOX(mainbox), GTK_WIDGET(scrolled_window));
	////////////////////////////////////////////////////////////////////////
	// Add box mainbox to main window, window
	////////////////////////////////////////////////////////////////////////
	gtk_window_set_child(GTK_WINDOW(window), mainbox);
	////////////////////////////////////////////////////////////////////////
	// Connect signals from above to callbacks
	// Connect window close signal
	g_signal_connect(window, "delete_event", G_CALLBACK(quit_program), NULL);

	// Note: We don't need to connect signals for menu items anymore
	// as they are handled by the GAction system in tpad_actions.c

	g_signal_connect(toolnew_btn, "clicked", G_CALLBACK(runn), NULL);
	g_signal_connect(toolopen_btn, "clicked", G_CALLBACK(open_file), NULL);
	g_signal_connect(toolsave_btn, "clicked", G_CALLBACK(save_file), NULL);
	g_signal_connect(toolsaveas_btn, "clicked", G_CALLBACK(save_as), NULL);
	g_signal_connect(toolundo_btn, "clicked", G_CALLBACK(do_undo), NULL);
	g_signal_connect(toolredo_btn, "clicked", G_CALLBACK(do_redo), NULL);
	g_signal_connect(toolreload_btn, "clicked", G_CALLBACK(tpad_reload), NULL);

	// In GTK4, key events are handled differently
	GtkEventController *key_controller = gtk_event_controller_key_new(window);
	g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_press), NULL);

	g_signal_connect(toolquit_btn, "clicked", G_CALLBACK(quit_program), NULL);
	return(window);
}

// GTK4 key event handling
gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
	GtkTextIter start, end;

	switch (keyval) {
	case GDK_KEY_End:
		gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(mBuff), &end);
		gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(mBuff), &end);
		return TRUE;

	case GDK_KEY_Home:
		gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(mBuff), &start);
		gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(mBuff), &start);
		return TRUE;

	default:
		return FALSE;
	}
}
