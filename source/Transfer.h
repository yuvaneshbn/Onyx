#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QFont>
#include <QString>

#include "resource.h"
#include "const.h"
#include "global.h"
#include "Contacts.h"

enum onyx_action {
	ONYX_ACTION_TRANSFER,
	ONYX_ACTION_ATTENDED_TRANSFER,
	ONYX_ACTION_INVITE,
	ONYX_ACTION_FORWARD,
};

class Transfer :
	public QDialog
{
    Q_OBJECT

public:
	explicit Transfer(QWidget* parent = nullptr);
	~Transfer() override;
	enum { IDD = IDD_TRANSFER };
	onyx_action action;
	pjsua_call_id callId;
	void SetAction(onyx_action action, pjsua_call_id call_id = PJSUA_INVALID_ID);
	bool Action(onyx_action action);
	void LoadFromContacts(Contact *selectedContact = nullptr);
protected:
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void done(int r) override;

private:
	void ClearDropdown();
	void Input(const QString &digits);
	void OnBnClickedOk();
	void OnBnClickedCancel();
	void OnBnClickedAttended();
	void OnBnClickedBlind();
	void OnBnClickedKey1();
	void OnBnClickedKey2();
	void OnBnClickedKey3();
	void OnBnClickedKey4();
	void OnBnClickedKey5();
	void OnBnClickedKey6();
	void OnBnClickedKey7();
	void OnBnClickedKey8();
	void OnBnClickedKey9();
	void OnBnClickedKeyStar();
	void OnBnClickedKey0();
	void OnBnClickedKeyGrate();

	QFont m_font;
	QComboBox *m_comboNumber = nullptr;
	QPushButton *m_btnOk = nullptr;
	QPushButton *m_btnCancel = nullptr;
	QPushButton *m_btnAttended = nullptr;
	QPushButton *m_btnBlind = nullptr;
	QPushButton *m_btnKey1 = nullptr;
	QPushButton *m_btnKey2 = nullptr;
	QPushButton *m_btnKey3 = nullptr;
	QPushButton *m_btnKey4 = nullptr;
	QPushButton *m_btnKey5 = nullptr;
	QPushButton *m_btnKey6 = nullptr;
	QPushButton *m_btnKey7 = nullptr;
	QPushButton *m_btnKey8 = nullptr;
	QPushButton *m_btnKey9 = nullptr;
	QPushButton *m_btnKeyStar = nullptr;
	QPushButton *m_btnKey0 = nullptr;
	QPushButton *m_btnKeyGrate = nullptr;
};
