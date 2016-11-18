/* 2016-11-16                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

int MainWindow::parseMetadataXML( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    QString s_error					= "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );

    QDomDocument    doc;

// **********************************************************************************************
// read file

    if ( doc.setContent( &fin, true, &s_error, &i_errorLine, &i_errorColumn ) == false )
        return( -1 );

    QDomElement root = doc.documentElement();

    if ( root.tagName() != "MetaData" )
        return( -2 );

// **********************************************************************************************

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    QString s_DatasetID = "";

    QDomNode Citation = root.firstChildElement( "citation" );

    for ( int i=0; i<Citation.attributes().count(); i++ )
    {
        if ( Citation.attributes().item( i ).toAttr().localName() == "id" )
            s_DatasetID = Citation.attributes().item( i ).toAttr().value();
    }

    tout << "CITATION" << s_EOL;
    tout << "Data set ID" << "\t" << "Local node name" << "\t" << "Author ID" << "\t" << "Name" << "\t" << "Given name" << "\t" << "e-Mail" << s_EOL;

    for ( int i=0; i<Citation.childNodes().count(); i++ )
    {
        if ( Citation.childNodes().at( i ).localName() == "author" )
        {
            tout << s_DatasetID << "\t" << "author" << "\t" << Citation.childNodes().at( i ).attributes().item( 0 ).toAttr().value().section( ".author", 1, 1 ) << "\t";

            for( int j=0; j<Citation.childNodes().at( i ).childNodes().count(); j++ )
            {
                if ( Citation.childNodes().at( i ).childNodes().at( j ).localName() == "lastName" )
                    tout << Citation.childNodes().at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for( int j=0; j<Citation.childNodes().at( i ).childNodes().count(); j++ )
            {
                if ( Citation.childNodes().at( i ).childNodes().at( j ).localName() == "firstName" )
                    tout << Citation.childNodes().at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for( int j=0; j<Citation.childNodes().at( i ).childNodes().count(); j++ )
            {
                if ( Citation.childNodes().at( i ).childNodes().at( j ).localName() == "eMail" )
                    tout << Citation.childNodes().at( i ).childNodes().at( j ).toElement().text();
            }

            tout << s_EOL;
        }

        if ( Citation.childNodes().at( i ).localName() == "year" )
        {
            tout << s_DatasetID << "\t" << "year" << "\t";
            tout << Citation.childNodes().at( i ).toElement().text();
            tout << s_EOL;
        }

        if ( Citation.childNodes().at( i ).localName() == "title" )
        {
            tout << s_DatasetID << "\t" << "title" << "\t";
            tout << Citation.childNodes().at( i ).toElement().text();
            tout << s_EOL;
        }

        if ( Citation.childNodes().at( i ).localName() == "URI" )
        {
            tout << s_DatasetID << "\t" << "URI" << "\t";
            tout << Citation.childNodes().at( i ).toElement().text();
            tout << s_EOL;
        }

        if ( Citation.childNodes().at( i ).localName() == "parentURI" )
        {
            tout << s_DatasetID << "\t" << "parentURI" << "\t";
            tout << Citation.childNodes().at( i ).toElement().text();
            tout << s_EOL;
        }
    }

// **********************************************************************************************

    QString s_ReferenceID = "";

    QDomNodeList Reference = root.elementsByTagName( "reference" );

    if ( Reference.count() > 0 )
    {
        tout << s_EOL;

        if ( Reference.count() > 1 )
            tout << "REFERENCES";

        for ( int i=0; i<Reference.count(); i++ )
        {
            tout << s_EOL;
            tout << "Reference type" << s_EOL;

            tout << "Data set ID" << "\t" << "Reference ID" << "\t" << "Local node name" << "\t" << "Content" << s_EOL;

            for ( int j=0; j<Reference.at( i ).attributes().count(); j++ )
            {
                if ( Reference.at( i ).attributes().item( j ).toAttr().localName() == "id" )
                {
                    s_ReferenceID = Reference.at( i ).attributes().item( j ).toAttr().value();

                    s_ReferenceID.replace( "refds", "" );
                    s_ReferenceID.replace( "ref", "" );
                }
            }

            for ( int j=0; j<Reference.at( i ).attributes().count(); j++ )
            {
                if ( Reference.at( i ).attributes().item( j ).toAttr().localName() != "id" )
                {
                    tout << s_DatasetID << "\t" << s_ReferenceID << "\t" << Reference.at( i ).attributes().item( j ).toAttr().localName() << "\t";
                    tout << Reference.at( i ).attributes().item( j ).toAttr().value();
                    tout << s_EOL;
                }
            }

            tout << s_EOL;
            tout << "Reference" << s_EOL;
            tout << "Data set ID" << "\t" << "Reference ID" << "\t" << "Local node name" << "\t" << "Author ID" << "\t" << "Name" << "\t" << "Given name" << "\t" << "e-Mail" << s_EOL;

            for ( int j=0; j<Reference.at( i ).childNodes().count(); j++ )
            {
                if ( Reference.at( i ).childNodes().at( j ).localName() == "author" )
                {
                    tout << s_DatasetID << "\t" << s_ReferenceID << "\t" << "author" << "\t" << Reference.at( i ).childNodes().at( j ).attributes().item( 0 ).toAttr().value().section( ".author", 1, 1 ) << "\t";

                    for( int k=0; k<Reference.at( i ).childNodes().at( j ).childNodes().count(); k++ )
                    {
                        if ( Reference.at( i ).childNodes().at( j ).childNodes().at( k ).localName() == "lastName" )
                            tout << Reference.at( i ).childNodes().at( j ).childNodes().at( k ).toElement().text();
                    }

                    tout << "\t";

                    for( int k=0; k<Reference.at( i ).childNodes().at( j ).childNodes().count(); k++ )
                    {
                        if ( Reference.at( i ).childNodes().at( j ).childNodes().at( k ).localName() == "firstName" )
                            tout << Reference.at( i ).childNodes().at( j ).childNodes().at( k ).toElement().text();
                    }

                    tout << "\t";

                    for( int k=0; k<Reference.at( i ).childNodes().at( j ).childNodes().count(); k++ )
                    {
                        if ( Reference.at( i ).childNodes().at( j ).childNodes().at( k ).localName() == "eMail" )
                            tout << Reference.at( i ).childNodes().at( j ).childNodes().at( k ).toElement().text();
                    }

                    tout << s_EOL;
                }

                if ( Reference.at( i ).childNodes().at( j ).localName() == "year" )
                {
                    tout << s_DatasetID << "\t" << s_ReferenceID << "\t" << "year" << "\t";
                    tout << Reference.at( i ).childNodes().at( j ).toElement().text();
                    tout << s_EOL;
                }

                if ( Reference.at( i ).childNodes().at( j ).localName() == "title" )
                {
                    tout << s_DatasetID << "\t" << s_ReferenceID << "\t" << "title" << "\t";
                    tout << Reference.at( i ).childNodes().at( j ).toElement().text();
                    tout << s_EOL;
                }

                if ( Reference.at( i ).childNodes().at( j ).localName() == "URI" )
                {
                    tout << s_DatasetID << "\t" << s_ReferenceID << "\t" << "URI" << "\t";
                    tout << Reference.at( i ).childNodes().at( j ).toElement().text();
                    tout << s_EOL;
                }
            }
        }
    }

// **********************************************************************************************

    QDomNode Coverage = root.firstChildElement( "extent" );

    tout << s_EOL;
    tout << "COVERAGE" << s_EOL;

    tout << "Data set ID" << "\t" << "Geocode ID" << "\t" << "Local node name" << "\t" << "Content" << s_EOL;

    for ( int i=0; i<Coverage.childNodes().count(); i++ )
    {
        if ( Coverage.childNodes().at( i ).localName() == "topoType" )
                tout << s_DatasetID << "\t" << "\t" << "topoType" << "\t" << Coverage.childNodes().at( i ).toElement().text() << s_EOL;
    }

    for ( int i=0; i<Coverage.childNodes().count(); i++ )
    {
        if ( Coverage.childNodes().at( i ).localName() == "geographic" )
        {
            for( int j=0; j<Coverage.childNodes().at( i ).childNodes().count(); j++ )
                tout << s_DatasetID << "\t" << "\t" << Coverage.childNodes().at( i ).childNodes().at( j ).localName() << "\t" << QString( "%1" ).arg( Coverage.childNodes().at( i ).childNodes().at( j ).toElement().text().toDouble(), 0, 'f', 5 ) << s_EOL;
        }
    }

    for ( int i=0; i<Coverage.childNodes().count(); i++ )
    {
        if ( Coverage.childNodes().at( i ).localName() == "temporal" )
        {
            for( int j=0; j<Coverage.childNodes().at( i ).childNodes().count(); j++ )
                tout << s_DatasetID << "\t" << "\t" << Coverage.childNodes().at( i ).childNodes().at( j ).localName() << "\t" << Coverage.childNodes().at( i ).childNodes().at( j ).toElement().text() << s_EOL;
        }

    }

    for ( int i=0; i<Coverage.childNodes().count(); i++ )
    {
        if ( Coverage.childNodes().at( i ).localName() == "elevation" )
        {
            QString s_GeocodeID = "";
            QString s_Parameter = "";
            QString s_Unit      = "";

            for ( int j=0; j<Coverage.childNodes().at( i ).attributes().count(); j++ )
            {
                if ( Coverage.childNodes().at( i ).attributes().item( j ).localName() == "geocodeId" )
                    s_GeocodeID = Coverage.childNodes().at( i ).attributes().item( j ).toAttr().value();

                if ( Coverage.childNodes().at( i ).attributes().item( j ).localName() == "name" )
                    s_Parameter = Coverage.childNodes().at( i ).attributes().item( j ).toAttr().value();

                if ( Coverage.childNodes().at( i ).attributes().item( j ).localName() == "unit" )
                    s_Unit = Coverage.childNodes().at( i ).attributes().item( j ).toAttr().value();
            }

            if ( s_Unit.isEmpty() == false )
                s_Unit = " [" + s_Unit + "]";

            for( int j=0; j<Coverage.childNodes().at( i ).childNodes().count(); j++ )
            {
                if ( s_Unit.isEmpty() == false )
                    tout << s_DatasetID << "\t" << s_GeocodeID << "\t" << s_Parameter << ", " << Coverage.childNodes().at( i ).childNodes().at( j ).localName() << s_Unit << "\t" << Coverage.childNodes().at( i ).childNodes().at( j ).toElement().text() << s_EOL;
                else
                    tout << s_DatasetID << "\t" << s_GeocodeID << "\t" << s_Parameter << ", " << Coverage.childNodes().at( i ).childNodes().at( j ).localName() << "\t" << Coverage.childNodes().at( i ).childNodes().at( j ).toElement().text() << s_EOL;
            }
        }
    }

// **********************************************************************************************

    QString s_EventID = "";

    QDomNodeList Event = root.elementsByTagName( "event" );

    if ( Event.count() > 0 )
    {
        tout << s_EOL;

        if ( Event.count() > 1 )
            tout << "EVENTS" << s_EOL;
        else
            tout << "EVENT" << s_EOL;

        tout << "Data set ID" << "\t" << "Event ID" << "\t" << "Event label" << "\t";
        tout << "Device" << "\t" << "Device" << "\t" << "Date/Time" << "\t";
        tout << "Latitude" << "\t" << "Longitude" << "\t" << "Elevation";
        tout << "\t" << "Location" << "\t" << "Campaign" << "\t" << "Basis" << s_EOL;

        for ( int i=0; i<Event.count(); i++ )
        {
            for ( int j=0; j<Event.at( i ).attributes().count(); j++ )
            {
                if ( Event.at( i ).attributes().item( j ).toAttr().localName() == "id" )
                    s_EventID = Event.at( i ).attributes().item( j ).toAttr().value().section( "event", 1, 1);
            }

            tout << s_DatasetID << "\t" << s_EventID << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "label" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "device" )
                {
                    for ( int k=0; k<Event.at( i ).childNodes().at( j ).childNodes().count(); k++ )
                        tout << Event.at( i ).childNodes().at( j ).childNodes().at( k ).toElement().text() << "\t";
                }
            }

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "dateTime" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "latitude" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "longitude" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "elevation" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "location" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "campaign" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << "\t";

            for ( int j=0; j<Event.at( i ).childNodes().count(); j++ )
            {
                if ( Event.at( i ).childNodes().at( j ).localName() == "basis" )
                    tout << Event.at( i ).childNodes().at( j ).toElement().text();
            }

            tout << s_EOL;
        }
    }

    fout.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doConvertMetadataXML()
{
    int         i				= 0;
    int         err				= 0;
    int         stopProgress	= 0;

    QString     s_FilenameIn    = "";
    QString     s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        setWaitCursor();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert PANGAEA metadata XML files..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = parseMetadataXML( s_FilenameIn, s_FilenameOut, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }
            else
            {
                err = _FILENOTEXISTS_;
            }
        }

        resetFileProgress( gsl_FilenameList.count() );

        setNormalCursor();
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting PANGAEA metadata XML was canceled" ), false, false );

    onError( err );
}

