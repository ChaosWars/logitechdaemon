FIND_PATH( DBUS_INCLUDE_DIR dbus/dbus.h PATHS /usr/include/dbus-1.0 /usr/local/include/dbus-1.0 )
FIND_PATH( DBUS_GLIB_INCLUDE_DIR dbus/dbus-glib.h PATHS /usr/include/dbus-1.0 /usr/local/include/dbus-1.0 )
FIND_PATH( DBUS_QT4_INCLUDE_DIR QtDBus PATHS /usr/include/QtDBus /usr/local/include/QtDBus /opt/qt4/include/QtDBus PATH_SUFFIXES QtDBus )
FIND_PATH( DBUS_INCLUDE_LIB_DIR dbus/dbus-arch-deps.h PATHS /usr/lib/dbus-1.0/include /usr/local/lib/dbus-1.0/include )
FIND_LIBRARY( DBUS_LIBRARY NAME dbus-1 PATHS /usr/lib /usr/local/lib )
FIND_LIBRARY( DBUS_GLIB_LIBRARY NAME dbus-glib-1 PATHS /usr/lib /usr/local/lib )
FIND_LIBRARY( DBUS_QT4_LIBRARY NAME QtDBus PATHS /usr/lib /usr/local/lib /opt/qt4/lib )
FIND_PROGRAM( QT4_DBUSXML_2CPP NAME qdbusxml2cpp PATHS /usr/bin/ /usr/local/bin /opt/qt4 )
FIND_PROGRAM( GLIB_DBUS_BINDING_TOOL NAME dbus-binding-tool PATHS /usr/bin /usr/local/bin )

IF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR AND DBUS_LIBRARY )
   SET( DBUS_FOUND TRUE )
ENDIF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR AND DBUS_LIBRARY )

IF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR )
   SET( DBUS_INCLUDES ${DBUS_INCLUDE_DIR} ${DBUS_INCLUDE_LIB_DIR} )
ENDIF( DBUS_INCLUDE_DIR AND DBUS_INCLUDE_LIB_DIR )

IF( DBUS_GLIB_INCLUDE_DIR AND DBUS_GLIB_LIBRARY )
   SET( DBUS_GLIB_FOUND TRUE )
ENDIF( DBUS_GLIB_INCLUDE_DIR AND DBUS_GLIB_LIBRARY )

IF( DBUS_QT4_INCLUDE_DIR AND DBUS_QT4_LIBRARY )
   SET( DBUS_QT4_FOUND TRUE )
ENDIF( DBUS_QT4_INCLUDE_DIR AND DBUS_QT4_LIBRARY )

IF( DBUS_GLIB_FOUND )
   IF( NOT dbus-glib_FIND_QUIETLY )
      MESSAGE( STATUS "Found dbus-glib: ${DBUS_GLIB_LIBRARY}" )
   ENDIF( NOT dbus-glib_FIND_QUIETLY )
ELSE( DBUS_GLIB__FOUND )
   IF( dbus-glib_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find dbus-glib" )
   ENDIF( dbus-glib_FIND_REQUIRED )
ENDIF( DBUS_GLIB_FOUND )

IF( DBUS_QT4_FOUND )
   IF( NOT dbus-qt4_FIND_QUIETLY )
      MESSAGE( STATUS "Found dbus-qt4: ${DBUS_QT4_LIBRARY}" )
   ENDIF( NOT dbus-qt4_FIND_QUIETLY )
ELSE( DBUS_QT4__FOUND )
   IF( dbus-qt4_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find dbus-qt4" )
   ENDIF( dbus-qt4_FIND_REQUIRED )
ENDIF( DBUS_QT4_FOUND )

IF( DBUS_FOUND )
   IF( NOT dbus_FIND_QUIETLY )
      MESSAGE( STATUS "Found dbus: ${DBUS_LIBRARY}" )
   ENDIF( NOT dbus_FIND_QUIETLY )
ELSE( DBUS_FOUND )
   IF( dbus_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find dbus" )
   ENDIF( dbus_FIND_REQUIRED )
ENDIF( DBUS_FOUND )

IF( QT4_DBUSXML_2CPP )
	SET( QT4_WRAP_DBUSXML_ADAPTOR "YES" )
	SET( QT4_WRAP_DBUSXML_PROXY "YES" )
ENDIF( QT4_DBUSXML_2CPP )

IF( GLIB_DBUS_BINDING_TOOL )
	SET( GLIB_BIND_XML_SERVER "YES" )
	SET( GLIB_BIND_XML_CLIENT "YES" )
ENDIF( GLIB_DBUS_BINDING_TOOL )

MACRO( QT4_WRAP_DBUSXML_ADAPTOR outfiles classname )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( adaptor ${outfile}_adaptor )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}_adaptor.cpp )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${QT4_DBUSXML_2CPP}
			ARGS -c ${classname} -a "${adaptor}.h:${adaptor}.cpp" -m ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( QT4_WRAP_DBUSXML_ADAPTOR )

MACRO( QT4_WRAP_DBUSXML_PROXY outfiles classname )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( proxy ${outfile}_interface )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}_interface.cpp )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${QT4_DBUSXML_2CPP}
			ARGS -c ${classname} -p "${proxy}.h:${proxy}.cpp" -m ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( QT4_WRAP_DBUSXML_PROXY )

MACRO( GLIB_BIND_XML_SERVER outfiles prefix )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}glue.h )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${GLIB_DBUS_BINDING_TOOL}
			ARGS --mode=glib-server --prefix=${prefix} --output=${outfile} ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( GLIB_BIND_XML_SERVER )

MACRO( GLIB_BIND_XML_CLIENT outfiles prefix )
	FOREACH( it ${ARGN} )
		GET_FILENAME_COMPONENT( outfile ${it} NAME_WE )
		GET_FILENAME_COMPONENT( in ${it} ABSOLUTE )
		SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}glue.h )
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
			COMMAND ${GLIB_DBUS_BINDING_TOOL}
			ARGS --mode=glib-client --prefix=${prefix} --output=${outfile} ${in}
			DEPENDS ${infile} )
		SET( ${outfiles} "${${outfiles}}" ${outfile} )
	ENDFOREACH( it )
ENDMACRO( GLIB_BIND_XML_CLIENT )
