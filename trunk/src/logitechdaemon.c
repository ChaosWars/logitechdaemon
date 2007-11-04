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
#include <stdbool.h>
#include <daemon.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <libg15.h>
#include <libg15render.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-bindings.h>
#include "logitechdaemon.h"
#include "blank.h"
#include "logo.h"

bool initializeUInput()
{
	uinput_fd = open("/dev/uinput", O_WRONLY | O_NDELAY );

	if( uinput_fd < 0 ){
		daemon_log( LOG_ERR, "Failed to open /dev/uinput.\nCheck if the uinput module is loaded, and that you are running the daemon as root.\n" );
		return false;
	}

	memset( &uinput, 0, sizeof( uinput ) );
	strncpy( uinput.name, DAEMON_NAME, UINPUT_MAX_NAME_SIZE );
	uinput.id.version = 4;
	uinput.id.bustype = BUS_USB;
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);

	int i;

	for( i = 0; i < 256; i++ ){
		ioctl( uinput_fd, UI_SET_KEYBIT, i );
	}

	write( uinput_fd, &uinput, sizeof( uinput ) );
	
	if( ioctl( uinput_fd, UI_DEV_CREATE ) ){
		daemon_log( LOG_ERR, "Unable to create uinput device.\n" );
		return false;
	}

	daemon_log( LOG_INFO, "%s successfully negotiated uinput.\n", DAEMON_NAME );

	return true;
}

bool initializeDBUS()
{
	DBusGConnection *connection;
	DBusMessage *message;
	GError *error;
	DBusGProxy *proxy;
	char **name_list;
	char **name_list_ptr;
	guint32 request_name_ret;
  
	g_type_init ();

	error = NULL;
	connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);

	if( connection == NULL )
	{
		daemon_log( LOG_ERR, "Failed to open connection to system bus: %s\n", error->message );
		g_error_free( error );
		return false;
	}

	proxy = dbus_g_proxy_new_for_name( connection, DBUS_SERVICE_DBUS, DBUS_PATH_DBUS, DBUS_INTERFACE_DBUS );

	if( !org_freedesktop_DBus_request_name (proxy, "org.freedesktop.LogitechDaemon", 0, &request_name_ret, &error ) ){
		daemon_log( LOG_ERR, "Failed to obtain adress on bus: %s\n", error->message );
		g_error_free( error );
		return false;
	}

	if (request_name_ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		daemon_log( LOG_ERR, "Adress is already registered on bus\n" );
		return false;
	}

	ld = g_object_new( logitech_daemon_get_type(), NULL );
	dbus_g_connection_register_g_object (connection, "/LogitechDaemon", G_OBJECT( ld ) );
	daemon_log( LOG_INFO, "%s successfully negotiated dbus connection.\n", DAEMON_NAME );

	return true;
}

bool initialize()
{
	int error;
	
	if( !initializeUInput() )
		return false;

	if( !initializeDBUS() )
		return false;

	error = initLibG15();

	if( error != G15_NO_ERROR ){
		daemon_log( LOG_ERR, "Failed to initialize libg15.\n" );
		return false;
	}else{
		if( writePixmapToLCD( logo_data ) != 0 )
			daemon_log( LOG_ERR, "Error displaying logo.\n" );

		setLEDs( G15_LED_M1 );
		setKBBrightness( G15_BRIGHTNESS_MEDIUM );
		setLCDBrightness( G15_BRIGHTNESS_MEDIUM );
		setLCDContrast( G15_CONTRAST_MEDIUM );
	}

	return true;
}

void shutdown()
{
	if( writePixmapToLCD( blank_data ) != 0 )
		daemon_log( LOG_ERR, "Error blanking screen.\n" );

	setKBBrightness( G15_BRIGHTNESS_DARK );
	setLCDBrightness( G15_BRIGHTNESS_DARK );
	setLCDContrast( G15_CONTRAST_LOW );
	setLEDs( 0 );
	exitLibG15();
	ioctl( uinput_fd, UI_DEV_DESTROY );
	close( uinput_fd );
}
