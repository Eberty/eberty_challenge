#include <QtWidgets>
#include "minipaint.h"

MiniPaint::MiniPaint(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents);
    modificado = false;
    desenhando = false;
    larguraDaCaneta  = 2;
    corDaCaneta = Qt::black;
    tipoDeDesenho = quadrado;
    //inserir primeira imagem na lista
}


bool MiniPaint::abrirImagem(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    QSize newSize = loadedImage.size().expandedTo(size());
    redimensionar(&loadedImage, newSize);
    imagem = loadedImage;
    modificado = false;
    update();
    return true;
}


bool MiniPaint::salvarImagem(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = imagem;
    redimensionar(&visibleImage, size());
    if (visibleImage.save(fileName, fileFormat)) {
        modificado = false;
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


void MiniPaint::setTtipoDeDesenho(int newFormat) {
    tipoDeDesenho = newFormat;
}


void MiniPaint::limparImagem() {
    imagem.fill(qRgb(255, 255, 255));
    modificado = true;
    update();
}


void MiniPaint::mousePressEvent(QMouseEvent *event) {
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
            //desenhoLivre(event->pos());
            break;
        case circulo:
            //desenhoLivre(event->pos());
            break;
        case triangulo:
            //desenhoLivre(event->pos());
            break;
        case reta:
            //desenhoLivre(event->pos());
            break;
        default:
            //desenhoLivre(event->pos());
            break;
        }
    }
}


void MiniPaint::mouseReleaseEvent(QMouseEvent *event) {
    //Salvar imagem na lista aqui
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
        redimensionar(&imagem, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


void MiniPaint::desenharQuadrado(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    modificado = true;
    painter.drawRect(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    update();
    lastPoint = endPoint;
}


void MiniPaint::desenharCirculo(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    modificado = true;
    painter.drawEllipse(lastPoint.x(), lastPoint.y(), endPoint.x()-lastPoint.x(), endPoint.y()-lastPoint.y());
    update();
    lastPoint = endPoint;
}


void MiniPaint::desenharTriangulo(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    modificado = true;
    const QPointF points[3] = {
        QPointF(lastPoint.x(), lastPoint.y()),
        QPointF((lastPoint.x() + endPoint.x())/2, endPoint.y()),
        QPointF(endPoint.x(), lastPoint.y())
    };
    painter.drawPolygon(points, 3);
    update();
    lastPoint = endPoint;
}


void MiniPaint::desenhoLivre(const QPoint &endPoint) {
    QPainter painter(&imagem);
    painter.setPen(QPen(corDaCaneta, larguraDaCaneta, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modificado = true;
    int rad = (larguraDaCaneta / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized() .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void MiniPaint::redimensionar(QImage *imagem, const QSize &newSize) {
    if (imagem->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *imagem);
    *imagem = newImage;
}

