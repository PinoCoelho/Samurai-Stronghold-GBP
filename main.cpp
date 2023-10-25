#include <QApplication>
#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Server server;

    Client client;


    client.connectToServer("localhost", 12345);

    // Establecer valores de la cuadrícula en el cliente (puedes modificar estos valores según tu lógica)
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            int value = server.getGridValue(row, col);
            client.setGridValue(row, col, value);
        }
    }


    return app.exec();
}

