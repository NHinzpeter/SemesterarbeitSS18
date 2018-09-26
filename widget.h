#include <QWidget>
#include "spielfeld.h"
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);

private:
    spielfeld *neuesSpielfeld;
    QPushButton *startButton;

private slots:
    void start(void);
    void safeFile(void);
    void loadFile(void);
};
