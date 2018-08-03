/****
*
* Título: mainwindow.h
*
* Autor: Eberty Alves
*
* Data de Criação: 31 de julho de 2018
* Última modificação: 03 de agosto de 2018
*
* Descrição: Classe relativa a tela onde a imagem é editada
*
****/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>


typedef enum {
    livre, quadrado, circulo, triangulo, reta, texto, nada
} t_desenho;


class MiniPaint;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots: // Trigger
    void abrir();
    void salvar();
    void salvarComo();
    void corCaneta();
    void larguraCaneta();
    void tipoDesenho();
    void desfazer();
    void refazer();
    void sobre();

private:
    void criarAcoes();
    void criarMenu();

    bool salvarAlteracoes();
    bool salvarArquivoComo(const QByteArray &fileFormat);

    MiniPaint *miniPaint;
    QString nomeArquivo;

    QAction *acaoAbrir;
    QAction *acaoSalvar;
    QList<QAction *> acaoSalvarComo;
    QAction *acaoSair;
    QAction *acaoCorCaneta;
    QAction *acaoLarguraCaneta;
    QAction *acaoTipoDesenho;
    QAction *acaoDesfazer;
    QAction *acaoRefazer;
    QAction *acaoLimparTela;
    QAction *acaoSobre;

    QMenu *menuArquivo;
    QMenu *menuSalvarComo;
    QMenu *menuOpcoes;
    QMenu *menuSobre;
};

#endif
