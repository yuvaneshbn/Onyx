#include <QCoreApplication>
#include <QDir>
#include <QGuiApplication>
#include <QLockFile>
#include <QLocalServer>
#include <QLocalSocket>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core/ConfigManager.h"
#include "models/ContactModel.h"
#include "network/FileTransferManager.h"
#include "network/LanDiscoveryManager.h"
#include "network/SipController.h"

class SingleInstanceController : public QObject
{
    Q_OBJECT

public:
    explicit SingleInstanceController(const QString &serverName, QObject *parent = nullptr)
        : QObject(parent), m_serverName(serverName)
    {
    }

    bool listen()
    {
        QLocalServer::removeServer(m_serverName);
        m_server = new QLocalServer(this);
        connect(m_server, &QLocalServer::newConnection, this, &SingleInstanceController::handleNewConnection);
        return m_server->listen(m_serverName);
    }

signals:
    void commandReceived(const QString &command);

private:
    void handleNewConnection()
    {
        QLocalSocket *socket = m_server->nextPendingConnection();
        connect(socket, &QLocalSocket::readyRead, this, [this, socket]() {
            const QString command = QString::fromUtf8(socket->readAll()).trimmed();
            emit commandReceived(command);
            socket->disconnectFromServer();
        });
    }

    QString m_serverName;
    QLocalServer *m_server = nullptr;
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("Onyx-voip"));
    QCoreApplication::setOrganizationName(QStringLiteral("Onyx"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("onyx-voip.local"));

    const QString appName = QStringLiteral("onyx-voip");
    const QString lockPath = QDir::tempPath() + QLatin1Char('/') + appName + QStringLiteral(".lock");
    QLockFile lockFile(lockPath);

    if (!lockFile.tryLock(150)) {
        QLocalSocket socket;
        socket.connectToServer(appName);
        if (socket.waitForConnected(500)) {
            const QString cmdLineArg = (argc > 1) ? QString::fromLocal8Bit(argv[1]) : QString();
            socket.write(cmdLineArg.toUtf8());
            socket.waitForBytesWritten(500);
        }
        return 0;
    }

    SingleInstanceController instanceBroker(appName);
    if (!instanceBroker.listen()) {
        return -1;
    }

    QQmlApplicationEngine engine;

    ConfigManager *configManager = new ConfigManager(QStringLiteral("onyx-voip.ini"), &app);
    SipController *sipController = SipController::instance();
    ContactModel *contactModel = new ContactModel(&app);
    contactModel->loadFromFile(QStringLiteral("Contacts.xml"));

    LanDiscoveryManager *discoveryManager = new LanDiscoveryManager(&app);
    FileTransferManager *fileTransfer = new FileTransferManager(25001, &app);

    QObject::connect(&instanceBroker, &SingleInstanceController::commandReceived,
                     sipController, &SipController::initiateCall);

    QObject::connect(discoveryManager, &LanDiscoveryManager::peerDiscovered,
                     contactModel, &ContactModel::addTemporaryPeer);

    if (!sipController->initializeEngine(configManager)) {
        return -1;
    }

    engine.rootContext()->setContextProperty(QStringLiteral("sipController"), sipController);
    engine.rootContext()->setContextProperty(QStringLiteral("contactModel"), contactModel);
    engine.rootContext()->setContextProperty(QStringLiteral("discoveryManager"), discoveryManager);
    engine.rootContext()->setContextProperty(QStringLiteral("fileTransfer"), fileTransfer);
    engine.rootContext()->setContextProperty(QStringLiteral("configManager"), configManager);

    QObject::connect(&engine,
                     &QQmlApplicationEngine::objectCreationFailed,
                     &app,
                     []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("OnyxVoip"), QStringLiteral("Main"));
    const int execResult = app.exec();

    sipController->shutdownEngine();
    return execResult;
}

#include "main.moc"
