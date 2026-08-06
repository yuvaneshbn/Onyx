#include "VisualStylesXP.h"
#include <QtGlobal>
#include <QLibrary>
#include <QDebug>

#ifndef LOAD_LIBRARY_SEARCH_SYSTEM32
#define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800
#endif

CVisualStylesXP g_xpStyle;

// Static member definition
QLibrary* CVisualStylesXP::m_pThemeLib = nullptr;

// ----------------------------------------------------------------------
// Constructor / Destructor
// ----------------------------------------------------------------------
CVisualStylesXP::CVisualStylesXP()
{
    if (!m_pThemeLib)
    {
        m_pThemeLib = new QLibrary("UxTheme.dll");
        // On Windows, QLibrary will try to load from system paths.
        // If we want to mimic LOAD_LIBRARY_SEARCH_SYSTEM32, we could set the path.
        // Not directly supported; but we can load by full path if needed.
        // For simplicity, we rely on standard search order.
        if (!m_pThemeLib->load())
        {
            qWarning() << "Failed to load UxTheme.dll";
            delete m_pThemeLib;
            m_pThemeLib = nullptr;
        }
    }
}

CVisualStylesXP::~CVisualStylesXP()
{
    if (m_pThemeLib)
    {
        m_pThemeLib->unload();
        delete m_pThemeLib;
        m_pThemeLib = nullptr;
    }
}

// ----------------------------------------------------------------------
// GetProc - resolve function from library
// ----------------------------------------------------------------------
void* CVisualStylesXP::GetProc(const char* szProc, void* pfnFail)
{
    if (m_pThemeLib && m_pThemeLib->isLoaded())
    {
        void* pRet = reinterpret_cast<void*>(m_pThemeLib->resolve(szProc));
        if (pRet)
            return pRet;
    }
    return pfnFail;
}

// ----------------------------------------------------------------------
// All theme functions (implemented with static function pointers)
// ----------------------------------------------------------------------

HTHEME CVisualStylesXP::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
    static PFNOPENTHEMEDATA pfn = nullptr;
    if (!pfn)
        pfn = (PFNOPENTHEMEDATA)GetProc("OpenThemeData", (void*)OpenThemeDataFail);
    return (*pfn)(hwnd, pszClassList);
}

HRESULT CVisualStylesXP::CloseThemeData(HTHEME hTheme)
{
    static PFNCLOSETHEMEDATA pfn = nullptr;
    if (!pfn)
        pfn = (PFNCLOSETHEMEDATA)GetProc("CloseThemeData", (void*)CloseThemeDataFail);
    return (*pfn)(hTheme);
}

HRESULT CVisualStylesXP::DrawThemeBackground(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
{
    static PFNDRAWTHEMEBACKGROUND pfn = nullptr;
    if (!pfn)
        pfn = (PFNDRAWTHEMEBACKGROUND)GetProc("DrawThemeBackground", (void*)DrawThemeBackgroundFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT CVisualStylesXP::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags,
    DWORD dwTextFlags2, const RECT *pRect)
{
    static PFNDRAWTHEMETEXT pfn = nullptr;
    if (!pfn)
        pfn = (PFNDRAWTHEMETEXT)GetProc("DrawThemeText", (void*)DrawThemeTextFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
}

HRESULT CVisualStylesXP::GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, const RECT *pBoundingRect,
    RECT *pContentRect)
{
    static PFNGETTHEMEBACKGROUNDCONTENTRECT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEBACKGROUNDCONTENTRECT)GetProc("GetThemeBackgroundContentRect", (void*)GetThemeBackgroundContentRectFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect);
}

HRESULT CVisualStylesXP::GetThemeBackgroundExtent(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, const RECT *pContentRect,
    RECT *pExtentRect)
{
    static PFNGETTHEMEBACKGROUNDEXTENT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEBACKGROUNDEXTENT)GetProc("GetThemeBackgroundExtent", (void*)GetThemeBackgroundExtentFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pContentRect, pExtentRect);
}

HRESULT CVisualStylesXP::GetThemePartSize(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, RECT *pRect, enum THEMESIZE eSize, SIZE *psz)
{
    static PFNGETTHEMEPARTSIZE pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEPARTSIZE)GetProc("GetThemePartSize", (void*)GetThemePartSizeFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, eSize, psz);
}

HRESULT CVisualStylesXP::GetThemeTextExtent(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, LPCWSTR pszText, int iCharCount,
    DWORD dwTextFlags, const RECT *pBoundingRect, RECT *pExtentRect)
{
    static PFNGETTHEMETEXTEXTENT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMETEXTEXTENT)GetProc("GetThemeTextExtent", (void*)GetThemeTextExtentFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);
}

HRESULT CVisualStylesXP::GetThemeTextMetrics(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, TEXTMETRIC* ptm)
{
    static PFNGETTHEMETEXTMETRICS pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMETEXTMETRICS)GetProc("GetThemeTextMetrics", (void*)GetThemeTextMetricsFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, ptm);
}

HRESULT CVisualStylesXP::GetThemeBackgroundRegion(HTHEME hTheme, HDC hdc,
    int iPartId, int iStateId, const RECT *pRect, HRGN *pRegion)
{
    static PFNGETTHEMEBACKGROUNDREGION pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEBACKGROUNDREGION)GetProc("GetThemeBackgroundRegion", (void*)GetThemeBackgroundRegionFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, pRegion);
}

HRESULT CVisualStylesXP::HitTestThemeBackground(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, DWORD dwOptions, const RECT *pRect, HRGN hrgn,
    POINT ptTest, WORD *pwHitTestCode)
{
    static PFNHITTESTTHEMEBACKGROUND pfn = nullptr;
    if (!pfn)
        pfn = (PFNHITTESTTHEMEBACKGROUND)GetProc("HitTestThemeBackground", (void*)HitTestThemeBackgroundFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, dwOptions, pRect, hrgn, ptTest, pwHitTestCode);
}

HRESULT CVisualStylesXP::DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId,
    const RECT *pDestRect, UINT uEdge, UINT uFlags, RECT *pContentRect)
{
    static PFNDRAWTHEMEEDGE pfn = nullptr;
    if (!pfn)
        pfn = (PFNDRAWTHEMEEDGE)GetProc("DrawThemeEdge", (void*)DrawThemeEdgeFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
}

HRESULT CVisualStylesXP::DrawThemeIcon(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, const RECT *pRect, HIMAGELIST himl, int iImageIndex)
{
    static PFNDRAWTHEMEICON pfn = nullptr;
    if (!pfn)
        pfn = (PFNDRAWTHEMEICON)GetProc("DrawThemeIcon", (void*)DrawThemeIconFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex);
}

BOOL CVisualStylesXP::IsThemePartDefined(HTHEME hTheme, int iPartId, int iStateId)
{
    static PFNISTHEMEPARTDEFINED pfn = nullptr;
    if (!pfn)
        pfn = (PFNISTHEMEPARTDEFINED)GetProc("IsThemePartDefined", (void*)IsThemePartDefinedFail);
    return (*pfn)(hTheme, iPartId, iStateId);
}

BOOL CVisualStylesXP::IsThemeBackgroundPartiallyTransparent(HTHEME hTheme,
    int iPartId, int iStateId)
{
    static PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT pfn = nullptr;
    if (!pfn)
        pfn = (PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)GetProc("IsThemeBackgroundPartiallyTransparent", (void*)IsThemeBackgroundPartiallyTransparentFail);
    return (*pfn)(hTheme, iPartId, iStateId);
}

HRESULT CVisualStylesXP::GetThemeColor(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, COLORREF *pColor)
{
    static PFNGETTHEMECOLOR pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMECOLOR)GetProc("GetThemeColor", (void*)GetThemeColorFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pColor);
}

HRESULT CVisualStylesXP::GetThemeMetric(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, int iPropId, int *piVal)
{
    static PFNGETTHEMEMETRIC pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEMETRIC)GetProc("GetThemeMetric", (void*)GetThemeMetricFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemeString(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars)
{
    static PFNGETTHEMESTRING pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESTRING)GetProc("GetThemeString", (void*)GetThemeStringFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars);
}

HRESULT CVisualStylesXP::GetThemeBool(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, BOOL *pfVal)
{
    static PFNGETTHEMEBOOL pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEBOOL)GetProc("GetThemeBool", (void*)GetThemeBoolFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pfVal);
}

HRESULT CVisualStylesXP::GetThemeInt(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, int *piVal)
{
    static PFNGETTHEMEINT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEINT)GetProc("GetThemeInt", (void*)GetThemeIntFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemeEnumValue(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, int *piVal)
{
    static PFNGETTHEMEENUMVALUE pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEENUMVALUE)GetProc("GetThemeEnumValue", (void*)GetThemeEnumValueFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, piVal);
}

HRESULT CVisualStylesXP::GetThemePosition(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, POINT *pPoint)
{
    static PFNGETTHEMEPOSITION pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEPOSITION)GetProc("GetThemePosition", (void*)GetThemePositionFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pPoint);
}

HRESULT CVisualStylesXP::GetThemeFont(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, int iPropId, LOGFONT *pFont)
{
    static PFNGETTHEMEFONT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEFONT)GetProc("GetThemeFont", (void*)GetThemeFontFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, pFont);
}

HRESULT CVisualStylesXP::GetThemeRect(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, RECT *pRect)
{
    static PFNGETTHEMERECT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMERECT)GetProc("GetThemeRect", (void*)GetThemeRectFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pRect);
}

HRESULT CVisualStylesXP::GetThemeMargins(HTHEME hTheme, HDC hdc, int iPartId,
    int iStateId, int iPropId, RECT *prc, MARGINS *pMargins)
{
    static PFNGETTHEMEMARGINS pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEMARGINS)GetProc("GetThemeMargins", (void*)GetThemeMarginsFail);
    return (*pfn)(hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
}

HRESULT CVisualStylesXP::GetThemeIntList(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, INTLIST *pIntList)
{
    static PFNGETTHEMEINTLIST pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEINTLIST)GetProc("GetThemeIntList", (void*)GetThemeIntListFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pIntList);
}

HRESULT CVisualStylesXP::GetThemePropertyOrigin(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, enum PROPERTYORIGIN *pOrigin)
{
    static PFNGETTHEMEPROPERTYORIGIN pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEPROPERTYORIGIN)GetProc("GetThemePropertyOrigin", (void*)GetThemePropertyOriginFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pOrigin);
}

HRESULT CVisualStylesXP::SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName,
    LPCWSTR pszSubIdList)
{
    static PFNSETWINDOWTHEME pfn = nullptr;
    if (!pfn)
        pfn = (PFNSETWINDOWTHEME)GetProc("SetWindowTheme", (void*)SetWindowThemeFail);
    return (*pfn)(hwnd, pszSubAppName, pszSubIdList);
}

HRESULT CVisualStylesXP::GetThemeFilename(HTHEME hTheme, int iPartId,
    int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars)
{
    static PFNGETTHEMEFILENAME pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEFILENAME)GetProc("GetThemeFilename", (void*)GetThemeFilenameFail);
    return (*pfn)(hTheme, iPartId, iStateId, iPropId, pszThemeFileName, cchMaxBuffChars);
}

COLORREF CVisualStylesXP::GetThemeSysColor(HTHEME hTheme, int iColorId)
{
    static PFNGETTHEMESYSCOLOR pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSCOLOR)GetProc("GetThemeSysColor", (void*)GetThemeSysColorFail);
    return (*pfn)(hTheme, iColorId);
}

HBRUSH CVisualStylesXP::GetThemeSysColorBrush(HTHEME hTheme, int iColorId)
{
    static PFNGETTHEMESYSCOLORBRUSH pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSCOLORBRUSH)GetProc("GetThemeSysColorBrush", (void*)GetThemeSysColorBrushFail);
    return (*pfn)(hTheme, iColorId);
}

BOOL CVisualStylesXP::GetThemeSysBool(HTHEME hTheme, int iBoolId)
{
    static PFNGETTHEMESYSBOOL pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSBOOL)GetProc("GetThemeSysBool", (void*)GetThemeSysBoolFail);
    return (*pfn)(hTheme, iBoolId);
}

int CVisualStylesXP::GetThemeSysSize(HTHEME hTheme, int iSizeId)
{
    static PFNGETTHEMESYSSIZE pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSSIZE)GetProc("GetThemeSysSize", (void*)GetThemeSysSizeFail);
    return (*pfn)(hTheme, iSizeId);
}

HRESULT CVisualStylesXP::GetThemeSysFont(HTHEME hTheme, int iFontId, LOGFONT *plf)
{
    static PFNGETTHEMESYSFONT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSFONT)GetProc("GetThemeSysFont", (void*)GetThemeSysFontFail);
    return (*pfn)(hTheme, iFontId, plf);
}

HRESULT CVisualStylesXP::GetThemeSysString(HTHEME hTheme, int iStringId,
    LPWSTR pszStringBuff, int cchMaxStringChars)
{
    static PFNGETTHEMESYSSTRING pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSSTRING)GetProc("GetThemeSysString", (void*)GetThemeSysStringFail);
    return (*pfn)(hTheme, iStringId, pszStringBuff, cchMaxStringChars);
}

HRESULT CVisualStylesXP::GetThemeSysInt(HTHEME hTheme, int iIntId, int *piValue)
{
    static PFNGETTHEMESYSINT pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMESYSINT)GetProc("GetThemeSysInt", (void*)GetThemeSysIntFail);
    return (*pfn)(hTheme, iIntId, piValue);
}

BOOL CVisualStylesXP::IsThemeActive()
{
    static PFNISTHEMEACTIVE pfn = nullptr;
    if (!pfn)
        pfn = (PFNISTHEMEACTIVE)GetProc("IsThemeActive", (void*)IsThemeActiveFail);
    return (*pfn)();
}

BOOL CVisualStylesXP::IsAppThemed()
{
    static PFNISAPPTHEMED pfn = nullptr;
    if (!pfn)
        pfn = (PFNISAPPTHEMED)GetProc("IsAppThemed", (void*)IsAppThemedFail);
    return (*pfn)();
}

HTHEME CVisualStylesXP::GetWindowTheme(HWND hwnd)
{
    static PFNGETWINDOWTHEME pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETWINDOWTHEME)GetProc("GetWindowTheme", (void*)GetWindowThemeFail);
    return (*pfn)(hwnd);
}

HRESULT CVisualStylesXP::EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags)
{
    static PFNENABLETHEMEDIALOGTEXTURE pfn = nullptr;
    if (!pfn)
        pfn = (PFNENABLETHEMEDIALOGTEXTURE)GetProc("EnableThemeDialogTexture", (void*)EnableThemeDialogTextureFail);
    return (*pfn)(hwnd, dwFlags);
}

BOOL CVisualStylesXP::IsThemeDialogTextureEnabled(HWND hwnd)
{
    static PFNISTHEMEDIALOGTEXTUREENABLED pfn = nullptr;
    if (!pfn)
        pfn = (PFNISTHEMEDIALOGTEXTUREENABLED)GetProc("IsThemeDialogTextureEnabled", (void*)IsThemeDialogTextureEnabledFail);
    return (*pfn)(hwnd);
}

DWORD CVisualStylesXP::GetThemeAppProperties()
{
    static PFNGETTHEMEAPPPROPERTIES pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEAPPPROPERTIES)GetProc("GetThemeAppProperties", (void*)GetThemeAppPropertiesFail);
    return (*pfn)();
}

void CVisualStylesXP::SetThemeAppProperties(DWORD dwFlags)
{
    static PFNSETTHEMEAPPPROPERTIES pfn = nullptr;
    if (!pfn)
        pfn = (PFNSETTHEMEAPPPROPERTIES)GetProc("SetThemeAppProperties", (void*)SetThemeAppPropertiesFail);
    (*pfn)(dwFlags);
}

HRESULT CVisualStylesXP::GetCurrentThemeName(
    LPWSTR pszThemeFileName, int cchMaxNameChars,
    LPWSTR pszColorBuff, int cchMaxColorChars,
    LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
    static PFNGETCURRENTTHEMENAME pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETCURRENTTHEMENAME)GetProc("GetCurrentThemeName", (void*)GetCurrentThemeNameFail);
    return (*pfn)(pszThemeFileName, cchMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
}

HRESULT CVisualStylesXP::GetThemeDocumentationProperty(LPCWSTR pszThemeName,
    LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars)
{
    static PFNGETTHEMEDOCUMENTATIONPROPERTY pfn = nullptr;
    if (!pfn)
        pfn = (PFNGETTHEMEDOCUMENTATIONPROPERTY)GetProc("GetThemeDocumentationProperty", (void*)GetThemeDocumentationPropertyFail);
    return (*pfn)(pszThemeName, pszPropertyName, pszValueBuff, cchMaxValChars);
}

HRESULT CVisualStylesXP::DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT* prc)
{
    static PFNDRAWTHEMEPARENTBACKGROUND pfn = nullptr;
    if (!pfn)
        pfn = (PFNDRAWTHEMEPARENTBACKGROUND)GetProc("DrawThemeParentBackground", (void*)DrawThemeParentBackgroundFail);
    return (*pfn)(hwnd, hdc, prc);
}

HRESULT CVisualStylesXP::EnableTheming(BOOL fEnable)
{
    static PFNENABLETHEMING pfn = nullptr;
    if (!pfn)
        pfn = (PFNENABLETHEMING)GetProc("EnableTheming", (void*)EnableThemingFail);
    return (*pfn)(fEnable);
}
