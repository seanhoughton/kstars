/***************************************************************************
                          skyobjectname.h  -  description
                             -------------------
    begin                : Wed Aug 22 2001
    copyright            : (C) 2001 by Thomas Kabelmann
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

#ifndef SKYOBJECTNAME_H
#define SKYOBJECTNAME_H

#include <qstring.h>
#include <klistbox.h>
#include <klocale.h>

#include "skyobject.h"

/**Convenience class which contains a SkyObject's name and a pointer to the SkyObject
	*itself.  This class is used to construct the List of named objects that may be
	*located with the FindDialog.
	*@short convenience class for indexing SkyObjects by name.
	*@author Thomas Kabelmann
	*@version 0.9
	*/

class SkyObjectName {
	
	public:
	/**Constructor*/
		SkyObjectName (const QString &str = QString::null, SkyObject *obj = 0);

	/**Destructor (empty)*/
		~SkyObjectName() {}

	/**@returns the name of the SkyObject*/
		QString text() { return Text; }

	/**@returns translated version of the SkyObject's name*/
		QString translatedText() { return i18n( Text.local8Bit().data()); }

	/**@returns pointer to the SkyObject*/
		SkyObject *skyObject() { return skyobject; }

	private:
	
		SkyObject *skyobject;
		QString Text;
};


/**Class for filling list of named objects in the Find Object dialog (FindDialog).
	*The class is derived from QListBoxText, and adds a SkyObjectName* member variable,
	*and a method to retrieve this variable (a pointer).  This makes it very easy
	*to add these items to the FindDialog's QListBox, and to sort and filter them.
	*@short Derivative of QListBoxItem specifically for SkyObjects
	*@author Thomas Kabelmann
	*@version 0.9
	*/

class SkyObjectNameListItem : public QListBoxText  {
	
	public:
	/**Constructor.  Pass useLatinConstellationNames to determine if name() or translatedName()
		*should be used for constellations.
		*/
		SkyObjectNameListItem (QListBox *parent, SkyObjectName *name, bool useLocalConstellNames = true);

	/**Destructor (empty)*/
		~SkyObjectNameListItem() {}

	/**@returns pointer to SkyObjectName associated with this SkyObjectNameListItem */
		SkyObjectName * objName() { return object; }
		
	private:
		SkyObjectName *object;
};

#endif
