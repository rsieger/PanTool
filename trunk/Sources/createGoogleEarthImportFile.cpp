// ***********************************************************************************************
// *                                                                                             *
// * createGoogleEarthImportFile.cpp - Create a Google Earth import file (KML)                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

/*! @brief Liest Zeilen aus einer Datei, verarbeitet sie und schreibt sie in eine neue Datei. */

int MainWindow::createGoogleEarthImportFile( const QString &s_FilenameIn, const QString &s_FilenameOut,
                                             const bool b_displayEventLabel, const bool b_displayDescription,
                                             const int i_IconSize, const int i_IconColor, const int i_IconSymbol )
{
    int             n                       = 0;
    int             err                     = _NOERROR_;

    float           f_IconSize              = (float) i_IconSize/100.;

    QStringList     sl_InputStr;

//-----------------------------------------------------------------------------------------------------------------------

    if ( ( n = readFile( s_FilenameIn, sl_InputStr, _SYSTEM_ ) ) < 1 )
        return( _ERROR_ );

//-----------------------------------------------------------------------------------------------------------------------

    QFile fkml( s_FilenameOut );

    if ( fkml.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

//-----------------------------------------------------------------------------------------------------------------------

    err = openKMLFile( fkml );

    for ( int i=1; i< n; i++ )
        err = writeKMLEntry( fkml, sl_InputStr.at( i ), b_displayEventLabel, b_displayDescription, f_IconSize, i_IconColor, i_IconSymbol );

    err = closeKMLFile( fkml );

//-----------------------------------------------------------------------------------------------------------------------

    fkml.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::openKMLFile( QFile& fkml )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    tkml << "<kml xmlns=\"http://earth.google.com/kml/2.1\">" << endl;
    tkml << "<Document>" << endl;
    tkml << "  <ScreenOverlay>" << endl;
    tkml << "    <name>Pan2Applic logo</name>" << endl;
    tkml << "    <Icon><href>http://epic.awi.de/40953/121/Pan2Applic_logo.png</href></Icon>" << endl;
    tkml << "    <overlayXY x=\"0\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>" << endl;
    tkml << "    <screenXY x=\"5\" y=\"5\" xunits=\"pixels\" yunits=\"insetPixels\"/>" << endl;
    tkml << "    <size x=\"200\" y=\"88\" xunits=\"pixel\" yunits=\"pixel\"/>" << endl;
    tkml << "  </ScreenOverlay>" << endl;
    tkml << "  <Style id=\"circle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/1/circle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/2/circle-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/3/circle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/4/circle-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/5/circle-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"circle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/6/circle-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/7/square-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/8/square-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/9/square-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/10/square-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/11/square-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"square-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/12/square-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/13/star-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/14/star-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/15/star-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/16/star-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/17/star-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"star-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/18/star-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/19/triangle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/20/triangle-green.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/21/triangle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/22/triangle-red.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/23/triangle-white.png</href></Icon></IconStyle></Style>" << endl;
    tkml << "  <Style id=\"triangle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/24/triangle-yellow.png</href></Icon></IconStyle></Style>" << endl;
//  tkml << "  <name>Document name</name>" << endl;
//  tkml << "  <description>Document description</description>" << endl;

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::closeKMLFile( QFile& fkml )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "</Document>\n";
    tkml << "</kml>\n";

    return( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::setIconSymbol( const int i_IconSymbol )
{
    QString s_IconSymbol    = "cricle";

    switch ( i_IconSymbol )
    {
        case _CIRCLE:
            s_IconSymbol = "circle";
            break;

        case _STAR:
            s_IconSymbol = "star";
            break;

        case _SQUARE:
            s_IconSymbol = "square";
            break;

        case _TRIANGLE:
            s_IconSymbol = "triangle";
            break;

        default:
            s_IconSymbol = "circle";
            break;
    }

    return( s_IconSymbol );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QString MainWindow::setIconColor( const int i_IconColor )
{
    QString s_IconColor     = "red";

    switch ( i_IconColor )
    {
        case _RED:
            s_IconColor = "red";
            break;

        case _GREEN:
            s_IconColor = "green";
            break;

        case _BLUE:
            s_IconColor = "blue";
            break;

        case _ORANGE:
            s_IconColor = "orange";
            break;

        case _YELLOW:
            s_IconColor = "yellow";
            break;

        case _WHITE:
            s_IconColor = "white";
            break;

        default:
            s_IconColor = "red";
            break;
    }

    return( s_IconColor );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::writeKMLEntry( QFile& fkml, const QString &InputStr, const bool b_displayEventLabel, const bool b_displayDescription, const float f_IconSize, const int i_IconColor, const int i_IconSymbol )
{
    QString s_EventLabel    = InputStr.section( "\t", _EVENTLABELPOS, _EVENTLABELPOS );         // Event label
    QString s_DateTime      = InputStr.section( "\t", _DATETIMEPOS, _DATETIMEPOS );             // Date/Time

    double d_Latitude      = InputStr.section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();   // Latitude
    double d_Longitude     = InputStr.section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble(); // Longitude
    double d_Altitude      = InputStr.section( "\t", _ALTITUDEPOS, _ALTITUDEPOS ).toDouble();   // Altitude

// **********************************************************************************************

    if ( d_Longitude > 180 )
        d_Longitude -= 360;

    QString s_Longitude = QString( "%1" ).arg( d_Longitude, 0, 'f', 5 );
    QString s_Latitude  = QString( "%1" ).arg( d_Latitude, 0, 'f', 5 );
    QString s_Altitude  = QString( "%1" ).arg( d_Altitude, 0, 'f', 1 );

// **********************************************************************************************

    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "  <Placemark>";

    if ( b_displayEventLabel == true )
        tkml << "<name>" << s_EventLabel << "</name>";

    if ( b_displayDescription == true )
    {
        tkml << "<description>";
        tkml << "Event: " << s_EventLabel << " * ";
        if ( s_DateTime.contains( ":" ) == true )
            tkml << "Date/Time: " << s_DateTime << " * ";
        else
            tkml << "Date: " << s_DateTime << " * ";
        tkml << "Latitude: " << s_Latitude << " * ";
        tkml << "Longitude: " << s_Longitude << " * ";
        tkml << "Altitude: " << s_Altitude;
        tkml << "<br /><![CDATA[<a href=\"";
        tkml << "https://pangaea.de/search?q=" << s_EventLabel;
        tkml << "\">search data</a>]]><br />";
        tkml << "</description>";
    }

    tkml << "<View>";
    tkml << "<longitude>" << s_Longitude << "</longitude>";
    tkml << "<latitude>" << s_Latitude << "</latitude>";
    tkml << "</View>";


    tkml << "<styleUrl>#" << setIconSymbol( i_IconSymbol ) << "-" << setIconColor( i_IconColor ) << "</styleUrl>";
    tkml << "<Style><IconStyle>" << QString( "<scale>%1</scale>" ).arg( f_IconSize ) << "</IconStyle></Style>";
    tkml << "<Point><coordinates>" << s_Longitude << "," << s_Latitude << ",0," << "</coordinates></Point>";
    tkml << "</Placemark>\n";

    return( 0 );
}
