# PanTool - the swiss army knife of PANGAEA
# Rainer Sieger
# Alfred Wegener Institute, Bremerhaven, Germany
# last change: 2014-10-04

macx {
    message( "MacOS X" )

    # creating cache file
    cache()

    # Set SDK
    QMAKE_MAC_SDK = macosx10.9

    # Only Intel binaries are accepted so force this
    CONFIG += x86_64

    # Minimum OS X version for submission is 10.6.6
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

    # Icon
    ICON = ./Resources/icon/Application.icns

    # Replace default Info.plist
    QMAKE_INFO_PLIST = ./Resources/Info.plist

    QMAKE_CFLAGS += -gdwarf-2
    QMAKE_CXXFLAGS += -gdwarf-2
    QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_OBJECTIVE_CFLAGS_RELEASE =  $$QMAKE_OBJECTIVE_CFLAGS_RELEASE_WITH_DEBUGINFO
    QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

win32 {
    message( "Windows" )

    # Icon
    RC_FILE = ./Resources/Application.rc
}

linux-g++ {
    message( "Linux" )
}

TARGET = PanTool
TEMPLATE = app
QT += widgets network

INCLUDEPATH += ./Headers

HEADERS = ./Headers/Application.h \
    ./Headers/Globals.h \
    ./Headers/Ellipsoid.h \
    ./Headers/findArea.h \
    ./Headers/Webfile.h \
    ./Forms/AboutDialog/AboutDialog.h \
    ./Forms/AddDialog/AddDialog.h \
    ./Forms/CharactersAtPositionDialog/CharactersAtPositionDialog.h \
    ./Forms/ConcatenateFilesOptionsDialog/ConcatenateFilesOptionsDialog.h \
    ./Forms/ColumnListDialog/ColumnListDialog.h \
    ./Forms/FindAreaDialog/FindAreaDialog.h \
    ./Forms/FilenameFolderDialog/FilenameFolderDialog.h \
    ./Forms/FourColumnsDialog/FourColumnsDialog.h \
    ./Forms/ExtractLinesDialog/ExtractLinesDialog.h \
    ./Forms/DateTimeDialog/DateTimeDialog.h \
    ./Forms/DeleteLinesDialog/DeleteLinesDialog.h \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.h \
    ./Forms/LatLongOptionsDialog/LatLongOptionsDialog.h \
    ./Forms/ODPSampleLabelDialog/ODPSampleLabelDialog.h \
    ./Forms/PanGetDialog/PanGetDialog.h \
    ./Forms/RecalcDialog/RecalcDialog.h \
    ./Forms/RenameFilesDialog/RenameFilesDialog.h \
    ./Forms/SearchDialog/SearchDialog.h \
    ./Forms/SkipLinesDialog/SkipLinesDialog.h \
    ./Forms/SplitFileDialog/SplitFileDialog.h \
    ./Forms/TranslateCharacterEncodingDialog/TranslateCharacterEncodingDialog.h \
    ./Forms/TransposeTableOptionsDialog/TransposeTableOptionsDialog.h

SOURCES = ./Sources/ApplicationInit.cpp \
    ./Sources/ApplicationMainWindow.cpp \
    ./Sources/ApplicationErrors.cpp \
    ./Sources/ApplicationPreferences.cpp \
    ./Sources/ApplicationCreateMenu.cpp \
    ./Sources/addColumn.cpp \
    ./Sources/addLine.cpp \
    ./Sources/buildSearchAndReplaceDatabase.cpp \
    ./Sources/calcDepthFromPressure.cpp \
    ./Sources/calcSalinityFromCondTempPress.cpp \
    ./Sources/checkTimeSeries.cpp \
    ./Sources/compressFiles.cpp \
    ./Sources/concatenateFiles.cpp \
    ./Sources/createDateTime.cpp \
    ./Sources/createODPSampleLabel.cpp \
    ./Sources/createScript.cpp \
    ./Sources/extractColumns.cpp \
    ./Sources/extractMatchedColumns.cpp \
    ./Sources/extractLines.cpp \
    ./Sources/extractMatchedLines.cpp \
    ./Sources/extract10minLines.cpp \
    ./Sources/findArea.cpp \
    ./Sources/findPolygonCentroid.cpp \
    ./Sources/fourColumnConverter.cpp \
    ./Sources/deleteColumns.cpp \
    ./Sources/deleteMatchedColumns.cpp \
    ./Sources/deleteLines.cpp \
    ./Sources/deleteMatchedLines.cpp \
    ./Sources/deleteCommentBlocks.cpp \
    ./Sources/deleteDoubleLines.cpp \
    ./Sources/insertCharactersAtPosition.cpp \
    ./Sources/recalcColumns.cpp \
    ./Sources/renameFiles.cpp \
    ./Sources/replaceCharactersAtPosition.cpp \
    ./Sources/saveFilelist.cpp \
    ./Sources/transposeTable.cpp \
    ./Sources/scanList.cpp \
    ./Sources/searchOneString.cpp \
    ./Sources/searchAndReplaceOneString.cpp \
    ./Sources/searchAndReplaceManyStringsAtOnce.cpp \
    ./Sources/splitFileByLines.cpp \
    ./Sources/splitFileByColumns.cpp \
    ./Sources/LineCanBeWritten.cpp \
    ./Sources/Webfile.cpp \
    ./Forms/AboutDialog/AboutDialog.cpp \
    ./Forms/AddDialog/AddDialog.cpp \
    ./Forms/CharactersAtPositionDialog/CharactersAtPositionDialog.cpp \
    ./Forms/ColumnListDialog/ColumnListDialog.cpp \
    ./Forms/ConcatenateFilesOptionsDialog/ConcatenateFilesOptionsDialog.cpp \
    ./Forms/DateTimeDialog/DateTimeDialog.cpp \
    ./Forms/DeleteLinesDialog/DeleteLinesDialog.cpp \
    ./Forms/ExtractLinesDialog/ExtractLinesDialog.cpp \
    ./Forms/FindAreaDialog/FindAreaDialog.cpp \
    ./Forms/FilenameFolderDialog/FilenameFolderDialog.cpp \
    ./Forms/FourColumnsDialog/FourColumnsDialog.cpp \
    ./Forms/GeneralOptionsDialog/GeneralOptionsDialog.cpp \
    ./Forms/LatLongOptionsDialog/LatLongOptionsDialog.cpp \
    ./Forms/ODPSampleLabelDialog/ODPSampleLabelDialog.cpp \
    ./Forms/PanGetDialog/PanGetDialog.cpp \
    ./Forms/RecalcDialog/RecalcDialog.cpp \
    ./Forms/RenameFilesDialog/RenameFilesDialog.cpp \
    ./Forms/SearchDialog/SearchDialog.cpp \
    ./Forms/SkipLinesDialog/SkipLinesDialog.cpp \
    ./Forms/SplitFileDialog/SplitFileDialog.cpp \
    ./Forms/TranslateCharacterEncodingDialog/TranslateCharacterEncodingDialog.cpp \
    ./Forms/TransposeTableOptionsDialog/TransposeTableOptionsDialog.cpp

FORMS = ./Forms/AboutDialog/aboutdialog.ui \
    ./Forms/AddDialog/adddialog.ui \
    ./Forms/CharactersAtPositionDialog/charactersatpositiondialog.ui \
    ./Forms/ColumnListDialog/columnlistdialog.ui \
    ./Forms/ConcatenateFilesOptionsDialog/concatenatefilesoptionsdialog.ui \
    ./Forms/DateTimeDialog/datetimedialog.ui \
    ./Forms/DeleteLinesDialog/deletelinesdialog.ui \
    ./Forms/ExtractLinesDialog/extractlinesdialog.ui \
    ./Forms/FindAreaDialog/findareadialog.ui \
    ./Forms/FilenameFolderDialog/filenamefolderdialog.ui \
    ./Forms/FourColumnsDialog/fourcolumnsdialog.ui \
    ./Forms/GeneralOptionsDialog/generaloptionsdialog.ui \
    ./Forms/LatLongOptionsDialog/latlongoptionsdialog.ui \
    ./Forms/ODPSampleLabelDialog/odpsamplelabeldialog.ui \
    ./Forms/PanGetDialog/pangetdialog.ui \
    ./Forms/RecalcDialog/recalcdialog.ui \
    ./Forms/RenameFilesDialog/renamefilesdialog.ui \
    ./Forms/SearchDialog/searchdialog.ui \
    ./Forms/SkipLinesDialog/skiplinesdialog.ui \
    ./Forms/SplitFileDialog/splitfiledialog.ui \
    ./Forms/TranslateCharacterEncodingDialog/translatecharacterencodingdialog.ui \
    ./Forms/TransposeTableOptionsDialog/transposetableoptionsdialog.ui

