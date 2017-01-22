#ifndef SPIMAGEPROVIDER_HPP
#define SPIMAGEPROVIDER_HPP

#include <QQuickImageProvider>

class SpImageProvider : public QQuickImageProvider
{

public:
    SpImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    void setPixMap(QPixmap &p);

private:
    QPixmap m_pixmap;
};


#endif // SPIMAGEPROVIDER_HPP
