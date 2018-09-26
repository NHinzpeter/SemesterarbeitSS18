#include <QtGui>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *quit = new QPushButton(tr("Ende"));
    quit->setFont(QFont("Times", 18, QFont::Bold));
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *saver = new QPushButton(tr("Sichern"));
    saver->setFont(QFont("Times", 18, QFont::Bold));
    connect(saver, SIGNAL(clicked()), this, SLOT(safeFile()));

    QPushButton *loader = new QPushButton(tr("Laden"));
    loader->setFont(QFont("Times", 18, QFont::Bold));
    connect(loader, SIGNAL(clicked()), this, SLOT(loadFile()));

    startButton = new QPushButton(tr("Start"));
    startButton->setFont(QFont("Times", 18, QFont::Bold));
    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));

    neuesSpielfeld = new spielfeld;

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 3);
    gridLayout->addWidget(saver, 0, 1);
    gridLayout->addWidget(loader, 0, 2);
    gridLayout->addWidget(startButton, 0, 0);
    gridLayout->addWidget(neuesSpielfeld, 1, 0, 1, 4);
    setLayout(gridLayout);

    setPalette(QPalette(QColor(255, 255, 255)));
    setAutoFillBackground(true);
}

//ruft die Start/Stop-Funktion des Spielfeldes auf und ändert entsprechend die Beschriftung des Buttons
void Widget::start(void)
{
    neuesSpielfeld->start();

    if (startButton->text()=="Start")
        startButton->setText("Pause");
    else startButton->setText("Start");
}

//Öffnet den Filedialog und startet dann die Speicherung, pausiert außerdem das Spiel, größtenteils aus Vorlesung übernommen
void Widget::safeFile(void)
{
    if (startButton->text()!="Start") start();
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getSaveFileName(this,
                                      tr("Speichern als"), ".", tr("Spielstand (*.std)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht verwendet werden: ") + fileName,QMessageBox::Ok);
        }

        neuesSpielfeld->serialize(file);
        file.close();
        return;
    }
}

//Öffnet den Filedialog und startet dann das Laden, pausiert außerdem das Spiel, größtenteils aus Vorlesung übernommen
void Widget::loadFile(void)
{
    if (startButton->text()!="Start") start();
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getOpenFileName(this,
                                      tr("Speichern als"), ".", tr("Spielstand (*.std)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht geöffnet werden: ") + fileName,QMessageBox::Ok);
        }

        neuesSpielfeld->deserialize(file);
        file.close();
        return;
    }
}
