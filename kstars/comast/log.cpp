/***************************************************************************
                          log.cpp  -  description

                             -------------------
    begin                : Friday June 19, 2009
    copyright            : (C) 2009 by Prakash Mohan
    email                : prakash.mohan@kdemail.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "comast/log.h"
#include "kstars.h"
#include "kstarsdata.h"
#include "skyobjects/skyobject.h"
#include "skymap.h"
#include "skycomponents/constellationboundarylines.h"
#include "skycomponents/skymapcomposite.h"
#include "kstarsdatetime.h"

void Comast::Log::writeBegin() {
    ks = KStars::Instance();
    output = "";
    m_targetList = ks->observingList()->sessionList();
    writer = new QXmlStreamWriter(&output);
    writer->setAutoFormatting( true );
    writer->writeStartDocument();
    writer->writeNamespace( "http://observation.sourceforge.net/openastronomylog", "oal" );
    writer->writeNamespace( "http://www.w3.org/2001/XMLSchema-instance", "xsi" );
    writer->writeNamespace( "http://observation.sourceforge.net/openastronomylog oal20.xsd", "schemaLocation" );
    writer->writeStartElement("oal:observations");
    writer->writeAttribute("version", "2.0");
}

QString Comast::Log::writeLog( bool _native ) {
    native = _native;
    writeBegin();
    if( native )
        writeGeoDate();
    writeObservers();
    writeSites();
    writeSessions();
    writeTargets();
    writeScopes();
    writeEyepieces();
    writeLenses();
    writeFilters();
    writeImagers();
    writeObservations();
    writeEnd();
    return output;
}

void Comast::Log::writeEnd() {
    writer->writeEndDocument();
    delete writer;
}

void Comast::Log::writeObservers() {
    writer->writeStartElement( "observers" );
    foreach( Comast::Observer *o, m_observerList )
        writeObserver( o );
    writer->writeEndElement();
}

void Comast::Log::writeSites() {
    writer->writeStartElement("sites");
    foreach( Comast::Site *o, m_siteList )
        writeSite( o );
    writer->writeEndElement();
}

void Comast::Log::writeSessions() {
    writer->writeStartElement("sessions");
    foreach( Comast::Session *o, m_sessionList )
        writeSession( o );
    writer->writeEndElement();
}

void Comast::Log::writeTargets() {
    writer->writeStartElement("targets");
    foreach( SkyObject *o, m_targetList ) {
        writeTarget( o );
    }
    writer->writeEndElement();
}

void Comast::Log::writeScopes() {
    writer->writeStartElement("scopes");
    foreach( Comast::Scope *o, m_scopeList )
        writeScope( o );
    writer->writeEndElement();
}

void Comast::Log::writeEyepieces() {
    writer->writeStartElement("eyepieces");
    foreach( Comast::Eyepiece *o, m_eyepieceList )
        writeEyepiece( o );
    writer->writeEndElement();
}

void Comast::Log::writeLenses() {
    writer->writeStartElement("lenses");
    foreach( Comast::Lens *o, m_lensList )
        writeLens( o );
    writer->writeEndElement();
}

void Comast::Log::writeFilters() {
    writer->writeStartElement("filters");
    foreach( Comast::Filter *o, m_filterList )
        writeFilter( o );
    writer->writeEndElement();
}

void Comast::Log::writeImagers() {
    writer->writeStartElement("imagers");
    writer->writeEndElement();
}

void Comast::Log::writeObservations() {
    foreach( Comast::Observation *o, m_observationList )
        writeObservation( o );
}

void Comast::Log::writeTarget( SkyObject *o ) {
    writer->writeStartElement( "target" );
    writer->writeAttribute("id", o->name().remove( ' ' ) );
    QString typeString;
    if( native )
        writer->writeAttribute( "type", o->typeName() );
    else {
        switch( o->type() ) {
            case 0: typeString = "oal:starTargetType"; break;
            case 1: typeString = "oal:starTargetType"; break;
            case 2: typeString = "oal:PlanetTargetType"; break;
            case 3: typeString = "oal:deepSkyOC"; break;
            case 4: typeString = "oal:deepSkyGC"; break;
            case 5: typeString = "oal:deepSkyGN"; break;
            case 6: typeString = "oal:deepSkyPN"; break;
            case 8: typeString = "oal:deepSkyGX"; break;
            case 9: typeString = "oal:CometTargetType"; break;
            case 12: typeString = "oal:MoonTargetType"; break;
            case 13: typeString = "oal:deepSkyAS"; break;
            case 14: typeString = "oal:deepSkyCG"; break;
            case 15: typeString = "oal:deepSkyDN"; break;
            case 16: typeString = "oal:deepSkyQS"; break;
            case 17: typeString = "oal:deepSkyMS"; break;
            default: typeString = "oal:deepSkyNA"; break;
        }
        writer->writeAttribute("xsi:type", typeString );
    }
    writer->writeStartElement( "datasource" );
    writer->writeCDATA( "KStars" );
    writer->writeEndElement();
    writer->writeStartElement("name");
    writer->writeCDATA( o->name() );
    writer->writeEndElement();
    writer->writeStartElement( "position" );
    writer->writeStartElement( "ra" );
    writer->writeAttribute("unit", "rad" );
    writer->writeCharacters( QString::number( o->ra().radians() ) );
    writer->writeEndElement();
    writer->writeStartElement( "dec" );
    writer->writeAttribute("unit", "rad" );
    writer->writeCharacters( QString::number( o->dec().radians() ) );
    writer->writeEndElement();
    writer->writeEndElement();
    if( native && ! ks->observingList()->getTime( o ).isEmpty() ) {
        writer->writeStartElement("time");
        writer->writeCDATA( ks->observingList()->getTime( o ) );
        writer->writeEndElement();
    }
    writer->writeStartElement( "constellation" );
    writer->writeCDATA(
        KStarsData::Instance()->skyComposite()->getConstellationBoundary()->constellationName( o ) );
    writer->writeEndElement();
    writer->writeStartElement( "notes" );
    writer->writeCDATA( o->notes() );
    writer->writeEndElement();
    writer->writeEndElement();
}

void Comast::Log::writeObserver( Comast::Observer *o ) {
    writer->writeStartElement( "observer" );
    writer->writeAttribute( "id", o->id() );
    writer->writeStartElement( "name" );
    writer->writeCDATA( o->name() );
    writer->writeEndElement();
    writer->writeStartElement( "surname" );
    writer->writeCDATA( o->surname() );
    writer->writeEndElement();
    writer->writeStartElement( "contact" );
    writer->writeCDATA( o->contact() );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeSite( Comast::Site *s ) {
    writer->writeStartElement( "site" );
    writer->writeAttribute( "id", s->id() );
    writer->writeStartElement( "name" );
    writer->writeCDATA( s->name() );
    writer->writeEndElement();
    writer->writeStartElement( "latitude" );
    writer->writeAttribute( "unit",  s->latUnit() );
    writer->writeCharacters( QString::number( s->latitude() ) );
    writer->writeStartElement( "longitude" );
    writer->writeAttribute( "unit", s->lonUnit() );
    writer->writeCharacters( QString::number( s->longitude() ) );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeSession( Comast::Session *s ) {
    writer->writeStartElement( "session" );
    writer->writeAttribute( "id", s->id() ); 
    writer->writeStartElement( "begin" );
    writer->writeCharacters( s->begin().date().toString( "yyyy-MM-dd" ) + 'T' + s->begin().time().toString( "hh:mm:ss" ) );
    writer->writeEndElement();
    writer->writeStartElement( "end" );
    writer->writeCharacters( s->end().date().toString( "yyyy-MM-dd" ) + 'T' + s->end().time().toString( "hh:mm:ss" ) );
    writer->writeEndElement();
    writer->writeStartElement( "site" );
    writer->writeCharacters( s->site() );
    writer->writeEndElement();
    writer->writeStartElement( "weather" );
    writer->writeCDATA( s->weather() );
    writer->writeEndElement();
    writer->writeStartElement( "equipment" );
    writer->writeCDATA( s->equipment() );
    writer->writeEndElement();
    writer->writeStartElement( "comments" );
    writer->writeCDATA( s->comments() );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeScope( Comast::Scope *s ) {
    writer->writeStartElement( "scope" );
    writer->writeAttribute( "id", s->id() ); 
    writer->writeStartElement( "model" );
    writer->writeCDATA( s->model() );
    writer->writeEndElement();
    writer->writeStartElement( "type" );
    writer->writeCDATA( s->type().at(0) );
    writer->writeEndElement();
    writer->writeStartElement( "vendor" );
    writer->writeCDATA( s->vendor() );
    writer->writeEndElement();
    writer->writeStartElement( "aperture" );
    writer->writeCharacters( QString::number( s->aperture() ) );
    writer->writeEndElement();
    writer->writeStartElement( "focalLength" );
    writer->writeCharacters( QString::number( s->focalLength() ) );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeEyepiece( Comast::Eyepiece *ep ) {
    writer->writeStartElement( "eyepiece" );
    writer->writeAttribute( "id", ep->id() ); 
    writer->writeStartElement( "model" );
    writer->writeCDATA( ep->model() );
    writer->writeEndElement();
    writer->writeStartElement( "vendor" );
    writer->writeCDATA( ep->vendor() );
    writer->writeEndElement();
    writer->writeStartElement( "focalLength" );
    writer->writeCharacters( QString::number( ep->focalLength() ) );
    writer->writeEndElement();
    writer->writeStartElement( "apparantFOV" );
    writer->writeAttribute( "unit", ep->fovUnit() );
    writer->writeCharacters( QString::number( ep->appFov() ) );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeLens( Comast::Lens *l ) {
    writer->writeStartElement( "lens" );
    writer->writeAttribute( "id", l->id() );
    writer->writeStartElement( "model" );
    writer->writeCDATA( l->model() );
    writer->writeEndElement();
    writer->writeStartElement( "vendor" );
    writer->writeCDATA( l->vendor() );
    writer->writeEndElement();
    writer->writeStartElement( "factor" );
    writer->writeCharacters( QString::number( l->factor() ) );
    writer->writeEndElement();
    writer->writeEndElement();
}

void Comast::Log::writeFilter( Comast::Filter *f ) {
    writer->writeStartElement( "filter" );
    writer->writeAttribute( "id", f->id() );
    writer->writeStartElement( "model" );
    writer->writeCDATA( f->model() );
    writer->writeEndElement();
    writer->writeStartElement( "vendor" );
    writer->writeCDATA( f->vendor() );
    writer->writeEndElement();
    writer->writeStartElement( "type" );
    writer->writeCDATA( f->type() );
    writer->writeEndElement();
    writer->writeStartElement( "color" );
    writer->writeCDATA( f->color() );
    writer->writeEndElement();
    writer->writeEndElement();
}

void Comast::Log::writeObservation( Comast::Observation *o ) {
    writer->writeStartElement( "observation" );
    writer->writeStartElement( "observer" );
    writer->writeCharacters( o->observer() );
    writer->writeEndElement();
    writer->writeStartElement( "site" );
    writer->writeCharacters( o->site() );
    writer->writeEndElement();
    writer->writeStartElement( "session" );
    writer->writeCharacters( o->session() );
    writer->writeEndElement();
    writer->writeStartElement( "target" );
    writer->writeCharacters( o->target().remove( ' ' ) );
    writer->writeEndElement();
    writer->writeStartElement( "begin" );
    writer->writeCharacters( o->begin().date().toString( "yyyy-MM-dd" ) + 'T' + o->begin().time().toString( "hh:mm:ss" ) );
    writer->writeEndElement();
    writer->writeStartElement( "faintestStar" );
    writer->writeCharacters( QString::number( o->faintestStar() ) );
    writer->writeEndElement();
    writer->writeStartElement( "seeing" );
    writer->writeCharacters( QString::number( o->seeing() ) );
    writer->writeEndElement();
    writer->writeStartElement( "scope" );
    writer->writeCharacters( o->scope() );
    writer->writeEndElement();
    writer->writeStartElement( "eyepiece" );
    writer->writeCharacters( o->eyepiece() );
    writer->writeEndElement();
    writer->writeStartElement( "lens" );
    writer->writeCharacters( o->lens() );
    writer->writeEndElement();
    writer->writeStartElement( "filter" );
    writer->writeCharacters( o->filter() );
    writer->writeEndElement();
    writer->writeStartElement( "result" );
    writer->writeAttribute( "xsi:type", "oal:findingsType" );
    writer->writeAttribute( "lang", o->lang() );
    writer->writeStartElement( "description" );
    writer->writeCDATA( o->result() );
    writer->writeEndElement();
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::writeGeoDate() {
    writer->writeStartElement( "geodate" );
    writer->writeStartElement( "name" );
    writer->writeCDATA( ks->observingList()->geoLocation()->name() );
    writer->writeEndElement();
    writer->writeStartElement( "province" );
    writer->writeCDATA( ks->observingList()->geoLocation()->province() );
    writer->writeEndElement();
    writer->writeStartElement( "country" );
    writer->writeCDATA( ks->observingList()->geoLocation()->country() );
    writer->writeEndElement();
    writer->writeStartElement( "date" );
    writer->writeCDATA( ks->observingList()->dateTime().date().toString( "ddMMyyyy" ) );
    writer->writeEndElement();
    writer->writeEndElement();
}
void Comast::Log::readBegin( QString input ) {
    reader = new QXmlStreamReader( input );
    ks = KStars::Instance();
    while( ! reader->atEnd() ) {
        reader->readNext();
        if( reader->isStartElement() ) {
     //       if( reader->name() == "oal:observations" && reader->attributes().value("version") == "2.0" )
                readLog();
        }
    }
}

void Comast::Log::readUnknownElement() {
    while( ! reader->atEnd() ) {
        reader->readNext();
        
        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() )
            readUnknownElement();
    }
}

void Comast::Log::readLog() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "targets" )
                readTargets();
           else if( reader->name() == "observers" )
                readObservers();
           else if( reader->name() == "sites" )
                readSites();
           else if( reader->name() == "sessions" )
                readSessions();
           else if( reader->name() == "scopes" )
                readScopes();
           else if( reader->name() == "eyepieces" )
                readEyepieces();
           else if( reader->name() =="lenses" )
                readLenses();
           else if( reader->name() =="filters" )
                readFilters();
           else if( reader->name() == "observation" ) 
                readObservation( reader->attributes().value( "id" ).toString() );
           else if( reader->name() == "geodate" )
                readGeoDate();
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readTargets() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "target" )
                readTarget();
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readObservers() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "observer" )
                readObserver( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readSites() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "site" )
                readSite( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readSessions() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "session" )
                readSession( reader->attributes().value( "id" ).toString(), reader->attributes().value( "lang" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readScopes() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "scope" )
                readScope( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readEyepieces() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "eyepiece" )
                readEyepiece( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readLenses() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "lens" )
                readLens( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readFilters() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "filter" )
                readFilter( reader->attributes().value( "id" ).toString() );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readTarget() {
    SkyObject *o = NULL;
    QString name, time, notes;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "name" ) {
                name = reader->readElementText();
                if( name != "star" ) {
                    o = ks->data()->objectNamed( name );
                    if( ! o ) o = ks->data()->skyComposite()->findStarByGenetiveName( name );
                    if( o ) targetList()->append( o );
                }
            } else if( reader->name() == "time" ) {
                time = reader->readElementText();
                if( o )
                    TimeHash.insert( o->name(), QTime::fromString( time, "h:mm:ss AP" ) );
            } else if( reader->name() == "notes" ) {
                notes = reader->readElementText();
                if( o )
                    o->setNotes( notes );
            }
       //   else  if( reader->name() == "datasource" )
       //         kDebug() << reader->readElementText();
       //     else if( reader->name() == "position" )
       //         readPosition();
       //     else if( reader->name() == "constellation" )
       //         kDebug() << reader->readElementText();
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readObserver( QString id ) {
    QString name, surname, contact;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "name" ) {
                name = reader->readElementText();
            } else if( reader->name() == "surname" ) {
                surname = reader->readElementText();
            } else if( reader->name() == "contact" ) {
                contact = reader->readElementText();
            } else
                readUnknownElement();
        }
    }
    Comast::Observer *o= new Comast::Observer( id, name, surname, contact );
    m_observerList.append( o );
}

void Comast::Log::readSite( QString id ) {
    QString name, latUnit, lonUnit, lat, lon;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "name" ) {
                name = reader->readElementText();
            } else if( reader->name() == "latitude" ) {
                lat = reader->readElementText() ;
                latUnit = reader->attributes().value( "unit" ).toString();
            } else if( reader->name() == "longitude" ) {
                lon = reader->readElementText() ;
                lonUnit = reader->attributes().value( "unit" ).toString();
            } else
                readUnknownElement();
        }
    }
    Comast::Site *o= new Comast::Site( id, name, lat.toDouble(), latUnit, lon.toDouble(), lonUnit );
    m_siteList.append( o );
}

void Comast::Log::readSession( QString id, QString lang ) {
    QString site, weather, equipment, comments, begin, end;
    KStarsDateTime beginDT, endDT;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "site" ) {
                site = reader->readElementText();
            } else if( reader->name() == "begin" ) {
                begin = reader->readElementText() ;
                beginDT.fromString( begin );
            } else if( reader->name() == "end" ) {
                end = reader->readElementText() ;
                endDT.fromString( begin );
            } else if( reader->name() == "weather" ) {
                weather = reader->readElementText() ;
            } else if( reader->name() == "equipment" ) {
                equipment = reader->readElementText() ;
            } else if( reader->name() == "comments" ) {
                comments = reader->readElementText() ;
            } else
                readUnknownElement();
        }
    }
    Comast::Session *o= new Comast::Session( id, site, beginDT, endDT, weather, equipment, comments, lang );
    m_sessionList.append( o );
}

void Comast::Log::readScope( QString id ) {
    QString model, focalLength, vendor, type, aperture;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "model" ) {
                model = reader->readElementText();
            } else if( reader->name() == "vendor" ) {
                vendor = reader->readElementText() ;
            } else if( reader->name() == "type" ) {
                type = reader->readElementText() ;
                if( type == "N" ) type = "Newtonian";
                if( type == "R" ) type = "Refractor";
                if( type == "M" ) type = "Maksutov";
                if( type == "S" ) type = "Schmidt-Cassegrain";
                if( type == "K" ) type = "Kutter (Schiefspiegler)";
                if( type == "C" ) type = "Cassegrain";
            } else if( reader->name() == "focalLength" ) {
                focalLength = reader->readElementText() ;
            } else if( reader->name() == "aperture" ) {
                aperture = reader->readElementText() ;
            } else
                readUnknownElement();
        }
    }
    
    Comast::Scope *o= new Comast::Scope( id, model, vendor, type, focalLength.toDouble(), aperture.toDouble() );
    m_scopeList.append( o );
}

void Comast::Log::readEyepiece( QString id ) {
    QString model, focalLength, vendor, fov, fovUnit;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "model" ) {
                model = reader->readElementText();
            } else if( reader->name() == "vendor" ) {
                vendor = reader->readElementText() ;
            } else if( reader->name() == "apparentFOV" ) {
                fov = reader->readElementText();
                fovUnit = reader->attributes().value( "unit" ).toString();
            } else if( reader->name() == "focalLength" ) {
                focalLength = reader->readElementText() ;
            } else
                readUnknownElement();
        }
    }
    
    Comast::Eyepiece *o= new Comast::Eyepiece( id, model, vendor, fov.toDouble(), fovUnit, focalLength.toDouble() );
    m_eyepieceList.append( o );
}

void Comast::Log::readLens( QString id ) {
    QString model, factor, vendor;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "model" ) {
                model = reader->readElementText();
            } else if( reader->name() == "vendor" ) {
                vendor = reader->readElementText() ;
            } else if( reader->name() == "factor" ) {
                factor = reader->readElementText() ;
            } else
                readUnknownElement();
        }
    }
    
    Comast::Lens *o= new Comast::Lens( id, model, vendor, factor.toDouble() );
    m_lensList.append( o );
}

void Comast::Log::readFilter( QString id ) {
    QString model, vendor, type, color;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "model" ) {
                model = reader->readElementText();
            } else if( reader->name() == "vendor" ) {
                vendor = reader->readElementText() ;
            } else if( reader->name() == "type" ) {
                type = reader->readElementText() ;
            } else if( reader->name() == "color" ) {
                color = reader->readElementText() ;
            } else
                readUnknownElement();
        }
    }
    Comast::Filter *o= new Comast::Filter( id, model, vendor, type, color );
    m_filterList.append( o );
}

void Comast::Log::readPosition() {
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "ra" )
                kDebug() << reader->readElementText() << reader->attributes().value( "unit" );
            else if( reader->name() == "dec" )
                kDebug() << reader->readElementText() << reader->attributes().value( "unit" );
            else
                readUnknownElement();
        }
    }
}

void Comast::Log::readObservation( QString id ) {
    QString observer, site, session, target, faintestStar, seeing, scope, eyepiece, lens, filter, result, lang;
    KStarsDateTime begin;
    while( ! reader->atEnd() ) {
        reader->readNext();
        if( reader->isEndElement() )
            break;
        if( reader->isStartElement() ) {
            if( reader->name() == "observer" )
                observer = reader->readElementText();
            else if( reader->name() == "site" )
                site = reader->readElementText();
            else if( reader->name() == "session" )
                session = reader->readElementText();
            else if( reader->name() == "target" )
                target = reader->readElementText();
            else if( reader->name() == "begin" )
                begin.fromString( reader->readElementText() );
            else if( reader->name() == "faintestStar" )
                faintestStar = reader->readElementText();
            else if( reader->name() == "seeing" )
                seeing = reader->readElementText();
            else if( reader->name() == "scope" )
                scope = reader->readElementText();
            else if( reader->name() == "eyepiece" )
                eyepiece = reader->readElementText();
            else if( reader->name() == "lens" )
                lens = reader->readElementText();
            else if( reader->name() == "filter" )
                filter = reader->readElementText();
            else if( reader->name() == "result" ) {
                lang = reader->attributes().value( "lang" ).toString();
                result = readResult();
            } else
                readUnknownElement();
        }
    }
        Comast::Observation *o = new Comast::Observation( id, observer, site, session, target, begin, faintestStar.toDouble(), seeing.toDouble(), scope, eyepiece, lens, filter, result, lang );
        m_observationList.append( o );
}

QString Comast::Log::readResult() {
    QString result;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;
        if( reader->isStartElement() ) {
            if( reader->name() == "description" )
                result = reader->readElementText();
            else
                readUnknownElement();
        }
    }
    return result;
}

void Comast::Log::readGeoDate() {
    QString name, province, country, date;
    while( ! reader->atEnd() ) {
        reader->readNext();

        if( reader->isEndElement() )
            break;

        if( reader->isStartElement() ) {
            if( reader->name() == "name" )
                name = reader->readElementText();
            else if( reader->name() == "province" )
                province = reader->readElementText();
            else if( reader->name() == "country" )
                country = reader->readElementText();
            else if( reader->name() == "date" ){
                date = reader->readElementText();
            } else
                readUnknownElement();
        }
    }
    geo = ks->data()->locationNamed( name, province, country );
    dt.setDate( QDate::fromString( date, "ddMMyyyy" ) );
}

Comast::Observer* Comast::Log::findObserverByName( QString name ) {
    foreach( Comast::Observer *obs, *observerList() )
        if( obs->name() + ' ' + obs->surname() == name )
            return obs;
    return NULL;
}

Comast::Observer* Comast::Log::findObserverById( QString id ) {
    foreach( Comast::Observer *obs, *observerList() )
        if( obs->id() == id )
            return obs;
    return NULL;
}

Comast::Session* Comast::Log::findSessionByName( QString id ) {
    foreach( Comast::Session *s, *sessionList() )
        if( s->id()  == id )
            return s;
    return NULL;
}

Comast::Site* Comast::Log::findSiteById( QString id ) {
    foreach( Comast::Site *s, *siteList() )
        if( s->id()  == id )
            return s;
    return NULL;
}

Comast::Site* Comast::Log::findSiteByName( QString name ) {
    foreach( Comast::Site *s, *siteList() )
        if( s->name()  == name )
            return s;
    return NULL;
}

Comast::Scope* Comast::Log::findScopeById( QString id ) {
    foreach( Comast::Scope *s, *scopeList() )
        if( s->id()  == id )
            return s;
    return NULL;
}

Comast::Eyepiece* Comast::Log::findEyepieceById( QString id ) {
    foreach( Comast::Eyepiece *e, *eyepieceList() )
        if( e->id()  == id )
            return e;
    return NULL;
}

Comast::Lens* Comast::Log::findLensById( QString id ) {
    foreach( Comast::Lens *l, *lensList() )
        if( l->id()  == id )
            return l;
    return NULL;
}

Comast::Filter* Comast::Log::findFilterById( QString id ) {
    foreach( Comast::Filter *f, *filterList() )
        if( f->id()  == id )
            return f;
    return NULL;
}

Comast::Scope* Comast::Log::findScopeByName( QString name ) {
    foreach( Comast::Scope *s, *scopeList() )
        if( s->name()  == name )
            return s;
    return NULL;
}

Comast::Eyepiece* Comast::Log::findEyepieceByName( QString name ) {
    foreach( Comast::Eyepiece *e, *eyepieceList() )
        if( e->name()  == name )
            return e;
    return NULL;
}

Comast::Filter* Comast::Log::findFilterByName( QString name ) {
    foreach( Comast::Filter *f, *filterList() )
        if( f->name()  == name )
            return f;
    return NULL;
}

Comast::Lens* Comast::Log::findLensByName( QString name ) {
    foreach( Comast::Lens *l, *lensList() )
        if( l->name()  == name )
            return l;
    return NULL;
}

Comast::Observation* Comast::Log::findObservationByName( QString id ) {
    foreach( Comast::Observation *o, *observationList() )
        if( o->id()  == id )
            return o;
    return NULL;
}