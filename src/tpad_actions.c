/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/********************************************************************************* 
 *     COPYRIGHT NOTICE:
 *     Copyright © 2023 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_actions.c , is part of tpad.
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
 ********************************************************************************/
#include "tpad_headers.h"
#include "tpad_actions.h"

// File menu action callbacks
static void action_new(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    runn(NULL);
}

static void action_open(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    open_file(NULL);
}

static void action_save(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    save_file(NULL);
}

static void action_saveas(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    save_as(NULL);
}

static void action_reload(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    tpad_reload(NULL);
}

static void action_quit(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    quit_program(NULL, NULL, NULL);
}

// Hash menu action callbacks
static void action_hash512(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    h512_connector(NULL);
}

static void action_hash256(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    h256_connector(NULL);
}

static void action_hashmd5(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    hmd5_connector(NULL);
}

static void action_tobase64(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    base64_connector(NULL);
}

static void action_frombase64(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    fbase64_connector(NULL);
}

static void action_hashfile512(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    h512_file_connector(NULL);
}

static void action_hashfile256(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    h256_file_connector(NULL);
}

static void action_hashfilemd5(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    hmd5_file_connector(NULL);
}

static void action_filetobase64(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    base64_file_connector(NULL);
}

static void action_filefrombase64(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    fbase64_file_connector(NULL);
}

// Search/Edit menu action callbacks
static void action_find(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    replace_dialog(NULL);
}

static void action_replace(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    replace_dialog(NULL);
}

static void action_undo(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    do_undo(NULL);
}

static void action_redo(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    do_redo(NULL);
}

static void action_copyfilename(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    tpad_copy_file_name_to_clipboard(NULL);
}

static void action_reverse(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    rev_document();
}

static void action_capsup(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    up_caps_document();
}

static void action_capsdown(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    low_caps_document();
}

// Settings menu action callbacks
static void action_selectfont(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    select_font(NULL);
}

static void action_linenumber(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_linenumber(NULL);
}

static void action_lang_chk(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_show_lang(NULL);
}

#ifdef AUTO_TAB_TOGGLE
static void action_autotab(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_auto_tab(NULL);
}
#endif

static void action_spelling(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_spelling(NULL);
}

static void action_showfullpath(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_show_full_path(NULL);
}

static void action_uiprefs(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    show_ui_prefs();
}

static void action_docstats(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    get_text_stats(NULL);
}

static void action_keepabove(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_keep_above(NULL);
}

static void action_linewp(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_line_wrap(NULL);
}

static void action_checkonopen(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_use_open_guard(NULL);
}

#ifdef HAVE_LIBMAGIC
static void action_usemagic(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    toggle_use_magic_settings(NULL);
}
#endif

// Help menu action callbacks
static void action_about(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    show_about(NULL);
}

// Function to set up all the application actions for the menu system
void tpad_setup_actions(GtkApplication *app) {
    // Define all the actions
    const GActionEntry app_actions[] = {
        // File menu actions
        { "new", action_new, NULL, NULL, NULL },
        { "open", action_open, NULL, NULL, NULL },
        { "save", action_save, NULL, NULL, NULL },
        { "saveas", action_saveas, NULL, NULL, NULL },
        { "reload", action_reload, NULL, NULL, NULL },
        { "quit", action_quit, NULL, NULL, NULL },
        
        // Hash menu actions
        { "hash512", action_hash512, NULL, NULL, NULL },
        { "hash256", action_hash256, NULL, NULL, NULL },
        { "hashmd5", action_hashmd5, NULL, NULL, NULL },
        { "tobase64", action_tobase64, NULL, NULL, NULL },
        { "frombase64", action_frombase64, NULL, NULL, NULL },
        { "hashfile512", action_hashfile512, NULL, NULL, NULL },
        { "hashfile256", action_hashfile256, NULL, NULL, NULL },
        { "hashfilemd5", action_hashfilemd5, NULL, NULL, NULL },
        { "filetobase64", action_filetobase64, NULL, NULL, NULL },
        { "filefrombase64", action_filefrombase64, NULL, NULL, NULL },
        
        // Search/Edit menu actions
        { "find", action_find, NULL, NULL, NULL },
        { "replace", action_replace, NULL, NULL, NULL },
        { "undo", action_undo, NULL, NULL, NULL },
        { "redo", action_redo, NULL, NULL, NULL },
        { "copyfilename", action_copyfilename, NULL, NULL, NULL },
        { "reverse", action_reverse, NULL, NULL, NULL },
        { "capsup", action_capsup, NULL, NULL, NULL },
        { "capsdown", action_capsdown, NULL, NULL, NULL },
        
        // Settings menu actions
        { "selectfont", action_selectfont, NULL, NULL, NULL },
        { "linenumber", action_linenumber, NULL, NULL, NULL },
        { "lang_chk", action_lang_chk, NULL, NULL, NULL },
#ifdef AUTO_TAB_TOGGLE
        { "autotab", action_autotab, NULL, NULL, NULL },
#endif
        { "spelling", action_spelling, NULL, NULL, NULL },
        { "showfullpath", action_showfullpath, NULL, NULL, NULL },
        { "uiprefs", action_uiprefs, NULL, NULL, NULL },
        { "docstats", action_docstats, NULL, NULL, NULL },
        { "keepabove", action_keepabove, NULL, NULL, NULL },
        { "linewp", action_linewp, NULL, NULL, NULL },
        { "checkonopen", action_checkonopen, NULL, NULL, NULL },
#ifdef HAVE_LIBMAGIC
        { "usemagic", action_usemagic, NULL, NULL, NULL },
#endif
        
        // Help menu actions
        { "about", action_about, NULL, NULL, NULL }
    };
    
    // Add the actions to the application
    g_action_map_add_action_entries(G_ACTION_MAP(app), app_actions, G_N_ELEMENTS(app_actions), app);
}
