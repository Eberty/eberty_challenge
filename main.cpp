/*
 * Entrega: Sexta feira (03/08) até as 15h
 * Codigo em C++: QT Desktop
 * Abrir uma imagem qualquer: um Minipaint
 *      Abrir uma imagem, opções para fazer quadrado, circulo, triangulo, e linha livre (qualquer cor)
 *      Interface: open, save, quit
 *      Painel: Visualizar a imagem e editar
 * Repositorio GIT: https://github.com/romulogcerqueira/eberty_challenge
 *
 * http://doc.qt.io/archives/qt-4.8/qpainter.html
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
