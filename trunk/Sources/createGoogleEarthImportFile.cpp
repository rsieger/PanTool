// ***********************************************************************************************
// *                                                                                             *
// * createGoogleEarthImportFile.cpp - Create a Google Earth import file (KML)                   *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-24                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

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

int MainWindow::openKMLFolder( QFile& fkml, const QString &s_Campaign )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "  <Folder>\n";

    if ( ( s_Campaign != "xxx" ) && ( s_Campaign != "unknown campaign" ) )
    {
        tkml << "    <name>Campaign: " << s_Campaign.section( " (", 0, 0 ) << "</name>\n";
        tkml << "    <description><![CDATA[<a href=\"https://pangaea.de/search?q=" << s_Campaign.section( " (", 0, 0 ) << "\">search all Datasets</a>]]></description>\n";
    }
    else
    {
        tkml << "    <name>Campaign unknown</name>\n";
    }

    tkml << "    <Folder>\n";
    tkml << "      <name>Events</name>\n";
    tkml << "      <open>0</open>\n";

    return( 2 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::closeKMLFolder( QFile& fkml, const int n )
{
    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    switch ( n )
    {
      case 4:
        tkml << "    </Folder>\n";
        break;
      default:
        tkml << "  </Folder>\n";
        break;
    }


    return( 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::writeKMLTrack( QFile& fkml, const QStringList &sl_MetadataList, const int i_TracklineWidth, const int i_TracklineColor, const int i_Start, const int i_End )
{
    if ( i_Start+1 < i_End )
    {
        QTextStream tkml( &fkml );
        tkml.setCodec("UTF-8");

        tkml << "    <Placemark><name>Trackline</name>" << endl;

        switch ( i_TracklineColor )
        {
            case _RED:
                tkml << "      <Style><LineStyle><color>ff0000ff</color>";
                break;
            case _GREEN:
                tkml << "      <Style><LineStyle><color>ff00ff00</color>";
                break;
            case _BLUE:
                tkml << "      <Style><LineStyle><color>ffff0000</color>";
                break;
            case _ORANGE:
                tkml << "      <Style><LineStyle><color>ff0099ff</color>";
                break;
            case _YELLOW:
                tkml << "      <Style><LineStyle><color>ff33ffff</color>";
                break;
            default:
                tkml << "      <Style><LineStyle><color>ff0099ff</color>";
                break;
        }

        tkml << "<width>" << (float) i_TracklineWidth/10. << "</width></LineStyle></Style>" << endl;
        tkml << "      <LineString>" << endl;
        tkml << "        <tessellate>1</tessellate>" << endl;
        tkml << "        <coordinates>" << endl;

        for ( int i=i_Start; i<i_End; i++ )
        {
            double d_Latitude  = sl_MetadataList.at( i ).section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();
            double d_Longitude = sl_MetadataList.at( i ).section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble();

            if ( d_Longitude > 180 )
                d_Longitude -= 360;

            tkml << "          " << QString( "%1,%2" ).arg( d_Longitude, 0, 'f', 5 ).arg( d_Latitude, 0, 'f', 5 ) << endl;
        }

        tkml << "        </coordinates>" << endl;
        tkml << "      </LineString>" << endl;
        tkml << "    </Placemark>" << endl;
    }

    return( i_End );
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

int MainWindow::writeKMLEntry( QFile& fkml, const QStringList &sl_MetadataList, const bool b_displayEventLabel, const bool b_displayDescription, const float f_IconSize, const int i_IconColor, const int i_IconSymbol, const int i )
{
    double d_Latitude      = sl_MetadataList.at( i ).section( "\t", _LATITUDEPOS, _LATITUDEPOS ).toDouble();   // Latitude
    double d_Longitude     = sl_MetadataList.at( i ).section( "\t", _LONGITUDEPOS, _LONGITUDEPOS ).toDouble(); // Longitude

    QString s_EventLabel    = sl_MetadataList.at( i ).section( "\t", _EVENTLABELPOS, _EVENTLABELPOS );         // Event label
    QString s_DateTime      = sl_MetadataList.at( i ).section( "\t", _DATETIMEPOS, _DATETIMEPOS );                     // Date

// **********************************************************************************************

    if ( d_Longitude > 180 )
        d_Longitude -= 360;

    QString s_Longitude = QString( "%1" ).arg( d_Longitude, 0, 'f', 5 );
    QString s_Latitude  = QString( "%1" ).arg( d_Latitude, 0, 'f', 5 );

// **********************************************************************************************

    QTextStream tkml( &fkml );
    tkml.setCodec("UTF-8");

    tkml << "      <Placemark>";

    if ( b_displayEventLabel == true )
        tkml << "<name>" << s_EventLabel << "</name>";

    if ( b_displayDescription == true )
    {
        tkml << "<description>";
        tkml << "Decription of photo";
        tkml << "</description>";
    }

    tkml << "<View>";
    tkml << "<longitude>" << s_Longitude << "</longitude>";
    tkml << "<latitude>" << s_Latitude << "</latitude>";
    tkml << "</View>";


    tkml << "<styleUrl>#" << setIconSymbol( i_IconSymbol ) << "-" << setIconColor( i_IconColor ) << "</styleUrl>";
    tkml << "<Style><IconStyle>" << QString( "<scale>%1</scale>" ).arg( f_IconSize ) << "</IconStyle></Style>";
    tkml << "<Point><coordinates>" << s_Longitude << "," << s_Latitude << ",0</coordinates></Point>";
    tkml << "</Placemark>\n";

    return( 0 );
}

