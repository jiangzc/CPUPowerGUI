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
    int currentIndex();
    QString currentText();
    QString getText(int index);
    bool setText(int index, QString text);
    bool setIndex(int index);
    int count();

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
private:
    QVector<QString> list;
    int m_currentIndex;

signals:
    void indexChanged(int);
public slots:
};

#endif // SWITCHHEADER_H
