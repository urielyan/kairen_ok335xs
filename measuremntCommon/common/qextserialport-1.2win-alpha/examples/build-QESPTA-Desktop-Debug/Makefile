#############################################################################
# Makefile for building: QESPTA
# Generated by qmake (2.01a) (Qt 4.8.1) on: Wed Sep 16 09:03:53 2020
# Project:  ../qespta/QESPTA.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../qespta/QESPTA.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -D_TTY_POSIX_ -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I../qespta -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I../../../qextserialport-1.2win-alpha -Imoc -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L../../build -L/usr/lib/i386-linux-gnu -lqextserialportd -lpthread -lQtGui -lQtCore 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = ../qespta/main.cpp \
		../qespta/MainWindow.cpp \
		../qespta/MessageWindow.cpp \
		../qespta/QespTest.cpp moc/moc_MainWindow.cpp \
		moc/moc_MessageWindow.cpp \
		moc/moc_QespTest.cpp
OBJECTS       = obj/main.o \
		obj/MainWindow.o \
		obj/MessageWindow.o \
		obj/QespTest.o \
		obj/moc_MainWindow.o \
		obj/moc_MessageWindow.o \
		obj/moc_QespTest.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		../qespta/QESPTA.pro
QMAKE_TARGET  = QESPTA
DESTDIR       = 
TARGET        = QESPTA

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

Makefile: ../qespta/QESPTA.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../qespta/QESPTA.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile ../qespta/QESPTA.pro

dist: 
	@$(CHK_DIR_EXISTS) obj/QESPTA1.0.0 || $(MKDIR) obj/QESPTA1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) obj/QESPTA1.0.0/ && $(COPY_FILE) --parents ../qespta/MainWindow.h ../qespta/MessageWindow.h ../qespta/QespTest.h obj/QESPTA1.0.0/ && $(COPY_FILE) --parents ../qespta/main.cpp ../qespta/MainWindow.cpp ../qespta/MessageWindow.cpp ../qespta/QespTest.cpp obj/QESPTA1.0.0/ && (cd `dirname obj/QESPTA1.0.0` && $(TAR) QESPTA1.0.0.tar QESPTA1.0.0 && $(COMPRESS) QESPTA1.0.0.tar) && $(MOVE) `dirname obj/QESPTA1.0.0`/QESPTA1.0.0.tar.gz . && $(DEL_FILE) -r obj/QESPTA1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc/moc_MainWindow.cpp moc/moc_MessageWindow.cpp moc/moc_QespTest.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc/moc_MainWindow.cpp moc/moc_MessageWindow.cpp moc/moc_QespTest.cpp
moc/moc_MainWindow.cpp: ../qespta/MainWindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../qespta/MainWindow.h -o moc/moc_MainWindow.cpp

moc/moc_MessageWindow.cpp: ../qespta/MessageWindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../qespta/MessageWindow.h -o moc/moc_MessageWindow.cpp

moc/moc_QespTest.cpp: ../qespta/QespTest.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) ../qespta/QespTest.h -o moc/moc_QespTest.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: uic/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) uic/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

obj/main.o: ../qespta/main.cpp ../qespta/defs.h \
		../qespta/MainWindow.h \
		../qespta/MessageWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o ../qespta/main.cpp

obj/MainWindow.o: ../qespta/MainWindow.cpp ../qespta/defs.h \
		../qespta/MainWindow.h \
		../qespta/MessageWindow.h \
		../qespta/QespTest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/MainWindow.o ../qespta/MainWindow.cpp

obj/MessageWindow.o: ../qespta/MessageWindow.cpp ../qespta/MessageWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/MessageWindow.o ../qespta/MessageWindow.cpp

obj/QespTest.o: ../qespta/QespTest.cpp ../qespta/QespTest.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/QespTest.o ../qespta/QespTest.cpp

obj/moc_MainWindow.o: moc/moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/moc_MainWindow.o moc/moc_MainWindow.cpp

obj/moc_MessageWindow.o: moc/moc_MessageWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/moc_MessageWindow.o moc/moc_MessageWindow.cpp

obj/moc_QespTest.o: moc/moc_QespTest.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/moc_QespTest.o moc/moc_QespTest.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

