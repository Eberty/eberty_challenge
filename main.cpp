/*
 * Titulo: MiniPaint
 *
 * Autor: Eberty Alves da Silva
 *
 * Entrega: Sexta-feira (03/08/2018) às 15h
 * Codigo em C++: QT Desktop
 *
 *
 * Data de Criação: 31 de julho de 2018
 * Última modificação: 02 de agosto de 2018
 *
 * Descrição:
 *      - Abre imagens ou cria novas
 *      - Oferece opções para fazer quadrados, circulos, triangulos, e desenhar livremente
 *          O programa oferece disponbilidade para canetas com diversas cores
 *      - Interface: Abrir imagens, Salvar imagens e Sair do progama.
 *      - Painel: Visualizar a imagem e editar
 *
 * Entrada:  Pode haver imagnes de entrada (Caso informado pelo usuário)
 * Saída:    Imagem com a devida edição
 *
 * Repositorio GIT: https://github.com/romulogcerqueira/eberty_challenge
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
