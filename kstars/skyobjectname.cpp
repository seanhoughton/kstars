/***************************************************************************
                          skyobjectname.cpp  -  description
                             -------------------
    begin                : Wed Aug 22 2001
    copyright            : (C) 2001 by Jason Harris
    email                : kstars@30doradus.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "skyobjectname.h"

#include <klocale.h>

SkyObjectName::SkyObjectName( const QString &str, SkyObject *obj )
	: skyobject ( obj ), Text ( str )
{
}

SkyObjectNameListItem::SkyObjectNameListItem ( QListBox *parent, SkyObjectName *obj, bool useLocalConstellNames )
	: QListBoxText ( parent ), object ( obj )
{
	if ( obj->skyObject()->type()==-1 ) { //constellation
		if ( useLocalConstellNames ) {
			//can't use translatedText(), because we need the i18n() comment string...
			setText( i18n( "Constellation name (optional)", obj->text().local8Bit().data() ) );
		} else {
			setText ( obj->text() );
		}
	} else {  //all non-constellation objects
		setText ( obj->translatedText() );
	}
}


