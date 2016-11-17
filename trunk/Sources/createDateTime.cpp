/* CreateDateTime.cpp         */
/* 2012-04-19                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::createDateTime( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL,
                                const int i_DateColumn, const int i_YearColumn, const int i_MonthColumn, const int i_DayColumn,
                                const int i_TimeColumn, const int i_HourColumn, const int i_MinuteColumn, const int i_SecondColumn,
                                const int i_DateTimeColumn, const int i_DayOfYearColumn, const int i_JulianDayColumn, const int i_MatLabDateColumn,
                                const bool b_writeDateTimeOnly, const int i_OutputFormatDateTime, const int i_NumOfFiles )
{
    int             i                               = 1;
    int             n                               = 0;
    int             m                               = 0;

    int             stopProgress                    = 0;

    QDateTime       dt = QDateTime().toUTC();
    QDate           d( 1970, 1, 1 );
    QTime           t( 0, 0, 0, 0 );

    QString			InputStr				= "";
    QString         s_OutputFormatDateTime  = "yyyy-MM-ddThh:mm";

    int				i_Day					= 0;
    int				i_Month					= 0;
    int				i_Year					= 0;

    int				i_Hour					= 0;
    int				i_Minute				= 0;

    float			f_Second				= 0.;

    int				i_DayOfYear				= 0;

    double          d_MatLabDate            = 0.;

    bool            b_nextDay               = false;
    bool            b_isTimeEmpty           = false;

    QString         s_EOL                   = setEOLChar( i_EOL );

    QStringList     sl_Input;

// **********************************************************************************************
// set output format of date/time values

    switch( i_OutputFormatDateTime )
    {
    case 0:
        s_OutputFormatDateTime = "yyyy";
        break;
    case 1:
        s_OutputFormatDateTime = "yyyy-MM";
        break;
    case 2:
        s_OutputFormatDateTime = "yyyy-MM-dd";
        break;
    case 3:
        s_OutputFormatDateTime = "yyyy-MM-ddThh:mm";
        break;
    case 4:
        s_OutputFormatDateTime = "yyyy-MM-ddThh:mm:ss";
        break;
    case 5:
        s_OutputFormatDateTime = "yyyy-MM-ddThh:mm:ss.zzz";
        break;
    case 6:
        s_OutputFormatDateTime = "hh:mm";
        break;
    case 7:
        s_OutputFormatDateTime = "hh:mm:ss";
        break;
    case 8:
        s_OutputFormatDateTime = "hh:mm:ss.zzz";
        break;
    default:
        s_OutputFormatDateTime = "yyyy-MM-ddThh:mm";
        break;
    }

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 2 )
        return( n );

    if ( ( m = NumOfSections( sl_Input.at( 0 ) ) ) < 1 )
        return( -80 );

// **********************************************************************************************
// open output file

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating date/time..." ), sl_Input.count() );

//-----------------------------------------------------------------------------------------------------------------------
// Header

    if ( b_writeDateTimeOnly == true )
    {
//      tout << tr( "Date/Time\tTime class 3h" ) << s_EOL;

        tout << tr( "Date/Time\t" ) << sl_Input.at( 0 ) << s_EOL;
    }
    else
    {
        tout << tr( "Date/Time\tDate/Time\tDate/Time\t" );
        tout << tr( "Date\tTime\tTime\tTime\tTime (dec)\tTime (msec)\t" );
        tout << tr( "Year\tMonth\tDay\tHour\tMinute\tSeconds\tTime class 3h\t" );
        tout << tr( "Day of year\tJulian day\tUnix timestamp\tMATLAB date" ) << s_EOL;
    }

//-----------------------------------------------------------------------------------------------------------------------

    while ( ( i < n )  && ( stopProgress != _APPBREAK_ ) )
    {
        InputStr    = sl_Input.at( i );

        i_Day       = 0;
        i_Month     = 0;
        i_Day       = 0;

        i_Hour      = 0;
        i_Minute    = 0;
        f_Second    = 0.;

        i_DayOfYear = 0;

        b_nextDay       = false;
        b_isTimeEmpty   = false;

        d.setDate( 1970, 1, 1 );
        t.setHMS( 0, 0, 0, 0 );

//-----------------------------------------------------------------------------------------------------------------------
// Day

        if ( i_DayColumn > 0 )
            i_Day = InputStr.section( "\t", i_DayColumn-1, i_DayColumn-1 ).toInt();

//-----------------------------------------------------------------------------------------------------------------------
// Month

        if ( i_MonthColumn > 0 )
            i_Month = getMonth( InputStr.section( "\t", i_MonthColumn-1, i_MonthColumn-1 ) );

//-----------------------------------------------------------------------------------------------------------------------
// Year

        if ( i_YearColumn > 0 )
            i_Year = getYear( InputStr.section( "\t", i_YearColumn-1, i_YearColumn-1 ) );

//-----------------------------------------------------------------------------------------------------------------------
// Hour

        if ( i_HourColumn > 0 )
            i_Hour = InputStr.section( "\t", i_HourColumn-1, i_HourColumn-1 ).toInt();

//-----------------------------------------------------------------------------------------------------------------------
// Minute

        if ( i_MinuteColumn > 0 )
            i_Minute = InputStr.section( "\t", i_MinuteColumn-1, i_MinuteColumn-1 ).toInt();

//-----------------------------------------------------------------------------------------------------------------------
// Second

        if ( i_SecondColumn > 0 )
            f_Second = InputStr.section( "\t", i_SecondColumn-1, i_SecondColumn-1 ).toFloat();

//-----------------------------------------------------------------------------------------------------------------------
// Date/Time calculated from Julian Day

        if ( i_JulianDayColumn > 0 )
            d = QDate::fromJulianDay( (qint64) InputStr.section( "\t", i_JulianDayColumn-1, i_JulianDayColumn-1 ).toLong() );

//-----------------------------------------------------------------------------------------------------------------------
// Date/Time calculated from MatLab date

        if ( i_MatLabDateColumn > 0 )
        {
            d_MatLabDate = InputStr.section( "\t", i_MatLabDateColumn-1, i_MatLabDateColumn-1 ).toDouble();

            d = d.addDays( (qint64) ( qFloor( d_MatLabDate ) - 719529. ) );
            t = t.addSecs( (qint64) ( ( d_MatLabDate - qFloor( d_MatLabDate ) )*3600.*24. ) );
         }

//-----------------------------------------------------------------------------------------------------------------------
// Date calculated from day, month and year

        if ( ( i_DayColumn > 0 ) && ( i_MonthColumn > 0 ) && ( i_YearColumn > 0 ) )
            d.setDate( i_Year, i_Month, i_Day );

//-----------------------------------------------------------------------------------------------------------------------
// Date and time calculated from year and day of the year

        if ( ( i_DayOfYearColumn > 0 ) && ( i_YearColumn > 0 ) )
        {
            i_Year	    = InputStr.section( "\t", i_YearColumn-1, i_YearColumn-1 ).toInt();
            i_DayOfYear	= InputStr.section( "\t", i_DayOfYearColumn-1, i_DayOfYearColumn-1 ).toInt();

            d.setDate( i_Year, 1, 1 );

            d = d.addDays( (qint64) ( i_DayOfYear - 1 ) );
        }

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn == 0 ) && ( i_SecondColumn == 0 ) )
            t.setHMS( i_Hour, 0, 0, 0 );

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour and minute

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn > 0 ) && ( i_SecondColumn == 0 ) )
            t.setHMS( i_Hour, i_Minute, 0, 0 );

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour, minute and second

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn > 0 ) && ( i_SecondColumn > 0 ) )
            t.setHMS( i_Hour, i_Minute, qFloor( f_Second ), ( f_Second - qFloor( f_Second ) )*1000. );

//-----------------------------------------------------------------------------------------------------------------------
// Date calculated from date string

        if ( i_DateColumn > 0 )
            d = getDate( InputStr.section( "\t", i_DateColumn-1, i_DateColumn-1 ) );

//-----------------------------------------------------------------------------------------------------------------------
// Date calculated from date/time string

        if ( i_DateTimeColumn > 0 )
            d = getDate( InputStr.section( "\t", i_DateTimeColumn-1, i_DateTimeColumn-1 ) );

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from time string

        if ( i_TimeColumn > 0 )
        {
            if ( InputStr.section( "\t", i_TimeColumn-1, i_TimeColumn-1 ).isEmpty() == false )
                t = getTime( InputStr.section( "\t", i_TimeColumn-1, i_TimeColumn-1 ), b_nextDay );
            else
                b_isTimeEmpty = true;
        }

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from date/time string

        if ( i_DateTimeColumn > 0 )
            t = getTime( InputStr.section( "\t", i_DateTimeColumn-1, i_DateTimeColumn-1 ), b_nextDay );

//-----------------------------------------------------------------------------------------------------------------------
// Output

        dt.setDate( d );
        dt.setTime( t );
        dt.setOffsetFromUtc( 0 );

        if ( b_nextDay == true )
            dt = dt.addDays( 1 );

        if ( b_writeDateTimeOnly == true )
        {
            // Date and Time
//          tout << dt.toString( s_QutputFormatDateTime ) << "\t" << QString( "%1" ).arg( getTimeClass3h( dt.time().hour() ) << sl_Input.at( i ) << s_EOL;
//          tout << dt.toString( s_QutputFormatDateTime ) << "\t"                                                            << sl_Input.at( i ) << s_EOL;
//          tout << dt.toString( s_QutputFormatDateTime ) << "\t"                                                            << sl_Input.at( i ) << s_EOL;

            if ( b_isTimeEmpty == true )
                tout << "\t"                                                                                                 << sl_Input.at( i ) << s_EOL;
            else
                tout << dt.toString( s_OutputFormatDateTime ) << "\t"                                                        << sl_Input.at( i ) << s_EOL;
        }
        else
        {
            if ( b_isTimeEmpty == false )
            {
                // Date
                tout << dt.toString( "yyyy-MM-ddThh:mm" )        << "\t";
                tout << dt.toString( "yyyy-MM-ddThh:mm:ss" )     << "\t";
                tout << dt.toString( "yyyy-MM-ddThh:mm:ss.zzz" ) << "\t";
                tout << dt.toString( "yyyy-MM-dd" )              << "\t";

                // Time
                tout << dt.toString( "hh:mm" )                   << "\t";
                tout << dt.toString( "hh:mm:ss" )                << "\t";
                tout << dt.toString( "hh:mm:ss.zzz" )            << "\t";

                // Time decimal
                tout << QString( "%1" ).arg( -dt.time().secsTo( QTime( 0, 0, 0, 0) )/86400., 0, 'f', 5 ) << "\t";

                // Time in milliseconds
                tout << QString( "%1" ).arg( dt.time().msecsSinceStartOfDay() ) << "\t";
            }
            else
            {
                tout << "\t\t\t";     // missing time
                tout << dt.toString( "yyyy-MM-dd" ) << "\t";
                tout << "\t\t\t\t\t"; // missing time
            }

            // Date
            tout << dt.toString( "yyyy" ) << "\t";
            tout << dt.toString( "M" )    << "\t";
            tout << dt.toString( "d" )    << "\t";

            if ( b_isTimeEmpty == false )
            {
                // Time
                tout << dt.toString( "h" )    << "\t";
                tout << dt.toString( "m" )    << "\t";
                tout << dt.toString( "s.z" )  << "\t";

                // 3 time class
                tout << QString( "%1" ).arg( getTimeClass3h( dt.time().hour() ) ) << "\t";
            }
            else
            {
                tout << "\t\t\t";   // missing time
                tout << "\t";       // missing time
            }

            // Day of Year
            tout << dt.date().dayOfYear() << "\t";

            // Julian day
            tout << QString( "%1" ).arg( dt.date().toJulianDay() ) << "\t";

            // Unix timestamp
            tout << QString( "%1" ).arg( dt.toTime_t() ) << "\t";

            // MatLab date
            tout << QString( "%1" ).arg( (double) dt.toMSecsSinceEpoch()/86400000. + 719529., 0, 'f', 6 ) << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getTimeClass3h( const int i_Hour )
{
    switch ( i_Hour )
    {
    case 0:
    case 1:
    case 2:
        return( 1 );
        break;
    case 3:
    case 4:
    case 5:
        return( 2 );
        break;
    case 6:
    case 7:
    case 8:
        return( 3 );
        break;
    case 9:
    case 10:
    case 11:
        return( 4 );
        break;
    case 12:
    case 13:
    case 14:
        return( 5 );
        break;
    case 15:
    case 16:
    case 17:
        return( 6 );
        break;
    case 18:
    case 19:
    case 20:
        return( 7 );
        break;
    case 21:
    case 22:
    case 23:
        return( 8 );
        break;
    case 24:
        return( 1 );
        break;
    default:
        ;
        break;
    }

    return( -9 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getYear( const QString s_YearIn )
{
    int i_Year = s_YearIn.toInt();

    if ( ( i_Year > 50 ) && ( i_Year < 100 ) ) // 51 ... 99 => 1950 ... 1999
        i_Year += 1900;

    if ( ( i_Year >= 0 ) && ( i_Year <= 50 ) ) // 0 ... 50 => 2000 ... 2050
        i_Year += 2000;

    return( i_Year );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::getMonth( const QString s_MonthIn )
{
    QString s_Month = s_MonthIn.toLower();

    int     i_Month = s_Month.toInt();

// **********************************************************************************************

    if ( ( i_Month > 0 ) && ( i_Month < 13 ) )
        return( i_Month);

    s_Month.replace( ".", "" );
    s_Month.replace( "january", "1" );
    s_Month.replace( "januar", "1" );
    s_Month.replace( "jan", "1" );
    s_Month.replace( "february", "2" );
    s_Month.replace( "februar", "2" );
    s_Month.replace( "feb", "2" );
    s_Month.replace( "march", "3" );
    s_Month.replace( "märz", "3" );
    s_Month.replace( "mrz", "3" );
    s_Month.replace( "mar", "3" );
    s_Month.replace( "april", "4" );
    s_Month.replace( "apr", "4" );
    s_Month.replace( "mai", "5" );
    s_Month.replace( "may", "5" );
    s_Month.replace( "june", "6" );
    s_Month.replace( "juni", "6" );
    s_Month.replace( "jun", "6" );
    s_Month.replace( "july", "7" );
    s_Month.replace( "juli", "7" );
    s_Month.replace( "jul", "7" );
    s_Month.replace( "august", "8" );
    s_Month.replace( "aug", "8" );
    s_Month.replace( "september", "9" );
    s_Month.replace( "sep", "9" );
    s_Month.replace( "october", "10" );
    s_Month.replace( "oktober", "10" );
    s_Month.replace( "okt", "10" );
    s_Month.replace( "oct", "10" );
    s_Month.replace( "november", "11" );
    s_Month.replace( "nov", "11" );
    s_Month.replace( "december", "12" );
    s_Month.replace( "dezember", "11" );
    s_Month.replace( "dez", "12" );
    s_Month.replace( "dec", "12" );

    return( s_Month.toInt() );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QTime MainWindow::getTime( const QString s_TimeIn, bool &b_nextDay )
{
    QString s_Time = s_TimeIn;

    QTime   t( 0, 0, 0, 0 );

    int     i_offsetHour	= 0;
    int     i_Hour			= 0;
    int     i_Minute		= 0;

    float   f_Second		= 0.0;

// **********************************************************************************************

    s_Time = s_Time.toLower();

    s_Time.replace( "  ", " " );
    s_Time.replace( " pm", "pm" );
    s_Time.replace( " am", "am" );

    if ( s_Time.count( " " ) == 1 )	// 27.11.2004 18:58:34.345
        s_Time = s_Time.section( " ", 1, 1 );

    if ( s_Time.count( "t" ) == 1 ) // 2004-11-27T18:58:34.345
        s_Time = s_Time.section( "t", 1, 1 );

    if ( s_Time.count( " " ) == 3 ) // 27 Nov 2004 18:58:34.345
        s_Time = s_Time.section( " ", 3, 3 );

    if ( ( s_Time.count( ":" ) == 0 ) && ( s_Time.count( "." ) == 0 ) ) // 185834
    {
        while ( s_Time.length() < 6 )
            s_Time.prepend( "0" );
    }

// **********************************************************************************************

    // 00:00	12:00 AM	-12
    // 00:30	12:30 AM	-12
    // 01:00	01:00 AM	+0
    // 02:00	02:00 AM	+0
    // 03:00	03:00 AM	+0
    // 04:00	04:00 AM	+0
    // 05:00	05:00 AM	+0
    // 06:00	06:00 AM	+0
    // 07:00	07:00 AM	+0
    // 08:00	08:00 AM	+0
    // 09:00	09:00 AM	+0
    // 10:00	10:00 AM	+0
    // 11:00	11:00 AM	+0
    // 12:00	12:00 PM	+0
    // 12:30	12:30 PM	+0
    // 13:00	01:00 PM	+12
    // 14:00	02:00 PM	+12
    // 15:00	03:00 PM	+12
    // 16:00	04:00 PM	+12
    // 17:00	05:00 PM	+12
    // 18:00	06:00 PM	+12
    // 19:00	07:00 PM	+12
    // 20:00	08:00 PM	+12
    // 21:00	09:00 PM	+12
    // 22:00	10:00 PM	+12
    // 23:00	11:00 PM	+12

    if ( s_Time.endsWith( "am" ) == true )
    {
        if ( s_Time.startsWith( "12" ) == false )
            i_offsetHour = 0;
        else
            i_offsetHour = -12;

        s_Time.replace( "am", "" );
    }

    if ( s_Time.endsWith( "pm" ) == true )
    {
        if ( s_Time.startsWith( "12" ) == false )
            i_offsetHour = 12;
        else
            i_offsetHour = 0;

        s_Time.replace( "pm", "" );
    }

    s_Time.replace( " ", "" );

// **********************************************************************************************

    if ( s_Time.count( ":" ) > 0 )
    {
        i_Hour	 = s_Time.section( ":", 0, 0 ).toInt() + i_offsetHour;
        i_Minute = s_Time.section( ":", 1, 1 ).toInt();
    }

    if ( s_Time.count( ":" ) > 1 )
        f_Second = s_Time.section( ":", 2, 2 ).toFloat();

    if ( ( s_Time.count( ":" ) == 0 ) && ( s_Time.count( "." ) == 1 ) )  // 8.5 = 08:30
        f_Second = s_Time.toFloat()*3600.;

    if ( ( s_Time.count( ":" ) == 0 ) && ( s_Time.count( "." ) == 0 ) )  // 085834 = 08:58:34
    {
        i_Hour	 = s_Time.mid( 0, 2 ).toInt();
        i_Minute = s_Time.mid( 2, 2 ).toInt();
        f_Second = s_Time.mid( 4, 2 ).toFloat();
    }

    if ( i_Hour == 24 )
    {
        i_Hour    = 0;
        b_nextDay = true;
    }

    t.setHMS( i_Hour, i_Minute, 0, 0 );
    t = t.addSecs( qFloor( f_Second ) );
    t = t.addMSecs( ( f_Second - qFloor( f_Second ) )*1000. );

    return( t );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

QDate MainWindow::getDate( const QString s_DateIn )
{
    QString s_Date = s_DateIn;

    QDate   d( 1970, 1, 1 );

// **********************************************************************************************

    s_Date.replace( "  ", " " );

    if ( s_Date.count( " " ) == 1 )	// 27.11.2004 18:00
        s_Date = s_Date.section( " ", 0, 0 );

    if ( s_Date.count( "T" ) == 1 ) // 2004-11-27T18:00
        s_Date = s_Date.section( "T", 0, 0 );

    if ( s_Date.count( " " ) > 1 ) 	// 27 Nov 2004 18:00
        s_Date = s_Date.section( " ", 0, 2 );

// **********************************************************************************************

    if ( s_Date.count( "." ) > 1 ) // German
    {
        d.setDate( getYear( s_Date.section( ".", 2, 2 ) ), s_Date.section( ".", 1, 1 ).toInt(), s_Date.section( ".", 0, 0 ).toInt() );

        return( d );
    }

    if ( s_Date.count( "/" ) > 1 ) // US
    {
        d.setDate( getYear( s_Date.section( "/", 2, 2 ) ), s_Date.section( "/", 0, 0 ).toInt(), s_Date.section( "/", 1, 1 ).toInt() );

        return( d );
    }

    if ( s_Date.count( "-" ) > 1 ) // ISO
    {
        d.setDate( getYear( s_Date.section( "-", 0, 0 ) ), s_Date.section( "-", 1, 1 ).toInt(), s_Date.section( "-", 2, 2 ).toInt() );

        return( d );
    }

    if ( s_Date.count( " " ) > 1 )
    {
        if ( s_Date.section( " ", 0, 0 ).length() > 2 )
            d.setDate( s_Date.section( " ", 2, 2 ).toInt(), getMonth( s_Date.section( " ", 0, 0 ) ), s_Date.section( " ", 1, 1 ).toInt() ); // Apr 21 2005
        else
            d.setDate( s_Date.section( " ", 2, 2 ).toInt(), getMonth( s_Date.section( " ", 1, 1 ) ), s_Date.section( " ", 0, 0 ).toInt() ); // 21 Apr 2005

        return( d );
    }

    if ( ( s_Date.length() == 6 ) && ( d.year()+d.month()-d.day() == 1970 ) ) // 082791  = mmddyy
    {
        d.setDate( getYear( s_Date.right( 2 ) ), s_Date.left( 2 ).toInt(), s_Date.mid( 2, 2 ).toInt() );

        return( d );
    }

    if ( ( s_Date.length() == 8 ) && ( d.year()+d.month()-d.day() == 1970 ) ) // 20040827 = yyyymmdd
    {
        d.setDate( s_Date.left( 4 ).toInt(), s_Date.mid( 4, 2 ).toInt(), s_Date.right( 2 ).toInt() );

        return( d );
    }

    return( d );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doCreateDateTime()
{
    int         i                   = 0;
    int         err                 = 0;
    int         stopProgress        = 0;

    QString     s_FilenameIn        = "";
    QString     s_FilenameOut       = "";

    bool        b_incActionNumber   = true;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
          if ( doDateTimeDialog( gi_dt_DateColumn, gi_dt_YearColumn, gi_dt_MonthColumn, gi_dt_DayColumn, gi_dt_TimeColumn, gi_dt_HourColumn, gi_dt_MinuteColumn, gi_dt_SecondColumn, gi_dt_DateTimeColumn, gi_dt_DayOfYearColumn, gi_dt_JulianDayColumn, gi_dt_MatLabDateColumn, gb_dt_WriteDateTimeOnly, gi_dt_OutputFormatDateTime ) == QDialog::Accepted )
          {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating date/time..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    QFileInfo fi( s_FilenameIn );

                    if ( gb_dt_WriteDateTimeOnly == false )
                    {
                        s_FilenameOut     = fi.absolutePath() + "/" + fi.completeBaseName() + "_DateTime" + setExtension( gi_Extension );
                        b_incActionNumber = false;
                    }

                    err = createDateTime( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_dt_DateColumn, gi_dt_YearColumn, gi_dt_MonthColumn, gi_dt_DayColumn, gi_dt_TimeColumn, gi_dt_HourColumn, gi_dt_MinuteColumn, gi_dt_SecondColumn, gi_dt_DateTimeColumn, gi_dt_DayOfYearColumn, gi_dt_JulianDayColumn, gi_dt_MatLabDateColumn, gb_dt_WriteDateTimeOnly, gi_dt_OutputFormatDateTime, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }

            resetFileProgress( gsl_FilenameList.count() );
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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating date/time was canceled" ), false, b_incActionNumber );

    onError( err );
}


