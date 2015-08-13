#-------------------------------------------------
#
# Project created by QtCreator 2013-08-18T15:52:09
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = occQt
TEMPLATE = app


FORMS    += \
    occQt.ui \
    gcodedialog.ui \
    toolsdialog.ui

SOURCES += main.cpp \
    occQt.cpp \
    OccView.cpp \
    test_creationclass.cpp \
    DXFSource/dl_dxf.cpp \
    DXFSource/dl_writer_ascii.cpp \
    dxfreader.cpp \
    gridDialog.cpp \
    gcodedialog.cpp \
    gcodewriter.cpp \
    toolsdialog.cpp \
    tool.cpp

HEADERS  += \
    occQt.h \
    OccView.h \
    DXFHeaders/dl_attributes.h \
    DXFHeaders/dl_codes.h \
    DXFHeaders/dl_creationadapter.h \
    DXFHeaders/dl_creationinterface.h \
    DXFHeaders/dl_dxf.h \
    DXFHeaders/dl_entities.h \
    DXFHeaders/dl_exception.h \
    DXFHeaders/dl_extrusion.h \
    DXFHeaders/dl_writer.h \
    DXFHeaders/dl_writer_ascii.h \
    test_creationclass.h \
    dxfreader.h \
    basicheaders.h \
    gridDialog.h \
    gcodedialog.h \
    gcodewriter.h \
    toolsdialog.h \
    tool.h

INCLUDEPATH += C:/OpenCASCADE6.9.0/opencascade-6.9.0/inc

win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKernel.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKMath.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKBRep.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKTopAlgo.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKPrim.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKBO.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKBool.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKOffset.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKService.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKV3d.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKOpenGl.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKFillet.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKIGES.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKXSBase.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKGeomBase.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKShapeSchema.lib
#win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKCDF.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKPShape.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\TKG3d.lib
win32:  LIBS += C:\OpenCASCADE6.9.0\opencascade-6.9.0\win32\vc10\lib\PTKernel.lib

DEFINES +=WNT WIN32 NO_COMMONSAMPLE_EXPORTS NO_IESAMPLE_EXPORTS

unix: LIBS += -lTKernel -lTKMath -lTKBRep -lTKTopAlgo -lTKPrim -lTKBO -lTKOffset -lTKService -lTKV3d -lTKOpenGl -lTKFillet

RESOURCES += \
    occqt.qrc

DISTFILES += \
    trainning.txt \
    todo.txt
