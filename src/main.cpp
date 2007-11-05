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
#include <QCoreApplication>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <daemon.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libg15.h>
#include <linux/uinput.h>
#include "blank.h"
#include "logo.h"
#include "dbusthread.h"

#define DAEMON_NAME "LogitechDaemon"

DBusThread *dbus_thread = NULL;
int uinput_fd;
struct uinput_user_dev uinput;

void exitLogitechDaemon( int status )
{
	if( dbus_thread != NULL )
		delete( dbus_thread );

	if( writePixmapToLCD( blank_data ) != 0 )
		daemon_log( LOG_ERR, "Error blanking screen.\n" );

	setKBBrightness( G15_BRIGHTNESS_DARK );
	setLCDBrightness( G15_BRIGHTNESS_DARK );
	setLCDContrast( G15_CONTRAST_LOW );
	setLEDs( 0 );
	exitLibG15();
	ioctl( uinput_fd, UI_DEV_DESTROY );
	close( uinput_fd );
	daemon_log(LOG_INFO, "Exiting LogitechDaemon");
	daemon_retval_send(-1);
	daemon_signal_done();
	daemon_pid_file_remove();
	exit( status );
}

bool initialize()
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

	int error = initLibG15();

	if( error != G15_NO_ERROR ){
		daemon_log( LOG_ERR, "Failed to initialize libg15.\n" );
		return false;
	}

	if( writePixmapToLCD( logo_data ) != 0 )
		daemon_log( LOG_ERR, "Error displaying logo.\n" );

	setLEDs( G15_LED_M1 );
	setKBBrightness( G15_BRIGHTNESS_MEDIUM );
	setLCDBrightness( G15_BRIGHTNESS_MEDIUM );
	setLCDContrast( G15_CONTRAST_MEDIUM );

	return true;
}

int main( int argc, char *argv[] )
{
	int error;
	pid_t pid;

	/* Set indetification string for the daemon for both syslog and PID file */
	daemon_pid_file_ident = daemon_log_ident = daemon_ident_from_argv0(argv[0]);

	/* Check if we are called with -k parameter */
	if (argc >= 2 && !strcmp(argv[1], "-k")) {
		int ret;

		/* Kill daemon with SIGINT */

		/* Check if the new function daemon_pid_file_kill_wait() is available, if it is, use it. */
		if ((ret = daemon_pid_file_kill_wait(SIGINT, 5)) < 0)
			daemon_log(LOG_WARNING, "Failed to kill LogitechDaemon");

		return ret < 0 ? 1 : 0;
	}

	/* Check that the daemon is not rung twice a the same time */
	if ((pid = daemon_pid_file_is_running()) >= 0) {
		daemon_log(LOG_ERR, "LogitechDaemon already running on PID file %u", pid);
		return 1;

	}

	/* Prepare for return value passing from the initialization procedure of the daemon process */
	daemon_retval_init();

	/* Do the fork */
	if ((pid = daemon_fork()) < 0) {

		/* Exit on error */
		daemon_retval_done();
		return 1;

	} else if (pid) { /* The parent */

		int retval;

		/* Wait for 20 seconds for the return value passed from the daemon process */
		if ((retval = daemon_retval_wait(20)) < 0) {
			daemon_log(LOG_ERR, "Could not recieve return value from LogitechDaemon process.");
			return 255;
		}

		return retval;

	} else { /* The daemon */
		//Install signal handlers
// 		signal(  );

		if (daemon_close_all(-1) < 0) {
			daemon_log(LOG_ERR, "Failed to close all file descriptors: %s", strerror(errno));
			exitLogitechDaemon( EXIT_FAILURE );
		}

		/* Create the PID file */
		if (daemon_pid_file_create() < 0) {
			daemon_log(LOG_ERR, "Could not create PID file (%s).", strerror(errno));

			/* Send the error condition to the parent process */
			daemon_retval_send(1);
			exitLogitechDaemon( EXIT_FAILURE );
		}

		/* Initialize signal handling */
		if (daemon_signal_init(SIGINT, SIGTERM, SIGQUIT, SIGHUP, 0) < 0) {
			daemon_log(LOG_ERR, "Could not register signal handlers (%s).", strerror(errno));
			daemon_retval_send(2);
			exitLogitechDaemon( EXIT_FAILURE );
		}

		/*... do some further init work here */

		if( !initialize() ){
			daemon_log( LOG_ERR, "Failed to initialize LogitechDaemon.\n" );
			daemon_retval_send( 1 );
			exitLogitechDaemon( EXIT_FAILURE );
		}

		QCoreApplication app( argc, argv );
		dbus_thread = new DBusThread();
		dbus_thread->start();

		if( !dbus_thread->isRunning() ){
			daemon_log( LOG_ERR, "Failed to start DBus thread.\n" );
			daemon_retval_send( 1 );
			exitLogitechDaemon( EXIT_FAILURE );
		}

		/* Send OK to parent process */
		daemon_retval_send(0);
		daemon_log(LOG_INFO, "Sucessfully started LogitechDaemon");

		
		app.exec();

		
		/* Prepare for select() on the signal fd */
// 		FD_ZERO(&fds);
// 		FD_SET(fd = daemon_signal_fd(), &fds);
// 
// 		while (!quit){
// 			fd_set fds2 = fds;
// 
// 			/* Wait for an incoming signal */
// 			if (select(FD_SETSIZE, &fds2, 0, 0, 0) < 0) {
// 
// 				/* If we've been interrupted by an incoming signal, continue */
// 				if (errno == EINTR)
// 					continue;
// 
// 				daemon_log(LOG_ERR, "select(): %s", strerror(errno));
// 				break;
// 			}
// 
// 			/* Check if a signal has been recieved */
// 			if (FD_ISSET(fd, &fds)) {
// 				int sig;
// 
// 				/* Get signal */
// 				if ((sig = daemon_signal_next()) <= 0) {
// 					daemon_log(LOG_ERR, "daemon_signal_next() failed.");
// 					break;
// 				}
// 
// 				/* Dispatch signal */
// 				switch (sig) {
// 
// 					case SIGINT:
// 					case SIGQUIT:
// 					case SIGTERM:
// 						daemon_log(LOG_WARNING, "Got SIGINT, SIGQUIT or SIGTERM");
// 						quit = 1;
// 						break;
// 
// 					case SIGHUP:
// 						daemon_log(LOG_INFO, "Got a HUP");
// 						daemon_exec("/", NULL, "/bin/ls", "ls", (char*) NULL);
// 						break;
// 
// 				}
// 			}
// 		}
	}

	exitLogitechDaemon( EXIT_SUCCESS );
}
