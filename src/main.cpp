#include "ui/main_window.h"
#include "core/event_bus.h"
#include "network/tree.h"
#include "audio/audio_engine.h"
#include "net/control_client.h"

#include <QApplication>
#include <QStyleFactory>
#include <QInputDialog>
#include <QMessageBox>
#include <QRandomGenerator>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    try {
        // Initialize audio at user action time (avoid immediate mic capture)
        AudioEngine uiAudio;

        // Prompt for unique client name
        QString clientId;
        while (true) {
            QString suggested = QStringLiteral("client_%1").arg(QRandomGenerator::global()->bounded(1000));
            bool ok = false;
            clientId = QInputDialog::getText(nullptr, "Choose Client Name",
                                             "Enter a unique client name:", QLineEdit::Normal,
                                             suggested, &ok);
            if (!ok || clientId.isEmpty()) {
                return 0;
            }
            if (ensure_unique_name(clientId.toStdString(), 47000)) {
                break;
            }
            QMessageBox::warning(nullptr, "Name in use",
                                 QString("Client name '%1' is already in use. Choose another.").arg(clientId));
        }

        // 1. Initialize Core modules
        auto& eventBus = core::EventBus::instance();

        // 2. Initialize Network modules
        // (In a real app, these would be instantiated and wired here)
        // network::PeerManager peerManager;
        // network::Tree tree(&peerManager, 1000 /* client id */);
        
        // 3. Initialize UI
        MainWindow w(clientId, "127.0.0.1", &uiAudio, "secret");
        
        // 4. Wire backend events to UI
        QObject::connect(&eventBus, &core::EventBus::eventFired, 
            [&w](const QString& topic, const QVariant& payload) {
                Q_UNUSED(payload);
                if (topic == "Network::RoleChanged") {
                    // e.g. w.setRoleIndicator(payload.toString());
                } else if (topic == "Audio::ActiveSpeaker") {
                    // e.g. w.highlightSpeaker(payload.toULongLong());
                }
            }
        );

        w.show();
        
        return app.exec();
    } catch (const std::exception& ex) {
        QMessageBox::critical(nullptr, "Fatal error", QString::fromUtf8(ex.what()));
        return -1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Fatal error", "Unknown exception");
        return -1;
    }
}
