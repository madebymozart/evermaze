"C:\Program Files\7-Zip\7z" a -tzip -p14611769812890929240 -r -mx9 -mm=Deflate -mfb=128 -mpass10 -mmt=on %~dp0..\xScripts\data %~dp0..\xScripts\
ren "%~dp0..\xScripts\data.zip" "evz.archive"
move "%~dp0..\xScripts\evz.archive" "%~dp0..\Resources"