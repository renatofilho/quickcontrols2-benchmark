This app is used to measure the time spent on font querying after import
"QtQuick.Controls 2.2".

After importing "QtQuick.Controls 2.2" the platform theme is replaced by a
default QtQuickTheme, and that new theme rely on QSettings values that is not
set on some environments (not set on My Ubuntu 17.04), because of that the 
theme return invalid fonts which causes the 'QFontaDatabase::systemFont' to 
fallback and use fontconfig to find system default font, which is very slow.

You can measure that using this app:

'queryFontFromNativeTheme': will query for system fonts before import 'QtQuick.Controls'
'queryFontAfterImportControls': will query for system fonts after import 'QtQuick.Controls'

to run the app:
./quickcontrols2-benchmark <num-interactions>
