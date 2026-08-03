# ----------------------------------------------------------
#  onyx.pro 
# ----------------------------------------------------------

QT += core gui widgets network xml
# Add these modules if you use them:
# QT += multimedia multimediawidgets

TARGET = onyx-voip
TEMPLATE = app

# ---- Build configurations (Debug / Release) ----
CONFIG += c++17
CONFIG(debug, debug|release) {
    CONFIG += console
    DEFINES += _DEBUG
} else {
    DEFINES += NDEBUG
    CONFIG += optimize_full
}

# ---- Defines (same as original vcxproj) ----
DEFINES += WIN32 _WINDOWS
DEFINES += _UNICODE UNICODE

# If you still have the global video flag
# DEFINES += _GLOBAL_VIDEO

# ---- Windows subsystem & manifest ----
win32:!winrt {
    QMAKE_SUBSYSTEM_SUFFIX = windows
    # Embed a custom manifest for DPI awareness and common controls
    # Use the same custom.manifest if available
    QMAKE_MANIFEST = custom.manifest
    # Or add additional manifest options directly:
    # CONFIG += embed_manifest_exe
    # QMAKE_LFLAGS_MANIFEST = /MANIFESTUAC:level='requireAdministrator'
}

# ---- Include paths (exactly the same as vcxproj) ----
INCLUDEPATH += \
    lib \
    lib/jsoncpp/json \
    ../pjlib/include \
    ../pjsip/include \
    ../pjmedia/include \
    ../pjnath/include \
    ../pjlib-util/include \
    ../third_party/sdl/include \
    ../third_party/openssl/Win32/include \
    ../third_party/hid \
    . \
    $$PWD

# ---- Library dependencies (same libraries) ----
LIBS += \
    opus.lib \
    Wtsapi32.lib \
    -L../lib \
    -L../third_party/lib \
    -L../third_party/openssl/$${PLATFORM}/lib \
    -L../third_party/ipp/$${PLATFORM} \
    -L../third_party/opus/$${PLATFORM} \
    -L../third_party/silk/$${PLATFORM}/$${CONFIGURATION} \
    -L../third_party/ffmpeg/$${PLATFORM} \
    -L../third_party/vpx/$${PLATFORM} \
    -L../third_party/sdl/$${PLATFORM} \
    -L../third_party/opencore-amrnb/$${PLATFORM} \
    -L../third_party/vo-amrwbenc/$${PLATFORM} \
    -L../third_party/opencore-amrwb/$${PLATFORM} \
    -L../third_party/hid/$${PLATFORM} \
    -L$$(WindowsSDK_LibraryPath_x86) \
    -L$$(WindowsSDK_LibraryPath_x64)

# ---- Source files (all .cpp from the original project) ----
SOURCES += \
    AAOptionsDlg.cpp \
    AccountDlg.cpp \
    AddDlg.cpp \
    addons.cpp \
    BaseDialog.cpp \
    ButtonBottom.cpp \
    ButtonDialer.cpp \
    ButtonEx.cpp \
    ButtonSafe.cpp \
    Calls.cpp \
    CListCtrl_Sortable.cpp \
    CListCtrl_SortItemsEx.cpp \
    ClosableTabCtrl.cpp \
    Contacts.cpp \
    Dialer.cpp \
    FeatureCodesDlg.cpp \
    global.cpp \
    IconButton.cpp \
    jumplist.cpp \
    LevelsSliderCtrl.cpp \
    mainDlg.cpp \
    MessagesDlg.cpp \
    onyx.cpp \
    Preview.cpp \
    RinginDlg.cpp \
    settings.cpp \
    SettingsDlg.cpp \
    ShortcutsDlg.cpp \
    StatusBar.cpp \
    stdafx.cpp \
    Transfer.cpp \
    lib/CListCtrl_ToolTip.cpp \
    lib/CMask.cpp \
    lib/Crypto.cpp \
    lib/CSVFile.cpp \
    lib/Hid.cpp \
    lib/jsoncpp/json_reader.cpp \
    lib/jsoncpp/json_value.cpp \
    lib/jsoncpp/json_writer.cpp \
    lib/langpack.cpp \
    lib/Markup.cpp \
    lib/MessageBoxX.cpp \
    lib/ModelessMessageBox.cpp \
    lib/OnyxUtils.cpp \
    lib/StdioFileEx.cpp \
    lib/VisualStylesXP.cpp

# ---- Header files (needed for MOC, if they contain Q_OBJECT) ----
HEADERS += \
    AAOptionsDlg.h \
    AccountDlg.h \
    AddDlg.h \
    addons.h \
    BaseDialog.h \
    ButtonBottom.h \
    ButtonDialer.h \
    ButtonEx.h \
    ButtonSafe.h \
    Calls.h \
    CListCtrl_Sortable.h \
    CListCtrl_SortItemsEx.h \
    ClosableTabCtrl.h \
    const.h \
    Contacts.h \
    define.h \
    Dialer.h \
    FeatureCodesDlg.h \
    global.h \
    IconButton.h \
    jumplist.h \
    LevelsSliderCtrl.h \
    mainDlg.h \
    MessagesDlg.h \
    onyx.h \
    MMNotificationClient.h \
    Preview.h \
    Resource.h \
    RinginDlg.h \
    settings.h \
    SettingsDlg.h \
    ShortcutsDlg.h \
    StatusBar.h \
    stdafx.h \
    targetver.h \
    Transfer.h \
    lib/CListCtrl_LabelTip.h \
    lib/CListCtrl_ToolTip.h \
    lib/Crypto.h \
    lib/CSVFile.h \
    lib/ggets.h \
    lib/Hid.h \
    lib/hidapi.h \
    lib/jsoncpp/json/json.h \
    lib/langpack.h \
    lib/Markup.h \
    lib/MessageBoxX.h \
    lib/ModelessMessageBox.h \
    lib/OnyxUtils.h \
    lib/StdioFileEx.h \
    lib/TemplateSmartPtr.h \
    lib/VisualStylesXP.h

# ---- Qt Forms (if you design UIs) ----
# FORMS += mainDlg.ui ...

# ---- Resources ----
RESOURCES = resources.qrc

# ---- Precompiled header ----
# The original uses stdafx.cpp/h. Qt projects can use PRECOMPILED_HEADER.
PRECOMPILED_HEADER = stdafx.h
CONFIG += precompile_header

# ---- Additional settings ----
# Enable multi‑processor compilation (if using MSVC)
win32-msvc*: QMAKE_CXXFLAGS += /MP
# Enable DPI awareness (embedded manifest already does this)
# QMAKE_LFLAGS += /MANIFESTUAC:level='asInvoker'
# Disable link‑time code generation in debug (like original)
CONFIG(debug, debug|release): QMAKE_LFLAGS_LTCG =

# The original disables LBR optimisation: /OPT:NOLBR
# QMAKE_LFLAGS += /OPT:NOLBR
