/***************************************************************************
 *   Copyright (C) 2007 by Lawrence Lee   *
 *   valheru@facticius.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _LOGITECHDAEMON_H_
#define _LOGITECHDAEMON_H_

#include <glib-object.h>

typedef struct _LogitechDaemon {
	GObject parent;
} LogitechDaemon;

typedef struct _LogitechDaemonClass {
	GObjectClass parent;
} LogitechDaemonClass;

GType logitechdaemon_get_type();

#define LOGITECHDAEMON_TYPE              ( logitechdaemon_get_type() )
#define LOGITECHDAEMON( object )           ( G_TYPE_CHECK_INSTANCE_CAST( ( object ), LOGITECHDAEMON_TYPE, LogitechDaemon ) )
#define LOGITECHDAEMON_CLASS( klass )      ( G_TYPE_CHECK_CLASS_CAST( ( klass ), LOGITECHDAEMON_TYPE, LogitechDaemonClass ) )
#define IS_LOGITECHDAEMON( object )        ( G_TYPE_CHECK_INSTANCE_TYPE( ( object ), LOGITECHDAEMON_TYPE ) )
#define IS_LOGITECHDAEMON_CLASS( klass )   ( G_TYPE_CHECK_CLASS_TYPE( ( klass ), LOGITECHDAEMON_TYPE ) )
#define LOGITECHDAEMON_GET_CLASS( obj )    ( G_TYPE_INSTANCE_GET_CLASS( ( obj ), LOGITECHDAEMON_TYPE, LogitechDaemonClass ) )

static gboolean logitechdaemon_set_lcd_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error );
static gboolean logitechdaemon_set_lcd_contrast( LogitechDaemon *ld, gint32 IN_contrast, GError **error );
static gboolean logitechdaemon_set_kb_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error );

#endif //	_LOGITECHDAEMON_H_
