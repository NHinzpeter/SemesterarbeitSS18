#include <QApplication>
#include "widget.h"

//VORWORT
//Erfüllt wurde die Aufgabenstellung bis einschließlich Stufe IV.
//Alle Objekte wurden als QPixmaps angelegt, sollten diese nicht angezeigt wurden, kann das am Dateipfad legen (Dieser referenziert aktuell relativ vom Buildverzeichnis aus)
//Das Programm wurde ausführlich kommentiert, um das Nachvollziehen zu vereinfachen
//Link zum Github-Repository: https://github.com/NHinzpeter/SemesterarbeitSS18/
//Die Readme-Datei enthält eine Dokumentation der Programmierschritte

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Widget widget;
    widget.setGeometry(100, 100, 1280, 720);
    widget.show();
    return app.exec();
}
