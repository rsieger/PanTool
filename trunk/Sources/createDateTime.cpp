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
                                const bool b_writeDateTimeOnly, const int i_NumOfFiles )
{
    int             i                               = 1;
    int             n                               = 0;
    int             m                               = 0;

    int             stopProgress                    = 0;

    QDate           firstDate( 2000, 1, 1 ) ;
    QDate           secondDate( 2000, 1, 1 ) ;

    QDateTime       dt( QDate( 1970, 1, 1 ), QTime( 0, 0 ) );

    QString			InputStr				= "";

    QString			s_Date  				= "";
    QString			s_DateISO				= "";

    QString			s_Time					= "";
    QString			s_Time_long				= "";
    QString			s_fTime					= "";

    int				i_Day					= 0;
    int				i_Month					= 0;
    int				i_Year					= 0;

    int				i_Hour					= 0;
    int				i_Minute				= 0;

    float			f_Second				= 0.;

    int				i_DayOfYear				= 0;
    int             i_TimeClass3h           = 0;

    int				i_offsetHour			= 0;

    long			l_JulianDay 			= 0;
    long			l_msecs					= 0;
    long            l_secs                  = 0;

    double          d_MatLabDate            = 0.;

    QString         s_EOL                   = setEOLChar( i_EOL );

    QStringList     sl_Input;

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

        l_JulianDay = 0;

//-----------------------------------------------------------------------------------------------------------------------
// Day

        if ( i_DayColumn > 0 )
            i_Day = InputStr.section( "\t", i_DayColumn-1, i_DayColumn-1 ).toInt();

//-----------------------------------------------------------------------------------------------------------------------
// Month

        if ( i_MonthColumn > 0 )
        {
            QString s_Month = InputStr.section( "\t", i_MonthColumn-1, i_MonthColumn-1 );

            s_Month.replace( "Jan", "1" );
            s_Month.replace( "Feb", "2" );
            s_Month.replace( "Mrz", "3" );
            s_Month.replace( "Mar", "3" );
            s_Month.replace( "Apr", "4" );
            s_Month.replace( "Mai", "5" );
            s_Month.replace( "May", "5" );
            s_Month.replace( "Jun", "6" );
            s_Month.replace( "Jul", "7" );
            s_Month.replace( "Aug", "8" );
            s_Month.replace( "Sep", "9" );
            s_Month.replace( "Okt", "10" );
            s_Month.replace( "Oct", "10" );
            s_Month.replace( "Nov", "11" );
            s_Month.replace( "Dez", "12" );
            s_Month.replace( "Dec", "12" );

            i_Month = s_Month.toInt();
        }

//-----------------------------------------------------------------------------------------------------------------------
// Year

        if ( i_YearColumn > 0 )
        {
            i_Year = InputStr.section( "\t", i_YearColumn-1, i_YearColumn-1 ).toInt();

            if ( ( 50 < i_Year ) && ( i_Year < 100 ) )
                i_Year += 1900;
            if ( i_Year <= 50 )
                i_Year += 2000;
        }

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
        {
            l_JulianDay = InputStr.section( "\t", i_JulianDayColumn-1, i_JulianDayColumn-1 ).toLong();

            dt.setDate( QDate::fromJulianDay( (qint64) l_JulianDay ) );
            dt.setTime( QTime( 0, 0, 0, 0 ) );
        }

//-----------------------------------------------------------------------------------------------------------------------
// Date/Time calculated from MatLab date

        if ( i_MatLabDateColumn > 0 )
        {
            d_MatLabDate = InputStr.section( "\t", i_MatLabDateColumn-1, i_MatLabDateColumn-1 ).toDouble();
            l_secs = (long) ( ( qFloor( d_MatLabDate ) - 719529. )*86400. + ( ( d_MatLabDate - qFloor( d_MatLabDate ) )*3600.*24. ) );
         }

//-----------------------------------------------------------------------------------------------------------------------
// Date calculated from day, month and year

        if ( ( i_DayColumn > 0 ) && ( i_MonthColumn > 0 ) && ( i_YearColumn > 0 ) )
        {
            firstDate.setDate( i_Year, 1, 1 );
            secondDate.setDate( i_Year, i_Month, i_Day );
            s_DateISO = secondDate.toString( "yyyy-MM-dd" );
        }

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn == 0 ) && ( i_SecondColumn == 0 ) )
            l_msecs = i_Hour*3600000;

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour and minute

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn > 0 ) && ( i_SecondColumn == 0 ) )
            l_msecs = i_Hour*3600000 + i_Minute*60000;

//-----------------------------------------------------------------------------------------------------------------------
// Time calculated from hour, minute and second

        if ( ( i_HourColumn > 0 ) && ( i_MinuteColumn > 0 ) && ( i_SecondColumn > 0 ) )
            l_msecs = i_Hour*3600000 + i_Minute*60000 + (long) (f_Second*1000.);

//-----------------------------------------------------------------------------------------------------------------------
// Date and time calculated from year and day of the year

        if ( ( i_DayOfYearColumn > 0 ) && ( i_YearColumn > 0 ) )
        {
            i_Year	    = InputStr.section( "\t", i_YearColumn-1, i_YearColumn-1 ).toInt();
            i_DayOfYear	= InputStr.section( "\t", i_DayOfYearColumn-1, i_DayOfYearColumn-1 ).toInt();

            dt.setDate( QDate( i_Year, 1, 1 ) );
            dt = dt.addDays( (qint64) i_DayOfYear );

            s_DateISO = dt.toString( "yyyy-MM-dd" );
        }

//-----------------------------------------------------------------------------------------------------------------------
// Date

        if ( ( i_DateColumn > 0 ) || ( i_DateTimeColumn > 0 ) )
        {
            i_Day	= 0;
            i_Month	= 0;
            i_Year	= 0;

            if ( i_DateColumn > 0 )
                s_Date = InputStr.section( "\t", i_DateColumn-1, i_DateColumn-1 );

            if ( i_DateTimeColumn > 0 )
            {
                QString s_DateTime = InputStr.section( "\t", i_DateTimeColumn-1, i_DateTimeColumn-1 );

                s_DateTime.replace( "  ", " " );

                s_Date = s_DateTime;

                if ( s_DateTime.count( " " ) == 1 )	// 27.11.2004 18:00
                    s_Date = s_DateTime.section( " ", 0, 0 );

                if ( s_DateTime.count( "T" ) == 1 ) 	// 2004-11-27T18:00
                    s_Date = s_DateTime.section( "T", 0, 0 );

                if ( s_DateTime.count( " " ) > 1 ) 	// 27 Nov 2004 18:00
                    s_Date = s_DateTime.section( " ", 0, 2 );
            }

            if ( s_Date.count( "." ) > 1 ) // German
            {
                i_Day	= s_Date.section( ".", 0, 0 ).toInt();
                i_Month	= s_Date.section( ".", 1, 1 ).toInt();
                i_Year	= s_Date.section( ".", 2, 2 ).toInt();
            }

            if ( s_Date.count( "/" ) > 1 ) // US
            {
                i_Day	= s_Date.section( "/", 1, 1 ).toInt();
                i_Month	= s_Date.section( "/", 0, 0 ).toInt();
                i_Year	= s_Date.section( "/", 2, 2 ).toInt();
            }

            if ( s_Date.count( "-" ) > 1 ) // ISO
            {
                i_Day	= s_Date.section( "-", 2, 2 ).toInt();
                i_Month	= s_Date.section( "-", 1, 1 ).toInt();
                i_Year	= s_Date.section( "-", 0, 0 ).toInt();
            }

            if ( s_Date.count( " " ) > 1 )
            {
                QString s_Month = "";

                s_Date.replace( ".", "" );

                if ( s_Date.section( " ", 0, 0 ).length() > 2 )
                {	// Apr 21 2005
                    s_Month	= s_Date.section( " ", 0, 0 );
                    i_Day	= s_Date.section( " ", 1, 1 ).toInt();
                    i_Year	= s_Date.section( " ", 2, 2 ).toInt();
                }
                else
                {	// 21 Apr 2005
                    i_Day	= s_Date.section( " ", 0, 0 ).toInt();
                    s_Month	= s_Date.section( " ", 1, 1 );
                    i_Year	= s_Date.section( " ", 2, 2 ).toInt();
                }

                s_Month.replace( "Jan", "1" );
                s_Month.replace( "Feb", "2" );
                s_Month.replace( "Mrz", "3" );
                s_Month.replace( "Mar", "3" );
                s_Month.replace( "Apr", "4" );
                s_Month.replace( "Mai", "5" );
                s_Month.replace( "May", "5" );
                s_Month.replace( "Jun", "6" );
                s_Month.replace( "Jul", "7" );
                s_Month.replace( "Aug", "8" );
                s_Month.replace( "Sep", "9" );
                s_Month.replace( "Okt", "10" );
                s_Month.replace( "Oct", "10" );
                s_Month.replace( "Nov", "11" );
                s_Month.replace( "Dez", "12" );
                s_Month.replace( "Dec", "12" );

                i_Month = s_Month.toInt();
            }

            if ( ( s_Date.length() == 6 ) && ( i_Day+i_Month+i_Year == 0 ) ) // 082791  = mmddyy
            {
                i_Month	= s_Date.left( 2 ).toInt();
                i_Day	= s_Date.mid( 2, 2 ).toInt();
                i_Year	= s_Date.right( 2 ).toInt();
            }

            if ( ( s_Date.length() == 8 ) && ( i_Day+i_Month+i_Year == 0 ) ) // 20040827 = yyyymmdd
            {
                i_Year  = s_Date.left( 4 ).toInt();
                i_Month = s_Date.mid( 4, 2 ).toInt();
                i_Day   = s_Date.right( 2 ).toInt();
            }

            if ( ( 50 < i_Year ) && ( i_Year < 100 ) )
                i_Year += 1900;
            if ( i_Year <= 50 )
                i_Year += 2000;

            firstDate.setDate( i_Year, 1, 1 );
            secondDate.setDate( i_Year, i_Month, i_Day );

            s_DateISO	 = secondDate.toString( "yyyy-MM-dd" );
        }

//-----------------------------------------------------------------------------------------------------------------------
// Time

        if ( ( i_TimeColumn > 0 ) || ( i_DateTimeColumn > 0 ) )
        {
            i_offsetHour	= 0;
            i_Hour			= 0;
            i_Minute		= 0;
            f_Second		= 0.0;

            if ( i_TimeColumn > 0 )
            {
                s_Time	= InputStr.section( "\t", i_TimeColumn-1, i_TimeColumn-1 );

                if ( s_Time.count( ":" ) == 0 )
                {
                    while ( s_Time.length() < 4 )
                        s_Time = "0" + s_Time;
                }
            }

            if ( i_DateTimeColumn > 0 )
            {
                QString s_DateTime = InputStr.section( "\t", i_DateTimeColumn-1, i_DateTimeColumn-1 );

                s_DateTime.replace( "  ", " " );

                s_Time = s_DateTime;

                if ( s_DateTime.count( " " ) == 1 )	// 27.11.2004 18:00
                    s_Time = s_DateTime.section( " ", 1, 1 );

                if ( s_DateTime.count( "T" ) == 1 ) 	// 2004-11-27T18:00
                    s_Time = s_DateTime.section( "T", 1, 1 );

                if ( s_DateTime.count( " " ) > 1 ) 	// 27 Nov 2004 18:00
                    s_Time = s_DateTime.section( " ", 3, 3 );
            }

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

            if ( s_Time.endsWith( "AM" ) == true )
            {
                if ( s_Time.startsWith( "12" ) == false )
                    i_offsetHour = 0;
                else
                    i_offsetHour = -12;

                s_Time.replace( "AM", "" );
            }

            if ( s_Time.endsWith( "PM" ) == true )
            {
                if ( s_Time.startsWith( "12" ) == false )
                    i_offsetHour = 12;
                else
                    i_offsetHour = 0;

                s_Time.replace( "PM", "" );
            }

            if ( s_Time.count( ":" ) > 0 )
            {
                i_Hour		= s_Time.section( ":", 0, 0 ).toInt() + i_offsetHour;
                i_Minute	= s_Time.section( ":", 1, 1 ).toInt();

                if ( s_Time.count( ":" ) > 1 )
                {
                    f_Second = s_Time.section( ":", 2, 2 ).toFloat();
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d:%06.3f", i_Hour, i_Minute, f_Second );
                }
                else
                {
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d", i_Hour, i_Minute );
                }
            }

            if ( ( s_Time.count( ":" ) == 0 ) && ( s_Time.count( "." ) == 1 ) )  // 8.5 = 08:30
            {
                i_Hour		= (int) s_Time.toFloat() + i_offsetHour;
                i_Minute	= (int) ( ( s_Time.toFloat() - (float) i_Hour ) * 60. );
                f_Second	= ( s_Time.toFloat() - (float) i_Hour - (float) i_Minute/60. ) * 3600.;

                if ( f_Second > 0 )
                {
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d:%06.3f", i_Hour, i_Minute, f_Second );
                }
                else
                {
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d", i_Hour, i_Minute );
                }
            }

            if ( ( s_Time.count( ":" ) == 0 ) && ( s_Time.count( "." ) == 0 ) )  // 085834 = 08:58:34
            {
                i_Hour		= s_Time.left( 2 ).toInt() + i_offsetHour;
                i_Minute	= s_Time.mid( 2, 2 ).toInt();

                if ( s_Time.length() > 4 )
                {
                    f_Second = s_Time.right( 2 ).toFloat();
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d:%06.3f", i_Hour, i_Minute, f_Second );
                }
                else
                {
                    s_Time.sprintf( "%02d:%02d", i_Hour, i_Minute );
                    s_Time_long.sprintf( "%02d:%02d", i_Hour, i_Minute );
                }
            }

            l_msecs = i_Hour*3600000 + i_Minute*60000 + (int) (f_Second*1000.);
        }

//-----------------------------------------------------------------------------------------------------------------------
// Time class

        switch ( i_Hour )
        {
        case 0:
        case 1:
        case 2:
            i_TimeClass3h = 1;
            break;
        case 3:
        case 4:
        case 5:
            i_TimeClass3h = 2;
            break;
        case 6:
        case 7:
        case 8:
            i_TimeClass3h = 3;
            break;
        case 9:
        case 10:
        case 11:
            i_TimeClass3h = 4;
            break;
        case 12:
        case 13:
        case 14:
            i_TimeClass3h = 5;
            break;
        case 15:
        case 16:
        case 17:
            i_TimeClass3h = 6;
            break;
        case 18:
        case 19:
        case 20:
            i_TimeClass3h = 7;
            break;
        case 21:
        case 22:
        case 23:
            i_TimeClass3h = 8;
            break;
        case 24:
            i_TimeClass3h = 1;
            break;
        default:
            i_TimeClass3h = -9;
            break;
        }

//-----------------------------------------------------------------------------------------------------------------------
// Output

        if ( s_DateISO.isEmpty() == false )
        {
            dt.setDate( QDate::fromString( s_DateISO, Qt::ISODate ) );
            dt.setTime( QTime( 0, 0, 0, 0 ) );
            dt.toUTC(); dt.setUtcOffset( 0 );
            dt = dt.addMSecs( (qint64) l_msecs );
        }
        else
        {
            dt.setDate( QDate( 1970, 1, 1 ) );
            dt.setTime( QTime( 0, 0, 0, 0 ) );
            dt.toUTC(); dt.setUtcOffset( 0 );
            dt = dt.addSecs( (qint64) l_secs );
        }

        if ( b_writeDateTimeOnly == true )
        {
//          tout << dt.toString( "yyyy-MM-ddThh:mm:ss.zzz" ) << "\t" << QString( "%1" ).arg( i_TimeClass3h ) << s_EOL;
//          tout << dt.toString( "yyyy-MM-ddThh:mm:ss.zzz" ) << "\t" << sl_Input.at( i ) << s_EOL;
//          tout << dt.toString( "yyyy-MM-ddThh:mm" ) << "\t" << sl_Input.at( i ) << s_EOL;
            tout << dt.toString( "yyyy-MM-ddThh:mm:ss" ) << "\t" << sl_Input.at( i ) << s_EOL;
        }
        else
        {
            tout << dt.toString( "yyyy-MM-ddThh:mm" ) << "\t";
            tout << dt.toString( "yyyy-MM-ddThh:mm:ss" ) << "\t";
            tout << dt.toString( "yyyy-MM-ddThh:mm:ss.zzz" ) << "\t";
            tout << dt.toString( "yyyy-MM-dd" ) << "\t";
            tout << dt.toString( "hh:mm" ) << "\t";
            tout << dt.toString( "hh:mm:ss" ) << "\t";
            tout << dt.toString( "hh:mm:ss.zzz" ) << "\t";

            // Time decimal
            tout << QString( "%1" ).arg( -dt.time().secsTo( QTime( 0, 0, 0, 0) )/86400., 0, 'f', 5 ) << "\t";

            // Time in milliseconds
            tout << QString( "%1" ).arg( dt.time().msecsSinceStartOfDay() ) << "\t";

            // Date
            tout << dt.toString( "yyyy" ) << "\t";
            tout << dt.toString( "M" ) << "\t";
            tout << dt.toString( "d" ) << "\t";

            // Time
            tout << dt.toString( "h" ) << "\t";
            tout << dt.toString( "m" ) << "\t";
            tout << dt.toString( "s" ) << "\t";

            // 3 time class
            tout << QString( "%1" ).arg( i_TimeClass3h ) << "\t";

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
          if ( doDateTimeDialog( gi_dt_DateColumn, gi_dt_YearColumn, gi_dt_MonthColumn, gi_dt_DayColumn, gi_dt_TimeColumn, gi_dt_HourColumn, gi_dt_MinuteColumn, gi_dt_SecondColumn, gi_dt_DateTimeColumn, gi_dt_DayOfYearColumn, gi_dt_JulianDayColumn, gi_dt_MatLabDateColumn, gb_dt_WriteDateTimeOnly ) == QDialog::Accepted )
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

                    err = createDateTime( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_dt_DateColumn, gi_dt_YearColumn, gi_dt_MonthColumn, gi_dt_DayColumn, gi_dt_TimeColumn, gi_dt_HourColumn, gi_dt_MinuteColumn, gi_dt_SecondColumn, gi_dt_DateTimeColumn, gi_dt_DayOfYearColumn, gi_dt_JulianDayColumn, gi_dt_MatLabDateColumn, gb_dt_WriteDateTimeOnly, gsl_FilenameList.count() );

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


