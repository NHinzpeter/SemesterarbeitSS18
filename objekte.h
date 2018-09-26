#ifndef OBJEKTE_H
#define OBJEKTE_H

#include <QtGui>
#include <QPixmap>
#include <QString>

//Elternklasse für alle Objekte inkl. dem Spieler
class ClObjekt {
public:
    ClObjekt();
    virtual ~ClObjekt(){delete avatar;}
    QPixmap *avatar;
    int speed, x, y;
    bool damage;
    QString Klasse;
    ClObjekt *naechster;
    ClObjekt *vorheriger;
    void virtual bewegung();
    void loeschdich();
    void loeschdichunddeineFreunde();
private:
};

//Klassendefinitionen aller Kinder
class ClSpieler: public ClObjekt {
public:
    ClSpieler();
private:
};

class ClRechteck: public ClObjekt {
public:
    ClRechteck();
private:
};

class ClDreieck: public ClObjekt {
public:
    ClDreieck();
private:
};

class ClLeben: public ClObjekt {
public:
    ClLeben();
private:
};

class ClFlummi: public ClObjekt {
public:
    ClFlummi();
    void bewegung();
private:
};

#endif // OBJEKTE_H
