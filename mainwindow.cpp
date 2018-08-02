/****
*
* Título: mainwindow.cpp
*
* Autor: Eberty Alves
*
* Data de Criação: 31 de julho de 2018
* Última modificação: 03 de agosto de 2018
*
* Descrição: Implementações para mainwindow.h
*
****/

#include <QtWidgets>
#include "mainwindow.h"
#include "minipaint.h"


MainWindow::MainWindow() {
    miniPaint = new MiniPaint;
    setCentralWidget(miniPaint);
    criarAcoes();
    criarMenu();
    resize(600, 450);
    fileName = "";
    setWindowTitle(tr("Eberty Challenge - MiniPaint"));
}


MainWindow::~MainWindow(){

}


void MainWindow::closeEvent(QCloseEvent *event) {
    if (salvarAlteracoes())
        event->accept();
    else
        event->ignore();
}


void MainWindow::abrir() {
    if (salvarAlteracoes()){
        fileName = QFileDialog::getOpenFileName(this, tr("Abrindo arquivo..."), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg *.bmp *ppm)"));
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
    int newWidth = QInputDialog::getInt(this, tr("MiniPaint"), tr("Selecione a largura da caneta:"), miniPaint->getLarguraCaneta(), 1, 30, 1, &ok);
    if (ok)
        miniPaint->setLarguraCaneta(newWidth);
}


void MainWindow::tipoDesenho() {
    bool ok;
    QStringList items;
    items << tr("Desenho Livre") << tr("Quadrado") << tr("Circulo") << tr("Triangulo") << tr("Reta") << tr("Texto") << tr("Desenhar nada");
    QString str = QInputDialog::getItem(this, tr("Tipo de desenho"), tr("Escolha como deseja desenhar:"), items, miniPaint->getTipoDesenho(), false, &ok);
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
        else if (str == "Texto")
            newFormat = texto;
        else if (str == "Desenhar nada")
            newFormat = nada;
        else
            newFormat = livre;
        miniPaint->setTipoDeDesenho(newFormat);
    }
}


void MainWindow::desfazer(){
    miniPaint->setImagemAntiga();
}


void MainWindow::refazer(){
    miniPaint->setImagemNova();
}


void MainWindow::sobre() {
    QMessageBox::about(this, tr("MiniPaint"), tr("<center>Desenvolvido por <b>Eberty Alves</b><br><br>Acesse o <a href=\"https://github.com/romulogcerqueira/eberty_challenge\">Github</a> da ferramenta e saiba mais!</center>"));
}


void MainWindow::criarAcoes() {
    acaoAbrir = new QAction(tr("&Abrir"), this);
    acaoAbrir->setShortcuts(QKeySequence::Open);
    connect(acaoAbrir, SIGNAL(triggered()), this, SLOT(abrir()));

    acaoSalvar = new QAction(tr("Salvar"), this);
    acaoSalvar->setShortcut(QKeySequence::Save);
    connect(acaoSalvar, SIGNAL(triggered()), this, SLOT(salvar()));

    //Salvar como
    foreach (QByteArray formato, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1").arg(QString(formato).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(formato);
        connect(action, SIGNAL(triggered()), this, SLOT(salvar()));
        acaoSalvarComo.append(action);
    }

    acaoSair = new QAction(tr("&Sair"), this);
    acaoSair->setShortcuts(QKeySequence::Quit);
    connect(acaoSair, SIGNAL(triggered()), this, SLOT(close()));

    acaoCorCaneta = new QAction(tr("Cor da caneta"), this);
    acaoCorCaneta->setShortcut(tr("Ctrl+1"));
    connect(acaoCorCaneta, SIGNAL(triggered()), this, SLOT(corCaneta()));

    acaoLarguraCaneta = new QAction(tr("Largura da caneta"), this);
    acaoLarguraCaneta->setShortcut(tr("Ctrl+2"));
    connect(acaoLarguraCaneta, SIGNAL(triggered()), this, SLOT(larguraCaneta()));

    acaoTipoDesenho = new QAction(tr("Definir tipo de desenho"), this);
    acaoTipoDesenho->setShortcut(tr("Ctrl+3"));
    connect(acaoTipoDesenho, SIGNAL(triggered()), this, SLOT(tipoDesenho()));

    acaoDesfazer = new QAction(tr("Desfazer"), this);
    acaoDesfazer->setShortcut(tr("Ctrl+Z"));
    connect(acaoDesfazer, SIGNAL(triggered()), this, SLOT(desfazer()));

    acaoRefazer = new QAction(tr("Refazer"), this);
    acaoRefazer->setShortcut(tr("Ctrl+Y"));
    connect(acaoRefazer, SIGNAL(triggered()), this, SLOT(refazer()));

    acaoLimparTela = new QAction(tr("Limpar tela"), this);
    acaoLimparTela->setShortcut(tr("Ctrl+L"));
    connect(acaoLimparTela, SIGNAL(triggered()), miniPaint, SLOT(limparImagem()));

    acaoSobre = new QAction(tr("Sobre"), this);
    acaoSobre->setShortcut(tr("Ctrl+h"));
    connect(acaoSobre, SIGNAL(triggered()), this, SLOT(sobre()));
}


void MainWindow::criarMenu() {
    menuArquivo = new QMenu(tr("&Arquivo"), this);

    menuSalvarComo = new QMenu(tr("&Salvar como"), this);
    foreach (QAction *action, acaoSalvarComo)
        menuSalvarComo->addAction(action);

    menuArquivo->addAction(acaoAbrir);
    menuArquivo->addAction(acaoSalvar);
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
    menuBar()->addMenu(menuSobre);
}


bool MainWindow::salvarAlteracoes() {
    if (miniPaint->getAlterado()) {
       QMessageBox::StandardButton ret = QMessageBox::question(this, tr("MiniPaint"), tr("Deseja salvar as modificações?"),
                                                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
       if (ret == QMessageBox::Save)
           return salvarArquivo("png");
        else if (ret == QMessageBox::Cancel)
           return false;
    }
    return true;
}


bool MainWindow::salvarArquivo(const QByteArray &fileFormat) {
    //QMessageBox::about(this, tr("teste"), tr("%1").arg(fileName));
    QString initialPath = QDir::homePath() + "/meuArquivo." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Salvar como..."), initialPath, tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    else
        return miniPaint->salvarImagem(fileName, fileFormat.constData());
}
