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
#include <linux/uinput.h>

#define DAEMON_NAME "LogitechDaemon"

typedef struct _LogitechDaemon {
	GObject parent;
} LogitechDaemon;

typedef struct _LogitechDaemonClass {
	GObjectClass parent;
} LogitechDaemonClass;

/* Define before including logitechdaemonglue.h */
static gboolean logitechdaemon_set_kb_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error );

#include "logitechdaemonglue.h"

int uinput_fd;
struct uinput_user_dev uinput;
LogitechDaemon *ld;

static gboolean logitechdaemon_set_kb_brightness( LogitechDaemon *ld, gint32 IN_brightness, GError **error )
{
	int retval = setKBBrightness( IN_brightness );

	if( retval < 0 ){
		g_set_error( error, 0, 0, "Failed to set keyboard brightness\n" );
		return false;
	}

	return true;
}

static void logitech_daemon_class_init( LogitechDaemonClass *logitechdaemonclass )
{
	dbus_g_object_type_install_info(G_TYPE_FROM_CLASS( logitechdaemonclass ), &dbus_glib_logitechdaemon_object_info );
}

static void logitech_daemon_init( LogitechDaemon *logitechdaemon )
{
}

G_DEFINE_TYPE(LogitechDaemon, logitech_daemon, G_TYPE_OBJECT);

bool initialize();
void shutdown();
bool initializeUInput();
bool initializeDBUS();

#endif //	_LOGITECHDAEMON_H_
