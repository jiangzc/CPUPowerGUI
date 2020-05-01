#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPalette>
#include <QMouseEvent>
#include "SwitchHeader.h"

const int spliterWidth = 4;

SwitchHeader::SwitchHeader(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(53, 152, 219));   // 背景颜色
    palette.setColor(QPalette::Highlight, QColor(40, 116, 166));    // 选中颜色
    palette.setColor(QPalette::WindowText, QColor(215, 219, 221));  // 文本颜色
    this->setPalette(palette);
    m_mouse.rx() = m_mouse.ry() = -1;
}

void SwitchHeader::append(const QString &text)
{
    list.append(text);
}

bool SwitchHeader::insert(int index, const QString &text)
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

bool SwitchHeader::setText(int index, const QString &text)
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
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().background());
    painter.drawRoundedRect(rect(), 6, 6);
    // draw selected rect
    painter.setBrush(palette().highlight());
    if (m_currentIndex > 0 && m_currentIndex < list.count() - 1)
    {
        painter.drawRect(indexToRect(m_currentIndex));
    }
    // draw spliter
    painter.setBrush(QColor(0,0,0, 50));
    QRect itemRect;
    for (int i = 0; i < list.count() - 1; i++)
    {
        itemRect = indexToRect(i);
        painter.drawRect(itemRect.x() + itemRect.width(), 0, spliterWidth, height());
    }

}

void SwitchHeader::mousePressEvent(QMouseEvent *e)
{

}

void SwitchHeader::mouseMoveEvent(QMouseEvent *e)
{
    m_mouse = e->pos();
}

void SwitchHeader::enterEvent(QEvent *event)
{
    isHover = true;
}

void SwitchHeader::leaveEvent(QEvent *event)
{
    isHover = false;
}

int SwitchHeader::pointToIndex(const QPoint &p) const
{
    int rectWidth = (width() + spliterWidth) / list.count();
    return p.x() / rectWidth;
}

QRect SwitchHeader::indexToRect(int index) const
{
    int rectWidth = (width() + spliterWidth) / list.count();
    return QRect(index * rectWidth, 0, rectWidth - spliterWidth, height());
}

int SwitchHeader::count() const
{
    return list.count();
}
