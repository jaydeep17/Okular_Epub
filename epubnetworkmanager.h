#ifndef EPUBNETWORKMANAGER_H
#define EPUBNETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QBuffer>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QDir>
#include <QImage>
#include <epub.h>
#include <QDebug>

class EpubNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit EpubNetworkManager(struct epub* mepub, QObject *parent = 0);
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
signals:
    
public slots:

private:
    struct epub* m_epub;
    
};


class RendererReply : public QNetworkReply
{
    Q_OBJECT
public:
    RendererReply(QObject* object, const QNetworkRequest& request, struct epub *mEpub)
        : QNetworkReply(object), position(0)
    {
        setRequest(request);
        setOperation(QNetworkAccessManager::GetOperation);
        QString path = request.url().host() + request.url().path();
        if(path.at(0)==QDir::separator())
            path = path.right(path.length()-1);
        if(path.at(path.length()-1)==QDir::separator())
            path.chop(1);
        filetype = getQVariantFromExt(QFileInfo(path).suffix());
        setHeader(QNetworkRequest::ContentTypeHeader,filetype);
        open(ReadOnly|Unbuffered);
        setUrl(request.url());
        QString pattern = request.url().queryItemValue("pattern");
        connect(&watcher, SIGNAL(finished()), SLOT(generateDone()));
        QFuture<QByteArray> future = QtConcurrent::run<QByteArray>(generate, path, mEpub);
        watcher.setFuture(future);
    }

    qint64 readData(char* data, qint64 maxSize)
    {
        const qint64 readSize = qMin(maxSize, (qint64)(buffer.size() - position));
        memcpy(data, buffer.constData() + position, readSize);
        position += readSize;
        return readSize;
    }

    virtual qint64 bytesAvailable() const
    {
        return buffer.size() - position;
    }

    virtual qint64 pos () const
    {
        return position;
    }

    virtual bool seek( qint64 pos )
    {
        if (pos < 0 || pos >= buffer.size())
            return false;
        position = pos;
        return true;
    }

    virtual qint64 size () const
    {
        return buffer.size();
    }

    static QByteArray generate(QString path, struct epub* Epub)
    {
        int size;
        char *data;

        // Get the data from the epub file
        size = epub_get_data(Epub, path.toUtf8(), &data);

        QByteArray saveData(data,size);
        return saveData;
    }

    static QVariant getQVariantFromExt(QString ext)
    {
        if(ext == "css"){
            return QVariant("text/css");
        } else if (ext == "js"){
            return QVariant("application/x-javascript");
        } else if(ext == "png"){
            return QVariant("image/"+ext);
        } else if(ext == "jpg" || ext == "jpeg"){
            return QVariant("image/jpeg");
        }
    }

public slots:
    void generateDone()
    {
        setHeader(QNetworkRequest::ContentTypeHeader, filetype);
        position = 0;
        buffer = watcher.result();
        emit readyRead();
        emit finished();
    }

    void abort()
    {
    }
public:
    QNetworkReply* rawReply;
private:
    QFutureWatcher<QByteArray> watcher;
    QVariant filetype;
    QByteArray buffer;
    int position;
};


#endif // EPUBNETWORKMANAGER_H
