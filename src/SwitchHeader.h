#ifndef SWITCHHEADER_H
#define SWITCHHEADER_H

#include <QWidget>
#include <QString>
#include <QVector>

// draw colors: QPalette::WindowText QPalette::Base
class SwitchHeader : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchHeader(QWidget *parent = nullptr);
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
