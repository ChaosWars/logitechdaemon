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
#ifndef _LOGITECH_DAEMON_H_
#define _LOGITECH_DAEMON_H_

#include <glib-object.h>

#define LOGITECH_DAEMON_TYPE                ( logitech_daemon_get_type() )
#define LOGITECH_DAEMON( object )           ( G_TYPE_CHECK_INSTANCE_CAST( ( object ), LOGITECH_DAEMON_TYPE, LogitechDaemon ) )
#define LOGITECH_DAEMON_CLASS( klass )      ( G_TYPE_CHECK_CLASS_CAST( ( klass ), LOGITECH_DAEMON_TYPE, LogitechDaemonClass ) )
#define IS_LOGITECH_DAEMON( object )        ( G_TYPE_CHECK_INSTANCE_TYPE( ( object ), LOGITECH_DAEMON_TYPE ) )
#define IS_LOGITECH_DAEMON_CLASS( klass )   ( G_TYPE_CHECK_CLASS_TYPE( ( klass ), LOGITECH_DAEMON_TYPE ) )
#define LOGITECH_DAEMON_GET_CLASS( object )    ( G_TYPE_INSTANCE_GET_CLASS( ( object ), LOGITECH_DAEMON_TYPE, LogitechDaemonClass ) )

typedef struct _LogitechDaemon LogitechDaemon;
typedef struct _LogitechDaemonClass LogitechDaemonClass;
typedef struct _LogitechDaemonPrivate LogitechDaemonPrivate;

struct _LogitechDaemon{
	GObject parent;

	/*< private >*/
	LogitechDaemonPrivate *priv;
};

struct _LogitechDaemonClass{
	GObjectClass parent;

	/*private>*/
	DBusGConnection *connection;
};

GType logitech_daemon_get_type();
static void logitech_daemon_class_init( LogitechDaemonClass *klass );
static void logitech_daemon_init( GTypeInstance *instance, gpointer g_class );
static void logitech_daemon_dispose( GObject *object );
static void logitech_daemon_finalize( GObject *object );
static gboolean logitech_daemon_set_lcd_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error );
static gboolean logitech_daemon_set_lcd_contrast( LogitechDaemon *ld, gint32 IN_contrast, GError **error );
static gboolean logitech_daemon_set_kb_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error );

#endif //	_LOGITECH_DAEMON_H_
