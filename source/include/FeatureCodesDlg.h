// FeatureCodesDlg.h
#ifndef FEATURECODESDLG_H
#define FEATURECODESDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class FeatureCodesDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FeatureCodesDlg(QWidget *parent = nullptr);
    virtual ~FeatureCodesDlg();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUi();
    void retranslateUi();

    QLineEdit *m_editCP;
    QCheckBox *m_checkCP;
    QLineEdit *m_editBT;
    QCheckBox *m_checkBT;
    QLineEdit *m_editAT;
    QCheckBox *m_checkAT;
};

#endif // FEATURECODESDLG_H