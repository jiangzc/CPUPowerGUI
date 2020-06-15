#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPixmap>
class SwitchButton;
class QStackedLayout;
class QVBoxLayout;
class QTimer;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

private:
    QPixmap background;
    SwitchButton *header;
    QVBoxLayout *mainLayout;
    QStackedLayout *stackLayout;
    QTimer *timer;
signals:

public slots:
};

#endif // MAINWINDOW_H
