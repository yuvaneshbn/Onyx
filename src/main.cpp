#include <QApplication>
#include <QSettings>
#include "ui/MainWindow.h"
#include "ui/DarkTheme.h"
#include "ui/LoginDialog.h"

int main(int argc, char* argv[]) {
    // Explicitly initialize Qt resources bundle
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    app.setApplicationName("Onyx");
    app.setOrganizationName("Onyx");
    app.setApplicationVersion("2.0.0");

    Onyx::DarkTheme::applyTheme(app);

    // Prompt user for their name and profile upon opening application
    Onyx::LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted) {
        return 0; // User closed startup dialog
    }

    Onyx::MainWindow window;
    window.initializeProfile(loginDlg.username(), loginDlg.statusMessage(), loginDlg.avatarColor());
    window.show();

    return app.exec();
}

