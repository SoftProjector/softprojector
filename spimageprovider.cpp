#include "spimageprovider.hpp"

SpImageProvider::SpImageProvider() :
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap SpImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return m_pixmap;
}

void SpImageProvider::setPixMap(QPixmap &p)
{
    m_pixmap = p;
}
