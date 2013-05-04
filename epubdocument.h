#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QWebPage>
#include <epub.h>
#include "epubnetworkmanager.h"

class EpubDocument : public QWebPage
{
    Q_OBJECT
public:
    EpubDocument(const QString &fileName);
    bool isValid();
    ~EpubDocument();
    struct epub *getEpub();
    QWebPage *convert();
    
signals:
    
public slots:

private:
    struct epub *mEpub;

    QString enableNetworkDownload(QString html);
};

#endif // EPUBDOCUMENT_H
