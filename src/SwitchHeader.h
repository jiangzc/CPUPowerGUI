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
    void append(QString text);
    bool insert(int index, QString text);
    bool remove(int index);
    int currentIndex() const;
    QString currentText() const;
    QString getText(int index) const;
    bool setText(int index, QString text);
    void setCurrentIndex(int index);
    int count() const;

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
private:
    QVector<QString> list;
    int m_currentIndex;

signals:
    void indexChanged(int);
public slots:
};

#endif // SWITCHHEADER_H
