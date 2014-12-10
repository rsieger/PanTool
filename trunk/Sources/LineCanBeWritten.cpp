/* 2011-07-08                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-11

bool MainWindow::LineCanBeWritten( const QString &InputStr, const bool b_SkipEmptyLines, const bool b_SkipCommentLines )
{
    int i_LineCanBeWritten = false;

    if ( ( InputStr.startsWith( "//") == false ) && ( InputStr.startsWith( "*" ) == false ) && ( InputStr.startsWith( "#" ) == false ) && ( InputStr.startsWith( "!" ) == false ) )
    {
        if ( ( InputStr.count( "\t" ) == InputStr.length() ) || ( InputStr.isEmpty() == true ) )
        {
            if ( b_SkipEmptyLines == false )
                i_LineCanBeWritten = true;  // Line is empty but SkipEmptyLine is true
        }
        else
        {
            i_LineCanBeWritten = true; // Line is not empty
        }
    }
    else
    {
         if ( b_SkipCommentLines == false )
             i_LineCanBeWritten = true; // Line is a comment but SkipCommentLines is false
    }

    return( i_LineCanBeWritten );
}


