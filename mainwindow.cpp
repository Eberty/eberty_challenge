#include <iostream>
#include <QtWidgets>
#include "mainwindow.h"
#include "minipaint.h"


MainWindow::MainWindow() {
    miniPaint = new MiniPaint;
    setCentralWidget(miniPaint);
    criarAcoes();
    criarMenu();
    resize(600, 450);
    setWindowTitle(tr("Eberty Challenge - MeuPaint"));
}


void MainWindow::closeEvent(QCloseEvent *event) {
    if (salvarMudancas()) {
        event->accept();
    } else {
        event->ignore();
    }
}


void MainWindow::abrir() {
    if (salvarMudancas()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Abrindo arquivo..."), QDir::currentPath());
        if (!fileName.isEmpty())
            miniPaint->abrirImagem(fileName);
    }
}


void MainWindow::salvar() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    salvarArquivo(fileFormat);
}


void MainWindow::corCaneta() {
    QColor newColor = QColorDialog::getColor(miniPaint->getCorCaneta());
    if (newColor.isValid())
        miniPaint->setCorCaneta(newColor);
}


void MainWindow::larguraCaneta() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Meu Paint"), tr("Selecione a largura da caneta:"), miniPaint->getLarguraCaneta(), 1, 50, 1, &ok);
    if (ok)
        miniPaint->setLarguraCaneta(newWidth);
}


void MainWindow::sobre() {
    QMessageBox::about(this, tr("Meu Paint"), tr("<center>Desenvolvido por <b>Eberty Alves</b><br><br>Acesse o <a href=\"https://github.com/romulogcerqueira/eberty_challenge\">Github</a> da ferramenta e saiba mais!</center>"));
}


void MainWindow::tipoDesenho() {
    bool ok;
    QStringList items;
    items << tr("Desenho Livre") << tr("Quadrado") << tr("Circulo") << tr("Triangulo") << tr("Reta") << tr("Desenhar nada");
    QString str = QInputDialog::getItem(this, tr("Tipo de desenho"), tr("Escolha como deseja desenhar:"), items, 0, false, &ok);
    if(ok && !str.isEmpty()){
        int newFormat;
        if (str == "Quadrado")
            newFormat = quadrado;
        else if (str == "Circulo")
            newFormat = circulo;
        else if (str == "Triangulo")
            newFormat = triangulo;
        else if (str == "Reta")
            newFormat = reta;
        else if (str == "Desenhar nada")
            newFormat = nada;
        else
            newFormat = livre;
        miniPaint->setTtipoDeDesenho(newFormat);
    }
}


void MainWindow::criarAcoes() {
    //Abrir aqruivos
    acaoAbrir = new QAction(tr("&Abrir"), this);
    acaoAbrir->setShortcuts(QKeySequence::Open);
    connect(acaoAbrir, SIGNAL(triggered()), this, SLOT(abrir()));

    //Salvar como: obter lista com todos os formatos de imagens suportados
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(salvar()));
        acaoSalvarComo.append(action);
    }

    //Sair do programa
    acaoSair = new QAction(tr("&Sair"), this);
    acaoSair->setShortcuts(QKeySequence::Quit);
    connect(acaoSair, SIGNAL(triggered()), this, SLOT(close()));

    //Definir cor da caneta
    acaoCorCaneta = new QAction(tr("Cor da caneta"), this);
    connect(acaoCorCaneta, SIGNAL(triggered()), this, SLOT(corCaneta()));

    //Definir largura da caneta
    acaoLarguraCaneta = new QAction(tr("Largura da caneta"), this);
    connect(acaoLarguraCaneta, SIGNAL(triggered()), this, SLOT(larguraCaneta()));

    //Definir tipo de desenho
    acaoTipoDesenho = new QAction(tr("Definir tipo de desenho"), this);
    connect(acaoTipoDesenho, SIGNAL(triggered()), this, SLOT(tipoDesenho()));

    //Desfazer...
    acaoDesfazer = new QAction(tr("Desfazer"), this);
    acaoDesfazer->setShortcut(tr("Ctrl+Z"));
    //connect(acaoDesfazer, SIGNAL(triggered()), miniPaint, SLOT(()));

    //Refazer...
    acaoRefazer = new QAction(tr("Refazer"), this);
    acaoRefazer->setShortcut(tr("Ctrl+Y"));
    //connect(acaoRefazer, SIGNAL(triggered()), miniPaint, SLOT(()));

    //Limpar tela...
    acaoLimparTela = new QAction(tr("Limpar tela"), this);
    acaoLimparTela->setShortcut(tr("Ctrl+L"));
    connect(acaoLimparTela, SIGNAL(triggered()), miniPaint, SLOT(limparImagem()));

    //Sobre...
    acaoSobre = new QAction(tr("Sobre"), this);
    connect(acaoSobre, SIGNAL(triggered()), this, SLOT(sobre()));
}


void MainWindow::criarMenu() {
    menuArquivo = new QMenu(tr("&Arquivo"), this);
    menuArquivo->addAction(acaoAbrir);

    menuSalvarComo = new QMenu(tr("&Salvar como"), this);
    foreach (QAction *action, acaoSalvarComo)
        menuSalvarComo->addAction(action);

    menuArquivo->addMenu(menuSalvarComo);
    menuArquivo->addSeparator();
    menuArquivo->addAction(acaoSair);

    menuOpcoes = new QMenu(tr("&Opções"), this);
    menuOpcoes->addAction(acaoCorCaneta);
    menuOpcoes->addAction(acaoLarguraCaneta);
    menuOpcoes->addSeparator();
    menuOpcoes->addAction(acaoTipoDesenho);
    menuOpcoes->addSeparator();
    menuOpcoes->addAction(acaoDesfazer);
    menuOpcoes->addAction(acaoRefazer);
    menuOpcoes->addAction(acaoLimparTela);

    menuSobre = new QMenu(tr("&Sobre"), this);
    menuSobre->addAction(acaoSobre);

    menuBar()->addMenu(menuArquivo);
    menuBar()->addMenu(menuOpcoes);

    //menuBar()->addAction(acaoSobre);
    menuBar()->addMenu(menuSobre);
}


bool MainWindow::salvarMudancas() {
    if (miniPaint->getModificado()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::question(this, tr("Meu Paint"), tr("Deseja salvar as modificações?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

       if (ret == QMessageBox::Save)
           return salvarArquivo("jpeg");
        else if (ret == QMessageBox::Cancel)
           return false;
    }
    return true;
}


bool MainWindow::salvarArquivo(const QByteArray &fileFormat) {
    QString initialPath = QDir::homePath() + "/meuArquivo." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Salvar como..."), initialPath, tr("%1 Files (*.%2);;All Files (*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty())
        return false;
    else
        return miniPaint->salvarImagem(fileName, fileFormat.constData());
}
