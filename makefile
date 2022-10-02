CXX=g++ -std=c++20
LIBS=`pkg-config --libs gtkmm-3.0`
FLAGS=`pkg-config --cflags gtkmm-3.0`

all: clean res
	@mkdir build
	@mkdir build/bin
	@${CXX} -Wall -o CardGen main.cpp src/*.cpp ${FLAGS} ${LIBS} -pthread
	@cp CardGen build/bin/
	@cp -R res/* build/
icon: clean res
	@windres.exe src/icon.rc src/icon.o
	@${CXX} -Wall -o CardGen main.cpp src/*.cpp src/icon.o ${FLAGS} ${LIBS} -pthread -mwindows
	@mkdir build
	@mkdir build/bin
	@./tools/get_dll.sh
	@cp CardGen.exe build/bin/
	@cp -R res/* build/
appimg:
	@mkdir CardGen.AppDir
	@mkdir CardGen.AppDir/usr
	@mkdir CardGen.AppDir/usr/bin
	@mkdir CardGen.AppDir/usr/lib
	@cp CardGen CardGen.AppDir/usr/bin/
	@cp tools/AppRun CardGen.AppDir/
	@cp src/template.bmp CardGen.AppDir/usr/
	@cp src/CardGen.desktop CardGen.AppDir/
	@cp src/CardGen.png CardGen.AppDir/
	@./tools/get_libs.sh
	@./tools/appimagetool* CardGen.AppDir/ CardGen.AppImage
	@rm -rf CardGen.AppDir
clean:
	@rm -rf build/
res:
	@cd src; glib-compile-resources --target=resources.cpp --generate-source resources.gresource.xml; cd ..
