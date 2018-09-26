#include <QtGui>
using namespace std;
#include <QWidget>
#include <QTimer>
#include <QFile>
#include <QPixmap>
#include "objekte.h"

class spielfeld : public QWidget
{
    Q_OBJECT
public:
    spielfeld(QWidget *parent = 0);
    ~spielfeld();
    void start(void);
    void serialize(QFile &file);
    void deserialize(QFile &file);
private:
    QTimer *timer;
    int increment,i,spawn, punkte, lebenspunkte, delay, random;
    ClSpieler *Spieler;
    ClObjekt *Wurzel;
    ClObjekt *aktuell;
    ClObjekt *loeschen;
    QPixmap *leben;
    QPixmap *tod;
    void restart();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
};
