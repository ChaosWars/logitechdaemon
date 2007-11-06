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
#include <stdlib.h>
#include <libg15.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-bindings.h>
#include <daemon.h>
#include "logitechdaemon.h"
#include "logitech_daemonglue.h"

static GObjectClass *parent_class;

struct _LogitechDaemonPrivate
{
	gboolean dispose_has_run;
};

GType logitech_daemon_get_type()
{
	static GType type = 0;
	if ( type == 0 )
	{
		static const GTypeInfo info =
		{
			sizeof ( LogitechDaemonClass ),
			NULL,   /* base_init */
			NULL,   /* base_finalize */
			logitech_daemon_class_init,   /* class_init */
			NULL,   /* class_finalize */
			NULL,   /* class_data */
			sizeof ( LogitechDaemon ),
			0,      /* n_preallocs */
			logitech_daemon_init    /* instance_init */
		};
		type = g_type_register_static ( G_TYPE_OBJECT, "LogitechDaemonType", &info, 0 );
	}
	return type;
}

static void logitech_daemon_class_init( LogitechDaemonClass *klass )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( klass );
	gobject_class->dispose = logitech_daemon_dispose;
	gobject_class->finalize = logitech_daemon_finalize;
	parent_class = g_type_class_peek_parent( klass );
	g_type_class_add_private( klass, sizeof( LogitechDaemonPrivate ) );
	DBusMessage *message = NULL;
	GError *error = NULL;
	klass->connection = dbus_g_bus_get( DBUS_BUS_SYSTEM, &error );
	
	if( klass->connection == NULL )
	{
		daemon_log( LOG_ERR, "Failed to open connection to system bus: %s\n", error->message );
		g_error_free( error );
		return;
	}
	
	dbus_g_object_type_install_info( LOGITECH_DAEMON_TYPE, &dbus_glib_logitech_daemon_object_info );
}

static void logitech_daemon_init( GTypeInstance *instance, gpointer g_class )
{
	LogitechDaemon *self = LOGITECH_DAEMON( instance );
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE( self, LOGITECH_DAEMON_TYPE, LogitechDaemonPrivate);
	self->priv = g_new0( LogitechDaemonPrivate,  1 );
	self->priv->dispose_has_run = FALSE;
	LogitechDaemonClass *klass = LOGITECH_DAEMON_GET_CLASS( instance );
	DBusGProxy *proxy = dbus_g_proxy_new_for_name( klass->connection, DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS );

	GError *error;
	guint32 request_name_ret;
	
	if( !org_freedesktop_DBus_request_name( proxy, "org.freedesktop.LogitechDaemon", 0, &request_name_ret, &error ) ){
		daemon_log( LOG_ERR, "Failed to obtain address on bus: %s\n", error->message );
		g_error_free( error );
	}

	if (request_name_ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		daemon_log( LOG_ERR, "Adress is already registered on bus\n" );
	}

	dbus_g_connection_register_g_object( klass->connection, "/org/freedesktop/LogitechDaemon", G_OBJECT( instance ) );
	g_object_unref( proxy );
}

static void logitech_daemon_dispose( GObject *object )
{
	daemon_log( LOG_INFO, "logitech_daemon_dispose().\n");
	LogitechDaemon *self = LOGITECH_DAEMON( object );

	if( self->priv->dispose_has_run ){
		/* If dispose did already run, return. */
		return;
	}
	
	/* Make sure dispose does not run twice. */
	self->priv->dispose_has_run = TRUE;

  /* 
	* In dispose, you are supposed to free all types referenced from this
	* object which might themselves hold a reference to self. Generally,
	* the most simple solution is to unref all members on which you own a
	* reference.
  */

	/* Chain up to the parent class */
	G_OBJECT_CLASS(parent_class)->dispose( object );
}

static void logitech_daemon_finalize( GObject *object )
{
	LogitechDaemon *self = LOGITECH_DAEMON( object );
	/* Chain up to the parent class */
	G_OBJECT_CLASS(parent_class)->finalize( object );
	g_free( self->priv );
}

static gboolean logitech_daemon_set_lcd_brightness ( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setLCDBrightness ( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitech_daemon_set_lcd_contrast ( LogitechDaemon *ld, gint32 IN_contrast, GError **error )
{
	int retval = setLCDContrast ( IN_contrast );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitech_daemon_set_kb_brightness ( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setKBBrightness ( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}
