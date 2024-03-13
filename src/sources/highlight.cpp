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

#include "highlight.hpp"

Highlight::Highlight(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QStringList patterns;

    // Verse formating
    verseFormat.setForeground(Qt::red);
    verseFormat.setBackground(Qt::yellow);
    patterns << "^Verse[^\n]*" << "^&Verse[^\n]*"
             << QString::fromUtf8("^Куплет[^\n]*") << QString::fromUtf8("^&Куплет[^\n]*")
             << QString::fromUtf8("^Strophe[^\n]*") << QString::fromUtf8("^&Strophe[^\n]*")
             << QString::fromUtf8("^Verš[^\n]*") << QString::fromUtf8("^&Verš[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = verseFormat;
        highlightingRules.append(rule);
    }

    patterns.clear();

    // Chorus formating
    chorusFormat.setFontItalic(true);
    chorusFormat.setForeground(Qt::darkBlue);
    chorusFormat.setBackground(QColor(212,240,28,255));
    patterns << "^Chorus[^\n]*" << "^&Chorus[^\n]*"
             << QString::fromUtf8("^Sbor[^\n]*") << QString::fromUtf8("^&Sbor[^\n]*")
             << "^Refrain[^\n]*" << "^&Refrain[^\n]*"
             << QString::fromUtf8("^Припев[^\n]*") << QString::fromUtf8("^&Припев[^\n]*")
             << QString::fromUtf8("^Приспів[^\n]*") << QString::fromUtf8("^&Приспів[^\n]*")
             << QString::fromUtf8("^Refrén[^\n]*") << QString::fromUtf8("^&Refrén[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = chorusFormat;
        highlightingRules.append(rule);
    }

    patterns.clear();

    // Vsavka formating
    vstavkaFormat.setForeground(Qt::darkMagenta);
    vstavkaFormat.setBackground(QColor(255,140,0,255));
    patterns << "^Slide[^\n]*" << "^Insert[^\n]*"
             << "^Intro[^\n]*" << "^Ending[^\n]*"
             << QString::fromUtf8("^Слайд[^\n]*") << QString::fromUtf8("^Вставка[^\n]*")
             << QString::fromUtf8("^Вступление[^\n]*") << QString::fromUtf8("^Окончание[^\n]*")
             << QString::fromUtf8("^Закінчення[^\n]*")
             << QString::fromUtf8("^Dia[^\n]*") << QString::fromUtf8("^Einfügung[^\n]*")
             << QString::fromUtf8("^Einleitung[^\n]*") << QString::fromUtf8("^Ende[^\n]*")
             << QString::fromUtf8("^Snímek[^\n]*") << QString::fromUtf8("^Vložka[^\n]*")
             << QString::fromUtf8("^Úvod[^\n]*") << QString::fromUtf8("^Závěr[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = vstavkaFormat;
        highlightingRules.append(rule);
    }
}

void Highlight::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

// *** Announcement Editor Highlighter

HighlightAnnounce::HighlightAnnounce(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QStringList patterns;

    // Verse formating
    announceFormat.setForeground(Qt::red);
    announceFormat.setBackground(Qt::yellow);
    patterns << "^Announce[^\n]*" << "^Slide[^\n]*"
             << QString::fromUtf8("^Объявление[^\n]*") << QString::fromUtf8("^Слайд[^\n]*")
             << QString::fromUtf8("^Оголошення[^\n]*")
             << QString::fromUtf8("^Ankündigung[^\n]*") << QString::fromUtf8("^Dia[^\n]*")
             << QString::fromUtf8("^Oznámení[^\n]*") << QString::fromUtf8("^Snímek[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = announceFormat;
        highlightingRules.append(rule);
    }
}

void HighlightAnnounce::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

// *** Highligting for search results ***
HighlightSearch::HighlightSearch(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
}

void HighlightSearch::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

void HighlightSearch::setHighlightText(QString text)
{
    HighlightingRule rule;
    highlightingRules.clear();
    resultFormat.setForeground(Qt::red);
    rule.pattern = QRegExp(text,Qt::CaseInsensitive);
    rule.format = resultFormat;
    highlightingRules.append(rule);
}

/**********************************************/
/**** Class for higlighting search results ****/
/**********************************************/
HighlighterDelegate::HighlighterDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    textDocument = new QTextDocument(this);
    highlighter = new HighlightSearch(textDocument);
}

void HighlighterDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const
{
    Q_UNUSED(option);
    textDocument->setDocumentMargin(0);
    textDocument->setPlainText(text);

    QPixmap pixmap(rect.size());
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);

    textDocument->drawContents(&p);

    painter->drawPixmap(rect, pixmap);
}
