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

#define DAEMON_NAME "LogitechDaemon"

#include <QObject>
#include <linux/uinput.h>
// #include "daemon_adaptor.h"

// class QDBusConnection;

class LogitechDaemon : public QObject
{
	Q_OBJECT

	public:
		LogitechDaemon();
		~LogitechDaemon();
		bool initialize();
		void shutdown();

	private:
	 	int uinput_fd;
 		struct uinput_user_dev uinput;
// 		LogitechDaemonAdaptor *adaptor;
		bool initializeUInput();
// 		bool initializeDBUS();

	public Q_SLOTS:
		void set_LCD_Brightness( int brightness );
		void set_LCD_Contrast( int contrast );
		void set_KB_Brightness( int brightness );

	Q_SIGNALS:
		void LCDBrightnessSet( int brightness );
		void LCDContrastSet( int contrast );
		void KBBrightnessSet( int brightness );
};

#endif //	_LOGITECHDAEMON_H_
