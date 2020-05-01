#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPalette>
#include "SwitchHeader.h"

SwitchHeader::SwitchHeader(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(53, 152, 219));   // 背景颜色
    palette.setColor(QPalette::Highlight, QColor(93, 173, 226));    // 悬浮颜色
    palette.setColor(QPalette::WindowText, QColor(215, 219, 221));  // 文本颜色
    this->setPalette(palette);
}

void SwitchHeader::append(QString text)
{
    list.append(text);
}

bool SwitchHeader::insert(int index, QString text)
{
    if (0 <= index && index <= list.count())
    {
        list.insert(index, text);
        return true;
    }
    else
        return false;
}

bool SwitchHeader::remove(int index)
{
    if (0 <= index && index < list.count())
    {
        list.remove(index);
        return true;
    }
    else
        return false;
}

int SwitchHeader::currentIndex() const
{
    return m_currentIndex;
}

QString SwitchHeader::currentText() const
{
    if (0 <= m_currentIndex && m_currentIndex < list.count())
        return list[m_currentIndex];
    return QString();
}

QString SwitchHeader::getText(int index) const
{
    if (0 <= index && index < list.count())
        return list[index];
    return QString();
}

bool SwitchHeader::setText(int index, QString text)
{
    if (0 <= index && index < list.count())
    {
        list[index] = text;
        return true;
    }
    else
        return false;
}

void SwitchHeader::setCurrentIndex(int index)
{
    m_currentIndex = index;
}

QSize SwitchHeader::sizeHint() const
{
    return QSize(100 * list.count(), 40);
}

void SwitchHeader::paintEvent(QPaintEvent *)
{

}

void SwitchHeader::mousePressEvent(QMouseEvent *e)
{

}

void SwitchHeader::mouseMoveEvent(QMouseEvent *e)
{


}

int SwitchHeader::count() const
{
    return list.count();
}
