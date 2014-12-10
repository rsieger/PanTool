/* 2011-06-11                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::calcSalinityFromCondTempPress( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         j                 = 1;
    int         n                 = 0;

    int         i_PressureID      = -1;
    int         i_ConductivityID  = -1;
    int         i_TemperatureID   = -1;

    int         stopProgress	  = 0;

    QString     s_EOL             = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************
// find pressure and latitude

    setWaitCursor();

    n = NumOfSections( sl_Input.at( 0 ) );

    for ( int i=0; i<n; i++ )
    {
        QString HeaderStr = sl_Input.at( 0 ).section( "\t", i, i );
        HeaderStr.replace( "\"", "" );

        if ( ( HeaderStr == "715" ) || ( HeaderStr.startsWith( "pressure, water [dbar]", Qt::CaseInsensitive ) == true ) || ( HeaderStr.startsWith( "press [dbar]", Qt::CaseInsensitive ) == true ) )
            i_PressureID = i;

        if ( ( HeaderStr == "1882" ) || ( HeaderStr.startsWith( "conductivity [ms/cm]", Qt::CaseInsensitive ) == true ) || ( HeaderStr.startsWith( "cond [ms/cm]", Qt::CaseInsensitive ) == true ) )
            i_ConductivityID = i;

        if ( ( HeaderStr == "717" ) || ( HeaderStr.startsWith( "temperature, water [deg c]", Qt::CaseInsensitive ) == true ) || ( HeaderStr.startsWith( "temp [deg c]", Qt::CaseInsensitive ) == true ) )
            i_TemperatureID = i;
    }

    if ( ( i_PressureID < 0 ) || ( i_ConductivityID < 0 ) || ( i_TemperatureID < 0 ) )
    {
        setNormalCursor();
        return( -40 );
    }

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Calculating salinity..." ), sl_Input.count() );

// **********************************************************************************************

    for ( int i=0; i<n; i++)
        tout << sl_Input.at( 0 ).section( "\t", i, i ) << "\t";

    tout << "Salinity []@PSS-78" << s_EOL;

    while ( ( j<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        for ( int i=0; i<n; i++)
            tout << sl_Input.at( j ).section( "\t", i, i ) << "\t";

        if ( ( sl_Input.at( j ).section( "\t", i_PressureID, i_PressureID ).isEmpty() == false ) && ( sl_Input.at( j ).section( "\t", i_ConductivityID, i_ConductivityID ).isEmpty() == false ) && ( sl_Input.at( j ).section( "\t", i_TemperatureID, i_TemperatureID ).isEmpty() == false ) )
        {
            tout << Cond2Sal78 ( sl_Input.at( j ).section( "\t", i_ConductivityID, i_ConductivityID ).toDouble(), sl_Input.at( j ).section( "\t", i_TemperatureID, i_TemperatureID ).toDouble(), sl_Input.at( j ).section( "\t", i_PressureID, i_PressureID ).toDouble() ) << s_EOL;
        }
        else
        {
            tout << s_EOL;
        }

        stopProgress = incProgress( i_NumOfFiles, ++j );
    }

    fout.close();

    resetProgress( i_NumOfFiles );
    setNormalCursor();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doCalcSalinityFromCondTempPress()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Calculating salinity..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = calcSalinityFromCondTempPress( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Calculating salinity was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

double MainWindow::Cond2Sal78 ( const double d_Cond, const double d_Temp, const double d_Press )
{
// Function Cond2Sal converts a conductivity value of seawater to a value
// of the pratical-salinity-scale 1978 (PSS-78) for given values of
// conductivity, temperature and pressure.
// UNITS:
//   PRESSURE      Press          DECIBARS
//   TEMPERATURE   Temp           DEG CELSIUS IPTS-68
//   CONDUCTIVITY  Cond           mS/cm
//   SALINITY      Salinity       PSS-78
// ----------------------------------------------------------
// CHECKVALUES:
//   2.) Salinity=40.00000 for CND=1.888091, T=40 DEG C, P=10000 DECIBARS
// ----------------------------------------------------------
// SAL78 RATIO: RETURNS ZERO FOR CONDUCTIVITY RATIO: < 0.0005
// ----------------------------------------------------------
// This source code is based on the original fortran code in:
//   UNESCO technical papers in marine science 44 (1983) -
//   'Algorithms for computation of fundamental properties of seawater'
// ----------------------------------------------------------
// Written in object pascal by:
//   Dr. Jan Schulz, 26. May 2008, www.code10.info
// convert to C++ by
//   Dr. Rainer Sieger, 2011-06-11, www.awi.de

    double RT       = 0.0;
    double d_Sal    = 0.0;

    // equation is not defined for conductivity values below 5E-4
    if ( d_Cond <= 0.2 )
        return( -999.999 );

// start conversion
    d_Sal = d_Cond/42.914;
    RT    = d_Sal/( RT35( d_Temp ) * ( 1.0 + C( d_Press )/( B( d_Temp ) + A( d_Temp ) * d_Sal ) ) );
    if ( RT < 0.0 ) RT *= -1.;
    RT    = sqrt( RT );
    d_Sal = SAL( RT, d_Temp - 15. );

// control, whether result is in the validity range of PSS-78
    if ( ( d_Sal < 2.0 ) || ( d_Sal > 42.0 ) )
        return( -999.999 );

    return( d_Sal );
}

double MainWindow::SAL( const double XR, const double XT)
{
// PRACTICAL SALINITY SCALE 1978 DEFINITION WITH TEMPERATURE
// CORRECTION;XT :=T-15.0; XR:=SQRT(RT);
  double d_salinity = 0.0;
  d_salinity = ((((2.7081*XR-7.0261)*XR+14.0941)*XR+25.3851)*XR
             - 0.1692)*XR+0.0080
             + (XT/(1.0+0.0162*XT))*(((((-0.0144*XR
             + 0.0636)*XR-0.0375)*XR-0.0066)*XR-0.0056)*XR+0.0005);
  return( d_salinity);
}

double MainWindow::RT35(const double XT)
{
// FUNCTION RT35: C(35,T,0)/C(35,15,0) VARIATION WITH TEMPERATURE
  double d_RT35 = 0.0;
  d_RT35 = (((1.0031E-9 * XT - 6.9698E-7) * XT + 1.104259E-4) * XT
         + 2.00564E-2) * XT + 0.6766097;
  return( d_RT35 );
}

double MainWindow::C( const double XP )
{
// C(XP) POLYNOMIAL CORRESPONDS TO A1-A3 CONSTANTS: LEWIS 1980
  return( ((3.989E-15 * XP - 6.370E-10) * XP + 2.070E-5) * XP );
}

double MainWindow::B( const double XT )
{
  return( (4.464E-4 * XT + 3.426E-2) * XT + 1.0 );
}

double MainWindow::A( const double XT )
//A(XT) POLYNOMIAL CORRESPONDS TO B3 AND B4 CONSTANTS: LEWIS 1980
{
  return( -3.107E-3 * XT + 0.4215 );
}

