/***************************************************************************
                          kswizard.cpp  -  description
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

#include <qfile.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <klineedit.h>
#include <klistbox.h>
#include <kpushbutton.h>

#include "kstarsdata.h"
#include "ksutils.h"
#include "geolocation.h"
#include "dmsbox.h"
#include "telescopewizardprocess.h"
#include "kswizardui.h"
#include "kswizard.h"

KSWizard::KSWizard( QWidget *parent, const char *name )
 : KSWizardUI( parent, name )
{
	ksw = (KStars *)parent;
	GeoID.resize(10000);
	
	//each page should have a finish button
	for ( unsigned int i=0; i<pageCount(); ++i ) {
		setFinishEnabled( page(i), true );
	}

	//Disable "Next" Button on last page
	setNextEnabled( page( pageCount() - 1 ), false );

	//Load images into banner frames.
	QFile imFile;
	QPixmap im = QPixmap();
	
	if ( KSUtils::openDataFile( imFile, "wzstars.png" ) ) {
		imFile.close(); //Just need the filename...
		im.load( imFile.name() );
	}
	Banner1->setPixmap( im );
	
	if ( KSUtils::openDataFile( imFile, "wzgeo.png" ) ) {
		imFile.close(); //Just need the filename...
		im.load( imFile.name() );
	}
	Banner2->setPixmap( im );
	
	if ( KSUtils::openDataFile( imFile, "wzscope.png" ) ) {
		imFile.close(); //Just need the filename...
		im.load( imFile.name() );
	}
	Banner3->setPixmap( im );

	//connect signals/slots
	connect( CityListBox, SIGNAL( selectionChanged() ), this, SLOT( slotChangeCity() ) );
	connect( CityFilter, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotFilterCities() ) );
	connect( ProvinceFilter, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotFilterCities() ) );
	connect( CountryFilter, SIGNAL( textChanged( const QString & ) ), this, SLOT( slotFilterCities() ) );
	connect( TelescopeWizardButton, SIGNAL( clicked() ), this, SLOT( slotTelescopeSetup() ) );

	//Initialize Geographic Location page
	initGeoPage();
}

KSWizard::~KSWizard() 
{}

void KSWizard::initGeoPage() {
	LongBox->setReadOnly( true );
	LatBox->setReadOnly( true );

	//Populate the CityListBox
	//flag the ID of the current City
	int index(0);
	for (GeoLocation *loc = ksw->data()->geoList.first(); loc; loc = ksw->data()->geoList.next()) {
		QString s;
		if ( loc->province().isEmpty() ) {
			s = loc->translatedName() + ", " + loc->translatedCountry();
		} else {
			s = loc->translatedName() + ", " + loc->translatedProvince() + 
					", " + loc->translatedCountry();
		}
		CityListBox->insertItem( s );
		GeoID[CityListBox->count() - 1] = ksw->data()->geoList.at();
		
		if ( loc->name() == ksw->data()->geo()->name() && loc->country() == ksw->data()->geo()->country() ) 
			index = ksw->data()->geoList.at();
	}
	
	//preset to current city
	CityListBox->setCurrentItem( index );
	Geo = ksw->data()->geoList.at( GeoID[ index ] );
}

void KSWizard::slotChangeCity() {
	Geo = ksw->data()->geoList.at(GeoID[CityListBox->currentItem()]);
	LongBox->showInDegrees( Geo->lng() );
	LatBox->showInDegrees( Geo->lat() );
}

void KSWizard::slotFilterCities() {
	CityListBox->clear();
	for (GeoLocation *loc = ksw->data()->geoList.first(); loc; loc = ksw->data()->geoList.next()) {
		QString sc( loc->translatedName() );
		QString ss( loc->translatedCountry() );
		QString sp = "";
		if ( !loc->province().isEmpty() )
			sp = loc->translatedProvince();

		if ( sc.lower().startsWith( CityFilter->text().lower() ) &&
				sp.lower().startsWith( ProvinceFilter->text().lower() ) &&
				ss.lower().startsWith( CountryFilter->text().lower() ) ) {
			sc.append( ", " );
			if ( !sp.isEmpty() ) {
				sc.append( sp );
				sc.append( ", " );
			}
			sc.append( ss );
			
			CityListBox->insertItem( sc );
			GeoID[CityListBox->count() - 1] = ksw->data()->geoList.at();
		}
	}

	if ( CityListBox->firstItem() ) {  // set first item in list as selected
		CityListBox->setCurrentItem( CityListBox->firstItem() );
		Geo = ksw->data()->geoList.at( GeoID[ CityListBox->currentItem() ] );
	}
}

void KSWizard::slotTelescopeSetup() {
	telescopeWizardProcess twiz(ksw);
	twiz.exec();
}

#include "kswizard.moc"
