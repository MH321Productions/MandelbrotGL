cd ../../bin/Debug/Extra
echo Starte Generierung von ResourceHandler
cmake ../../../src/Extra -DCMAKE_BUILD_TYPE=Debug
make
echo Starte ResourceHandler
./ResourceCompiler

echo Starte Kompilierung Hauptprogramm
cd ..
cmake ../../ -DCMAKE_BUILD_TYPE=RelWithDebInfo
make