/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2017  Vladislav Kobzar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

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
