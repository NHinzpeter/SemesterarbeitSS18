#include <QtGui>
#include <QPixmap>
#include <QString>
#include <QMessageBox>
#include <cmath>
using namespace std;
#include "spielfeld.h"

spielfeld::spielfeld(QWidget *parent)
    : QWidget(parent)
{

    //Spawnen eines ersten Objekts, dies kann nur ein Rechteck oder ein Quadrat sein
    random=rand()%2;
    switch (random) {
    case 0:
        Wurzel=new ClRechteck;
        break;
    case 1:
        Wurzel=new ClDreieck;
        break;
    default:
        break;
    }

    //Sonstige Starteinstellungen
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    Spieler=new ClSpieler;
    aktuell=Wurzel;
    loeschen=NULL;
    leben=new QPixmap("../HinzpeterSoSe18/leben.png");
    tod=new QPixmap("../HinzpeterSoSe18/tod.png");
    punkte=0;
    delay=0;
    increment=0;
    i=0;
    spawn=250;
    lebenspunkte=3;

    setFixedSize(1280,620);
    setPalette(QPalette(QColor(240, 240, 240)));
    setAutoFillBackground(true);
}

spielfeld::~spielfeld(){
    delete timer;
    delete Wurzel;
    delete Spieler;
    delete aktuell;
    delete loeschen;
    delete leben;
    delete tod;
}

void spielfeld::paintEvent(QPaintEvent *event)
{
    //Neustart bei 0 Lebenspunkten
    if (lebenspunkte==0) {
        restart();
    }


    //die meisten Dinge werden nur ausgeführt, wenn aktuell kein delay besteht, also der Spieler kürzlich mit einem Objekt kollidiert ist
    if (delay==0) punkte++;

    QPainter painter;
    painter.begin( this );
    aktuell=Wurzel;

    //for-Schleife geht jedes Objekt durch, bewegt dieses und testet auf Kollision oder Austritt aus dem Bild
    for(;;){
        if (loeschen!=NULL) loeschen->loeschdich();
        loeschen=NULL;
        if (Wurzel==NULL) {
            random=rand()%2;
            switch (random) {
            case 0:
                Wurzel=new ClRechteck;
                break;
            case 1:
                Wurzel=new ClDreieck;
                break;
            default:
                break;
            }
            aktuell=Wurzel;
        }


        //Bewegung und Kollisionserkennung
        if(delay==0){
            aktuell->bewegung();

            //Kollisionserkennung
            if (sqrt((aktuell->x-Spieler->x)*(aktuell->x-Spieler->x)+(aktuell->y-Spieler->y)*(aktuell->y-Spieler->y))<50){
                if (aktuell->damage==true) {
                    lebenspunkte--;
                }
                else if (lebenspunkte<3) {
                    lebenspunkte++;
                }
                delay=250;
                loeschen=aktuell;
            }
        }

        painter.drawPixmap(aktuell->x,aktuell->y,50,50,*aktuell->avatar);

        //Löschung der Blöcke, die aus dem Bild sind
        if (delay==0){
            if (aktuell->y>=700){
                loeschen=aktuell;
            }
        }

        //Test, ob das Wurzelobjekt gelöscht werden soll
        if (loeschen==Wurzel && Wurzel->naechster!=NULL) Wurzel=aktuell->naechster;
        else if (loeschen==Wurzel) Wurzel=NULL;

        //ggf wird zum nächsten Objekt übergegangen
        if (aktuell->naechster==NULL) break;
        aktuell=aktuell->naechster;

        //wenn ein Objekt zum löschen markiert wurde, wird es jetzt gelöscht
        if (loeschen!=NULL){
            loeschen->loeschdich();
            loeschen=NULL;
        }
    }


    //Spawnen neuer Blöcke oder ggf eines Regenerationsobjektes
    if (delay==0){
        i++;
        random=rand()%2500;
        if (random==0){
            aktuell->naechster=new ClLeben;
            aktuell->naechster->vorheriger=aktuell;
        }
        else {
            if(i>=spawn){
                if (Wurzel!=NULL){
                    random=rand()%19;
                    if (random<9) aktuell->naechster=new ClRechteck;
                    else if (random <18) aktuell->naechster=new ClDreieck;
                    else aktuell->naechster=new ClFlummi;
                    aktuell->naechster->vorheriger=aktuell;
                }
                if (spawn>50) spawn-=20;
                else if (spawn>15) spawn-=1;
                i=0;
            }
        }
    }

    //Zeichnen des Punktestands
    QString punktestring=QString::number(punkte);
    painter.setFont(QFont("times",22));
    painter.drawText(20,20,200,100,Qt::AlignLeft,punktestring);

    //Zeichnen des Spieler-Avatars
    painter.setOpacity(0.4+lebenspunkte*0.2);
    painter.drawPixmap(Spieler->x,Spieler->y,50,50,*Spieler->avatar);
    painter.setOpacity(1.0);

    //Zeichnen der Herzen
    for (int j=0;j<3;j++){
        if (j<lebenspunkte) painter.drawPixmap(1230-50*j,10,50,50,*leben);
        else painter.drawPixmap(1230-50*j,10,50,50,*tod);
    }

    //ggf Zeichnen des Endscreens
    if (lebenspunkte==0){
        painter.setFont(QFont("times",50));
        painter.drawText(0,300,1280,100,Qt::AlignCenter,punktestring);
        punktestring="GAME OVER!";
        painter.drawText(0,200,1280,100,Qt::AlignCenter,punktestring);
    }

    painter.end();
    if (delay>0) delay--;
}

//startet bzw. stoppt den Timer und setzt nach dem Start den Fokus in das Spielfeld
void spielfeld::start()
{

    if (increment==0)
    {timer->start(10); increment=1; setFocus();}
    else {timer->stop(); increment=0;}
}

//Bewegung des Avatars durch die Pfeiltasten
void spielfeld::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        if(Spieler->x>=30)
            Spieler->x-=30;
    if (event->key()==Qt::Key_Right)
        if (Spieler->x<=1200)
            Spieler->x+=30;
}

//Export aller benötigten Parameter ind die Spielstands-Datei
void spielfeld::serialize(QFile &file)
{

    ClObjekt *current=Wurzel;
    QTextStream out(&file);

    out << "Spielstand" << endl;
    out << "Spieler " << Spieler->x << endl;
    out << "Stats " << lebenspunkte << " " << punkte << " " << i << " " << spawn << endl;
    for (;;){
        out << "Objekt " << current->Klasse << " " << current->x << " " << current->y << endl;
        if (current->naechster==NULL) break;
        current=current->naechster;
    }
}

//Parsing der Spielstands-Datei und wiederherstellen des Spielstands
void spielfeld::deserialize(QFile &file)
{
    ClObjekt *current;
    QString test;
    QTextStream in(&file);

    //Test, ob das Format stimmt
    in >> test;
    if (test != "Spielstand")
    {
        QMessageBox::warning(this, tr("Formatfehler"),
                             tr("Das ist kein Spielstand!"),QMessageBox::Ok);
        return;
    }

    //löschen aller Objekte
    Wurzel->loeschdichunddeineFreunde();
    Wurzel=NULL;
    Spieler->loeschdich();

    //Parsing und Überschreiben/Erstellen der Objekte und der Stats
    in >> test;
    for (;test!="";in >> test){
        if (test=="Spieler"){
            in >> test;
            Spieler=new ClSpieler;
            Spieler->x=test.toInt();
        }
        else if (test=="Stats"){
            in >> test;
            lebenspunkte=test.toInt();
            in >> test;
            punkte=test.toInt();
            in >> test;
            i=test.toInt();
            in >> test;
            spawn=test.toInt();
        }
        else if (test=="Objekt"){
            in >> test;
            if (Wurzel==NULL){
                if (test=="Rechteck") Wurzel=new ClRechteck;
                else if (test=="Dreieck") Wurzel=new ClDreieck;
                current=Wurzel;
            }
            else {
                if (test=="Rechteck") current->naechster=new ClRechteck;
                else if (test=="Dreieck") current->naechster=new ClDreieck;
                else if (test=="Leben") current->naechster=new ClLeben;
                else if (test=="Flummi") current->naechster=new ClFlummi;
                current->naechster->vorheriger=current;
                current=current->naechster;
            }
            in >> test;
            current->x=test.toInt();
            in >> test;
            current->y=test.toInt();
        }
    }
}

//Sleep, um den Endscreen zu halten, danach zurücksetzen aller Variablen
void spielfeld::restart(){
    Sleep(5000);
    Wurzel->loeschdichunddeineFreunde();
    Spieler->loeschdich();
    Spieler=new ClSpieler;
    random=rand()%2;
    switch (random) {
    case 0:
        Wurzel=new ClRechteck;
        break;
    case 1:
        Wurzel=new ClDreieck;
        break;
    default:
        break;
    }
    aktuell=Wurzel;
    loeschen=NULL;
    punkte=0;
    delay=0;
    i=0;
    spawn=250;
    lebenspunkte=3;
}
