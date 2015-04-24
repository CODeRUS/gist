#ifndef GIST_H
#define GIST_H

#include <QObject>
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>

#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDateTime>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>

#include <QDebug>

class Gist : public QObject
{
    Q_OBJECT
public:
    explicit Gist(QObject *parent = 0);
    ~Gist();

public slots:
    void start();

private:
    void paste(const QString &description, const QString &fileName, const QString &data);

private slots:
    void pasteFinished();
    void pasteError(QNetworkReply::NetworkError error);

};

#endif // GIST_H
