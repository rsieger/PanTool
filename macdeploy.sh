#!/bin/bash
echo
echo PanTool

echo - macdeployqt

cd ~/Development/GitHub/PanTool

rm -R '../../Distribution/PanTool/PanTool.app'
cp -R './build-PanTool-Desktop_Qt_5_9_2_clang_64bit-Release/PanTool.app' '../../Distribution/PanTool/PanTool.app'
cp './trunk/Resources/Info.plist' '../../Distribution/PanTool/PanTool.app/Contents/Info.plist'

/Developer/Qt/5.9.2/clang_64/bin/macdeployqt '../../Distribution/PanTool/PanTool.app'

echo - code signing

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtCore.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtGui.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtNetwork.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtPrintSupport.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtWidgets.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtSvg.framework'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/Frameworks/QtXml.framework'

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/bearer/libqcorewlanbearer.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/bearer/libqgenericbearer.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqgif.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqicns.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqico.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqjpeg.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqmacjp2.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqtga.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqtiff.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqwbmp.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/imageformats/libqwebp.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/platforms/libqcocoa.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/printsupport/libcocoaprintersupport.dylib'
codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app/Contents/PlugIns/iconengines/libqsvgicon.dylib'

codesign --force --verify --sign 'Developer ID Application: Alfred-Wegener-Institut fur Polar- und Meeresforschung (AWI)' '../../Distribution/PanTool/PanTool.app'

echo - mount package

cd ~/Development/Distribution/PanTool
hdiutil attach ~/Development/Distribution/PanTool_OSX.dmg

rm -R '/Volumes/PanTool/PanTool.app'
cp -R PanTool.app '/Volumes/PanTool'

cd '/Volumes/PanTool'
rm -rf .fseventsd
mkdir .fseventsd
touch .fseventsd/no_log
cd ~/Development/Distribution/PanTool

echo - verify package

codesign -d '/Volumes/PanTool/PanTool.app'

echo
hdiutil detach '/Volumes/PanTool'

echo - move application

rm -R /Applications/PanTool.app
cp -R PanTool.app /Applications
cd ~/Development/Distribution

echo - finished
