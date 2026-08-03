#pragma once

#include <QtGlobal>
#include <QLibrary>
#include <QString>
#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>

// Disable "unreferenced formal parameter" warnings for fail functions
#pragma warning(push)
#pragma warning(disable:4100)

class CVisualStylesXP
{
public:
    CVisualStylesXP();
    ~CVisualStylesXP();

    // Public theme functions (same signatures as original)
    HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList);
    HRESULT CloseThemeData(HTHEME hTheme);
    HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
    HRESULT DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags,
        DWORD dwTextFlags2, const RECT *pRect);
    HRESULT GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, const RECT *pBoundingRect,
        RECT *pContentRect);
    HRESULT GetThemeBackgroundExtent(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, const RECT *pContentRect,
        RECT *pExtentRect);
    HRESULT GetThemePartSize(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, RECT *pRect, enum THEMESIZE eSize, SIZE *psz);
    HRESULT GetThemeTextExtent(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, LPCWSTR pszText, int iCharCount,
        DWORD dwTextFlags, const RECT *pBoundingRect,
        RECT *pExtentRect);
    HRESULT GetThemeTextMetrics(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, TEXTMETRIC* ptm);
    HRESULT GetThemeBackgroundRegion(HTHEME hTheme, HDC hdc,
        int iPartId, int iStateId, const RECT *pRect, HRGN *pRegion);
    HRESULT HitTestThemeBackground(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, DWORD dwOptions, const RECT *pRect, HRGN hrgn,
        POINT ptTest, WORD *pwHitTestCode);
    HRESULT DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId,
        const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect);
    HRESULT DrawThemeIcon(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex);
    BOOL IsThemePartDefined(HTHEME hTheme, int iPartId, int iStateId);
    BOOL IsThemeBackgroundPartiallyTransparent(HTHEME hTheme,
        int iPartId, int iStateId);
    HRESULT GetThemeColor(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, COLORREF *pColor);
    HRESULT GetThemeMetric(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, int iPropId, int *piVal);
    HRESULT GetThemeString(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars);
    HRESULT GetThemeBool(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, BOOL *pfVal);
    HRESULT GetThemeInt(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, int *piVal);
    HRESULT GetThemeEnumValue(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, int *piVal);
    HRESULT GetThemePosition(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, POINT *pPoint);
    HRESULT GetThemeFont(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, int iPropId, LOGFONT *pFont);
    HRESULT GetThemeRect(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, RECT *pRect);
    HRESULT GetThemeMargins(HTHEME hTheme, HDC hdc, int iPartId,
        int iStateId, int iPropId, RECT *prc, MARGINS *pMargins);
    HRESULT GetThemeIntList(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, INTLIST *pIntList);
    HRESULT GetThemePropertyOrigin(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, enum PROPERTYORIGIN *pOrigin);
    HRESULT SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName,
        LPCWSTR pszSubIdList);
    HRESULT GetThemeFilename(HTHEME hTheme, int iPartId,
        int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars);
    COLORREF GetThemeSysColor(HTHEME hTheme, int iColorId);
    HBRUSH GetThemeSysColorBrush(HTHEME hTheme, int iColorId);
    BOOL GetThemeSysBool(HTHEME hTheme, int iBoolId);
    int GetThemeSysSize(HTHEME hTheme, int iSizeId);
    HRESULT GetThemeSysFont(HTHEME hTheme, int iFontId, LOGFONT *plf);
    HRESULT GetThemeSysString(HTHEME hTheme, int iStringId,
        LPWSTR pszStringBuff, int cchMaxStringChars);
    HRESULT GetThemeSysInt(HTHEME hTheme, int iIntId, int *piValue);
    BOOL IsThemeActive();
    BOOL IsAppThemed();
    HTHEME GetWindowTheme(HWND hwnd);
    HRESULT EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags);
    BOOL IsThemeDialogTextureEnabled(HWND hwnd);
    DWORD GetThemeAppProperties();
    void SetThemeAppProperties(DWORD dwFlags);
    HRESULT GetCurrentThemeName(
        LPWSTR pszThemeFileName, int cchMaxNameChars,
        LPWSTR pszColorBuff, int cchMaxColorChars,
        LPWSTR pszSizeBuff, int cchMaxSizeChars);
    HRESULT GetThemeDocumentationProperty(LPCWSTR pszThemeName,
        LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars);
    HRESULT DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT* prc);
    HRESULT EnableTheming(BOOL fEnable);

private:
    static QLibrary* m_pThemeLib;   // replaced HMODULE
    static void* GetProc(const char* szProc, void* pfnFail);

    // Fallback functions (same as original, but static)
    static HTHEME OpenThemeDataFail(HWND, LPCWSTR) { return nullptr; }
    static HRESULT CloseThemeDataFail(HTHEME) { return E_FAIL; }
    static HRESULT DrawThemeBackgroundFail(HTHEME, HDC, int, int, const RECT*, const RECT*) { return E_FAIL; }
    static HRESULT DrawThemeTextFail(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, DWORD, const RECT*) { return E_FAIL; }
    static HRESULT GetThemeBackgroundContentRectFail(HTHEME, HDC, int, int, const RECT*, RECT*) { return E_FAIL; }
    static HRESULT GetThemeBackgroundExtentFail(HTHEME, HDC, int, int, const RECT*, RECT*) { return E_FAIL; }
    static HRESULT GetThemePartSizeFail(HTHEME, HDC, int, int, RECT*, enum THEMESIZE, SIZE*) { return E_FAIL; }
    static HRESULT GetThemeTextExtentFail(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, const RECT*, RECT*) { return E_FAIL; }
    static HRESULT GetThemeTextMetricsFail(HTHEME, HDC, int, int, TEXTMETRIC*) { return E_FAIL; }
    static HRESULT GetThemeBackgroundRegionFail(HTHEME, HDC, int, int, const RECT*, HRGN*) { return E_FAIL; }
    static HRESULT HitTestThemeBackgroundFail(HTHEME, HDC, int, int, DWORD, const RECT*, HRGN, POINT, WORD*) { return E_FAIL; }
    static HRESULT DrawThemeEdgeFail(HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT*) { return E_FAIL; }
    static HRESULT DrawThemeIconFail(HTHEME, HDC, int, int, const RECT*, HIMAGELIST, int) { return E_FAIL; }
    static BOOL IsThemePartDefinedFail(HTHEME, int, int) { return FALSE; }
    static BOOL IsThemeBackgroundPartiallyTransparentFail(HTHEME, int, int) { return FALSE; }
    static HRESULT GetThemeColorFail(HTHEME, int, int, int, COLORREF*) { return E_FAIL; }
    static HRESULT GetThemeMetricFail(HTHEME, HDC, int, int, int, int*) { return E_FAIL; }
    static HRESULT GetThemeStringFail(HTHEME, int, int, int, LPWSTR, int) { return E_FAIL; }
    static HRESULT GetThemeBoolFail(HTHEME, int, int, int, BOOL*) { return E_FAIL; }
    static HRESULT GetThemeIntFail(HTHEME, int, int, int, int*) { return E_FAIL; }
    static HRESULT GetThemeEnumValueFail(HTHEME, int, int, int, int*) { return E_FAIL; }
    static HRESULT GetThemePositionFail(HTHEME, int, int, int, POINT*) { return E_FAIL; }
    static HRESULT GetThemeFontFail(HTHEME, HDC, int, int, int, LOGFONT*) { return E_FAIL; }
    static HRESULT GetThemeRectFail(HTHEME, int, int, int, RECT*) { return E_FAIL; }
    static HRESULT GetThemeMarginsFail(HTHEME, HDC, int, int, int, RECT*, MARGINS*) { return E_FAIL; }
    static HRESULT GetThemeIntListFail(HTHEME, int, int, int, INTLIST*) { return E_FAIL; }
    static HRESULT GetThemePropertyOriginFail(HTHEME, int, int, int, enum PROPERTYORIGIN*) { return E_FAIL; }
    static HRESULT SetWindowThemeFail(HWND, LPCWSTR, LPCWSTR) { return E_FAIL; }
    static HRESULT GetThemeFilenameFail(HTHEME, int, int, int, LPWSTR, int) { return E_FAIL; }
    static COLORREF GetThemeSysColorFail(HTHEME, int) { return RGB(255,255,255); }
    static HBRUSH GetThemeSysColorBrushFail(HTHEME, int) { return nullptr; }
    static BOOL GetThemeSysBoolFail(HTHEME, int) { return FALSE; }
    static int GetThemeSysSizeFail(HTHEME, int) { return 0; }
    static HRESULT GetThemeSysFontFail(HTHEME, int, LOGFONT*) { return E_FAIL; }
    static HRESULT GetThemeSysStringFail(HTHEME, int, LPWSTR, int) { return E_FAIL; }
    static HRESULT GetThemeSysIntFail(HTHEME, int, int*) { return E_FAIL; }
    static BOOL IsThemeActiveFail() { return FALSE; }
    static BOOL IsAppThemedFail() { return FALSE; }
    static HTHEME GetWindowThemeFail(HWND) { return nullptr; }
    static HRESULT EnableThemeDialogTextureFail(HWND, DWORD) { return E_FAIL; }
    static BOOL IsThemeDialogTextureEnabledFail(HWND) { return FALSE; }
    static DWORD GetThemeAppPropertiesFail() { return 0; }
    static void SetThemeAppPropertiesFail(DWORD) { return; }
    static HRESULT GetCurrentThemeNameFail(LPWSTR, int, LPWSTR, int, LPWSTR, int) { return E_FAIL; }
    static HRESULT GetThemeDocumentationPropertyFail(LPCWSTR, LPCWSTR, LPWSTR, int) { return E_FAIL; }
    static HRESULT DrawThemeParentBackgroundFail(HWND, HDC, RECT*) { return E_FAIL; }
    static HRESULT EnableThemingFail(BOOL) { return E_FAIL; }

    // Typedefs for function pointers (same as original)
    typedef HTHEME (__stdcall *PFNOPENTHEMEDATA)(HWND, LPCWSTR);
    typedef HRESULT (__stdcall *PFNCLOSETHEMEDATA)(HTHEME);
    typedef HRESULT (__stdcall *PFNDRAWTHEMEBACKGROUND)(HTHEME, HDC, int, int, const RECT*, const RECT*);
    typedef HRESULT (__stdcall *PFNDRAWTHEMETEXT)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, DWORD, const RECT*);
    typedef HRESULT (__stdcall *PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME, HDC, int, int, const RECT*, RECT*);
    typedef HRESULT (__stdcall *PFNGETTHEMEBACKGROUNDEXTENT)(HTHEME, HDC, int, int, const RECT*, RECT*);
    typedef HRESULT (__stdcall *PFNGETTHEMEPARTSIZE)(HTHEME, HDC, int, int, RECT*, THEMESIZE, SIZE*);
    typedef HRESULT (__stdcall *PFNGETTHEMETEXTEXTENT)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, const RECT*, RECT*);
    typedef HRESULT (__stdcall *PFNGETTHEMETEXTMETRICS)(HTHEME, HDC, int, int, TEXTMETRIC*);
    typedef HRESULT (__stdcall *PFNGETTHEMEBACKGROUNDREGION)(HTHEME, HDC, int, int, const RECT*, HRGN*);
    typedef HRESULT (__stdcall *PFNHITTESTTHEMEBACKGROUND)(HTHEME, HDC, int, int, DWORD, const RECT*, HRGN, POINT, WORD*);
    typedef HRESULT (__stdcall *PFNDRAWTHEMEEDGE)(HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT*);
    typedef HRESULT (__stdcall *PFNDRAWTHEMEICON)(HTHEME, HDC, int, int, const RECT*, HIMAGELIST, int);
    typedef BOOL (__stdcall *PFNISTHEMEPARTDEFINED)(HTHEME, int, int);
    typedef BOOL (__stdcall *PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)(HTHEME, int, int);
    typedef HRESULT (__stdcall *PFNGETTHEMECOLOR)(HTHEME, int, int, int, COLORREF*);
    typedef HRESULT (__stdcall *PFNGETTHEMEMETRIC)(HTHEME, HDC, int, int, int, int*);
    typedef HRESULT (__stdcall *PFNGETTHEMESTRING)(HTHEME, int, int, int, LPWSTR, int);
    typedef HRESULT (__stdcall *PFNGETTHEMEBOOL)(HTHEME, int, int, int, BOOL*);
    typedef HRESULT (__stdcall *PFNGETTHEMEINT)(HTHEME, int, int, int, int*);
    typedef HRESULT (__stdcall *PFNGETTHEMEENUMVALUE)(HTHEME, int, int, int, int*);
    typedef HRESULT (__stdcall *PFNGETTHEMEPOSITION)(HTHEME, int, int, int, POINT*);
    typedef HRESULT (__stdcall *PFNGETTHEMEFONT)(HTHEME, HDC, int, int, int, LOGFONT*);
    typedef HRESULT (__stdcall *PFNGETTHEMERECT)(HTHEME, int, int, int, RECT*);
    typedef HRESULT (__stdcall *PFNGETTHEMEMARGINS)(HTHEME, HDC, int, int, int, RECT*, MARGINS*);
    typedef HRESULT (__stdcall *PFNGETTHEMEINTLIST)(HTHEME, int, int, int, INTLIST*);
    typedef HRESULT (__stdcall *PFNGETTHEMEPROPERTYORIGIN)(HTHEME, int, int, int, PROPERTYORIGIN*);
    typedef HRESULT (__stdcall *PFNSETWINDOWTHEME)(HWND, LPCWSTR, LPCWSTR);
    typedef HRESULT (__stdcall *PFNGETTHEMEFILENAME)(HTHEME, int, int, int, LPWSTR, int);
    typedef COLORREF (__stdcall *PFNGETTHEMESYSCOLOR)(HTHEME, int);
    typedef HBRUSH (__stdcall *PFNGETTHEMESYSCOLORBRUSH)(HTHEME, int);
    typedef BOOL (__stdcall *PFNGETTHEMESYSBOOL)(HTHEME, int);
    typedef int (__stdcall *PFNGETTHEMESYSSIZE)(HTHEME, int);
    typedef HRESULT (__stdcall *PFNGETTHEMESYSFONT)(HTHEME, int, LOGFONT*);
    typedef HRESULT (__stdcall *PFNGETTHEMESYSSTRING)(HTHEME, int, LPWSTR, int);
    typedef HRESULT (__stdcall *PFNGETTHEMESYSINT)(HTHEME, int, int*);
    typedef BOOL (__stdcall *PFNISTHEMEACTIVE)();
    typedef BOOL (__stdcall *PFNISAPPTHEMED)();
    typedef HTHEME (__stdcall *PFNGETWINDOWTHEME)(HWND);
    typedef HRESULT (__stdcall *PFNENABLETHEMEDIALOGTEXTURE)(HWND, DWORD);
    typedef BOOL (__stdcall *PFNISTHEMEDIALOGTEXTUREENABLED)(HWND);
    typedef DWORD (__stdcall *PFNGETTHEMEAPPPROPERTIES)();
    typedef void (__stdcall *PFNSETTHEMEAPPPROPERTIES)(DWORD);
    typedef HRESULT (__stdcall *PFNGETCURRENTTHEMENAME)(LPWSTR, int, LPWSTR, int, LPWSTR, int);
    typedef HRESULT (__stdcall *PFNGETTHEMEDOCUMENTATIONPROPERTY)(LPCWSTR, LPCWSTR, LPWSTR, int);
    typedef HRESULT (__stdcall *PFNDRAWTHEMEPARENTBACKGROUND)(HWND, HDC, RECT*);
    typedef HRESULT (__stdcall *PFNENABLETHEMING)(BOOL);
};

extern CVisualStylesXP g_xpStyle;

#pragma warning(pop) // restore warning level
