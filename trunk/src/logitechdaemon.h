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

/* Define before include logitechdaemonglue.h */
static gboolean logitechdaemon_set_kb_brightness( gint32 IN_brightness );

int uinput_fd;
struct uinput_user_dev uinput;
LogitechDaemon *ld;

// bool initialize();
// void shutdown();

/* private */

// bool initializeUInput();
// bool initializeDBUS();

#endif //	_LOGITECHDAEMON_H_
