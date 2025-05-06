#!/usr/bin/bash

dir_build=./build
ls=/usr/bin/ls

prefix64=/usr/x86_64-w64-mingw32
prefix32=/usr/i686-w64-mingw32
libwinpthread=/sys-root/mingw/bin/libwinpthread-1.dll
libstdcpp=/sys-root/mingw/bin/libstdc++-6.dll
libgcc=/sys-root/mingw/bin/libgcc_s*.dll

for i in $($ls $dir_build | grep 64)
do
    echo "Copying dependencies to $i"
    cp $prefix64/$libwinpthread $dir_build/$i
    cp $prefix64/$libstdcpp $dir_build/$i
    cp $prefix64/$libgcc $dir_build/$i
done

for i in $($ls $dir_build | grep 32)
do
    echo "Copying dependencies to $i"
    cp $prefix32/$libwinpthread $dir_build/$i
    cp $prefix32/$libstdcpp $dir_build/$i
    cp $prefix32/$libgcc $dir_build/$i
done

echo "Done"
