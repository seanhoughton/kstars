/***************************************************************************
                          kswizard.h  -  description
                             -------------------
    begin                : Wed 28 Jan 2004
    copyright            : (C) 2004 by Jason Harris
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

#ifndef KSWIZARD_H
#define KSWIZARD_H

#include <qmemarray.h>

#include "kswizardui.h"
#include "kstars.h"
#include "geolocation.h"

/**@short Setup Wizard for KStars
	*@author Jason Harris
	*@version 1.0
	*/

class KSWizard : public KSWizardUI
{
Q_OBJECT
public:
	KSWizard( QWidget *parent=0, const char *name=0 );
	~KSWizard();
	GeoLocation* geo() const { return Geo; }

private slots:
	void slotChangeCity();
	void slotFilterCities();
	void slotTelescopeSetup();

private:
	void initGeoPage();
	
	KStars *ksw;
	QMemArray<int> GeoID;
	GeoLocation *Geo;
};

#endif
