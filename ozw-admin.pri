top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

unix {
    CONFIG-= no-pkg-config
    CONFIG+= link_pkgconfig
    !isEmpty(OZW_LIB_PATH) {
	!exists($$OZW_LIB_PATH/libopenzwave.so): error("Can't find libopenzwave.so")
	!exists($$OZW_INCLUDE_PATH/Manager.h) {
		!exists($$OZW_LIB_PATH/cpp/src/Manager.h) {
			error("Can't find Manager.h")
		} else {
			OZW_INCLUDE_PATH=$$OZW_LIB_PATH/cpp/src/
		}
	}
	!exists($$OZW_DATABASE_PATH/manufacturer_specific.xml) {
		!exists($$OZW_LIB_PATH/config/manufacturer_specific.xml) {
			error("Can't find manufacturer_specific.xml")
		} else {
			OZW_DATABASE_PATH=$$absolute_path($$OZW_LIB_PATH/config/)
		}
	}
	OZW_LIB_PATH=$$absolute_path($$OZW_LIB_PATH)
	OZW_INCLUDE_PATH=$$absolute_path($$OZW_INCLUDE_PATH)
	OZW_LIBS="-L$$OZW_LIB_PATH"
	OZW_LIBS+="-lopenzwave"
	OZW_DATABASE_PATH=$$absolute_path($$OZW_DATABASE_PATH)
	message(" ")
    	message("OpenZWave Summary:")
    	message("    OpenZWave Library Path: $$OZW_LIB_PATH")
    	message("    OpenZWave Include Path: $$OZW_INCLUDE_PATH")
    	message("    OpenZWave Libs Command: $$OZW_LIBS")
    	message("    OpenZWave Database Path: $$OZW_DATABASE_PATH")
    	message(" ")
    	INCLUDEPATH+=$$OZW_INCLUDE_PATH
    	LIBS+=$$OZW_LIBS
    } else { 
	exists( $$top_srcdir/../open-zwave/cpp/src/) {
		message("Found OZW in $$absolute_path($$top_srcdir/../open-zwave/)")
               	OZW_LIB_PATH = "$$absolute_path($$top_srcdir/../open-zwave/)"
               	OZW_INCLUDE_PATH = $$absolute_path($$top_srcdir/../open-zwave/cpp/src/)
		OZW_LIBS="-L$$OZW_LIB_PATH"
		OZW_LIBS+="-lopenzwave"
		OZW_DATABASE_PATH = $$absolute_path($$top_srcdir/../open-zwave/config/)
		message(" ")
    		message("OpenZWave Summary:")
    		message("    OpenZWave Library Path: $$OZW_LIB_PATH")
    		message("    OpenZWave Include Path: $$OZW_INCLUDE_PATH")
    		message("    OpenZWave Libs Command: $$OZW_LIBS")
    		message("    OpenZWave Database Path: $$OZW_DATABASE_PATH")
    		message(" ")
    		INCLUDEPATH+=$$OZW_INCLUDE_PATH
    		LIBS+=$$OZW_LIBS
	} else {
		packagesExist("libopenzwave") {
			PKGCONFIG += libopenzwave
			OZW_DATABASE_PATH=$$system($$PKG_CONFIG --variable=sysconfdir libopenzwave)
	    		message(" ")
    			message("OpenZWave Summary:")
			message("	Using OpenZWave from pkg-config:")
			message("	CXXFLAGS: $$system($$PKG_CONFIG --cflags libopenzwave)")
			message("	LDFLAGS: $$system($$PKG_CONFIG --libs libopenzwave)")
			message("	DATABASE: $$system($$PKG_CONFIG --variable=sysconfdir libopenzwave)")
			message(" ")
		} else {
			error("Can't find OpenZWave Library and Headers");
		}
	}
    }

    !isEmpty(QTOZW_LIB_PATH) {
	!exists($$QTOZW_LIB_PATH/qt-openzwave/libqt-openzwave.so): error("Can't find libqt-openzwave.so")
	!exists($$QTOZW_INCLUDE_PATH/qt-openzwave/qtopenzwave.h) {
		!exists($$QTOZW_LIB_PATH/qt-openzwave/include/qt-openzwave/qtopenzwave.h) {
			error("Can't find Manager.h")
		} else {
			QTOZW_INCLUDE_PATH=$$QTOZW_LIB_PATH/qt-openzwave/include/
		}
	}
	QTOZW_LIB_PATH=$$absolute_path($$QTOZW_LIB_PATH)
	QTOZW_INCLUDE_PATH=$$absolute_path($$QTOZW_INCLUDE_PATH)
	QTOZW_INCLUDE_PATH+=$$absolute_path($$QTOZW_INCLUDE_PATH/../../qt-openzwavedatabase/include/)
	QTOZW_LIBS="-L$$absolute_path($$QTOZW_LIB_PATH/qt-openzwave/)"
	QTOZW_LIBS+="-lqt-openzwave"
	QTOZW_LIBS+="-L$$absolute_path($$QTOZW_LIB_PATH/qt-openzwavedatabase/)"
	QTOZW_LIBS+="-lqt-openzwavedatabase"
	message(" ")
    	message("QT-OpenZWave Summary:")
    	message("    QT-OpenZWave Library Path: $$QTOZW_LIB_PATH")
    	message("    QT-OpenZWave Include Path: $$QTOZW_INCLUDE_PATH")
    	message("    Qt-OpenZWave Libs Command: $$QTOZW_LIBS")
    	message(" ")
    	INCLUDEPATH+=$$QTOZW_INCLUDE_PATH
    	LIBS+=$$QTOZW_LIBS
    } else { 
	exists($$top_srcdir/../qt-openzwave/qt-openzwave/include/qt-openzwave/qtopenzwave.h) {
		message("Found QTOZW in $$absolute_path($$top_srcdir/../qt-openzwave/)")
               	QTOZW_LIB_PATH = "$$absolute_path($$top_srcdir/../qt-openzwave/qt-openzwave/)"
               	QTOZW_INCLUDE_PATH = $$absolute_path($$top_srcdir/../qt-openzwave/qt-openzwave/include/)
               	QTOZW_INCLUDE_PATH += $$absolute_path($$top_srcdir/../qt-openzwave/qt-openzwavedatabase/include/)
		QTOZW_LIBS="-L$$QTOZW_LIB_PATH"
		QTOZW_LIBS+="-lqt-openzwave"
		QTOZW_LIBS+="-L$$absolute_path($$OTOZW_LIB_PATH/../qt-openzwavedatabase/)"
		QTZOW_LIBS+="-lqt-openzwavedatabase"
		message(" ")
    		message("QT-OpenZWave Summary:")
    		message("    QT-OpenZWave Library Path: $$QTOZW_LIB_PATH")
    		message("    QT-OpenZWave Include Path: $$QTOZW_INCLUDE_PATH")
    		message("    QT-OpenZWave Libs Command: $$QTOZW_LIBS")
    		message(" ")
    		INCLUDEPATH+=$$QTOZW_INCLUDE_PATH
    		LIBS+=$$QTOZW_LIBS
	} else {
		packagesExist("libqt-openzwave") {
			PKGCONFIG += libqt-openzwave
	    		message(" ")
    			message("QT-OpenZWave Summary:")
			message("	Using QT-OpenZWave from pkg-config:")
			message("	CXXFLAGS: $$system($$PKG_CONFIG --cflags libqt-openzwave)")
			message("	LDFLAGS: $$system($$PKG_CONFIG --libs libqt-openzwave)")
			message(" ")
		} else {
			error("Can't find QT-OpenZWave Library and Headers");
		}
	}
    }


}

win32 {
    CONFIG(debug, debug|release) {
        BUILDTYPE = debug
    } else {
        BUILDTYPE = release
    }
    message(Checking for $$BUILDTYPE build of OZW)
    exists( $$top_srcdir/../open-zwave/cpp/src/) {
        message("Found OZW in $$absolute_path($$top_srcdir/../open-zwave/cpp/src)")
        INCLUDEPATH += $$absolute_path($$top_srcdir/../open-zwave/cpp/src/)/
        equals(BUILDTYPE, "release") {
            exists( $$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/ReleaseDLL/OpenZWave.dll ) ) {
                LIBS += -L$$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/ReleaseDLL) -lopenzwave
                OZW_LIB_PATH = $$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/ReleaseDLL)
            } else {
                error("Can't find a copy of OpenZWave.dll in the ReleaseDLL Directory");
            }
        } else {
            equals(BUILDTYPE, "debug") {
                exists ( $$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/DebugDLL/OpenZWaved.dll )) {
                    LIBS += -L$$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/DebugDLL) -lOpenZWaved
                    OZW_LIB_PATH = $$absolute_path($$top_srcdir/../open-zwave/cpp/build/windows/vs2010/ReleaseDLL)
            } else {
                    error("Can't find a copy of OpenZWaved.dll in the DebugDLL  Directory");
                }
            }
        }
        isEmpty(OZW_LIB_PATH) {
            error("Can't find a copy of OpenZWave with the right builds");
        }
    } else {
        error("Can't Find a copy of OpenZwave")
    }
}
