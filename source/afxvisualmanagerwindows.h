#pragma once

#include "afxvisualmanager.h"

class CMFCVisualManagerWindows : public CMFCVisualManager
{
public:
    static void SetDefaultManager(CMFCVisualManager *) {}
};
