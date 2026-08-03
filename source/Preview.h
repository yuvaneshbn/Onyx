#pragma once

#include "const.h"

#ifdef _GLOBAL_VIDEO

#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "langpack.h"

// Forward declarations for external globals (as in original)
class CmainDlg;
extern CmainDlg* mainDlg;
extern LangPackStruct langPack;   // assumed to have a boolean 'rtl' member

class Preview : public QWidget
{
    Q_OBJECT

public:
    explicit Preview(QWidget *pParent = nullptr);
    ~Preview();

    void Start(int id);

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    pjmedia_vid_dev_index widx;

private slots:
    void onWindowDestroyed();
};

#endif // _GLOBAL_VIDEO
