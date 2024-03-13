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

#ifndef BIBLESETTINGWIDGET_HPP
#define BIBLESETTINGWIDGET_HPP

#include <QtWidgets>
#include <QtSql>
#include "theme.hpp"
#include "settings.hpp"
#include "spfunctions.hpp"

namespace Ui {
class BibleSettingWidget;
}

class BibleSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BibleSettingWidget(QWidget *parent = 0);
    ~BibleSettingWidget();

public slots:
    void setSettings(BibleSettings &settings, BibleSettings &settings2, BibleSettings &settings3, BibleSettings &settings4);
    void getSettings(BibleSettings &settings, BibleSettings &settings2, BibleSettings &settings3, BibleSettings &settings4);
    void setBibleVersions(BibleVersionSettings &bver, BibleVersionSettings &bver2, BibleVersionSettings &bver3, BibleVersionSettings &bver4);
    void getBibleVersions(BibleVersionSettings &bver, BibleVersionSettings &bver2, BibleVersionSettings &bver3, BibleVersionSettings &bver4);
    void setDispScreen2Visible(bool visible);
    void setDispScreen3Visible(bool visible);
    void setDispScreen4Visible(bool visible);
    void setBackgroungds(QString name, QPixmap back);

signals:
    void applyBackToAll(int t, QString backName, QPixmap background);

private slots:
    void loadSettings();
    void loadBibleVersions();
    void updateSecondaryBibleMenu();
    void updateSecondaryBibleMenu2();
    void updateSecondaryBibleMenu3();
    void updateSecondaryBibleMenu4();
    void updateTrinaryBibleMenu();
    void updateTrinaryBibleMenu2();
    void updateTrinaryBibleMenu3();
    void updateTrinaryBibleMenu4();
    void updateOperatorBibleMenu();

    void on_comboBoxPrimaryBible_activated(const QString &arg1);
    void on_comboBoxPrimaryBible2_activated(const QString &arg1);
    void on_comboBoxPrimaryBible3_activated(const QString &arg1);
    void on_comboBoxPrimaryBible4_activated(const QString &arg1);
    void on_comboBoxSecondaryBible_activated(const QString &arg1);
    void on_comboBoxSecondaryBible2_activated(const QString &arg1);
    void on_comboBoxSecondaryBible3_activated(const QString &arg1);
    void on_comboBoxSecondaryBible4_activated(const QString &arg1);
    void on_buttonBrowseBackground_clicked();
    void on_buttonBrowseBackground2_clicked();
    void on_buttonBrowseBackground3_clicked();
    void on_buttonBrowseBackground4_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_checkBoxUseShadow2_stateChanged(int arg1);
    void on_checkBoxUseShadow3_stateChanged(int arg1);
    void on_checkBoxUseShadow4_stateChanged(int arg1);
    void on_toolButtonTextColor_clicked();
    void on_toolButtonTextColor2_clicked();
    void on_toolButtonTextColor3_clicked();
    void on_toolButtonTextColor4_clicked();
    void on_toolButtonTextFont_clicked();
    void on_toolButtonTextFont2_clicked();
    void on_toolButtonTextFont3_clicked();
    void on_toolButtonTextFont4_clicked();
    void on_toolButtonCaptionColor_clicked();
    void on_toolButtonCaptionColor2_clicked();
    void on_toolButtonCaptionColor3_clicked();
    void on_toolButtonCaptionColor4_clicked();
    void on_toolButtonCaptionFont_clicked();
    void on_toolButtonCaptionFont2_clicked();
    void on_toolButtonCaptionFont3_clicked();
    void on_toolButtonCaptionFont4_clicked();
    void on_groupBoxAddBackgroundToText_toggled(bool arg1);
    void on_groupBoxAddBackgroundToText2_toggled(bool arg1);
    void on_groupBoxAddBackgroundToText3_toggled(bool arg1);
    void on_groupBoxAddBackgroundToText4_toggled(bool arg1);
    void on_toolButtonAddBKColorText_clicked();
    void on_toolButtonChangeGenBKColor_clicked();
    void on_toolButtonAddBKColorText2_clicked();
    void on_toolButtonChangeGenBKColor2_clicked();
    void on_toolButtonAddBKColorText3_clicked();
    void on_toolButtonChangeGenBKColor3_clicked();
    void on_toolButtonAddBKColorText4_clicked();
    void on_toolButtonChangeGenBKColor4_clicked();
    void on_groupBoxUseDisp2_toggled(bool arg1);
    void on_groupBoxUseDisp3_toggled(bool arg1);
    void on_groupBoxUseDisp4_toggled(bool arg1);
    void on_pushButtonDefault_clicked();

    QString getFontText(QFont font);
    void on_pushButtonApplyToAll_clicked();

private:
    QStringList bibles, secondary_bibles;
    QStringList bible_id_list, secondary_id_list, trinary_id_list, operator_id_list;
    QStringList secondary_bibles2, secondary_id_list2, trinary_id_list2;
    QStringList secondary_bibles3, secondary_id_list3, trinary_id_list3;
    QStringList secondary_bibles4, secondary_id_list4, trinary_id_list4;
    BibleSettings mySettings, mySettings2, mySettings3, mySettings4;
    BibleVersionSettings bversion,bversion2,bversion3,bversion4;
    Ui::BibleSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // BIBLESETTINGWIDGET_HPP
