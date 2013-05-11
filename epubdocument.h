#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QWebPage>
#include <epub.h>
#include <QWebView>
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
    void renderPage();
    QImage renderImage(int i);

signals:

public slots:

private:
    struct epub *mEpub;
    QWebView *view;
    QString enableNetworkDownload(QString html);
};

#endif // EPUBDOCUMENT_H
