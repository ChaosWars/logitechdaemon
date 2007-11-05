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
#include "logitechdaemon.h"
#include "logitechdaemonglue.h"

G_DEFINE_TYPE( LogitechDaemon, logitechdaemon, G_TYPE_OBJECT );

static void logitechdaemon_class_init( LogitechDaemonClass *ldc )
{
	dbus_g_object_type_install_info( G_TYPE_FROM_CLASS/*G_OBJECT_CLASS*/( ldc ), &dbus_glib_logitechdaemon_object_info );
}

static void logitechdaemon_init( LogitechDaemon *ld )
{
}

static gboolean logitechdaemon_set_lcd_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setLCDBrightness( IN_brightness );

	if( retval < 0 ){
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitechdaemon_set_lcd_contrast( LogitechDaemon *ld, gint32 IN_contrast, GError **error )
{
	int retval = setLCDContrast( IN_contrast );

	if( retval < 0 ){
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static gboolean logitechdaemon_set_kb_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setKBBrightness( IN_brightness );

	if( retval < 0 ){
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}
