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
    QPixmap renderPixmap(int i);
    int pages();
    void generateTextBounds(int pg);
    QList<QRect> textBounds();
    QStringList pageText();

signals:

public slots:

private:
    struct epub *mEpub;
    QWebView *mView;
    QList<QRect> m_textBounds;
    QStringList m_pageText;
    QString enableNetworkDownload(QString html);
    bool intersectsWithAny(const QRect &rec);
};

#endif // EPUBDOCUMENT_H
