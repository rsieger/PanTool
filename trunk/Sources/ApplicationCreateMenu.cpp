// ***********************************************************************************************
// *                                                                                             *
// * createMenu.cpp - creates application menus                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2008-05-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erstellen der Menue-Aktionen. */

void MainWindow::createActions()
{
// File menu
    newWindowAction = new QAction(trUtf8("&New window"), this);
    newWindowAction->setShortcut(trUtf8("Ctrl+N"));
    connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(chooseFiles()));

    openFolderAction = new QAction(tr("Select &Folder..."), this);
    openFolderAction->setShortcut(tr("Ctrl+F"));
    connect(openFolderAction, SIGNAL(triggered()), this, SLOT(chooseFolder()));

    saveAction = new QAction(trUtf8("&Save"), this);
    saveAction->setShortcut(trUtf8("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(trUtf8("Save &as..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    hideWindowAction = new QAction(tr("&Close window"), this);
    hideWindowAction->setShortcut(tr("Ctrl+W"));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hideWindow()));

    setOptionsAction = new QAction(trUtf8("General options..."), this);
    setOptionsAction->setShortcut(trUtf8("Ctrl+,"));
    connect(setOptionsAction, SIGNAL(triggered()), this, SLOT(doGeneralOptionsDialog()));

    exitAction = new QAction(trUtf8("&Quit"), this);
    exitAction->setShortcut(trUtf8("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitApplication()));

    // Basic tools

    extractColumnsAction = new QAction(trUtf8("Extract columns..."), this);
    connect(extractColumnsAction, SIGNAL(triggered()), this, SLOT(doExtractColumns()));

    extractMatchedColumnsAction = new QAction(trUtf8("Extract matched columns..."), this);
    connect(extractMatchedColumnsAction, SIGNAL(triggered()), this, SLOT(doExtractMatchedColumns()));

    extractLinesAction = new QAction(trUtf8("Extract lines..."), this);
    connect(extractLinesAction, SIGNAL(triggered()), this, SLOT(doExtractLines()));

    extractMatchedLinesAction = new QAction(trUtf8("Extract matched lines..."), this);
    connect(extractMatchedLinesAction, SIGNAL(triggered()), this, SLOT(doExtractMatchedLines()));

    extract10minLinesAction = new QAction(trUtf8("Extract 10 min lines..."), this);
    connect(extract10minLinesAction, SIGNAL(triggered()), this, SLOT(doExtract10minLines()));

    deleteColumnsAction = new QAction(trUtf8("Delete columns..."), this);
    connect(deleteColumnsAction, SIGNAL(triggered()), this, SLOT(doDeleteColumns()));

    deleteMatchedColumnsAction = new QAction(trUtf8("Delete matched columns..."), this);
    connect(deleteMatchedColumnsAction, SIGNAL(triggered()), this, SLOT(doDeleteMatchedColumns()));

    deleteLinesAction = new QAction(trUtf8("Delete lines..."), this);
    connect(deleteLinesAction, SIGNAL(triggered()), this, SLOT(doDeleteLines()));

    deleteMatchedLinesAction = new QAction(trUtf8("Delete matched lines..."), this);
    connect(deleteMatchedLinesAction, SIGNAL(triggered()), this, SLOT(doDeleteMatchedLines()));

    deleteCommentBlocksAction = new QAction(trUtf8("Delete comment blocks..."), this);
    connect(deleteCommentBlocksAction, SIGNAL(triggered()), this, SLOT(doDeleteCommentBlocks()));

    deleteDoubleLinesAction = new QAction(trUtf8("Delete double lines..."), this);
    connect(deleteDoubleLinesAction, SIGNAL(triggered()), this, SLOT(doDeleteDoubleLines()));

    searchOneStringAction = new QAction(trUtf8("Search one string..."), this);
    connect(searchOneStringAction, SIGNAL(triggered()), this, SLOT(doSearchOneString()));

    searchAndReplaceOneStringAction = new QAction(trUtf8("Search and replace one string..."), this);
    connect(searchAndReplaceOneStringAction, SIGNAL(triggered()), this, SLOT(doSearchAndReplaceOneString()));

    searchAndReplaceManyStringsAtOnceAction = new QAction(trUtf8("Search and replace many strings at once..."), this);
    connect(searchAndReplaceManyStringsAtOnceAction, SIGNAL(triggered()), this, SLOT(doSearchAndReplaceManyStringsAtOnce()));

    buildSearchAndReplaceDatabaseAction = new QAction(trUtf8("Build search and replace database"), this);
    connect(buildSearchAndReplaceDatabaseAction, SIGNAL(triggered()), this, SLOT(doBuildSearchAndReplaceDatabase()));

    insertCharactersAtPositionAction = new QAction(trUtf8("Insert characters at given positions..."), this);
    connect(insertCharactersAtPositionAction, SIGNAL(triggered()), this, SLOT(doInsertCharactersAtPosition()));

    replaceCharactersAtPositionAction = new QAction(trUtf8("Replace characters at given positions..."), this);
    connect(replaceCharactersAtPositionAction, SIGNAL(triggered()), this, SLOT(doReplaceCharactersAtPosition()));

    concatenateFilesByColumnsAction = new QAction(trUtf8("Concatenate files by columns..."), this);
    connect(concatenateFilesByColumnsAction, SIGNAL(triggered()), this, SLOT(doConcatenateFilesByColumns()));

    concatenateFilesByLinesAction = new QAction(trUtf8("Concatenate files by lines..."), this);
    connect(concatenateFilesByLinesAction, SIGNAL(triggered()), this, SLOT(doConcatenateFilesByLines()));

    splitFilesByColumnsAction = new QAction(trUtf8("Split file by columns..."), this);
    connect(splitFilesByColumnsAction, SIGNAL(triggered()), this, SLOT(doSplitFilesByColumns()));

    splitFilesByLinesAction = new QAction(trUtf8("Split file by lines..."), this);
    connect(splitFilesByLinesAction, SIGNAL(triggered()), this, SLOT(doSplitFilesByLines()));

    splitLargeFilesAction = new QAction(trUtf8("Split large file (> 100 MB) by lines..."), this);
    connect(splitLargeFilesAction, SIGNAL(triggered()), this, SLOT(doSplitLargeFiles()));

    recalcColumnsAction = new QAction(trUtf8("Recalculation of columns..."), this);
    connect(recalcColumnsAction, SIGNAL(triggered()), this, SLOT(doRecalcColumns()));

    addColumnAction = new QAction(trUtf8("Add column..."), this);
    connect(addColumnAction, SIGNAL(triggered()), this, SLOT(doAddColumn()));

    addLineAction = new QAction(trUtf8("Add text line..."), this);
    connect(addLineAction, SIGNAL(triggered()), this, SLOT(doAddLine()));

    addBlockAction = new QAction(trUtf8("Add text block..."), this);
    connect(addBlockAction, SIGNAL(triggered()), this, SLOT(doAddBlock()));

    translateCharacterEncodingAction = new QAction(trUtf8("Character encoding translation..."), this);
    connect(translateCharacterEncodingAction, SIGNAL(triggered()), this, SLOT(doTranslateCharacterEncoding()));

    transposeTableAction = new QAction(trUtf8("Transpose table..."), this);
    connect(transposeTableAction, SIGNAL(triggered()), this, SLOT(doTransposeTable()));

    createScriptAction = new QAction(trUtf8("Create script files"), this);
    connect(createScriptAction, SIGNAL(triggered()), this, SLOT(doCreateScript()));

    compressFolderZipAction = new QAction(trUtf8("Compress folder with zip"), this);
    connect(compressFolderZipAction, SIGNAL(triggered()), this, SLOT(doCompressFolderZip()));

    compressFolderGZipAction = new QAction(trUtf8("Compress folder with gz"), this);
    connect(compressFolderGZipAction, SIGNAL(triggered()), this, SLOT(doCompressFolderGZip()));

    compressFilesZipAction = new QAction(trUtf8("Compress files with zip"), this);
    connect(compressFilesZipAction, SIGNAL(triggered()), this, SLOT(doCompressFilesZip()));

    compressFilesGZipAction = new QAction(trUtf8("Compress files with gz"), this);
    connect(compressFilesGZipAction, SIGNAL(triggered()), this, SLOT(doCompressFilesGZip()));

    decompressFilesAction = new QAction(trUtf8("Decompress files"), this);
    connect(decompressFilesAction, SIGNAL(triggered()), this, SLOT(doDecompressFiles()));

    renameFilesAction = new QAction(trUtf8("Rename files..."), this);
    renameFilesAction->setShortcut(tr("Ctrl+R"));
    connect(renameFilesAction, SIGNAL(triggered()), this, SLOT(doRenameFiles()));

    saveFilelistAction = new QAction(trUtf8("Save list of files..."), this);
    connect(saveFilelistAction, SIGNAL(triggered()), this, SLOT(doSaveFilelist()));

    // Special tools

    getDatasetsAction = new QAction(trUtf8("Download PANGAEA datasets..."), this);
    getDatasetsAction->setShortcut(trUtf8("Ctrl+D"));
    connect(getDatasetsAction, SIGNAL(triggered()), this, SLOT(doGetDatasets()));

    convertMetadataXMAction = new QAction(trUtf8("Convert PANGAEA metadata XML..."), this);
    connect(convertMetadataXMAction, SIGNAL(triggered()), this, SLOT(doConvertMetadataXML()));

    checkTimeSeriesAction = new QAction(trUtf8("Check time series data"), this);
    connect(checkTimeSeriesAction, SIGNAL(triggered()), this, SLOT(doCheckTimeSeries()));

    calcDepthAction = new QAction(trUtf8("Pressure -> Depth"), this);
    connect(calcDepthAction, SIGNAL(triggered()), this, SLOT(doCalcDepthFromPressure()));

    calcSalinityAction = new QAction(trUtf8("Conductivity, temperature, pressure -> salinity"), this);
    connect(calcSalinityAction, SIGNAL(triggered()), this, SLOT(doCalcSalinityFromCondTempPress()));

    createDateTimeAction = new QAction(trUtf8("Create date/time..."), this);
    connect(createDateTimeAction, SIGNAL(triggered()), this, SLOT(doCreateDateTime()));

    createODPSampleLabelAction = new QAction(trUtf8("Create ODP sample label..."), this);
    connect(createODPSampleLabelAction, SIGNAL(triggered()), this, SLOT(doCreateODPSampleLabel()));

    findPolygonCentroidAction = new QAction(trUtf8("Find area centroid"), this);
    connect(findPolygonCentroidAction, SIGNAL(triggered()), this, SLOT(doFindPolygonCentroid()));

    findAreaAction = new QAction(trUtf8("Find area"), this);
    connect(findAreaAction, SIGNAL(triggered()), this, SLOT(doFindArea()));

    buildAreaDatabaseAction = new QAction(trUtf8("Build area database"), this);
    connect(buildAreaDatabaseAction, SIGNAL(triggered()), this, SLOT(doBuildAreaDatabase()));

    Table2ColumnsFormatAction = new QAction(trUtf8("Table -> columns"), this);
    connect(Table2ColumnsFormatAction, SIGNAL(triggered()), this, SLOT(doTable2ColumnsFormat()));

    Columns2TableFormatAction = new QAction(trUtf8("Columns -> table"), this);
    connect(Columns2TableFormatAction, SIGNAL(triggered()), this, SLOT(doColumns2TableFormat()));

    extractExifAction = new QAction(tr("Extract exif record from images..."), this);
    connect(extractExifAction, SIGNAL(triggered()), this, SLOT(doExtractExif()));

    writeExifAction = new QAction(tr("Write exif record to images..."), this);
    connect(writeExifAction, SIGNAL(triggered()), this, SLOT(doWriteExif()));

    // Help menu
    aboutAction = new QAction(trUtf8("&About ") + getApplicationName( true ), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(trUtf8("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    showHelpAction = new QAction(getApplicationName( true ) + trUtf8(" &Help"), this);
    showHelpAction->setShortcut(trUtf8("F1"));
    connect(showHelpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Verbindet Menues mit Aktionen. */

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( trUtf8( "&File" ) );
    fileMenu->addAction( openFileAction );
    fileMenu->addAction( openFolderAction );
    fileMenu->addSeparator();

#if defined(Q_OS_LINUX)
    ;
#endif

#if defined(Q_OS_MAC)
    fileMenu->addAction( newWindowAction );
    newWindowAction->setEnabled( false );
    fileMenu->addAction( hideWindowAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addAction( hideWindowAction );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( setOptionsAction );

    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );

// **********************************************************************************************

    basicToolsMenu = menuBar()->addMenu( trUtf8( "Basic tools" ) );

    basicToolsMenu->addAction( extractColumnsAction );
    basicToolsMenu->addAction( extractMatchedColumnsAction );
    basicToolsMenu->addAction( extractLinesAction );
    basicToolsMenu->addAction( extractMatchedLinesAction );
    basicToolsMenu->addAction( extract10minLinesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( deleteColumnsAction );
    basicToolsMenu->addAction( deleteMatchedColumnsAction );
    basicToolsMenu->addAction( deleteLinesAction );
    basicToolsMenu->addAction( deleteMatchedLinesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( deleteCommentBlocksAction );
    basicToolsMenu->addAction( deleteDoubleLinesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( searchOneStringAction );
    basicToolsMenu->addAction( searchAndReplaceOneStringAction );
    basicToolsMenu->addAction( searchAndReplaceManyStringsAtOnceAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( insertCharactersAtPositionAction );
    basicToolsMenu->addAction( replaceCharactersAtPositionAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( concatenateFilesByColumnsAction );
    basicToolsMenu->addAction( concatenateFilesByLinesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( splitFilesByColumnsAction );
    basicToolsMenu->addAction( splitFilesByLinesAction );
    basicToolsMenu->addAction( splitLargeFilesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( addColumnAction );
    basicToolsMenu->addAction( addLineAction );
    basicToolsMenu->addAction( addBlockAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( recalcColumnsAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( transposeTableAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( translateCharacterEncodingAction );
    basicToolsMenu->addAction( renameFilesAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( saveFilelistAction );
    basicToolsMenu->addAction( createScriptAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( compressFolderZipAction );
    basicToolsMenu->addAction( compressFilesZipAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( compressFolderGZipAction );
    basicToolsMenu->addAction( compressFilesGZipAction );
    basicToolsMenu->addSeparator();
    basicToolsMenu->addAction( decompressFilesAction );

// **********************************************************************************************

    specialToolsMenu = menuBar()->addMenu( trUtf8( "Special tools" ) );

    specialToolsMenu->addAction( getDatasetsAction );
    specialToolsMenu->addAction( convertMetadataXMAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( calcDepthAction );
    specialToolsMenu->addAction( calcSalinityAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( createDateTimeAction );
    specialToolsMenu->addAction( createODPSampleLabelAction );
    specialToolsMenu->addAction( checkTimeSeriesAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( findAreaAction );
    specialToolsMenu->addAction( findPolygonCentroidAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( Table2ColumnsFormatAction );
    specialToolsMenu->addAction( Columns2TableFormatAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( buildAreaDatabaseAction );
    specialToolsMenu->addAction( buildSearchAndReplaceDatabaseAction );
    specialToolsMenu->addSeparator();
    specialToolsMenu->addAction( extractExifAction );
    specialToolsMenu->addAction( writeExifAction );

// **********************************************************************************************

    helpMenu = menuBar()->addMenu( trUtf8( "&Help" ) );
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
    helpMenu->addSeparator();
    helpMenu->addAction( showHelpAction );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::enableMenuItems( const QStringList &sl_FilenameList )
{
    bool b_containsBinaryFile = containsBinaryFile( sl_FilenameList );

// **********************************************************************************************

    QList<QAction*> basicToolsMenuActions = basicToolsMenu->actions();

    if ( b_containsBinaryFile == false )
    {
        for ( int i=0; i<basicToolsMenuActions.count(); ++i )
            basicToolsMenuActions.at( i )->setEnabled( true );
    }
    else
    {
        for ( int i=0; i<basicToolsMenuActions.count(); ++i )
            basicToolsMenuActions.at( i )->setEnabled( false );

        compressFolderZipAction->setEnabled( true );
        compressFilesZipAction->setEnabled( true );
        compressFolderGZipAction->setEnabled( true );
        compressFilesGZipAction->setEnabled( true );
        decompressFilesAction->setEnabled( true );

        renameFilesAction->setEnabled( true );
        saveFilelistAction->setEnabled( true );
    }

// **********************************************************************************************

    QList<QAction*> specialToolsMenuActions = specialToolsMenu->actions();

    if ( b_containsBinaryFile == false )
    {
        for ( int i=0; i<specialToolsMenuActions.count(); ++i )
            specialToolsMenuActions.at( i )->setEnabled( true );
    }
    else
    {
        for ( int i=0; i<specialToolsMenuActions.count(); ++i )
            specialToolsMenuActions.at( i )->setEnabled( false );

        getDatasetsAction->setEnabled( true );
        extractExifAction->setEnabled( true );
    }
}
