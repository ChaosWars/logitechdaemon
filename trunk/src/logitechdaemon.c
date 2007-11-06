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
#include <libg15.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-bindings.h>
#include "logitechdaemon.h"
#include "logitechdaemonglue.h"

typedef struct _LogitechDaemonPrivate;

struct _LogitechDaemonPrivate
{
	DBusGConnection *connection;
	DBusGProxy *proxy;
} LogitechDaemonPrivate;

GType logitechdaemon_get_type()
{
	static GType type = 0;
	if ( type == 0 )
	{
		static const GTypeInfo info =
		{
			sizeof ( LogitechDaemonClass ),
			NULL,   /* base_init */
			NULL,   /* base_finalize */
			NULL,   /* class_init */
			NULL,   /* class_finalize */
			NULL,   /* class_data */
			sizeof ( LogitechDaemon ),
			0,      /* n_preallocs */
			NULL    /* instance_init */
		};
		type = g_type_register_static ( G_TYPE_OBJECT, "LogitechDaemonType", &info, 0 );
	}
	return type;
}

static void logitechdaemon_class_init( LogitechDaemonClass *klass )
{
	g_type_class_add_private( klass, sizeof( LogitechDaemonPrivate ) );
	dbus_g_object_type_install_info ( G_TYPE_FROM_CLASS( ldc )/*LOGITECH_DAEMON_TYPE*/, &dbus_glib_logitechdaemon_object_info );
}

static void logitechdaemon_init( GTypeInstance *instance, gpointer g_class )
{
	LogitechDaemon *self = LOGITECH_DAEMON( instance );
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE( self, LOGITECH_DAEMON_TYPE, LogitechDaemonPrivate);
}

static void logitechdaemon_finalize( GObject *object )
{
	LogitechDaemon *self = LOGITECH_DAEMON( object );
	g_free( self->priv );
}

static gboolean logitechdaemon_set_lcd_brightness ( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setLCDBrightness ( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitechdaemon_set_lcd_contrast ( LogitechDaemon *ld, gint32 IN_contrast, GError **error )
{
	int retval = setLCDContrast ( IN_contrast );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitechdaemon_set_kb_brightness ( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setKBBrightness ( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}
