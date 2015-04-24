#include "gist.h"

Gist::Gist(QObject *parent) : QObject(parent)
{

}

Gist::~Gist()
{

}

void Gist::start()
{
    QClipboard *clip = qGuiApp->clipboard();
    const QMimeData *mime = clip->mimeData();
    if (mime->hasText()) {
        QString text = mime->text();
        if (text.startsWith("file://")) {
            text = text.mid(7);
        }
        if (QFile(text).exists()) {
            QMimeDatabase db;
            QMimeType type = db.mimeTypeForFile(text);
            if (type.inherits("text/plain")) {
                QFile file(text);
                if (file.open(QFile::ReadOnly)) {
                    paste("GistPaster", text.split("/").last(), QString(file.readAll()));
                    file.close();
                    return;
                }
            }
        }
        else {
            paste("GistPaster", "clipboard.txt", text);
            return;
        }
    }

    qGuiApp->exit(1);
}

void Gist::paste(const QString &description, const QString &fileName, const QString &data)
{
    QVariantMap content;
    content["content"] = data;

    QVariantMap files;
    files[fileName] = content;

    QVariantMap payload;
    payload["description"] = description;
    payload["public"] = true;
    payload["files"] = files;

    QByteArray json = QJsonDocument::fromVariant(payload).toJson();

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QUrl url("https://api.github.com/gists");
    QNetworkRequest req(url);
    QNetworkReply *reply = nam->post(req, json);
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(pasteFinished()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(pasteError(QNetworkReply::NetworkError)));
}

void Gist::pasteFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply) {
        QByteArray json = reply->readAll();
        QVariantMap result = QJsonDocument::fromJson(json).toVariant().toMap();
        if (result.contains("html_url")) {
            qDebug() << result["html_url"].toString();
        }
    }

    qGuiApp->exit(0);
}

void Gist::pasteError(QNetworkReply::NetworkError error)
{
    qGuiApp->exit(2);
}

