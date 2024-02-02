#include "mainwindow.h"
#include <QGraphicsRectItem>
#include <QtCore>
#include <QtGui>
#include <QPainter>
#include <QDebug>
#include <math.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Window layout
    this->setWindowTitle("Elementary Cellular Automata");
    this->setWindowIcon(QIcon("icons/app-icon.ico"));
    this->setStyleSheet("QMainWindow {"
                        "color: #FFFFFF;"
                        "background-color: #454545;"
                        "font-family: Corbel Light;"
                        "}");

    QRect screenRect = qApp->screens()[0]->availableGeometry();

    scene = new QGraphicsScene(0, 0, screenRect.width() - 38, screenRect.height() - 80, this);
    boxColor = Qt::black;

    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setGeometry(screenRect);

    ruleNumberSpinBox = new QSpinBox();
    ruleNumberSpinBox->setMaximum(255);
    ruleNumberSpinBox->setMinimum(0);
    ruleNumberSpinBox->setValue(18);

    boxSizeSpinBox = new QSpinBox();
    boxSizeSpinBox->setMinimum(1);
    boxSizeSpinBox->setMaximum(15);
    boxSizeSpinBox->setValue(3);

    displayButton = new QPushButton("Reload Canvas");
    connect(displayButton, SIGNAL(clicked(bool)), this, SLOT(reloadCanvas()));

    QString spinbox_stylesheet = "QSpinBox {"
                                 "width: 150;"
                                 "height: 20;"
                                 "}";
    boxSizeSpinBox->setStyleSheet(spinbox_stylesheet);
    ruleNumberSpinBox->setStyleSheet(spinbox_stylesheet);

    displayButton->setStyleSheet("QPushButton {"
                                 "width: 150;"
                                 "height: 20;"
                                 "}"
                                 ""
                                 "QPushButton:hover {"
                                 "background-color: #4E8A56;"
                                 "color: white;"
                                 "border-radius: 5;"
                                 "}");

    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *labelOne = new QLabel("Rule Number: ");
    labelOne->setStyleSheet("QLabel{color: #FFFFFF}");
    layout->addWidget(labelOne);
    layout->addWidget(ruleNumberSpinBox);
    layout->addSpacing(50);
    QLabel *labelTwo = new QLabel("Box Size:");
    labelTwo->setStyleSheet("QLabel{color: #FFFFFF}");
    layout->addWidget(labelTwo);
    layout->addWidget(boxSizeSpinBox);
    layout->addSpacing(50);
    QLabel *labelThree = new QLabel("Box Color:");
    labelThree->setStyleSheet("QLabel{color: #FFFFFF}");
    layout->addWidget(labelThree);
    colorButton = new QPushButton();
    connect(colorButton, SIGNAL(clicked(bool)), this, SLOT(changeBoxColor()));
    colorButton->setStyleSheet("QPushButton{background-color: #000000; width: 30; height: 20; border-radius: 5}");
    layout->addWidget(colorButton);
    layout->addSpacing(200);
    layout->addWidget(displayButton);
    layout->addStretch();
    layout->addSpacing(200);
    QPushButton *aboutButton = new QPushButton("About");
    aboutButton->setStyleSheet("QPushButton {"
                               "width: 75;"
                               "height: 20;"
                               "}"
                               ""
                               "QPushButton:hover {"
                               "background-color: #4E8A56;"
                               "color: white;"
                               "border-radius: 5;"
                               "}");
    QPushButton *aboutQTButton = new QPushButton("About QT");
    aboutQTButton->setStyleSheet("QPushButton {"
                               "width: 75;"
                               "height: 20;"
                               "}"
                               ""
                               "QPushButton:hover {"
                               "background-color: #4E8A56;"
                               "color: white;"
                               "border-radius: 5;"
                               "}");
    connect(aboutQTButton, SIGNAL(clicked(bool)), this, SLOT(aboutQT()));
    connect(aboutButton, SIGNAL(clicked(bool)), this, SLOT(about()));
    layout->addWidget(aboutButton);
    layout->addWidget(aboutQTButton);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(layout);
    mainLayout->addWidget(view);

    QWidget *placeholder = new QWidget();
    placeholder->setLayout(mainLayout);
    this->setCentralWidget(placeholder);

    view->setScene(scene);

    // Automata setup
    boxWidth = 3;
    boxHeight = 3;
    y = 0;
    ruleset = QString::number(18, 2).rightJustified(8, '0');

    int total = (screenRect.width() - 38) / boxWidth;
    for(int i = 0; i < total; i++)
    {
        if(i == total/2)
            cells.append(1);
        cells.append(0);
    }

    timer = new QTimer(this);
    timer->setInterval(8);
    connect(timer, SIGNAL(timeout()), this, SLOT(draw()));
    timer->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::draw()
{
    if(y > view->height()-15)
        timer->stop();

    for(int i = 0; i < cells.length(); i++)
    {
        if(cells[i] == 0)
            scene->addRect((boxWidth * i), y, boxWidth, boxHeight, QPen(Qt::white), Qt::white);
        else
            scene->addRect((boxWidth * i), y, boxWidth, boxHeight, QPen(boxColor), boxColor);
    }
    y += boxHeight;

    QList<int> nextCells = cells;
    nextCells[0] = calculateState(nextCells[nextCells.length() - 1], nextCells[0], nextCells[1]);
    nextCells[cells.length() - 1] = calculateState(nextCells[nextCells.length() - 2], nextCells[nextCells.length() - 1], nextCells[0]);

    for(int i = 1; i < cells.length() - 1; i++)
    {
        int left = cells[i - 1];
        int right = cells[i + 1];
        int state = cells[i];
        nextCells[i] = calculateState(left, state, right);
    }
    this->cells = nextCells;
}

void MainWindow::reloadCanvas()
{
    if(timer->isActive())
        timer->stop();
    y = 0;
    boxWidth = boxSizeSpinBox->value();
    boxHeight = boxSizeSpinBox->value();
    cells.clear();
    ruleset = QString::number(ruleNumberSpinBox->value(), 2).rightJustified(8, '0');
    int total = (screen()->availableGeometry().width() - 38) / boxWidth;
    for(int i = 0; i < total; i++)
    {
        if(i == total/2)
            cells.append(1);
        cells.append(0);
    }

    scene->clear();
    scene->update();
    view->update();
    timer->start();
}

void MainWindow::changeBoxColor()
{
    boxColor = QColorDialog::getColor(boxColor, this, "Choose a color");
    colorButton->setStyleSheet(QString("QPushButton{background-color: %1; width: 30; height: 20; border-radius: 5}").arg(boxColor.name()));
}

void MainWindow::about()
{
    QMessageBox::about(this, "About", "Author: Malone Napier-Jameson.\nEmail: MK.Napier-Jameson@outlook.com\n\nThis program was created using QtCreator and the Qt Framework, C++.");
}

void MainWindow::aboutQT()
{
    QMessageBox::aboutQt(this, "About QT");
}

int MainWindow::calculateState(int left, int state, int right)
{
    QString neighborhood = QString("%1%2%3").arg(left).arg(state).arg(right);
    return QString(ruleset[7 - binaryToInt(neighborhood)]).toInt();
}

int MainWindow::binaryToInt(QString binary)
{
    int total = 0;
    int pos = 1;
    for(int i = 0; i < binary.length(); i++)
    {
        if(binary[i] == '1')
            total += pow(2, binary.length() - pos);
        pos++;
    }
    return total;
}

