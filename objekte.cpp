#include <QtGui>
#include <QPixmap>
using namespace std;
#include "objekte.h"

ClObjekt::ClObjekt()
{
    naechster=NULL;
    vorheriger=NULL;
}

ClSpieler::ClSpieler()
    :ClObjekt()
{
    avatar=new QPixmap("../HinzpeterSoSe18/avatar2.png");
    damage=false;
    speed=30;
    x=615;
    y=560;
    Klasse="Spieler";
}

ClRechteck::ClRechteck()
    :ClObjekt()
{
    avatar=new QPixmap("../HinzpeterSoSe18/rechteck.png");
    damage=true;
    speed=1;
    x=rand()%1230;
    y=-100;
    Klasse="Rechteck";
}

ClDreieck::ClDreieck()
    :ClObjekt()
{
    avatar=new QPixmap("../HinzpeterSoSe18/dreieck.png");
    damage=true;
    speed=2;
    x=rand()%1230;
    y=-100;
    Klasse="Dreieck";
}

ClFlummi::ClFlummi()
    :ClObjekt()
{
    avatar=new QPixmap("../HinzpeterSoSe18/flummi.png");
    damage=true;
    speed=2;
    x=rand()%1230;
    y=-100;
    Klasse="Flummi";
}

ClLeben::ClLeben()
    :ClObjekt()
{
    avatar=new QPixmap("../HinzpeterSoSe18/leben2.png");
    damage=false;
    speed=1;
    x=rand()%1230;
    y=-100;
    Klasse="Leben";
}

//Standard-Bewegung aller Objekte, als virtual abgelegt, um durch ClFlummi überschrieben werden zu können
void ClObjekt::bewegung(){
    y+=speed;
}

//Überschreibt die Standard-Bewegung mit einer ZickZack-Bewegung.
//Ob sich der Flummi nach links oder rechts bewegt, hängt vom y-Wert ab
void ClFlummi::bewegung(){
    y+=speed;
    if (y%300>149) x++;
    else x--;
}

//rekursiv, löscht das Objekt und alle Objekte die in der verketteten Liste nach diesem kommen
void ClObjekt::loeschdichunddeineFreunde(){
    if (naechster!=NULL) naechster->loeschdichunddeineFreunde();
    delete this;
}

//löscht nur ein einziges Objekt, nachdem die Pointer des nächsten und vorherigen Objektes entsprechend angepasst wurden.
void ClObjekt::loeschdich(){
    if (vorheriger!=NULL){
        if (naechster!=NULL) vorheriger->naechster=naechster;
        else vorheriger->naechster=NULL;
    }
    if (naechster!=NULL){
        if (vorheriger!=NULL) naechster->vorheriger=vorheriger;
        else naechster->vorheriger=NULL;
    }
    delete this;
}
