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
#include <daemon.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libg15.h>
#include <libg15render.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include "logitechdaemon.h"

LogitechDaemon::LogitechDaemon()
{
}

LogitechDaemon::~LogitechDaemon()
{
}

bool LogitechDaemon::initialize()
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
		setLEDs( G15_LED_M1 );
		setKBBrightness( G15_BRIGHTNESS_MEDIUM );
		setLCDBrightness( G15_BRIGHTNESS_MEDIUM );
		setLCDContrast( G15_CONTRAST_MEDIUM );
	}

	return true;
}

bool LogitechDaemon::initializeUInput()
{
	uinput_fd = open("/dev/uinput", O_WRONLY | O_NDELAY );

	if( uinput_fd < 0 ){
		daemon_log( LOG_ERR, "Failed to open /dev/uinput.\nCheck if the uinput module is loaded, and that you are running the daemon as root.\n" );
		return false;
	}

	memset( &uinput, 0, sizeof( uinput ) );
	strncpy( uinput.name, "LogitechDaemon", UINPUT_MAX_NAME_SIZE );
	uinput.id.version = 4;
	uinput.id.bustype = BUS_USB;
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinput_fd, UI_SET_EVBIT, EV_REL);

	for( int i = 0; i < 256; i++ ){
		ioctl( uinput_fd, UI_SET_KEYBIT, i );
	}

	write( uinput_fd, &uinput, sizeof( uinput ) );
	
	if( ioctl( uinput_fd, UI_DEV_CREATE ) ){
		daemon_log( LOG_ERR, "Unable to create uinput device.\n" );
		return false;
	}

	daemon_log( LOG_INFO, "LogitechDaemon successfully negotiated uinput.\n" );

	return true;
}

bool LogitechDaemon::initializeDBUS()
{
	DBusGConnection *connection;
	GError *error;
	DBusGProxy *proxy;
	char **name_list;
	char **name_list_ptr;
  
	g_type_init ();

	error = NULL;
	connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);

	if( connection == NULL )
	{
		daemon_log( LOG_ERR, "Failed to open connection to bus: %s\n", error->message );
		g_error_free( error );
		return false;
	}

	daemon_log( LOG_INFO, "LogitechDaemon successfully negotiated dbus connection.\n" );

	return true;
}

void LogitechDaemon::shutdown()
{
	setKBBrightness( G15_BRIGHTNESS_DARK );
	setLCDBrightness( G15_BRIGHTNESS_DARK );
	setLCDContrast( G15_CONTRAST_LOW );
	exitLibG15();
	ioctl( uinput_fd, UI_DEV_DESTROY );
	close( uinput_fd );
}
