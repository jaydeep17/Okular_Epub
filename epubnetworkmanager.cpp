#include "epubnetworkmanager.h"



EpubNetworkManager::EpubNetworkManager(struct epub *mepub, QObject *parent)
{
    m_epub = mepub;
}

QNetworkReply *EpubNetworkManager::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    if (op != GetOperation)
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
    return new RendererReply(this, request, m_epub);
}




