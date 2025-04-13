/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2014, 2015, 2016, 2017 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_spelling.c , is part of tpad.
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
pthread_mutex_t spellingmutex = PTHREAD_MUTEX_INITIALIZER;
extern int spell_state;
extern gboolean spell_disabled;
extern GtkSourceView *view;
extern gboolean spelling_init;
extern GtkWidget *spelling;
gboolean spell_disabled = (gboolean) FALSE;
gboolean spelling_init  = (gboolean) FALSE;

// Libspelling objects
SpellingChecker *spell_checker = NULL;
SpellingTextBufferAdapter *spell_adapter = NULL;


int toggle_spelling () {
	int rc;
	pthread_t thread;
	if( (rc=pthread_create( &thread, NULL, ttogle_spelling, NULL)) ){
    	return(1);
	}
	else {
	 pthread_join( thread, NULL);
	 return(0);
	}
}


void *ttogle_spelling(){
if (pthread_mutex_lock( &spellingmutex ) == 0) {
if(!spelling_init && !spell_disabled) {
	spelling_init = (gboolean) TRUE;
	GError *errSP = NULL;

	// Initialize the spelling checker
	spell_checker = spelling_checker_new();

	// Set the language
	if (!spelling_checker_set_language(spell_checker, _SPELL_LANG, &errSP)) {
		if (errSP != NULL) {
			spell_disabled = TRUE;
			cfg_set_show_spelling(FALSE);
			cfg_save();
			gerror_warn(_SPELLING_FAILED_INIT, errSP->message, (gboolean) TRUE, (gboolean) FALSE);
		}
	}

	// Create the adapter for the text buffer
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	spell_adapter = spelling_text_buffer_adapter_new(spell_checker, buffer);

	// Enable or disable based on config
	spelling_text_buffer_adapter_set_enabled(spell_adapter, FALSE);
	}

if (!spell_disabled) {
	// Update config based on menu item state
	cfg_set_show_spelling((int)gtk_check_button_get_active(GTK_CHECK_BUTTON(spelling)));
	cfg_save();

	// Enable or disable spell checking based on config
	if (spell_adapter && view && !spell_disabled) {
		spelling_text_buffer_adapter_set_enabled(spell_adapter, cfg_spell());
	}
	}
pthread_mutex_unlock( &spellingmutex );
	}
pthread_exit ( (void*) NULL);
}

void tpad_free_spelling() {
if (!spell_disabled && spelling_init) {

	// Disable spell checking
	if (spell_adapter) {
		spelling_text_buffer_adapter_set_enabled(spell_adapter, FALSE);
		g_object_unref(spell_adapter);
		spell_adapter = NULL;
	}

	// Free the spell checker
	if (spell_checker) {
		g_object_unref(spell_checker);
		spell_checker = NULL;
	}

	// Do not permit further use of spelling system
	spell_disabled = TRUE;
}
}


