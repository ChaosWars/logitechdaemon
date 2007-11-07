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
#include "dbusobject.h"
#include "dbusobjectglue.h"
#include "blank.h"
#include "logo.h"

static GObjectClass *parent_class;

struct _DBusObjectPrivate
{
	gboolean dispose_has_run;
};

GType dbus_object_get_type()
{
	static GType type = 0;
	if ( type == 0 )
	{
		static const GTypeInfo info =
		{
			sizeof ( DBusObjectClass ),
			NULL,   /* base_init */
			NULL,   /* base_finalize */
			dbus_object_class_init,   /* class_init */
			NULL,   /* class_finalize */
			NULL,   /* class_data */
			sizeof ( DBusObject ),
			0,      /* n_preallocs */
			dbus_object_init    /* instance_init */
		};
		type = g_type_register_static ( G_TYPE_OBJECT, "DBusObjectType", &info, 0 );
	}
	return type;
}

static void dbus_object_class_init( DBusObjectClass *klass )
{
	GObjectClass *gobject_class = G_OBJECT_CLASS( klass );
	gobject_class->dispose = dbus_object_dispose;
	gobject_class->finalize = dbus_object_finalize;
	parent_class = g_type_class_peek_parent( klass );
	g_type_class_add_private( klass, sizeof( DBusObjectPrivate ) );
	DBusMessage *message = NULL;
	GError *error = NULL;
	klass->connection = dbus_g_bus_get( DBUS_BUS_SYSTEM, &error );
	
	if( klass->connection == NULL )
	{
		daemon_log( LOG_ERR, "Failed to open connection to system bus: %s\n", error->message );
		g_error_free( error );
		return;
	}
	
	dbus_g_object_type_install_info( DBUS_OBJECT_TYPE, &dbus_glib_dbus_object_object_info );
}

static void dbus_object_init( GTypeInstance *instance, gpointer g_class )
{
	DBusObject *self = DBUS_OBJECT( instance );
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE( self, DBUS_OBJECT_TYPE, DBusObjectPrivate);
	self->priv = g_new0( DBusObjectPrivate,  1 );
	self->priv->dispose_has_run = FALSE;
	DBusObjectClass *klass = DBUS_OBJECT_GET_CLASS( instance );
	DBusGProxy *proxy = dbus_g_proxy_new_for_name( klass->connection, DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS );

	GError *error;
	guint32 request_name_ret;
	
	if( !org_freedesktop_DBus_request_name( proxy, "org.freedesktop.DBusObject", 0, &request_name_ret, &error ) ){
		daemon_log( LOG_ERR, "Failed to obtain address on bus: %s\n", error->message );
		g_error_free( error );
	}

	if (request_name_ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		daemon_log( LOG_ERR, "Adress is already registered on bus\n" );
	}

	dbus_g_connection_register_g_object( klass->connection, "/org/freedesktop/DBusObject", G_OBJECT( instance ) );
	g_object_unref( proxy );
}

static void dbus_object_dispose( GObject *object )
{
	daemon_log( LOG_INFO, "dbus_object_dispose().\n");
	DBusObject *self = DBUS_OBJECT( object );

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

static void dbus_object_finalize( GObject *object )
{
	DBusObject *self = DBUS_OBJECT( object );
	/* Chain up to the parent class */
	G_OBJECT_CLASS(parent_class)->finalize( object );
	g_free( self->priv );
}

static gboolean dbus_object_set_lcd_brightness( DBusObject *ld, gint32 IN_brightness, GError **error )
{
	int retval = setLCDBrightness( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean dbus_object_set_lcd_contrast( DBusObject *ld, gint32 IN_contrast, GError **error )
{
	int retval = setLCDContrast( IN_contrast );

	if ( retval < 0 )
	{
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean dbus_object_set_kb_brightness( DBusObject *ld, gint32 IN_brightness, GError **error )
{
	int retval = setKBBrightness ( IN_brightness );

	if ( retval < 0 )
	{
		g_set_error ( error, 0, 0, "Failed to set keyboard brightness.\n" );
		return false;
	}

	return true;
}

static gboolean dbus_object_blank_screen( DBusObject *ld, GError **error )
{
	if( writePixmapToLCD( blank_data ) != 0 ){
		daemon_log( LOG_ERR, "Error blanking screen.\n" );
		g_set_error( error, 0, 0, "Failed to write to screen.\n" );
		return false;
	}

	return true;
}

static gboolean dbus_object_show_logo( DBusObject *ld, GError **error )
{
	if( writePixmapToLCD( logo_data ) != 0 ){
		daemon_log( LOG_ERR, "Error displaying logo.\n" );
		g_set_error( error, 0, 0, "Failed to write to screen.\n" );
		return false;
	}

	return true;
}
