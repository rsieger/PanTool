/* 2016-11-16                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-19

QString MainWindow::getAttributeValue( QDomNode Node, const QString &s_Attribute )
{
    for ( int i=0; i<Node.attributes().count(); i++ )
    {
        if ( Node.attributes().item( i ).toAttr().localName() == s_Attribute )
            return ( Node.attributes().item( i ).toAttr().value() );
    }

    return( "Attribute not found" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-19

QString MainWindow::getNodeValue( QDomNode Node )
{
    return( Node.toElement().text() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-19

QString MainWindow::getStaffEntry( QDomNode Node )
{
    QString     s_ORCID = "";

    QStringList sl_StaffEntry;

// **********************************************************************************************

    s_ORCID = getNodeValue( Node.firstChildElement( "orcid" ) );

    if ( s_ORCID.isEmpty() == false )
        s_ORCID = tr( "http://orcid.org/" ) + s_ORCID;

    sl_StaffEntry.append( getAttributeValue( Node, "id" ).section( ".author", 1, 1 ) );
    sl_StaffEntry.append( getNodeValue( Node.firstChildElement( "lastName" ) ) );
    sl_StaffEntry.append( getNodeValue( Node.firstChildElement( "firstName" ) ) );
    sl_StaffEntry.append( getNodeValue( Node.firstChildElement( "eMail" ) ) );
    sl_StaffEntry.append( getNodeValue( Node.firstChildElement( "phone" ) ) );
    sl_StaffEntry.append( s_ORCID );
    sl_StaffEntry.append( getNodeValue( Node.firstChildElement( "URI" ) ) );

    return( sl_StaffEntry.join( "\t" ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-20

QString MainWindow::getCitationEntry( QDomNode Node )
{
    QStringList sl_CitationEntry;

    sl_CitationEntry.append( getAttributeValue( Node, "id" ) );
    sl_CitationEntry.append( getNodeValue( Node.firstChildElement( "year" ) ) );
    sl_CitationEntry.append( getNodeValue( Node.firstChildElement( "title" ) ) );
    sl_CitationEntry.append( getNodeValue( Node.firstChildElement( "source" ) ) );
    sl_CitationEntry.append( getNodeValue( Node.firstChildElement( "URI" ) ) );

    return( sl_CitationEntry.join( "\t" ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-20

QString MainWindow::getReferenceEntry( QDomNode Node )
{
    QStringList sl_ReferenceEntry;

    sl_ReferenceEntry.append( getAttributeValue( Node, "id" ) );
    sl_ReferenceEntry.append( getAttributeValue( Node, "relationType" ) );

    if ( getNodeValue( Node.firstChildElement( "prepubStatus" ) ).isEmpty() == false )
        sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "prepubStatus" ) ) );
    else
        sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "year" ) ) );

    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "title" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "source" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "volume" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "pages" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "URI" ) ) );

    return( sl_ReferenceEntry.join( "\t" ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2016-11-23

QString MainWindow::getProjectEntry( QDomNode Node )
{
    QStringList sl_ProjectEntry;

    sl_ProjectEntry.append( getAttributeValue( Node, "id" ).replace( "project", "" ) );
    sl_ProjectEntry.append( getNodeValue( Node.firstChildElement( "label" ) ) );
    sl_ProjectEntry.append( getNodeValue( Node.firstChildElement( "name" ) ) );
    sl_ProjectEntry.append( getNodeValue( Node.firstChildElement( "URI" ) ) );

    return( sl_ProjectEntry.join( "\t" ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

QString MainWindow::createCitationOutputString( const QStringList &sl_Authors, const QString &s_Citation )
{
    QString OutputStr = s_Citation.section( "\t", 0, 0 ) + "\t" + "@citation@" + "\t";

    if ( sl_Authors.count() > 0 )
    {
        OutputStr.append( sl_Authors.at( 0 ).section( "\t", 1, 1 ) + ", " + sl_Authors.at( 0 ).section( "\t", 2, 2 ) );

        for ( int i=1; i<sl_Authors.count(); i++ )
            OutputStr.append( "; " + sl_Authors.at( i ).section( "\t", 1, 1 ) + ", " + sl_Authors.at( i ).section( "\t", 2, 2 ) );
    }

    if ( s_Citation.isEmpty() == false )
    {
        OutputStr.append( "\t" + s_Citation.section( "\t", 1, 1 ) ); // year
        OutputStr.append( "\t" + s_Citation.section( "\t", 2, 2 ) ); // title
        OutputStr.append( "\t" + s_Citation.section( "\t", 3, 3 ) ); // source
        OutputStr.append( "\t" + s_Citation.section( "\t", 4, 4 ) ); // URI
    }

    return( OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-01-21

QString MainWindow::createReferenceOutputString( const QString &s_DatasetID, const QString &s_ReferenceNumber, const QStringList &sl_Authors, const QString &s_Reference )
{
    QString OutputStr = s_DatasetID + "\t" + "@reference@" + "\t" + s_ReferenceNumber + "\t" + s_Reference.section( "\t", 0, 0 ) + "\t" + s_Reference.section( "\t", 1, 1 ) + "\t";

    if ( sl_Authors.count() > 0 )
    {
        OutputStr.append( sl_Authors.at( 0 ).section( "\t", 1, 1 ) + ", " + sl_Authors.at( 0 ).section( "\t", 2, 2 ) );

        for ( int i=1; i<sl_Authors.count(); i++ )
            OutputStr.append( "; " + sl_Authors.at( i ).section( "\t", 1, 1 ) + ", " + sl_Authors.at( i ).section( "\t", 2, 2 ) );
    }

    if ( s_Reference.isEmpty() == false )
    {
        OutputStr.append( "\t" + s_Reference.section( "\t", 2, 2 ) ); // year
        OutputStr.append( "\t" + s_Reference.section( "\t", 3, 3 ) ); // title
        OutputStr.append( "\t" + s_Reference.section( "\t", 4, 4 ) ); // source
        OutputStr.append( "\t" + s_Reference.section( "\t", 5, 5 ) ); // volume
        OutputStr.append( "\t" + s_Reference.section( "\t", 6, 6 ) ); // pages
        OutputStr.append( "\t" + s_Reference.section( "\t", 7, 7 ) ); // URI
    }

    return( OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

int MainWindow::parseMetadataXML( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecOutput, const int i_EOL,
                                  const bool b_Citation, const bool b_Authors, const bool b_References, const bool b_Projects, const bool b_Events,
                                  const bool b_Parameters, const bool b_Coverage, const bool b_Keywords, const bool b_TechnicalInfo)
{
    QString s_error					= "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );
    QFileInfo       fi( s_FilenameIn );

    QDomDocument    doc;

    QString      s_DatasetID = "";
    QString      s_Reference = "";
    QString      s_Citation  = "";

    QStringList  sl_Authors;

// **********************************************************************************************
// read file

    if ( fi.suffix().toLower() != "xml" )
        return( 0 );

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

    QDomNode Citation = root.firstChildElement( "citation" );

    s_DatasetID = getAttributeValue( Citation, "id" );

    tout << "METADATA of data set https://doi.pangaea.de/10.1594/PANGAEA." << s_DatasetID.section( "dataset", 1, 1 ) << s_EOL;

    for ( int i=0; i<Citation.childNodes().count(); i++ )
    {
        if ( Citation.childNodes().at( i ).localName() == "author" )
            sl_Authors.append( getStaffEntry( Citation.childNodes().at( i ) ) );
    }

    s_Citation = getCitationEntry( Citation );

// **********************************************************************************************

    if ( b_Authors == true )
    {
        tout << s_EOL << "********************************************" << s_EOL << s_EOL;

        if ( sl_Authors.count() > 1 )
            tout << "AUTHORS" << s_EOL;
        else
            tout << "AUTHOR" << s_EOL;

        if ( sl_Authors.count() > 0 )
        {
            tout << "Data set ID" << "\t" << "Type" << "\t" << "Author ID" << "\t" << "Name" << "\t" << "Given name" << "\t";
            tout << "e-Mail" << "\t" << "Phone" << "\t" << "ORCID" << "\t" << "URI" << s_EOL;

            for ( int i=0; i<sl_Authors.count(); i++ )
                tout << s_DatasetID << "\t" << "@author@" << "\t" << sl_Authors.at( i ) << s_EOL;
        }
    }

// **********************************************************************************************

    if ( b_Citation == true )
    {
        tout << s_EOL << "********************************************" << s_EOL << s_EOL;

        tout << "CITATION" << s_EOL;

        tout << "Data set ID" << "\t" << "Type" << "\t" << "Authors" << "\t" << "Year" << "\t" << "Title" << "\t" << "Source" << "\t" << "URI" << s_EOL;

        tout << createCitationOutputString( sl_Authors, s_Citation ) << s_EOL;
    }

// **********************************************************************************************

    if ( b_References == true )
    {
        QDomNodeList ReferenceList     = root.elementsByTagName( "reference" );

        QString      s_ReferenceID     = "";
        QString      s_ReferenceType   = "";
        QString      s_ReferenceNumber = "";

        if ( ReferenceList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            if ( ReferenceList.count() > 1 )
                tout << "REFERENCES";
            else
                tout << "REFERENCE";

            for ( int i=0; i<ReferenceList.count(); i++ )
            {
                QDomNode Reference = ReferenceList.at( i );

                sl_Authors.clear();

                s_ReferenceNumber = QString( "Reference %1" ).arg( i+1 );
                s_ReferenceID     = getAttributeValue( Reference, "id" );
                s_ReferenceType   = getAttributeValue( Reference, "relationType" );

                for ( int j=0; j<Reference.childNodes().count(); j++ )
                {
                    if ( Reference.childNodes().at( j ).localName() == "author" )
                        sl_Authors.append( getStaffEntry( Reference.childNodes().at( j ) ) );
                }

                s_Reference = getReferenceEntry( Reference );

                if ( sl_Authors.count() > 0 )
                {
                    tout << s_EOL;
                    tout << "Data set ID" << "\t" << "Type" << "\t" << "Reference number" << "\t" << "Reference ID" << "\t";
                    tout << "Author ID" << "\t" << "Name" << "\t" << "Given name" << "\t";
                    tout << "e-Mail" << "\t" << "Phone" << "\t" << "ORCID" << "\t" << "URI" << s_EOL;

                    for ( int i=0; i<sl_Authors.count(); i++ )
                        tout << s_DatasetID << "\t" << "@author@" << "\t" << s_ReferenceNumber << "\t" << s_ReferenceID << "\t" << sl_Authors.at( i ) << s_EOL;
                }

                tout << s_EOL;
                tout << "Data set ID" << "\t" << "Type" << "\t" << "Reference number" << "\t" << "Reference ID" << "\t";
                tout << "Reference type" << "\t" << "Authors" << "\t" << "Year" << "\t" << "Title" << "\t";
                tout << "Journal or source" << "\t" << "Volume" << "\t" << "Pages" << "\t" << "URI" << s_EOL;

                tout << createReferenceOutputString( s_DatasetID, s_ReferenceNumber, sl_Authors, s_Reference ) << s_EOL;

                if ( ( ReferenceList.count() > 1 ) && ( i < ReferenceList.count() - 1 ) )
                    tout << s_EOL << "--------------------------------------------" << s_EOL;
            }
        }
    }

// **********************************************************************************************

    if ( b_Projects == true )
    {
        QDomNodeList ProjectList = root.elementsByTagName( "project" );

        if ( ProjectList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            if ( ProjectList.count() > 1 )
                tout << "PROJECTS" << s_EOL;
            else
                tout << "PROJECT" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Project ID" << "\t" << "Project label" << "\t";
            tout << "Project name" << "\t" << "URI" << s_EOL;

            for ( int i=0; i<ProjectList.count(); i++ )
                tout << s_DatasetID << "\t" << "@project@" << "\t" << getProjectEntry( ProjectList.at( i ) ) << s_EOL;
        }
    }

// **********************************************************************************************

    if ( b_Coverage == true )
    {
        QDomNodeList CoverageList = root.elementsByTagName( "extent" );

        if ( CoverageList.count() > 0 )
        {
            QDomNode Coverage = CoverageList.at( 0 );

            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            tout << "COVERAGE" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Geocode ID" << "\t" << "Local node name" << "\t" << "Content" << s_EOL;

            if ( Coverage.firstChildElement( "topoType" ).isNull() == false )
                tout << s_DatasetID << "\t" << "@coverage@" << "\t" << "\t" << "topoType" << "\t" << getNodeValue( Coverage.firstChildElement( "topoType" ) ) << s_EOL;

            if ( Coverage.firstChildElement( "geographic" ).isNull() == false )
            {
                QDomNode Geographic = Coverage.firstChildElement( "geographic" );

                for( int j=0; j<Geographic.childNodes().count(); j++ )
                    tout << s_DatasetID << "\t" << "@coverage@" << "\t" << "\t" << Geographic.childNodes().at( j ).localName() << "\t" << QString( "%1" ).arg( getNodeValue( Geographic.childNodes().at( j ) ).toDouble(), 0, 'f', 5 ) << s_EOL;
            }

            for ( int i=0; i<Coverage.childNodes().count(); i++ )
            {
                if ( Coverage.childNodes().at( i ).localName() == "temporal" )
                {
                    QDomNode Temporal = Coverage.childNodes().at( i );

                    for( int j=0; j<Temporal.childNodes().count(); j++ )
                        tout << s_DatasetID << "\t" << "@coverage@" << "\t" << "\t" << Temporal.childNodes().at( j ).localName() << "\t" << getNodeValue( Temporal.childNodes().at( j ) ) << s_EOL;
                }
            }

            for ( int i=0; i<Coverage.childNodes().count(); i++ )
            {
                if ( Coverage.childNodes().at( i ).localName() == "elevation" )
                {
                    QDomNode Elevation = Coverage.childNodes().at( i );

                    QString s_GeocodeID = getAttributeValue( Elevation, "geocodeId" );
                    QString s_Parameter = getAttributeValue( Elevation, "name" );
                    QString s_Unit      = getAttributeValue( Elevation, "unit" );;

                    if ( s_Unit.isEmpty() == false )
                        s_Unit = " [" + s_Unit + "]";

                    for( int j=0; j<Elevation.childNodes().count(); j++ )
                    {
                        if ( s_Unit.isEmpty() == false )
                            tout << s_DatasetID << "\t" << "@coverage@" << "\t" << s_GeocodeID << "\t" << s_Parameter << ", " << Elevation.childNodes().at( j ).localName() << s_Unit << "\t" << getNodeValue( Elevation.childNodes().at( j ) ) << s_EOL;
                        else
                            tout << s_DatasetID << "\t" << "@coverage@" << "\t" << s_GeocodeID << "\t" << s_Parameter << ", " << Elevation.childNodes().at( j ).localName() << "\t" << getNodeValue( Elevation.childNodes().at( j ) ) << s_EOL;
                    }
                }
            }
        }
    }

// **********************************************************************************************

    if ( b_Events == true )
    {
        QString s_EventID = "";

        QDomNodeList EventList = root.elementsByTagName( "event" );

        if ( EventList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            if ( EventList.count() > 1 )
                tout << "EVENTS" << s_EOL;
            else
                tout << "EVENT" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Event ID" << "\t" << "Event label" << "\t";
            tout << "Device" << "\t" << "Device" << "\t" << "URI device" << "\t" << "Penetration [m]" << "\t";
            tout << "Recovery [m]" << "\t" << "Date/Time" << "\t" << "Latitude" << "\t" << "Longitude" << "\t" ;
            tout << "Elevation" << "\t" << "Lake water depth [m]" << "\t";
            tout << "Location" << "\t" << "URI location" << "\t" << "Campaign" << "\t" << "Optional label" << "\t";
            tout << "URI campaign" << "\t" << "Basis" << "\t" << "URI basis" << s_EOL;

            for ( int i=0; i<EventList.count(); i++ )
            {
                QDomNode Event            = EventList.at( i );

                QString  s_AttributeValue = "";
                QString  s_Penetration    = "";
                QString  s_Recovery       = "";
                QString  s_LakeWaterDepth = "";

                bool     b_hasDevice      = false;
                bool     b_hasCampaign    = false;
                bool     b_hasBasis       = false;
                bool     b_hasLocation    = false;

                s_EventID = getAttributeValue( Event, "id" ).replace( "event", "" );

                tout << s_DatasetID << "\t" << "@event@" << "\t" << s_EventID << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "label" )
                        tout << getNodeValue( Event.childNodes().at( j ) );
                }

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "device" )
                    {
                        b_hasDevice = true;

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "name" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "optionalName" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "URI" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";
                    }
                }

                if ( b_hasDevice == false )
                    tout << "\t\t\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "attribute" )
                    {
                        s_AttributeValue = getAttributeValue( Event.childNodes().at( j ), "name" );

                        if ( s_AttributeValue == "Penetration" )
                            s_Penetration = getNodeValue( Event.childNodes().at( j ) ).replace( " m", "" );

                        if ( s_AttributeValue == "Recovery" )
                            s_Recovery = getNodeValue( Event.childNodes().at( j ) ).replace( " m", "" );

                        if ( s_AttributeValue == "Lake water depth" )
                            s_LakeWaterDepth = getNodeValue( Event.childNodes().at( j ) ).replace( " m", "" );
                    }
                }

                if ( s_Penetration.isEmpty() == false )
                    tout << s_Penetration;

                tout << "\t";

                if ( s_Recovery.isEmpty() == false )
                    tout << s_Recovery;

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "dateTime" )
                        tout << getNodeValue( Event.childNodes().at( j ) );
                }

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "latitude" )
                        tout << getNodeValue( Event.childNodes().at( j ) );
                }

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "longitude" )
                        tout << getNodeValue( Event.childNodes().at( j ) );
                }

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "elevation" )
                        tout << getNodeValue( Event.childNodes().at( j ) );
                }

                tout << "\t";

                if ( s_LakeWaterDepth.isEmpty() == false )
                    tout << s_LakeWaterDepth;

                tout << "\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "location" )
                    {
                        b_hasLocation = true;

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "name" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "URI" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";
                    }
                }

                if ( b_hasLocation == false )
                    tout << "\t\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "campaign" )
                    {
                        b_hasCampaign = true;

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "name" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "optionalName" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "URI" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";
                    }
                }

                if ( b_hasCampaign == false )
                    tout << "\t\t\t";

                for ( int j=0; j<Event.childNodes().count(); j++ )
                {
                    if ( Event.childNodes().at( j ).localName() == "basis" )
                    {
                        b_hasBasis = true;

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "name" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";

                        for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        {
                            if ( Event.childNodes().at( j ).childNodes().at( k ).localName() == "URI" )
                                tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) );
                        }

                        tout << "\t";
                    }
                }

                if ( b_hasBasis == false )
                    tout << "\t\t";

                tout << s_EOL;
            }
        }
    }

// **********************************************************************************************
// 2016-11-30 - Schön Klinik Hamburg

    if ( b_Parameters == true )
    {
        QDomNodeList MatrixColumnList = root.elementsByTagName( "matrixColumn" );

        if ( MatrixColumnList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            if ( MatrixColumnList.count() > 1 )
                tout << "PARAMETERS" << s_EOL;
            else
                tout << "PARAMETER" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Column" << "\t" << "Parameter ID" << "\t";
            tout << "Parameter" << "\t" << "Format" << "\t" << "Method" << "\t" << "Comment" << "\t" << "PI" << s_EOL;

            for ( int i=0; i<MatrixColumnList.count(); i++ )
            {
                QDomNode MatrixColumn = MatrixColumnList.at( i );

                tout << s_DatasetID << "\t" << "@parameter@";

                for ( int j=0; j<MatrixColumn.childNodes().count(); j++ )
                {
                    if ( MatrixColumn.childNodes().at( j ).localName() == "parameter" )
                    {
                        QDomNode Parameter = MatrixColumn.childNodes().at( j );
                        QString  s_ID      = getAttributeValue( Parameter, "id" );

                        tout << "\t" << s_ID.section( ".", 0, 0 ).section( "col", 1, 1 ) << "\t" << s_ID.section( ".", 2, 2 );

                        for ( int k=0; k<Parameter.childNodes().count(); k++ )
                        {
                            if ( Parameter.childNodes().at( k ).localName() == "name" )
                                tout << "\t" << getNodeValue( Parameter.childNodes().at( k ) );
                        }

                        for ( int k=0; k<Parameter.childNodes().count(); k++ )
                        {
                            if ( Parameter.childNodes().at( k ).localName() == "unit" )
                                tout << " [" << getNodeValue( Parameter.childNodes().at( k ) ) << "]";
                        }

                        tout << "\t" << getAttributeValue( MatrixColumn, "format" ) << "\t";

                        for ( int j=0; j<MatrixColumn.childNodes().count(); j++ )
                        {
                            if ( MatrixColumn.childNodes().at( j ).localName() == "method" )
                            {
                                for ( int k=0; k<MatrixColumn.childNodes().at( j ).childNodes().count(); k++ )
                                {
                                    if ( MatrixColumn.childNodes().at( j ).childNodes().at( k ).localName() == "name" )
                                       tout << getNodeValue( MatrixColumn.childNodes().at( j ).childNodes().at( k ) );
                                }
                            }
                        }

                        tout << "\t";

                        for ( int j=0; j<MatrixColumn.childNodes().count(); j++ )
                        {
                            if ( MatrixColumn.childNodes().at( j ).localName() == "comment" )
                                tout << getNodeValue( MatrixColumn.childNodes().at( j ) );
                        }

                        tout << "\t";

                        for ( int j=0; j<MatrixColumn.childNodes().count(); j++ )
                        {
                            if ( MatrixColumn.childNodes().at( j ).localName() == "PI" )
                            {
                                QString s_PI = getStaffEntry( MatrixColumn.childNodes().at( j ) );

                                tout << s_PI.section( "\t", 1, 1 ) << ", " <<  s_PI.section( "\t", 2, 2 );
                            }
                        }

                        tout << s_EOL;
                    }
                }
            }
        }
    }

// **********************************************************************************************

    if ( b_Keywords == true )
    {
        QDomNodeList KeywordList = root.elementsByTagName( "keywords" );

        if ( KeywordList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            tout << "KEYWORDS" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Keyword type" << "\t" << "Value" << s_EOL;

            QDomNode Keyword = KeywordList.at( 0 );

            for ( int i=0; i<Keyword.childNodes().count(); i++ )
            {
                if ( Keyword.childNodes().at( i ).localName() == "techKeyword" )
                    tout << s_DatasetID << "\t" << "@keyword@" << "\t" << getAttributeValue( Keyword.childNodes().at( i ), "type" ) << "\t" << getNodeValue( Keyword.childNodes().at( i ) ) << s_EOL;
            }
        }
    }

// **********************************************************************************************

    if ( b_TechnicalInfo == true )
    {
        QDomNodeList technicalInfoList = root.elementsByTagName( "technicalInfo" );

        if ( technicalInfoList.count() > 0 )
        {
            tout << s_EOL << "********************************************" << s_EOL << s_EOL;

            tout << "TECHNICAL INFO" << s_EOL;

            tout << "Data set ID" << "\t" << "Type" << "\t" << "Technical info type" << "\t" << "Value" << s_EOL;

            QDomNode technicalInfo = technicalInfoList.at( 0 );

            for ( int i=0; i<technicalInfo.childNodes().count(); i++ )
            {
                if ( technicalInfo.childNodes().at( i ).localName() == "entry" )
                    tout << s_DatasetID << "\t" << "@technicalInfo@" << "\t" << getAttributeValue( technicalInfo.childNodes().at( i ), "key" ) << "\t" << getAttributeValue( technicalInfo.childNodes().at( i ), "value" ) << s_EOL;
            }
        }
    }

// **********************************************************************************************

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

    QStringList sl_FilenameList;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doMetadataXMLDialog( gb_xml_Citation, gb_xml_Authors, gb_xml_References, gb_xml_Projects, gb_xml_Events, gb_xml_Parameters, gb_xml_Coverage, gb_xml_Keywords, gb_xml_TechnicalInfo ) == QDialog::Accepted )
        {
            setWaitCursor();

            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert PANGAEA metadata XML files..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                QFileInfo fi( gsl_FilenameList.at( i ) );

                if ( fi.suffix().toLower() == "xml" )
                {
                    s_FilenameIn  = fi.absoluteFilePath();
                    s_FilenameOut = fi.absolutePath() + "/" + fi.completeBaseName() + setExtension( gi_Extension );

                    err = parseMetadataXML( s_FilenameIn, s_FilenameOut, gi_CodecOutput, gi_EOL, gb_xml_Citation, gb_xml_Authors, gb_xml_References, gb_xml_Projects, gb_xml_Events, gb_xml_Parameters, gb_xml_Coverage, gb_xml_Keywords, gb_xml_TechnicalInfo );

                    if ( err == _NOERROR_ )
                        sl_FilenameList.append( s_FilenameOut );

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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    gsl_FilenameList = sl_FilenameList;
    gi_ActionNumber  = 0;

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting PANGAEA metadata XML was canceled" ), false, true );

    onError( err );
}

