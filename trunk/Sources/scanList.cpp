// ***********************************************************************************************
// *                                                                                             *
// * scanList.cpp - scan the list of columns or positions                                        *
// *                                                                                             *
// * Dr. Rainer Sieger - 2011-08-11                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// ******************************************************************************************************
// ******************************************************************************************************
// ******************************************************************************************************
// 2017-07-11

QList<int> MainWindow::scanList( const int mode, const int i_maxNumOfPositions, const QString &s_List )
{
    int			n						= 0;

    QString		s_tempList				= "";

    QList<int>	il_PositionList;
    QList<int>	il_tempPositionList;

// ******************************************************************************************************

    il_PositionList.clear();
    il_tempPositionList.clear();

    s_tempList = s_List;

    if ( s_tempList.startsWith( "-" ) == true ) // -5
        s_tempList = s_List.section( ",", 1 );

    if ( s_tempList.startsWith( "0" ) == true ) // 0
        s_tempList = s_List.section( ",", 1 );

    if ( s_tempList.endsWith( "-" ) == true )
        s_tempList.append( "end" );

    s_tempList.replace( "--", "-" );
    s_tempList.replace( "  ", " " );
    s_tempList.replace( "-end", QString( "-%1" ).arg( i_maxNumOfPositions ) );
    s_tempList.replace( "-e", QString( "-%1" ).arg( i_maxNumOfPositions ) );
    s_tempList.replace( " ", "," );
    s_tempList.replace( "-", "@" );
    s_tempList.replace( "@", ",-1," );
    s_tempList.replace( ",0,", "," );
    s_tempList.replace( ",,", "," );

// ******************************************************************************************************

    if ( s_tempList.isEmpty() == true )
        return( il_PositionList );

    n = s_tempList.count( "," ) + 1;

    for( int i=0; i<n; i++ )
        if ( s_tempList.section( ",", i, i ).toInt() != 0 )
            il_PositionList.append( s_tempList.section( ",", i, i ).toInt() );

// ******************************************************************************************************

    if ( ( n = il_PositionList.count() ) < 1 )
        return( il_PositionList );

    for( int i=0; i<n; i++ )
    {
        if ( il_PositionList.at( i ) > 0 )
        {
            il_tempPositionList.append( il_PositionList.at( i ) );
        }
        else
        {
            if ( il_PositionList.at( i-1 ) < il_PositionList.at( i+1 ) )
                for ( int j=il_PositionList.at( i-1 ) + 1; j<il_PositionList.at( i+1 ); j++ )
                    il_tempPositionList.append( j );
            else
                i++;
        }
    }

// ******************************************************************************************************

    il_PositionList.clear();

    if ( ( n = il_tempPositionList.count() ) < 1 )
        return( il_PositionList );

    switch ( mode )
    {
    case _DELETECOLUMNS:
        for ( int i=1; i<=i_maxNumOfPositions; i++ )
        {
            if ( il_tempPositionList.contains( i ) == false )
                il_PositionList.append( i-1 );
        }
        break;

    case _INSERTCHARS:
    case _REPLACECHARS:
        for ( int i=1; i<=i_maxNumOfPositions; i++ )
        {
            if ( il_tempPositionList.contains( i ) == true )
                il_PositionList.append( i-1 );
        }
        break;

    default:
        for ( int i=0; i<n; i++ )
        {
            if ( il_tempPositionList.at( i ) <= i_maxNumOfPositions )
                il_PositionList.append( il_tempPositionList.at( i ) - 1 );
        }
        break;
    }

    return( il_PositionList );
}

// ******************************************************************************************************
// ******************************************************************************************************
// ******************************************************************************************************

bool MainWindow::isInColumnList( const QList<int> il_ColumnList, const int i_ColumnNo )
{
    for ( int i=0; i<il_ColumnList.count(); i++ )
    {
        if ( i_ColumnNo == il_ColumnList.at( i ) )
            return( true );
    }

    return( false );
}
