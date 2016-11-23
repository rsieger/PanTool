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
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "year" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "title" ) ) );
    sl_ReferenceEntry.append( getNodeValue( Node.firstChildElement( "source" ) ) );
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
// 2011-07-12

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
        OutputStr.append( "\t" + s_Reference.section( "\t", 5, 5 ) ); // URI
    }

    return( OutputStr );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

int MainWindow::parseMetadataXML( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecOutput, const int i_EOL )
{
    QString s_error					= "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );

    QDomDocument    doc;

    QString      s_DatasetID = "";
    QString      s_Reference = "";
    QString      s_Citation  = "";

    QStringList  sl_Authors;

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

    QDomNode Citation = root.firstChildElement( "citation" );

    s_DatasetID = getAttributeValue( Citation, "id" );

    for ( int i=0; i<Citation.childNodes().count(); i++ )
    {
        if ( Citation.childNodes().at( i ).localName() == "author" )
            sl_Authors.append( getStaffEntry( Citation.childNodes().at( i ) ) );
    }

    s_Citation = getCitationEntry( Citation );

// **********************************************************************************************

    tout << "CITATION" << s_EOL;

    if ( sl_Authors.count() > 0 )
    {
        tout << "Data set ID" << "\t" << "Type" << "\t" << "Author ID" << "\t" << "Name" << "\t" << "Given name" << "\t";
        tout << "e-Mail" << "\t" << "Phone" << "\t" << "ORCID" << "\t" << "URI" << s_EOL;

        for ( int i=0; i<sl_Authors.count(); i++ )
            tout << s_DatasetID << "\t" << "@author@" << "\t" << sl_Authors.at( i ) << s_EOL;
    }

// **********************************************************************************************

    tout << s_EOL;
    tout << "Data set ID" << "\t" << "Type" << "\t" << "Authors" << "\t" << "Year" << "\t" << "Title" << "\t" << "Source" << "\t" << "URI" << s_EOL;

    tout << createCitationOutputString( sl_Authors, s_Citation ) << s_EOL;

    tout << s_EOL << "********************************************" << s_EOL;

// **********************************************************************************************

    QDomNodeList ReferenceList     = root.elementsByTagName( "reference" );

    QString      s_ReferenceID     = "";
    QString      s_ReferenceType   = "";
    QString      s_ReferenceNumber = "";

    if ( ReferenceList.count() > 0 )
    {
        tout << s_EOL;

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
            tout << "Reference type" << "\t" << "Authors" << "\t" << "Year" << "\t" << "Title" << "\t" << "Journal or source" << "\t" << "URI" << s_EOL;

            tout << createReferenceOutputString( s_DatasetID, s_ReferenceNumber, sl_Authors, s_Reference ) << s_EOL;

            if ( ( ReferenceList.count() > 1 ) && ( i < ReferenceList.count() - 1 ) )
                tout << s_EOL << "--------------------------------------------" << s_EOL;
        }

        tout << s_EOL << "********************************************" << s_EOL;
    }

// **********************************************************************************************

    QDomNodeList ProjectList = root.elementsByTagName( "project" );

    if ( ProjectList.count() > 0 )
    {
        tout << s_EOL;

        if ( ProjectList.count() > 1 )
            tout << "PROJECTS" << s_EOL;
        else
            tout << "PROJECT" << s_EOL;

        tout << "Data set ID" << "\t" << "Type" << "\t" << "Project ID" << "\t" << "Project label" << "\t";
        tout << "Project name" << "\t" << "URI" << s_EOL;

        for ( int i=0; i<ProjectList.count(); i++ )
            tout << s_DatasetID << "\t" << "@project@" << "\t" << getProjectEntry( ProjectList.at( i ) ) << s_EOL;

        tout << s_EOL << "********************************************" << s_EOL;
    }

// **********************************************************************************************

    QDomNodeList CoverageList = root.elementsByTagName( "extent" );

    if ( CoverageList.count() > 0 )
    {
        QDomNode Coverage = CoverageList.at( 0 );

        tout << s_EOL;
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

        tout << s_EOL << "********************************************" << s_EOL;
    }

// **********************************************************************************************

    QString s_EventID = "";

    QDomNodeList EventList = root.elementsByTagName( "event" );

    if ( EventList.count() > 0 )
    {
        tout << s_EOL;

        if ( EventList.count() > 1 )
            tout << "EVENTS" << s_EOL;
        else
            tout << "EVENT" << s_EOL;

        tout << "Data set ID" << "\t" << "Type" << "\t" << "Event ID" << "\t" << "Event label" << "\t";
        tout << "Device" << "\t" << "Device" << "\t" << "Date/Time" << "\t";
        tout << "Latitude" << "\t" << "Longitude" << "\t" << "Elevation";
        tout << "\t" << "Location" << "\t";
        tout << "Campaign" << "\t" << "Optional label" << "\t" << "URI campaign" << "\t";
        tout << "Basis" << "\t" << "URI basis" << s_EOL;

        for ( int i=0; i<EventList.count(); i++ )
        {
            QDomNode Event           = EventList.at( i );

            bool     b_hasCampaign   = false;
            bool     b_hasBasis      = false;

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
                    for ( int k=0; k<Event.childNodes().at( j ).childNodes().count(); k++ )
                        tout << getNodeValue( Event.childNodes().at( j ).childNodes().at( k ) ) << "\t";
                }
            }

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

            for ( int j=0; j<Event.childNodes().count(); j++ )
            {
                if ( Event.childNodes().at( j ).localName() == "location" )
                    tout << getNodeValue( Event.childNodes().at( j ) );
            }

            tout << "\t";

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

        tout << s_EOL << "********************************************" << s_EOL;
    }

// **********************************************************************************************

    QDomNodeList KeywordList = root.elementsByTagName( "keywords" );

    if ( KeywordList.count() > 0 )
    {
        tout << s_EOL;
        tout << "KEYWORDS" << s_EOL;

        tout << "Data set ID" << "\t" << "Type" << "\t" << "Keyword type" << "\t" << "Value" << s_EOL;

        QDomNode Keyword = KeywordList.at( 0 );

        for ( int i=0; i<Keyword.childNodes().count(); i++ )
        {
            if ( Keyword.childNodes().at( i ).localName() == "techKeyword" )
                tout << s_DatasetID << "\t" << "@keyword@" << "\t" << getAttributeValue( Keyword.childNodes().at( i ), "type" ) << "\t" << getNodeValue( Keyword.childNodes().at( i ) ) << s_EOL;
        }

        tout << s_EOL << "********************************************" << s_EOL;
    }

// **********************************************************************************************

    QDomNodeList technicalInfoList = root.elementsByTagName( "technicalInfo" );

    if ( technicalInfoList.count() > 0 )
    {
        tout << s_EOL;
        tout << "TECHNICAL INFO" << s_EOL;

        tout << "Data set ID" << "\t" << "Type" << "\t" << "Technical info type" << "\t" << "Value" << s_EOL;

        QDomNode technicalInfo = technicalInfoList.at( 0 );

        for ( int i=0; i<technicalInfo.childNodes().count(); i++ )
        {
            if ( technicalInfo.childNodes().at( i ).localName() == "entry" )
                tout << s_DatasetID << "\t" << "@technicalInfo@" << "\t" << getAttributeValue( technicalInfo.childNodes().at( i ), "key" ) << "\t" << getAttributeValue( technicalInfo.childNodes().at( i ), "value" ) << s_EOL;
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

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        setWaitCursor();

        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert PANGAEA metadata XML files..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = parseMetadataXML( s_FilenameIn, s_FilenameOut, gi_CodecOutput, gi_EOL );

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

