/*********************************************************************************
 *     COPYRIGHT NOTICE:
 *     Copyright © 2025 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, gtk4_types.h , is part of tpad.
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

#ifndef GTK4_TYPES_H
#define GTK4_TYPES_H

/*
 * This file provides forward declarations for GTK4 types that might be needed
 * before the actual headers are included. This is particularly important for
 * GtkSourceView which uses some GTK4 types in its headers.
 */

/* Forward declarations for GTK4 types */
typedef struct _GdkPaintable GdkPaintable;
typedef struct _GtkSnapshot GtkSnapshot;

/* Forward declarations for libspelling types */
typedef struct _SpellingChecker SpellingChecker;
typedef struct _SpellingTextBufferAdapter SpellingTextBufferAdapter;

#endif /* GTK4_TYPES_H */
