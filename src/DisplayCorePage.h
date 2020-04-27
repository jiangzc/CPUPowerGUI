#ifndef DISPLAYCOREPAGE_H
#define DISPLAYCOREPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
class DisplayCorePage : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayCorePage(QWidget *parent = nullptr);

private:
    QPlainTextEdit *infoList;
signals:

public slots:
};

#endif // DISPLAYCOREPAGE_H
