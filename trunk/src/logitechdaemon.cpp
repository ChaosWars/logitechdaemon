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
#include "logitechdaemon.hpp"
#include "logitechdaemonadaptor.h"
#include "blank.h"
#include "logo.h"

LogitechDaemon::LogitechDaemon()
{
}

LogitechDaemon::~LogitechDaemon()
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

bool LogitechDaemon::initializeDBUS()
{
	adaptor = new LogitechDaemonAdaptor( this );
	QDBusConnection connection = QDBusConnection::systemBus();

	if( !connection.registerObject( "/org/freedesktop/LogitechDaemon", this ) ){
		daemon_log( LOG_ERR, "Unable to register object on system bus.\n" );
		return false;
	}
		
	if( !connection.registerService( "org.freedesktop.LogitechDaemon" ) ){
		daemon_log( LOG_ERR, "Unable to register service.\n" );
		return false;
	}

	return true;
}

bool LogitechDaemon::initialize()
{
	int error;
	
	if( !initializeUInput() )
		return false;

	initializeDBUS();
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

void LogitechDaemon::set_lcd_brightness( int brightness )
{
	setLCDBrightness( brightness );
	daemon_log( LOG_INFO, "set_LCD_Brightness(%d)\n", brightness );
}

void LogitechDaemon::set_lcd_contrast( int contrast )
{
	setLCDContrast( contrast );
	daemon_log( LOG_INFO, "set_LCD_Contrast(%d)\n", contrast );
}

void LogitechDaemon::set_kb_brightness( int brightness )
{
	setKBBrightness( brightness );
	daemon_log( LOG_INFO, "set_KB_Brightness(%d)\n", brightness );
}

// #include "logitechdaemon.moc"
