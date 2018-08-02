#include <QtWidgets>
#include "minipaint.h"

MiniPaint::MiniPaint(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    alterado = false;
    desenhando = false;
    larguraDaCaneta  = 2;
    corDaCaneta = Qt::black;
    tipoDeDesenho = quadrado;
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
    } else {
        return false;
    }
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
    //QMessageBox::about(this, tr("Desfazer"), tr("%1").arg(imagens.size()));
    QPainter painter(&imagem);
    painter.drawImage(QPoint(0, 0), *imagens.at(--posImagem));
    update();
}

void MiniPaint::setImagemNova(){
    QMessageBox::about(this, tr("Refazer"), tr("%1").arg(imagens.size()));
    //QPainter painter(&imagem);
    //painter.drawImage(QPoint(0, 0), image_copy);
    //update();
}

void MiniPaint::limparImagem() {
    imagem.fill(qRgb(255, 255, 255));
    alterado = true;
    update();
}


void MiniPaint::mousePressEvent(QMouseEvent *event) {
    //Salvar imagem na lista aqui
    image_copy = imagem.copy();
    //se posicao atual != size-1
    //remover occorrencias
    imagens.append(&image_copy);
    posImagem++;

    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        desenhando = true;
    }
}


void MiniPaint::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && desenhando){
        switch (MiniPaint::getTipoDesenho()) {
        case livre:
            desenhoLivre(event->pos(), true);
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
            desenhoLivre(event->pos(), false);
            break;
        case quadrado:
            desenharQuadrado(event->pos(), false);
            break;
        case circulo:
            desenharCirculo(event->pos(), false);
            break;
        case triangulo:
            desenharTriangulo(event->pos(), false);
            break;
        case reta:
            desenhoLivre(event->pos(), false);
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
        int newWidth = qMax(width() + 128, imagem.width());
        int newHeight = qMax(height() + 128, imagem.height());
        redimensionarTela(&imagem, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void MiniPaint::desenharQuadrado(const QPoint &endPoint, const bool movendo) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    alterado = true;
    painter.drawRect(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    update();
    //QImage image_copy = imagem.copy();
    //painter.drawImage(QPoint(0, 0), image_copy);
}


void MiniPaint::desenharCirculo(const QPoint &endPoint, const bool movendo) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    alterado = true;
    painter.drawEllipse(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    update();
}


void MiniPaint::desenharTriangulo(const QPoint &endPoint, const bool movendo) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    alterado = true;
    const QPointF points[3] = {
        QPointF(lastPoint.x(), lastPoint.y()),
        QPointF((lastPoint.x() + endPoint.x())/2, endPoint.y()),
        QPointF(endPoint.x(), lastPoint.y())
    };
    painter.drawPolygon(points, 3);
    update();
}


void MiniPaint::desenhoLivre(const QPoint &endPoint, const bool movendo) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    alterado = true;
    int rad = (larguraDaCaneta / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}


void MiniPaint::redimensionarTela(QImage *imagem, const QSize &newSize) {
    if (imagem->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *imagem);
    *imagem = newImage;
}

