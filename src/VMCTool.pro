

TEMPLATE =  app


VERSION = 1.1.7

DESTDIR += ../Out/bin
UI_DIR  += ../Out/ui
RCC_DIR += ../Out/rc
MOC_DIR += ../Out/moc
OBJECTS_DIR += ../Out/obj




INCLUDEPATH += \
    ./dialog    \
    ../Out/ui


HEADERS += \
    dialog/basedialog.h \
    dialog/titlewidget.h \
    dialog/mainwindow.h \
    dialog/pushbutton.h \
    dialog/toolbutton.h \
    data_export/printdialog.h \
    system_para/sysparadialog.h \
    aisle/button.h \
    aisle/aislemanagementdialog.h \
    aisle/aisleconfigdialog.h \
    aisle/aisleconfigbuttondialog.h \
    aisle/aislebutton.h \
    aisle/addgoodsdialog.h \
    comconfigdialog.h \
    data_export/printinfodialog.h \
    config.h \
    dialog/datedialog.h \
    dialog/mainmenu.h \
    dialog/abountusdialog.h \
    SerialThead/serialthread.h \
    SerialAssistant/userial.h \
    SerialAssistant/serialassistantdialog.h \
    dialog/label.h \
    aisle/aisletestdialog.h \
    dialog/settingdialog.h \
    dialog/hdbutton.h \
    dialog/vmcdebugdialog.h \
    VMCStatus/statusdialog.h \
    serial/qextserialbase.h \
    serial/qextserialenumerator.h \
    serial/qextserialport.h \
    serial/win_qextserialport.h \
    HefanCabinet/hefandialog.h


SOURCES += \
    dialog/basedialog.cpp \
    main.cpp \
    dialog/titlewidget.cpp \
    dialog/mainwindow.cpp \
    dialog/pushbutton.cpp \
    dialog/toolbutton.cpp \
    data_export/printdialog.cpp \
    system_para/sysparadialog.cpp \
    aisle/button.cpp \
    aisle/aislemanagementdialog.cpp \
    aisle/aisleconfigdialog.cpp \
    aisle/aisleconfigbuttondialog.cpp \
    aisle/aislebutton.cpp \
    aisle/addgoodsdialog.cpp \
    comconfigdialog.cpp \
    data_export/printinfodialog.cpp \
    config.cpp \
    dialog/datedialog.cpp \
    dialog/mainmenu.cpp \
    dialog/abountusdialog.cpp \
    SerialThead/serialthread.cpp \
    SerialAssistant/userial.cpp \
    SerialAssistant/serialassistantdialog.cpp \
    dialog/label.cpp \
    aisle/aisletestdialog.cpp \
    dialog/settingdialog.cpp \
    dialog/hdbutton.cpp \
    dialog/vmcdebugdialog.cpp \
    VMCStatus/statusdialog.cpp \
    serial/qextserialbase.cpp \
    serial/qextserialenumerator.cpp \
    serial/qextserialport.cpp \
    serial/win_qextserialport.cpp \
    HefanCabinet/hefandialog.cpp

RESOURCES += \
    images.qrc

FORMS += \
    data_export/printdialog.ui \
    system_para/sysparadialog.ui \
    aisle/aislemanagementdialog.ui \
    aisle/aisleconfigdialog.ui \
    aisle/aisleconfigbuttondialog.ui \
    aisle/addgoodsdialog.ui \
    data_export/printinfodialog.ui \
    dialog/datedialog.ui \
    SerialAssistant/serialassistantdialog.ui \
    dialog/vmcdebugdialog.ui


TRANSLATIONS += \
    vmctool_ch.ts\
    vmctool_tzh.ts



RC_FILE +=\
    tool.rc







win32:DEFINES          += _TTY_WIN_
win32:LIBS             += -lsetupapi

OTHER_FILES +=

