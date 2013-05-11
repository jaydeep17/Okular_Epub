#include "epubdocument.h"
#include <QDir>
#include <QWebFrame>
#include <QtXml/QXmlStreamReader>
#include "epubnetworkmanager.h"
#include <QDomDocument>
#include <QDomNodeList>
#include <QWebView>
#include <QtCore/qmath.h>
#include <QFile>

EpubDocument::EpubDocument(const QString &fileName)
{
    mEpub = epub_open(qPrintable(fileName), 3);
    if(!isValid())
        return;

    setNetworkAccessManager(new EpubNetworkManager(mEpub, this));
    view = new QWebView();
    view->setFixedSize(600,800);
}

bool EpubDocument::isValid()
{
    return (mEpub?true:false);
}

EpubDocument::~EpubDocument()
{
    if (mEpub)
        epub_close(mEpub);

    epub_cleanup();
}

struct epub *EpubDocument::getEpub()
{
    return mEpub;
}

static void saveToFile(QString filename,QString html){
    QFile file(QDir::current().absoluteFilePath(filename));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << html;
    file.close();
}

QWebPage *EpubDocument::convert()
{
    struct eiterator *it = epub_get_iterator(mEpub, EITERATOR_SPINE, 0);
    QString html;
    do {
        if (epub_it_get_curr(it)) {
            QString temp = QString::fromUtf8(epub_it_get_curr(it));
            html += enableNetworkDownload(temp);
        }
    } while (epub_it_get_next(it));

    mainFrame()->setHtml(html);
    epub_free_iterator(it);
    return this;
}

void EpubDocument::renderPage()
{
    QWebPage *page = convert();
    view->setPage(page);
//    view->show();
}

QPixmap EpubDocument::renderPixmap(int i)
{
    QWebFrame *webFrame = view->page()->mainFrame();
    webFrame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    webFrame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    int wd = 600, ht = 800, x = 0, y = 0;
    if(i > 0){
        y = i * 800;
    }
    view->page()->mainFrame()->setScrollPosition(QPoint(x,y));

    return QPixmap::grabWidget(view,0, 0, wd, ht);
}

int EpubDocument::pages()
{
    return qCeil(view->page()->mainFrame()->contentsSize().height()/800);
}

QString EpubDocument::enableNetworkDownload(QString html)
{
    QDomDocument doc;
    doc.setContent(html);
    // images
    QDomNodeList list = doc.elementsByTagName("img");
    for(int i=0;i<list.count();i++){
        QString src = list.at(i).attributes().namedItem("src").nodeValue();
        list.at(i).attributes().namedItem("src").setNodeValue("http://"+src);
    }

    // style sheets
    list = doc.elementsByTagName("link");
    for(int i=0;i<list.count();i++){
        if(list.at(i).attributes().namedItem("type").nodeValue() == "text/css"){
            // Check for case sensitivity above

            QString href = list.at(i).attributes().namedItem("href").nodeValue();
            list.at(i).attributes().namedItem("href").setNodeValue("http://"+href);
        }
    }

    // svgs
    list = doc.elementsByTagName("image");
    for(int i=0;i<list.count();i++){
        QString src = list.at(i).attributes().namedItem("xlink:href").nodeValue();
        list.at(i).attributes().namedItem("xlink:href").setNodeValue("http://"+src);
    }
    return doc.toString();
}

