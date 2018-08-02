#include <QtWidgets>
#include "minipaint.h"

MiniPaint::MiniPaint(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    alterado = false;
    desenhando = false;
    larguraDaCaneta  = 3;
    corDaCaneta = Qt::black;
    tipoDeDesenho = quadrado;
    b_imagemAntiga = false;
}


bool MiniPaint::abrirImagem(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    QSize newSize = loadedImage.size().expandedTo(size());
    redimensionarTela(&loadedImage, newSize);
    imagem = loadedImage;
    alterado = false;
    update();
    return true;
}


bool MiniPaint::salvarImagem(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = imagem;
    redimensionarTela(&visibleImage, size());
    if (visibleImage.save(fileName, fileFormat)) {
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
    if(!b_imagemAntiga){
        imagemNova = imagem.copy();
        QPainter painter(&imagem);
        painter.drawImage(QPoint(0, 0), imagemAntiga);
        update();
        b_imagemAntiga = true;
    } else {
        QMessageBox::about(this, tr("Imagem Anterior!"), tr("Só pode voltar uma vez"));
    }
}


void MiniPaint::setImagemNova(){
    if(b_imagemAntiga){
        QPainter painter(&imagem);
        painter.drawImage(QPoint(0, 0), imagemNova);
        update();
        b_imagemAntiga = false;
    } else {
        QMessageBox::about(this, tr("Imagem Nova!"), tr("Não há mais o que avançar"));
    }
}


void MiniPaint::limparImagem() {
    imagem.fill(Qt::white);
    alterado = true;
    update();
}


void MiniPaint::mousePressEvent(QMouseEvent *event) {
    imagemAntiga = imagem.copy();
    b_imagemAntiga = false;

    if (event->button() == Qt::LeftButton) {
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
        case quadrado:
            //desenharQuadrado(event->pos(), true);
            break;
        case circulo:
            //desenharCirculo(event->pos(), true);
            break;
        case triangulo:
            //desenharTriangulo(event->pos(), true);
            break;
        case reta:
            //desenhoLivre(event->pos(), true);
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
        default:
            break;
        }
        desenhando = false;
        lastPoint = event->pos();
    }
}


void MiniPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, imagem, dirtyRect);
}


void MiniPaint::resizeEvent(QResizeEvent *event) {
    if (width() > imagem.width() || height() > imagem.height()) {
        redimensionarTela(&imagem, QSize(qMax(width() + 128, imagem.width()), qMax(height() + 128, imagem.height())));
        update();
    }
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

