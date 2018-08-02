#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

typedef enum {
    livre, quadrado, circulo, triangulo, reta, nada
} t_desenho;

class MiniPaint;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots: // Triggered
    void abrir();
    void salvar();
    void corCaneta();
    void larguraCaneta();
    void tipoDesenho();
    void desfazer();
    void refazer();
    void sobre();

private:
    void criarAcoes();
    void criarMenu();

    bool salvarMudancas();
    bool salvarArquivo(const QByteArray &fileFormat);

    MiniPaint *miniPaint;

    QMenu *menuArquivo;
    QMenu *menuSalvarComo;
    QMenu *menuOpcoes;
    QMenu *menuSobre;

    QAction *acaoAbrir;
    QList<QAction *> acaoSalvarComo;
    QAction *acaoSair;
    QAction *acaoCorCaneta;
    QAction *acaoLarguraCaneta;
    QAction *acaoTipoDesenho;
    QAction *acaoDesfazer;
    QAction *acaoRefazer;
    QAction *acaoLimparTela;
    QAction *acaoSobre;
};

#endif