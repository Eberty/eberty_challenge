/****
*
* Título: minipaint.cpp
*
* Autor: Eberty Alves
*
* Data de Criação: 31 de julho de 2018
* Última modificação: 03 de agosto de 2018
*
* Descrição: Implementações para minipaint.h
*
****/

#include <QtWidgets>
#include "minipaint.h"

MiniPaint::MiniPaint(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    alterado = false;
    desenhando = false;
    larguraDaCaneta  = 3;
    indiceImagem = 0;
    indiceUltimoClean = 0;
    tipoDeDesenho = livre;
    corDaCaneta = Qt::black;
}


bool MiniPaint::abrirImagem(const QString &nomeArquivo) {
    QImage loadedImage;
    if (!loadedImage.load(nomeArquivo))
        return false;
    QSize newSize = loadedImage.size().expandedTo(size());
    redimensionarTela(&loadedImage, newSize);
    imagem = loadedImage;
    alterado = false;
    imagens.clear();
    indiceImagem = 0;
    update();
    return true;
}


bool MiniPaint::salvarImagem(const QString &nomeArquivo, const char *fileFormat) {
    QImage visibleImage = imagem;
    redimensionarTela(&visibleImage, size());
    if (visibleImage.save(nomeArquivo, fileFormat)) {
        alterado = false;
        return true;
    } else
        return false;
}


void MiniPaint::setCorCaneta(const QColor &newColor) {
    corDaCaneta = newColor;
}


void MiniPaint::setLarguraCaneta(int newWidth) {
    larguraDaCaneta = newWidth;
}


void MiniPaint::setTipoDeDesenho(int newFormat) {
    tipoDeDesenho = newFormat;
}


void MiniPaint::setImagemAntiga(){
    if(indiceImagem > 0){
        if(indiceImagem == imagens.size()){
            MiniPaint::addImagem();
            --indiceImagem;
        }
        QPainter painter(&imagem);
        outraImagem = *imagens.at(--indiceImagem);
        painter.drawImage(QPoint(0, 0), outraImagem);
        update();
        if (indiceImagem == 0)
            alterado = false;
    } else {
        QMessageBox::about(this, tr("Ultima Imagem!"), tr("Não é mais possível recuar"));
    }
}


void MiniPaint::setImagemNova(){
    if(indiceImagem < imagens.size()-1){
        QPainter painter(&imagem);
        outraImagem = *imagens.at(++indiceImagem);
        painter.drawImage(QPoint(0, 0), outraImagem);
        update();
        if (indiceImagem != 0)
            alterado = true;
    } else {
        QMessageBox::about(this, tr("Imagem mais recente!"), tr("Não há mais o que avançar"));
    }
}


bool MiniPaint::getAlterado() const {
    return alterado;
}


QColor MiniPaint::getCorCaneta() const {
    return corDaCaneta;
}


int MiniPaint::getLarguraCaneta() const {
    return larguraDaCaneta;
}


int MiniPaint::getTipoDesenho() const {
    return tipoDeDesenho;
}


QImage MiniPaint::getImagem() const{
    return imagem;
}

void MiniPaint::limparImagem() {
    while (indiceImagem < imagens.size())
        imagens.pop_back();
    if (indiceUltimoClean != indiceImagem)
        MiniPaint::addImagem();

    imagem.fill(Qt::white);
    alterado = true;
    indiceUltimoClean = indiceImagem;
    update();
}


void MiniPaint::addImagem(){
    QImage *aux = new QImage;
    *aux = imagem.copy();
    imagens.push_back(aux);
    indiceImagem++;
    indiceUltimoClean = 0;
}


void MiniPaint::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(MiniPaint::getTipoDesenho() != nada){
            while (indiceImagem < imagens.size())
                imagens.pop_back();
            MiniPaint::addImagem();
        }
        lastPoint = event->pos();
        desenhando = true;
    }
}


void MiniPaint::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && desenhando){
        switch (MiniPaint::getTipoDesenho()) {
        case livre:
            desenhoLivre(event->pos());
            break;
        default:
            break;
        }
    }
}


void MiniPaint::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && desenhando) {
        switch (MiniPaint::getTipoDesenho()) {
        case livre:
            desenhoLivre(event->pos());
            break;
        case quadrado:
            desenharQuadrado(event->pos());
            break;
        case circulo:
            desenharCirculo(event->pos());
            break;
        case triangulo:
            desenharTriangulo(event->pos());
            break;
        case reta:
            desenhoLivre(event->pos());
            break;
        case texto:
            desenharTexto(event->pos());
            break;
        default:
            break;
        }
        desenhando = false;
        lastPoint = event->pos();
    }
}


void MiniPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), imagem);
    QWidget::paintEvent(event);
}


void MiniPaint::resizeEvent(QResizeEvent *event) {
    redimensionarTela(&imagem, QSize(qMax(width(), imagem.width()), qMax(height(), imagem.height())));
    QWidget::resizeEvent(event);
}


void MiniPaint::desenharQuadrado(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    alterado = true;
    update();
}


void MiniPaint::desenharCirculo(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    alterado = true;
    update();
}


void MiniPaint::desenharTexto(const QPoint &endPoint) {
    QPainter painter(&imagem);
    bool ok;
    QString str = QInputDialog::getText(this, tr("Seu texto"), tr("Digite aqui seu texto:"), QLineEdit::Normal, "", &ok);
    if(ok && !str.isEmpty()){
        painter.drawText(endPoint, str);
        alterado = true;
    } else {
        imagens.pop_back();
        indiceImagem--;
    }
    update();
}


void MiniPaint::desenharTriangulo(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    const QPointF points[3] = {
        QPointF(lastPoint.x(), lastPoint.y()),
        QPointF((lastPoint.x() + endPoint.x())/2, endPoint.y()),
        QPointF(endPoint.x(), lastPoint.y())
    };
    painter.drawPolygon(points, 3);
    alterado = true;
    update();
}


void MiniPaint::desenhoLivre(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    int rad = (larguraDaCaneta / 2) + 2;
    alterado = true;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


void MiniPaint::redimensionarTela(QImage *imagem, const QSize &newSize) {
    if (imagem->size() != newSize){
        QImage newImage(newSize, QImage::Format_RGB32);
        newImage.fill(Qt::white);
        QPainter painter(&newImage);
        painter.drawImage(QPoint(0, 0), *imagem);
        *imagem = newImage;
    }
}

