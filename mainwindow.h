#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "session.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    Session session_;
    QImage image_;

public slots:
    void repaintFrame(QImage frame);
    void onStartButtonClicked();
    void onStopButtonClicked();

signals:
    void repainted();
};

#endif // MAINWINDOW_H
