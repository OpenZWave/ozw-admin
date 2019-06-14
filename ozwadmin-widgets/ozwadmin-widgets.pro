#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T16:10:29
#
#-------------------------------------------------

QT       += core gui xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ozwadmin-widgets
TEMPLATE = lib
CONFIG += staticlib


SOURCES += widgets.cpp \
    bitsetwidget.cpp \
    node_delegate.cpp \
    nodeflagswidget.cpp \
    propertybrowser/qtbuttonpropertybrowser.cpp \
    propertybrowser/qteditorfactory.cpp \
    propertybrowser/qtgroupboxpropertybrowser.cpp \
    propertybrowser/qtpropertybrowser.cpp \
    propertybrowser/qtpropertybrowserutils.cpp \
    propertybrowser/qtpropertymanager.cpp \
    propertybrowser/qttreepropertybrowser.cpp \
    propertybrowser/qtvariantproperty.cpp \
    value_delegate.cpp

HEADERS  += widgets.h \
    bitsetwidget.h \
    node_delegate.h \
    nodeflagswidget.h \
    propertybrowser/QtAbstractEditorFactoryBase \
    propertybrowser/QtAbstractPropertyBrowser \
    propertybrowser/QtAbstractPropertyManager \
    propertybrowser/QtBoolPropertyManager \
    propertybrowser/QtBrowserItem \
    propertybrowser/QtButtonPropertyBrowser \
    propertybrowser/QtCharEditorFactory \
    propertybrowser/QtCharPropertyManager \
    propertybrowser/QtCheckBoxFactory \
    propertybrowser/QtColorEditorFactory \
    propertybrowser/QtColorPropertyManager \
    propertybrowser/QtCursorEditorFactory \
    propertybrowser/QtCursorPropertyManager \
    propertybrowser/QtDateEditFactory \
    propertybrowser/QtDatePropertyManager \
    propertybrowser/QtDateTimeEditFactory \
    propertybrowser/QtDateTimePropertyManager \
    propertybrowser/QtDoublePropertyManager \
    propertybrowser/QtDoubleSpinBoxFactory \
    propertybrowser/QtEnumEditorFactory \
    propertybrowser/QtEnumPropertyManager \
    propertybrowser/QtFlagPropertyManager \
    propertybrowser/QtFontEditorFactory \
    propertybrowser/QtFontPropertyManager \
    propertybrowser/QtGroupBoxPropertyBrowser \
    propertybrowser/QtGroupPropertyManager \
    propertybrowser/QtIntPropertyManager \
    propertybrowser/QtKeySequenceEditorFactory \
    propertybrowser/QtKeySequencePropertyManager \
    propertybrowser/QtLineEditFactory \
    propertybrowser/QtLocalePropertyManager \
    propertybrowser/QtPointFPropertyManager \
    propertybrowser/QtPointPropertyManager \
    propertybrowser/QtProperty \
    propertybrowser/QtRectFPropertyManager \
    propertybrowser/QtRectPropertyManager \
    propertybrowser/QtScrollBarFactory \
    propertybrowser/QtSizeFPropertyManager \
    propertybrowser/QtSizePolicyPropertyManager \
    propertybrowser/QtSizePropertyManager \
    propertybrowser/QtSliderFactory \
    propertybrowser/QtSpinBoxFactory \
    propertybrowser/QtStringPropertyManager \
    propertybrowser/QtTimeEditFactory \
    propertybrowser/QtTimePropertyManager \
    propertybrowser/QtTreePropertyBrowser \
    propertybrowser/QtVariantEditorFactory \
    propertybrowser/QtVariantProperty \
    propertybrowser/QtVariantPropertyManager \
    propertybrowser/qtbuttonpropertybrowser.h \
    propertybrowser/qteditorfactory.h \
    propertybrowser/qtgroupboxpropertybrowser.h \
    propertybrowser/qtpropertybrowser.h \
    propertybrowser/qtpropertybrowserutils_p.h \
    propertybrowser/qtpropertymanager.h \
    propertybrowser/qttreepropertybrowser.h \
    propertybrowser/qtvariantproperty.h \
    value_delegate.h

FORMS    += HelpEditorDlg.ui \
	ListDialog.ui \
	bitsetwidget.ui \
	nodeflagswidget.ui

INCLUDEPATH += ../devicedb-lib ../ozwadmin-main ../../qt-openzwave/qt-openzwave/include/


macx: {
    CONFIG += c++11
    LIBS += -framework IOKit -framework CoreFoundation
    #QMAKE_MAC_SDK = macosx10.11
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

}

RESOURCES += \
    propertybrowser/qtpropertybrowser.qrc

DISTFILES += \
    propertybrowser/images/cursor-arrow.png \
    propertybrowser/images/cursor-busy.png \
    propertybrowser/images/cursor-closedhand.png \
    propertybrowser/images/cursor-cross.png \
    propertybrowser/images/cursor-forbidden.png \
    propertybrowser/images/cursor-hand.png \
    propertybrowser/images/cursor-hsplit.png \
    propertybrowser/images/cursor-ibeam.png \
    propertybrowser/images/cursor-openhand.png \
    propertybrowser/images/cursor-sizeall.png \
    propertybrowser/images/cursor-sizeb.png \
    propertybrowser/images/cursor-sizef.png \
    propertybrowser/images/cursor-sizeh.png \
    propertybrowser/images/cursor-sizev.png \
    propertybrowser/images/cursor-uparrow.png \
    propertybrowser/images/cursor-vsplit.png \
    propertybrowser/images/cursor-wait.png \
    propertybrowser/images/cursor-whatsthis.png \
    propertybrowser/qtpropertybrowser.pri
