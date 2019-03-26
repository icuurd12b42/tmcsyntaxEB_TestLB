mkdir bin
mkdir bin\x64
mkdir bin\x64\Release
mkdir bin\x64\Debug
copy ..\tmcsyntaxEB\Bin\x64\Release\*.* bin\x64\Release
copy ..\tmcsyntaxEB\Bin\x64\Debug\*.* bin\x64\Debug

mkdir bin\x86
mkdir bin\x86\Release
mkdir bin\x86\Debug
copy ..\tmcsyntaxEB\Bin\x86\Release\*.* bin\x86\Release
copy ..\tmcsyntaxEB\Bin\x86\Debug\*.* bin\x86\Debug