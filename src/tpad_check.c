/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2013 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_check.c , is part of tpad.
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

int tpad_check(char* file) {
    gchar* content = NULL;
    gchar* unknownContents = NULL;
    gsize length;
    GError *err = NULL;
    
    if(g_file_get_contents(file, &unknownContents, &length, &err)) {
        if (err != NULL) {
            fprintf(stdout, "Error \"%s\"\n", err->message);
            g_error_free(err);
            return(-1);
        }
        
        GError *error = NULL;
        content = g_strdup(g_convert_with_fallback(unknownContents, length, "UTF-8", 
                                                g_get_codeset(), NULL, NULL, NULL, &error));
        g_free(unknownContents);
        
        if (error != NULL) {
            fprintf(stdout, "Error \"%s\"\n", error->message);
            g_error_free(error);
            return(-2);
        }
        
        // If we got here, the file is valid
        g_free(content);
        return(0);
    }
    
    return(1);
}
