#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QDockWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QColor>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int calculateState(int left, int state, int right);
    int binaryToInt(QString binary);

public slots:
    void draw();
    void reloadCanvas();
    void changeBoxColor();
    void about();
    void aboutQT();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QDockWidget *menuDock;
    QSpinBox *ruleNumberSpinBox;
    QSpinBox *boxSizeSpinBox;
    QPushButton *displayButton;
    QPushButton *colorButton;
    QTimer *timer;
    QList<int> cells;
    QColor boxColor;
    QString ruleset;
    int y;
    int boxWidth;
    int boxHeight;
};
#endif // MAINWINDOW_H
