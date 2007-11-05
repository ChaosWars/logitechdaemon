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
#include <QDBusConnection>
#include <libg15.h>
#include <daemon.h>
#include "dbusobject.h"
#include "dbusobjectadaptor.h"

DBusObject::DBusObject()
{
}

DBusObject::~DBusObject()
{
}

bool DBusObject::connectToDBus()
{
	adaptor = new LogitechDaemonAdaptor( this );
	daemon_log( LOG_INFO, "Created adaptor.\n" );
	QDBusConnection connection = QDBusConnection::systemBus();
	daemon_log( LOG_INFO, "Got connection to system bus.\n" );

	if( !connection.registerObject( "/org/freedesktop/LogitechDaemon", this ) ){
		daemon_log( LOG_ERR, "Unable to register object on system bus.\n" );
		return false;
	}else{
		daemon_log( LOG_INFO, "Registered object on system bus.\n" );
	}
		
	if( !connection.registerService( "org.freedesktop.LogitechDaemon" ) ){
		daemon_log( LOG_ERR, "Unable to register service.\n" );
		return false;
	}else{
		daemon_log( LOG_INFO, "Registered service.\n" );
	}

// 	dbusconnection = &connection;
	return true;
}

void DBusObject::set_lcd_brightness( int brightness )
{
	setLCDBrightness( brightness );
	daemon_log( LOG_INFO, "set_LCD_Brightness(%d)\n", brightness );
}

void DBusObject::set_lcd_contrast( int contrast )
{
	setLCDContrast( contrast );
	daemon_log( LOG_INFO, "set_LCD_Contrast(%d)\n", contrast );
}

void DBusObject::set_kb_brightness( int brightness )
{
	setKBBrightness( brightness );
	daemon_log( LOG_INFO, "set_KB_Brightness(%d)\n", brightness );
}

#include "dbusobject.moc"
