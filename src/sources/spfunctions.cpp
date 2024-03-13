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

#include "spfunctions.hpp"
//spFunctions::spFunctions()
//{
//}

QByteArray pixToByte(const QPixmap & pmap)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pmap.save(&buffer, "JPG",90);
    return buffer.data();
}

bool isAnnounceTitle(QString string)
{
    // Check if the line is verse title line
    if(string.startsWith("Announce"))
        return true;
    else if (string.startsWith("Slide"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Объявление")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Слайд")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Оголошення")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Ankündigung")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Oznámení")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Snímek")))
        return true;
    else
        return false;
}

QString getSupportedImageFormats()
{
    QList<QByteArray> im = QImageReader::supportedImageFormats();
    QString imfor;
    foreach (QByteArray f,im)
        imfor += " *." + f;
    return imfor.trimmed();
}
