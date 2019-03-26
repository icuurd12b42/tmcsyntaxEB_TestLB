mkdir bin
mkdir bin\x64
mkdir bin\x64\Release
mkdir bin\x64\Release\SyntaxEBLangFiles
mkdir bin\x64\Debug
mkdir bin\x64\Debug\SyntaxEBLangFiles
copy ..\SyntaxEBLangFiles\SyntaxEBLangFiles\*.* bin\x64\Release\SyntaxEBLangFiles
copy ..\SyntaxEBLangFiles\SyntaxEBLangFiles\*.* bin\x64\Debug\SyntaxEBLangFiles

mkdir bin\x86
mkdir bin\x86\Release
mkdir bin\x86\Release\SyntaxEBLangFiles
mkdir bin\x86\Debug
mkdir bin\x86\Debug\SyntaxEBLangFiles
copy ..\SyntaxEBLangFiles\SyntaxEBLangFiles\*.* bin\x86\Release\SyntaxEBLangFiles
copy ..\SyntaxEBLangFiles\SyntaxEBLangFiles\*.* bin\x86\Debug\SyntaxEBLangFiles