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
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <libg15.h>
#include <libdaemon/daemon.h>
#include <linux/uinput.h>
#include "handlekeys.h"

#define GKEY_OFFSET 167
#define MKEY_OFFSET 185
#define LKEY_OFFSET 189

extern int uinput_fd;
static unsigned int keys = 0;
static unsigned int lastkeys = 0;
static bool m1_key = true;
static bool m2_key = false;
static bool m3_key = false;
static bool mr_key = false;

/* The majority of the code is taken verbatim from g15daemon */
void handlekeys()
{
	getPressedKeys ( &keys, 40 );

	if ( keys != lastkeys )
	{
		/* Handle the M keys */

		if ( ( keys  & G15_KEY_M1 ) && ! ( lastkeys & G15_KEY_M1 ) )
		{
			m1_key = 1;
			m2_key = 0;
			m3_key = 0;
			setLEDs ( G15_LED_M1 | ( mr_key ? G15_LED_MR : 0 ) );
			keydown ( MKEY_OFFSET );
		}
		else if ( ! ( keys  & G15_KEY_M1 ) && ( lastkeys & G15_KEY_M1 ) )
		{
			keyup ( MKEY_OFFSET );
		}

		if ( ( keys  & G15_KEY_M2 ) && ! ( lastkeys & G15_KEY_M2 ) )
		{
			m1_key = 0;
			m2_key = 1;
			m3_key = 0;
			setLEDs ( G15_LED_M2 | ( mr_key ? G15_LED_MR : 0 ) );
			keydown ( MKEY_OFFSET+1 );
		}
		else if ( ! ( keys  & G15_KEY_M2 ) && ( lastkeys & G15_KEY_M2 ) )
		{
			keyup ( MKEY_OFFSET+1 );
		}

		if ( ( keys  & G15_KEY_M3 ) && ! ( lastkeys & G15_KEY_M3 ) )
		{
			m1_key = 0;
			m2_key = 0;
			m3_key = 1;
			setLEDs ( G15_LED_M3 | ( mr_key ? G15_LED_MR : 0 ) );
			keydown ( MKEY_OFFSET+2 );
		}
		else if ( ! ( keys  & G15_KEY_M3 ) && ( lastkeys & G15_KEY_M3 ) )
		{
			keyup ( MKEY_OFFSET+2 );
		}

		if ( ( keys  & G15_KEY_MR ) && ! ( lastkeys & G15_KEY_MR ) )
		{

			if ( mr_key == 0 )
			{
				mr_key = 1;
                setLEDs ( G15_LED_MR | ( m1_key ? G15_LED_M1 : 0 ) | ( m2_key ? G15_LED_M2 : 0 ) | ( m3_key ? G15_LED_M3 : 0 ) );
			}
			else
			{
				mr_key = 0;
                setLEDs ( ( m1_key ? G15_LED_M1 : 0 ) | ( m2_key ? G15_LED_M2 : 0 ) | ( m3_key ? G15_LED_M3 : 0 ) );
			}

			keydown ( MKEY_OFFSET+3 );

		}
		else if ( ! ( keys  & G15_KEY_MR ) && ( lastkeys & G15_KEY_MR ) )
		{
			keyup ( MKEY_OFFSET+3 );
		}

		/* Handle the G keys */

		if ( ( keys & G15_KEY_G1 ) && ! ( lastkeys & G15_KEY_G1 ) )
			keydown ( GKEY_OFFSET );
		else if ( ! ( keys & G15_KEY_G1 ) && ( lastkeys & G15_KEY_G1 ) )
			keyup ( GKEY_OFFSET );

		if ( ( keys & G15_KEY_G2 ) && ! ( lastkeys & G15_KEY_G2 ) )
			keydown ( GKEY_OFFSET+1 );
		else if ( ! ( keys & G15_KEY_G2 ) && ( lastkeys & G15_KEY_G2 ) )
			keyup ( GKEY_OFFSET+1 );

		if ( ( keys & G15_KEY_G3 ) && ! ( lastkeys & G15_KEY_G3 ) )
			keydown ( GKEY_OFFSET+2 );
		else if ( ! ( keys & G15_KEY_G3 ) && ( lastkeys & G15_KEY_G3 ) )
			keyup ( GKEY_OFFSET+2 );

		if ( ( keys & G15_KEY_G4 ) && ! ( lastkeys & G15_KEY_G4 ) )
			keydown ( GKEY_OFFSET+3 );
		else if ( ! ( keys & G15_KEY_G4 ) && ( lastkeys & G15_KEY_G4 ) )
			keyup ( GKEY_OFFSET+3 );

		if ( ( keys & G15_KEY_G5 ) && ! ( lastkeys & G15_KEY_G5 ) )
			keydown ( GKEY_OFFSET+4 );
		else if ( ! ( keys & G15_KEY_G5 ) && ( lastkeys & G15_KEY_G5 ) )
			keyup ( GKEY_OFFSET+4 );

		if ( ( keys & G15_KEY_G6 ) && ! ( lastkeys & G15_KEY_G6 ) )
			keydown ( GKEY_OFFSET+5 );
		else if ( ! ( keys & G15_KEY_G6 ) && ( lastkeys & G15_KEY_G6 ) )
			keyup ( GKEY_OFFSET+5 );

		if ( ( keys & G15_KEY_G7 ) && ! ( lastkeys & G15_KEY_G7 ) )
			keydown ( GKEY_OFFSET+6 );
		else if ( ! ( keys & G15_KEY_G7 ) && ( lastkeys & G15_KEY_G7 ) )
			keyup ( GKEY_OFFSET+6 );

		if ( ( keys & G15_KEY_G8 ) && ! ( lastkeys & G15_KEY_G8 ) )
			keydown ( GKEY_OFFSET+7 );
		else if ( ! ( keys & G15_KEY_G8 ) && ( lastkeys & G15_KEY_G8 ) )
			keyup ( GKEY_OFFSET+7 );

		if ( ( keys & G15_KEY_G9 ) && ! ( lastkeys & G15_KEY_G9 ) )
			keydown ( GKEY_OFFSET+8 );
		else if ( ! ( keys & G15_KEY_G9 ) && ( lastkeys & G15_KEY_G9 ) )
			keyup ( GKEY_OFFSET+8 );

		if ( ( keys & G15_KEY_G10 ) && ! ( lastkeys & G15_KEY_G10 ) )
			keydown ( GKEY_OFFSET+9 );
		else if ( ! ( keys & G15_KEY_G10 ) && ( lastkeys & G15_KEY_G10 ) )
			keyup ( GKEY_OFFSET+9 );

		if ( ( keys & G15_KEY_G11 ) && ! ( lastkeys & G15_KEY_G11 ) )
			keydown ( GKEY_OFFSET+10 );
		else if ( ! ( keys & G15_KEY_G11 ) && ( lastkeys & G15_KEY_G11 ) )
			keyup ( GKEY_OFFSET+10 );

		if ( ( keys & G15_KEY_G12 ) && ! ( lastkeys & G15_KEY_G12 ) )
			keydown ( GKEY_OFFSET+11 );
		else if ( ! ( keys & G15_KEY_G12 ) && ( lastkeys & G15_KEY_G12 ) )
			keyup ( GKEY_OFFSET+11 );

		if ( ( keys & G15_KEY_G13 ) && ! ( lastkeys & G15_KEY_G13 ) )
			keydown ( GKEY_OFFSET+12 );
		else if ( ! ( keys & G15_KEY_G13 ) && ( lastkeys & G15_KEY_G13 ) )
			keyup ( GKEY_OFFSET+12 );

		if ( ( keys & G15_KEY_G14 ) && ! ( lastkeys & G15_KEY_G14 ) )
			keydown ( GKEY_OFFSET+13 );
		else if ( ! ( keys & G15_KEY_G14 ) && ( lastkeys & G15_KEY_G14 ) )
			keyup ( GKEY_OFFSET+13 );

		if ( ( keys & G15_KEY_G15 ) && ! ( lastkeys & G15_KEY_G15 ) )
			keydown ( GKEY_OFFSET+14 );
		else if ( ! ( keys & G15_KEY_G15 ) && ( lastkeys & G15_KEY_G15 ) )
			keyup ( GKEY_OFFSET+14 );

		if ( ( keys & G15_KEY_G16 ) && ! ( lastkeys & G15_KEY_G16 ) )
			keydown ( GKEY_OFFSET+15 );
		else if ( ! ( keys & G15_KEY_G16 ) && ( lastkeys & G15_KEY_G16 ) )
			keyup ( GKEY_OFFSET+15 );

		if ( ( keys & G15_KEY_G17 ) && ! ( lastkeys & G15_KEY_G17 ) )
			keydown ( GKEY_OFFSET+16 );
		else if ( ! ( keys & G15_KEY_G17 ) && ( lastkeys & G15_KEY_G17 ) )
			keyup ( GKEY_OFFSET+16 );

		if ( ( keys & G15_KEY_G18 ) && ! ( lastkeys & G15_KEY_G18 ) )
			keydown ( GKEY_OFFSET+17 );
		else if ( ! ( keys & G15_KEY_G18 ) && ( lastkeys & G15_KEY_G18 ) )
			keyup ( GKEY_OFFSET+17 );

		/* Handle the L keys */

        if ( ( keys & G15_KEY_L1 ) && ! ( lastkeys & G15_KEY_L1 ) )
            keydown ( LKEY_OFFSET );
        else if ( ! ( keys & G15_KEY_L1 ) && ( lastkeys & G15_KEY_L1 ) )
            keyup ( LKEY_OFFSET );

		if ( ( keys & G15_KEY_L2 ) && ! ( lastkeys & G15_KEY_L2 ) )
			keydown ( LKEY_OFFSET+1 );
		else if ( ! ( keys & G15_KEY_L2 ) && ( lastkeys & G15_KEY_L2 ) )
			keyup ( LKEY_OFFSET+1 );

		if ( ( keys & G15_KEY_L3 ) && ! ( lastkeys & G15_KEY_L3 ) )
			keydown ( LKEY_OFFSET+2 );
		else if ( ! ( keys & G15_KEY_L3 ) && ( lastkeys & G15_KEY_L3 ) )
			keyup ( LKEY_OFFSET+2 );

		if ( ( keys & G15_KEY_L4 ) && ! ( lastkeys & G15_KEY_L4 ) )
			keydown ( LKEY_OFFSET+3 );
		else if ( ! ( keys & G15_KEY_L4 ) && ( lastkeys & G15_KEY_L4 ) )
			keyup ( LKEY_OFFSET+3 );

		if ( ( keys & G15_KEY_L5 ) && ! ( lastkeys & G15_KEY_L5 ) )
			keydown ( LKEY_OFFSET+4 );
		else if ( ! ( keys & G15_KEY_L5 ) && ( lastkeys & G15_KEY_L5 ) )
			keyup ( LKEY_OFFSET+4 );
	}

	lastkeys = keys;
}

void keydown ( unsigned char code )
{
	struct input_event event;
	memset ( &event, 0, sizeof ( event ) );

	event.type = EV_KEY;
	event.code = code;
	event.value = 1;

	write ( uinput_fd, &event, sizeof ( event ) );
}

void keyup ( unsigned char code )
{
	struct input_event event;
	memset ( &event, 0, sizeof ( event ) );

	event.type = EV_KEY;
	event.code = code;
	event.value = 0;

	write ( uinput_fd, &event, sizeof ( event ) );
}
