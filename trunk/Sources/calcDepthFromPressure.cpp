/* 2011-06-11                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::calcDepthFromPressure( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_LatitudeColumn, const int i_PressureColumn, const int i_NumOfDigits, const bool b_DeleteInputFile, const int i_NumOfFiles )
{
    int         j                 = 1;
    int         n                 = 0;

    int         i_PressureID      = i_PressureColumn - 1;
    int         i_LatitudeID      = i_LatitudeColumn - 1;

    float       f_depth           = (float) 0.;
    float       f_gravity         = (float) 0.981;

    int         stopProgress      = 0;

    QString     s_EOL             = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

    n = NumOfSections( sl_Input.at( 0 ) );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting pressure to depth..." ), sl_Input.count() );

// **********************************************************************************************

    for ( int i=0; i<i_PressureID; i++ )
        tout << sl_Input.at( 0 ).section( "\t", i, i ) << "\t";

    tout << "Depth, water [m]\tPressure, water [dbar]\t";

    for ( int i=i_PressureID+1; i<n-1; i++)
        tout << sl_Input.at( 0 ).section( "\t", i, i ) << "\t";

    tout << sl_Input.at( 0 ).section( "\t", n-1, n-1 ) << s_EOL;

    while ( ( j<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        for ( int i=0; i<i_PressureID; i++ )
            tout << sl_Input.at( j ).section( "\t", i, i ) << "\t";

        if ( sl_Input.at( j ).section( "\t", i_PressureID, i_PressureID ).isEmpty() == false )
        {
            if ( i_LatitudeID >= 0 )
                f_gravity = (float) gravity( sl_Input.at( j ).section( "\t", i_LatitudeID, i_LatitudeID ).toFloat() );

            if ( f_gravity > -1. )
            {
                f_depth = pzcon( 0, f_gravity, (float) 0., sl_Input.at( j ).section( "\t", i_PressureID, i_PressureID ).toFloat(), (float) 0. );
                tout << QString( "%1" ).arg( f_depth, 0, 'f', i_NumOfDigits ) << "\t";
            }
            else
            {
                tout << "\t";
            }
        }
        else
        {
            tout << "\t";
        }

        for ( int i=i_PressureID; i<n-1; i++ )
            tout << sl_Input.at( j ).section( "\t", i, i) << "\t";

        tout << sl_Input.at( j ).section( "\t", n-1, n-1 ) << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++j );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    if ( b_DeleteInputFile == true )
        removeFile( s_FilenameIn );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doCalcDepthFromPressure()
{
    int     i                        = 0;
    int     err                      = 0;
    int     stopProgress             = 0;

    QString s_FilenameIn             = "";
    QString s_FilenameOut            = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        if ( doLatLongOptionsDialog( gi_pd_LatitudeColumn,  gi_pd_LongitudeColumn, gi_pd_PressureColumn,  gi_pd_NumOfDigits, gb_pd_DeleteInputFile ) == QDialog::Accepted )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting pressure to depth..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = calcDepthFromPressure( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gi_pd_LatitudeColumn, gi_pd_PressureColumn, gi_pd_NumOfDigits, gb_pd_DeleteInputFile, gsl_FilenameList.count() );

                    stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
                }
                else
                {
                    err = _FILENOTEXISTS_;
                }
            }
        }
        else
        {
            err = _CHOOSEABORTED_;
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting pressure to depth was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

// subroutine pzcon(cnvrsn,grav,dynz,prsdb,depth)
//
//	Author: Dr. Reiner Schlitzer, Alfred Wegener Institute Bremerhaven
//
//  pzcon  -- convert pressure in decibars to depth in meters
//            (or vice versa)
//  purpose:
//    to calculate depth in meters (mantyla,saunders)
//    from pressure in decibars (or pressure from depth).
//    ref: j. physical ocean., 11 (4), 1981 (saunders)
//         priv. correspondence 1982-1983   (mantyla)
//  method:
//    a standard ocean (sal=35.0, t=0.0) is used plus a dynamic
//    height correction to account for deviations from the standard
//    ocean. pressure to depth conversion is effected as:
//                 z = p/(b*c) + dynz/b
//    where:
//       p     = insitu pressure (decibars)
//       b     = insitu gravity as a function of lat. and pressure
//               (decimeter/sec**2)
//       c     = insitu mean density rho(35.0,0.0,p)
//       dynz  = dynamic height in dynamic meters
//       z     = depth in meters
//  parameters:
//    cnvrsn   -> conversion to be performed:
//                0 = pressure to depth
//                1 = depth to pressure
//    grav     -> acceleration of gravity decameters/sec**2 at
//                station latitude, pressure=0.0db.
//    dynz     -> dynamic height in dynamic meters
//    prsdb    -> pressure in decibars (cnvrsn=0)
//             <- pressure in decibars (cnvrsn=1)
//    depth    <- depth in meters (cnvrsn=0)
//             -> depth in meters (cnvrsn=1)
//
// call	Depth = (double) pzcon(0,(float) 0.981,(float) 0.,InputStr.section( "\t", 4, 4).toFloat(), (float) 0.);

float MainWindow::pzcon(int cnvrsn,float grav,float dynz,float prsdb,float depth)
{
      float   a,b,c;
      double  dd,dg,dh,da,db,dc;

//    (convert pressure to depth):

      if (cnvrsn == 0)
      {
                a = (float) 2.2e-6*prsdb;
        b = grav+(float) 0.05*a;
            c = (float) 1.0285+a;
        depth = prsdb/(b*c);
        if (dynz != 0)
          depth = depth + dynz/b;
        return (depth);
      }
//    (convert depth to pressure):
      else
      {
        dd = (double) depth;
        dg = (double) grav;
        dh = (double) dynz;
        da = 2.42e-13*dd;
        db = dd*(1.13135e-7+2.2e-6*dg)-1.0;
        dc = dg*dd;
        if (dynz != 0 )
        {
          db = db - 2.2e-6*dh;
          dc = dc - dh;
        }
        dc = 1.0285*dc;
        prsdb = 0.0;
        if (da != 0.0)
          prsdb = (float) (-db-sqrt(db*db-4.0*da*dc) )/(da+da);
        return (prsdb);
      }
}

/*------------------------------------------------------------------------*/
float MainWindow::gravity(float lat)
/*------------------------------------------------------------------------
  float gravity(float lat)

  compute gravity [m/s^2] at latitude 'lat'
  R. Schlitzer (aug/14/96)
  ------------------------------------------------------------------------*/
{
  const float dgrad=0.017453293f;
  double plat = 0;

    if ( ( -90. <= lat ) && ( lat <= 90.0 ) )
    {
        plat	= (double) fabs(lat*dgrad);
        return ( (float) (0.9780318*(1.+5.3024e-3*sin(plat)*sin(plat)-5.9e-6*sin(2.*plat)*sin(2.*plat))));
    }
    else
        return ( (float) 0.981 );

  return ( (float) -1. );
}

