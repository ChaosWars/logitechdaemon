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
#ifndef _DBUS_OBJECT_H_
#define _DBUS_OBJECT_H_

#include <glib-object.h>

#define DBUS_OBJECT_TYPE                ( dbus_object_get_type() )
#define DBUS_OBJECT( object )           ( G_TYPE_CHECK_INSTANCE_CAST( ( object ), DBUS_OBJECT_TYPE, DBusObject ) )
#define DBUS_OBJECT_CLASS( klass )      ( G_TYPE_CHECK_CLASS_CAST( ( klass ), DBUS_OBJECT_TYPE, DBusObjectClass ) )
#define IS_DBUS_OBJECT( object )        ( G_TYPE_CHECK_INSTANCE_TYPE( ( object ), DBUS_OBJECT_TYPE ) )
#define IS_DBUS_OBJECT_CLASS( klass )   ( G_TYPE_CHECK_CLASS_TYPE( ( klass ), DBUS_OBJECT_TYPE ) )
#define DBUS_OBJECT_GET_CLASS( object )    ( G_TYPE_INSTANCE_GET_CLASS( ( object ), DBUS_OBJECT_TYPE, DBusObjectClass ) )

typedef struct _DBusObject DBusObject;
typedef struct _DBusObjectClass DBusObjectClass;
typedef struct _DBusObjectPrivate DBusObjectPrivate;

g15canvas *canvas;

struct _DBusObject{
	GObject parent;

	/*< private >*/
	DBusObjectPrivate *priv;
};

struct _DBusObjectClass{
	GObjectClass parent;

	/*<private>*/
	DBusGConnection *connection;
	void(*dbus_object_lcd_brightness_set)( DBusObject *object, gint32 IN_brightness );
	void(*dbus_object_lcd_contrast_set)( DBusObject *object, gint32 IN_contrast );
	void(*dbus_object_kb_brightness_set)( DBusObject *object, gint32 IN_brightness );
};

GType dbus_object_get_type();
static void dbus_object_class_init( DBusObjectClass *klass );
static void dbus_object_init( GTypeInstance *instance, gpointer g_class );
static void dbus_object_dispose( GObject *object );
static void dbus_object_finalize( GObject *object );
static gboolean dbus_object_set_lcd_brightness( DBusObject *object, gint32 IN_brightness, GError **error );
static gboolean dbus_object_set_lcd_contrast( DBusObject *object, gint32 IN_contrast, GError **error );
static gboolean dbus_object_set_kb_brightness( DBusObject *object, gint32 IN_brightness, GError **error );
static gboolean dbus_object_blank_screen( DBusObject *object, GError **error );
static gboolean dbus_object_show_logo( DBusObject *object, GError **error );

#endif //	_DBUS_OBJECT_H_
