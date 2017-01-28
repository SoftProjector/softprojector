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

#ifndef CONTROLBUTTON_HPP
#define CONTROLBUTTON_HPP

#include <QtWidgets>
//#include <QtGui>

class ControlButton : public QPushButton
{
    Q_OBJECT

public:
    ControlButton(QWidget * parent = 0);
    ControlButton(const QIcon & icon, const QIcon & iconHovered, const QIcon & iconPressed, QWidget * parent = 0);

    ~ControlButton();

    //Range: 0.0 [invisible] - 1.0 [opaque]
    void setOpacity(qreal opacity)        { m_opacity = opacity; }

protected:
    void paintEvent(QPaintEvent * pe);
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

private:
    QRect calculateIconPosition(QRect button_rect, QSize icon_size);

private:
    bool m_hovered;
    bool m_pressed;

    qreal m_opacity;
    QIcon m_icon;
    QIcon m_iconHovered;
    QIcon m_iconPressed;
};

#endif // CONTROLBUTTON_HPP
