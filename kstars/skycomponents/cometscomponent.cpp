/***************************************************************************
                          cometscomponent.cpp  -  K Desktop Planetarium
                             -------------------
    begin                : 2005/24/09
    copyright            : (C) 2005 by Jason Harris
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

#include "cometscomponent.h"
#include "solarsystemcomposite.h"

#include <QFile>
#include <QPen>
#include <QPainter>
#include <kglobal.h>

#include "Options.h"
#include "skyobjects/kscomet.h"
#include "kstarsdata.h"
#include "ksutils.h"
#include "ksfilereader.h"
#include "skymap.h"
#include "skylabeler.h"

CometsComponent::CometsComponent( SolarSystemComposite *parent, bool (*visibleMethod)(), int msize )
        : SolarSystemListComponent( parent, visibleMethod, msize )
{
}

CometsComponent::~CometsComponent() {
    //object deletion handled in grandparent class (ListComponent)
}

// FIXME: is KStarsData needed?
void CometsComponent::init() {
    QFile file;

    if ( KSUtils::openDataFile( file, "comets.dat" ) ) {
        emitProgressText( i18n("Loading comets") );

        KSFileReader fileReader( file );
        while( fileReader.hasMoreLines() ) {
            QString line, name;
            int mJD;
            double q, e, dble_i, dble_w, dble_N, Tp;
            float H, G;
            bool ok;
            long double JD;
            KSComet *com = 0;

            line = fileReader.readLine();

            name = line.mid( 3, 35 ).trimmed();
            mJD  = line.mid( 38, 5 ).toInt();
            q    = line.mid( 44, 10 ).toDouble();
            e    = line.mid( 55, 10 ).toDouble();
            dble_i = line.mid( 66, 9 ).toDouble();
            dble_w = line.mid( 76, 9 ).toDouble();
            dble_N = line.mid( 86, 9 ).toDouble();
            Tp = line.mid( 96, 14 ).toDouble();

            // Read the Absolute Magnitude (H) and Slope Parameter (G).
            // These might not be available in the data file always and we must be open to that fact
            H = line.mid( 124, 4 ).toFloat( &ok );
            if( !ok ) H = -101.0; // Any absolute mag brighter than -100 should be treated as nonsense
            G = line.mid( 129, 4 ).toFloat( &ok );
            if( !ok ) G = -101.0; // Same with slope parameter.

            JD = double( mJD ) + 2400000.5;

            com = new KSComet( name, QString(), JD, q, e, dms(dble_i), dms(dble_w), dms(dble_N), Tp, H, G );
            com->setAngularSize( 0.005 );

            objectList().append( com );

            //Add *short* name to the list of object names
            objectNames(SkyObject::COMET).append( com->name() );
        }
    }
}

void CometsComponent::draw( QPainter& psky )
{
    if ( !visible() || Options::zoomFactor() < 10*MINZOOM ) return;

    SkyMap *map = SkyMap::Instance();

    bool hideLabels =  ! Options::showCometNames() || (map->isSlewing() && Options::hideLabels() );
    double rsunLabelLimit = Options::maxRadCometName();

    psky.setPen( QPen( QColor( "darkcyan" ) ) );
    psky.setBrush( QBrush( QColor( "darkcyan" ) ) );

    foreach ( SkyObject *so, objectList() ) {
        KSComet *com = (KSComet*)so;

        if ( ! map->checkVisibility( com ) ) continue;

        QPointF o = map->toScreen( com );

        if ( ! map->onScreen( o ) ) continue;
        //if ( ( o.x() >= 0. && o.x() <= Width && o.y() >= 0. && o.y() <= Height ) )

        float size = com->angSize() * map->scale() * dms::PI * Options::zoomFactor()/10800.0;
	if ( size < 1.0 ) {
            psky.drawPoint( o );
        } else {
            float x1 = o.x() - 0.5*size;
            float y1 = o.y() - 0.5*size;

            psky.drawEllipse( QRectF( x1, y1, size, size ) );
        }

//        float tailsize = com->getTailAngSize().Degrees() * map->scale() * dms::PI * Options::zoomFactor()/10800.0;

        if ( hideLabels || com->rsun() >= rsunLabelLimit ) continue;
        SkyLabeler::AddLabel( o, com, SkyLabeler::COMET_LABEL );
    }
}
