#ifndef SAMURAI_H
#define SAMURAI_H

#include <QWidget>

class SamuraiApp : public QWidget
{
    Q_OBJECT
public:
    SamuraiApp(QWidget *parent = nullptr);
    virtual ~SamuraiApp();

    // Declaración de otras funciones y miembros si es necesario
};

#endif // SAMURAI_H
