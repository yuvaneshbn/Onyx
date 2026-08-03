// ContactModel.h
#pragma once

#include <QAbstractListModel>
#include <QDomDocument>
#include <QFile>
#include <QVector>

struct ContactEntry {
    QString name;
    QString sipUri;
    bool isOnline = false;
};

class ContactModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ContactRoles {
        NameRole = Qt::UserRole + 1,
        SipUriRole,
        PresenceRole
    };

    explicit ContactModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    void loadFromFile(const QString &filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        QDomDocument doc;
        if (!doc.setContent(&file)) {
            file.close();
            return;
        }
        file.close();

        beginResetModel();
        m_contacts.clear();

        const QDomElement root = doc.documentElement();
        const QDomNodeList list = root.elementsByTagName(QStringLiteral("DirectoryEntry"));
        for (int i = 0; i < list.count(); ++i) {
            const QDomElement element = list.at(i).toElement();
            ContactEntry contact;
            contact.name = element.firstChildElement(QStringLiteral("Name")).text();

            const QString number = element.firstChildElement(QStringLiteral("Telephone")).text();
            if (!number.isEmpty()) {
                contact.sipUri = number.startsWith(QStringLiteral("sip:"))
                                     ? number
                                     : QStringLiteral("sip:%1").arg(number);
            }

            m_contacts.append(contact);
        }
        endResetModel();
    }

    Q_INVOKABLE void addTemporaryPeer(const QString &name, const QString &sipUri)
    {
        for (const auto &peer : m_contacts) {
            if (peer.sipUri == sipUri) {
                return;
            }
        }

        beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
        m_contacts.append({name, sipUri, true});
        endInsertRows();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.isValid()) {
            return 0;
        }
        return m_contacts.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_contacts.size()) {
            return {};
        }

        const ContactEntry &contact = m_contacts.at(index.row());
        switch (role) {
        case NameRole:
            return contact.name;
        case SipUriRole:
            return contact.sipUri;
        case PresenceRole:
            return contact.isOnline;
        default:
            return {};
        }
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            {NameRole, "name"},
            {SipUriRole, "sipUri"},
            {PresenceRole, "isOnline"},
        };
    }

private:
    QVector<ContactEntry> m_contacts;
};
