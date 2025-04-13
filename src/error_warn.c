/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2013 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, error_warn.c , is part of tpad.
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

/* External declarations */
extern GtkWidget *window;

/**
 * Log an error message to stderr
 */
void gerror_log(gchar *c_msg)
{
    /* Disabled
        gchar *error_msg;
        error_msg=g_strconcat(_ERROR_INFO,"\n",c_msg,NULL);
        openlog("tpad", LOG_CONS, LOG_SYSLOG);
        syslog(LOG_ERR, _ERROR_OCCURED, (char*)error_msg);
        closelog();
        g_free(error_msg);
    */
    gchar *error_msg;
    error_msg = g_strconcat(_ERROR_INFO, "\n", c_msg, NULL);

    fprintf(stderr, "%s", error_msg);
    g_free(error_msg);
}

/**
 * Structure to hold dialog response data
 */
typedef struct {
    gint response;
    gboolean response_received;
    GMainLoop *loop;
} DialogResponseData;

/**
 * Callback for dialog response
 */
static void dialog_response_cb(GtkDialog *dialog, gint response, gpointer user_data)
{
    DialogResponseData *data = (DialogResponseData *)user_data;
    data->response = response;
    data->response_received = TRUE;
    g_main_loop_quit(data->loop);
    gtk_window_destroy(GTK_WINDOW(dialog));
}

/**
 * Display a popup dialog for open guard
 * This implementation properly handles the user's response in GTK4
 */
gint gerror_openguard_popup(gchar *c_fileName, gchar *foundFilePath)
{
    GtkWidget* ogDialog;
    gint iret = 0;
    
    // Create a dialog using GTK4 API
    ogDialog = gtk_message_dialog_new(GTK_WINDOW(window), 
                                     GTK_DIALOG_MODAL, 
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_YES_NO, 
                                     _FILE_MAY_BE_ALREADY_OPEN);
                                     
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(ogDialog), 
                                           _FILE_MAY_BE_ALREADY_OPEN_DETAIL, 
                                           c_fileName, foundFilePath);

    // Create a main loop to wait for the response
    DialogResponseData response_data;
    response_data.response_received = FALSE;
    response_data.loop = g_main_loop_new(NULL, FALSE);
    
    // Connect the response signal
    g_signal_connect(ogDialog, "response", G_CALLBACK(dialog_response_cb), &response_data);
    
    // Show the dialog
    gtk_window_present(GTK_WINDOW(ogDialog));
    
    // Run the main loop until we get a response
    g_main_loop_run(response_data.loop);
    g_main_loop_unref(response_data.loop);
    
    // Process the response
    if (response_data.response_received) {
        switch (response_data.response) {
            case GTK_RESPONSE_YES:
                iret = 1;
                break;
            default:
                iret = 0;
                break;
        }
    }
    
    return iret;
}

/**
 * Display an error popup dialog
 */
void gerror_popup(gchar *c_msg)
{
    GtkWidget* errorDialog;
    
    // Create a dialog using GTK4 API
    errorDialog = gtk_message_dialog_new(GTK_WINDOW(window), 
                                       GTK_DIALOG_MODAL, 
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_OK, 
                                       "ERROR!");
                                       
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(errorDialog),
                                           _ERROR_MSG,
                                           c_msg);

    // In GTK4, we use signals instead of gtk_dialog_run
    g_signal_connect(errorDialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
    
    // Show the dialog
    gtk_window_present(GTK_WINDOW(errorDialog));
}

/**
 * Display a warning dialog with optional logging and abort
 */
void gerror_warn(gchar *c_msg, gchar *extra_information, gboolean slog, gboolean abort)
{
    gchar *msg;
    msg = g_strconcat(c_msg, "\n", extra_information, NULL);
    
    if(slog) gerror_log(msg);
    gerror_popup(msg);
    g_free(msg);
    
    if(abort) {
        new_thread_tpad(NULL);
        force_quit_program();
    }
}
