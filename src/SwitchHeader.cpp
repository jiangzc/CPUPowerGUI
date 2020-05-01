#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPalette>
#include <QDebug>
#include <QMouseEvent>
#include "SwitchHeader.h"

const int spliterWidth = 2;
const int radius = 6;
SwitchHeader::SwitchHeader(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(53, 152, 219));       // 背景颜色
    palette.setColor(QPalette::Highlight, QColor(40, 116, 166));    // 选中颜色
    palette.setColor(QPalette::WindowText, QColor(215, 219, 221));  // 文本颜色
    palette.setColor(QPalette::Light, QColor(93, 173, 226));        // 悬浮颜色
    this->setPalette(palette);
    QFont font = this->font();
    font.setPixelSize(16);
    this->setFont(font);

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
    if (index != m_currentIndex)
    {
        m_currentIndex = index;
        update();
        emit indexChanged(index);
    }

}

QSize SwitchHeader::sizeHint() const
{
    return QSize(80 * list.count(), 40);
}

void SwitchHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().window());
    painter.drawRoundedRect(rect(), radius, radius);
    // draw hover rect
    if (isHover)
    {
        int hoverIndex = pointToIndex(m_mouse);
        drawIndexRect(painter, hoverIndex, palette().light());
    }
    // draw selected rect
    drawIndexRect(painter, m_currentIndex, palette().highlight());
    // draw spliter
    painter.setBrush(QColor(0,0,0, 50));
    QRect itemRect;
    for (int i = 0; i < list.count() - 1; i++)
    {
        itemRect = indexToRect(i);
        painter.drawRect(itemRect.x() + itemRect.width(), 0, spliterWidth, height());
    }
    // draw text
    painter.setFont(font());
    painter.setPen(QPen(palette().windowText().color()));
    for (int i = 0; i < list.count(); i++)
    {
        itemRect = indexToRect(i);
        painter.drawText(itemRect, Qt::AlignCenter, list[i]);
    }

}

void SwitchHeader::mousePressEvent(QMouseEvent *e)
{
    setCurrentIndex(pointToIndex(e->pos()));
}

void SwitchHeader::mouseMoveEvent(QMouseEvent *e)
{
    m_mouse = e->pos();
    update();
}

void SwitchHeader::enterEvent(QEvent *event)
{
    isHover = true;
    update();
}

void SwitchHeader::leaveEvent(QEvent *event)
{
    isHover = false;
    update();
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

void SwitchHeader::drawIndexRect(QPainter &painter, int index, QBrush brush) const
{
    painter.setBrush(brush);
    if (index > 0 && index < list.count() - 1)
    {
        painter.drawRect(indexToRect(index));
    }
    if (index == 0)
    {
        painter.drawRoundedRect(0, 0, radius * 2, height(), radius, radius);
        painter.drawRect(QRect(QPoint(radius, 0), QPoint(indexToRect(0).width() - 1, height())));
    }
    if (index == count() - 1)
    {
        QRect lastRect = indexToRect(count() - 1);
        painter.drawRoundedRect(width() - radius * 2, 0, radius * 2, height(), radius, radius);
        painter.drawRect(QRect(lastRect.x() , 0, lastRect.width() - radius, height()));
    }
}

int SwitchHeader::count() const
{
    return list.count();
}
