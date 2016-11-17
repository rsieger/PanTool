/* 2016-11-16                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-12

int MainWindow::parseMetadataXML( const QString &s_FilenameIn, const int i_CodecOutput, const int i_EOL, const int i_Extension, const int i_NumOfFiles )
{
    QString s_error					= "";
    QString s_EOL                   = setEOLChar( i_EOL );

    int		i_error					= 0;
    int		i_errorLine				= 0;
    int		i_errorColumn			= 0;

    QFile           fin( s_FilenameIn );

    QDomDocument    doc;

// **********************************************************************************************
// read file

    if ( doc.setContent( &fin, true, &s_error, &i_errorLine, &i_errorColumn ) == false )
        i_error = -1;

    if ( i_error == 0 )
    {
        QDomElement root = doc.documentElement();

        if ( root.tagName() != "MetaData" )
            i_error = -2;

        if ( i_error == 0 )
        {
            QDomNode Citation = root.firstChildElement( "citation" );

            for ( int i=0; i<Citation.attributes().count(); i++ )
                qDebug() << Citation.attributes().item( i ).toAttr().value();

            for ( int i=0; i<Citation.childNodes().count(); i++ )
            {
                if ( Citation.childNodes().at( i ).localName() == "author" )
                {
                    qDebug() << Citation.childNodes().at( i ).attributes().item( 0 ).toAttr().value();

                    for( int j=0; j<Citation.childNodes().at( i ).childNodes().count(); j++ )
                    {
                        qDebug() << Citation.childNodes().at( i ).childNodes().at( j ).localName() << ": " << Citation.childNodes().at( i ).childNodes().at( j ).toElement().text();
                    }
                }

                if ( Citation.childNodes().at( i ).localName() == "year" )
                    qDebug() << Citation.childNodes().at( i ).localName() << ": " << Citation.childNodes().at( i ).toElement().text();

                if ( Citation.childNodes().at( i ).localName() == "title" )
                    qDebug() << Citation.childNodes().at( i ).localName() << ": " << Citation.childNodes().at( i ).toElement().text();

                if ( Citation.childNodes().at( i ).localName() == "URI" )
                    qDebug() << Citation.childNodes().at( i ).localName() << ": " << Citation.childNodes().at( i ).toElement().text();

                if ( Citation.childNodes().at( i ).localName() == "parentURI" )
                    qDebug() << Citation.childNodes().at( i ).localName() << ": " << Citation.childNodes().at( i ).toElement().text();

                Citation.nextSibling();
            }

            qDebug() << root.elementsByTagName( "reference" ).count();

            QDomNodeList Reference = root.elementsByTagName( "reference" );

            for ( int i=0; i<Reference.count(); i++ )
            {
                for ( int j=0; j<Reference.at( i ).attributes().count(); j++ )
                    qDebug() << Reference.at( i ).attributes().item( j ).toAttr().value();
            }

/*
            i_error = parseTextEntry( MetadataObject.firstChildElement( "citation"), "id", s_AttributeContent, s_Content );

            qDebug() << s_AttributeContent;
            qDebug() << s_Content;
*/
        }
    }

    return( _NOERROR_ );
}

/*
            QDomNode    Project  = root.firstChild();

            i_error = parseTextEntry( Project.firstChildElement( "title" ), s_ProjectTitle );
            i_error = parseTextEntry( Project.firstChildElement( "identifier" ), s_Event );
            i_error = parseTextEntry( Project.firstChildElement( "category" ), s_Category );
            i_error = parseTextEntry( Project.firstChildElement( "investigator" ), s_PI );
            i_error = parseTextEntry( Project.firstChildElement( "period" ), s_Date );

            QDomElement Object   = Project.firstChildElement( "object" );
            QDomElement Location = Object.firstChildElement( "location" );

            i_error = parseTextEntry( Object.firstChildElement( "title" ), s_ObjectTitle );
            i_error = parseTextEntry( Object.firstChildElement( "type" ), s_ObjectType );

            QDomElement Position = Location.firstChildElement( "locationGeometry" ).firstChildElement( "Point" );

            i_error = parseTextEntry( Position.firstChildElement( "pos" ), s_Position );

            QDomElement Address  = Location.firstChildElement( "address" );

            i_error = parseTextEntry( Address.firstChildElement( "cityOrTown" ), s_Town );
            i_error = parseTextEntry( Address.firstChildElement( "country" ), s_Country );
*/
// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-05

int MainWindow::parseValueEntry( const QDomElement &element )
{
    int i_Value = -1;

    if ( element.attribute( "value" ).isEmpty() == false )
        i_Value = element.attribute( "value" ).toInt();

    return ( i_Value );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseNumericEntry( const QDomElement &element, int &i_Content )
{
    if ( element.text().isEmpty() == true )
        return( _ERROR_ );

    i_Content = element.text().toInt();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseNumericEntry( const QDomElement &element, const QString &s_Attribute, QString &s_AttributeContent, int &i_Content )
{
    if ( ( element.attribute( s_Attribute ).isEmpty() == true ) && ( element.text().isEmpty() == true ) )
        return( _ERROR_ );

    s_AttributeContent = element.attribute( s_Attribute );
    i_Content          = element.text().toInt();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseTextEntry( const QDomElement &element, QString &s_Content )
{
    if ( element.text().isEmpty() == true )
        return( _ERROR_ );

    s_Content = element.text();

    return ( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-04

int MainWindow::parseTextEntry( const QDomElement &element, const QString &s_Attribute, QString &s_AttributeContent, QString &s_Content )
{
    int err = _NOERROR_;

    if ( ( element.attribute( s_Attribute ).isEmpty() == true ) && ( element.text().isEmpty() == true ) )
        return( _ERROR_ );

    s_AttributeContent = element.attribute( s_Attribute );
    s_Content          = element.text();

    return ( err );
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
                err = parseMetadataXML( s_FilenameIn, gi_CodecOutput, gi_EOL,  gi_Extension, gsl_FilenameList.count() );

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

