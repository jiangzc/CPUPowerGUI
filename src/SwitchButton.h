#ifndef SWITCHHEADER_H
#define SWITCHHEADER_H

#include <QWidget>
#include <QString>
#include <QVector>

//palette.setColor(QPalette::Window, QColor(46, 134, 193));       // 背景颜色
//palette.setColor(QPalette::Highlight, QColor(33, 97, 140));     // 选中颜色
//palette.setColor(QPalette::WindowText, QColor(215, 219, 221));  // 文本颜色
//palette.setColor(QPalette::Light, QColor(93, 173, 226));        // 悬浮颜色

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    void append(const QString &text);
    bool insert(int index, const QString &text);
    bool remove(int index);
    int currentIndex() const;
    QString currentText() const;
    QString getText(int index) const;
    bool setText(int index, const QString &text);
    void setCurrentIndex(int index, bool sig=true);
    int count() const;

    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
private:
    QVector<QString> list;
    int m_currentIndex = -1;
    QPoint m_mouse;
    bool isHover = false;
    inline int pointToIndex(const QPoint &p) const;
    inline QRect indexToRect(int index) const;
    inline void drawIndexRect(QPainter &painter, int index, QBrush brush) const;


signals:
    void indexChanged(int);

public slots:
};

#endif // SWITCHHEADER_H
