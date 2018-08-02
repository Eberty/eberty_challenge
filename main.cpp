/*
 * Programa: MiniPaint - Eberty Alves
 *
 * Entrega: Sexta-feira (03/08/2018) às 15h
 * Codigo em C++: QT Desktop
 *
 * O que o prgrama deve fazer:
 *      - Abrir uma imagem
 *      - Opções para fazer quadrado, circulo, triangulo, e linha livre (qualquer cor)
 *      - Interface: open, save, quit
 *      - Painel: Visualizar a imagem e editar
 *
 * Uppar no repositorio GIT: https://github.com/romulogcerqueira/eberty_challenge
 *
 * Help: http://doc.qt.io/archives/qt-4.8/qpainter.html
 *       http://doc.qt.io/qt-5/qtwidgets-widgets-scribble-example.html
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    return a.exec();
}
