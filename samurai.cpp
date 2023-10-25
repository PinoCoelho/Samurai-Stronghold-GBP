// samurai.cpp

#include "samurai.h"
#include <QPushButton>
#include <QApplication>

SamuraiApp::SamuraiApp(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Samurai Application");
    resize(400, 300);

    QPushButton *button = new QPushButton("Haz clic aquí", this);
                          button->setGeometry(10, 10, 150, 30);

    connect(button, &QPushButton::clicked, this, [this]() {
        // Lógica a ejecutar cuando se hace clic en el botón
        // Puedes colocar aquí el código que desees ejecutar
    });
}

int pinga(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SamuraiApp samuraiWindow;
    samuraiWindow.show();

    return app.exec();
}

