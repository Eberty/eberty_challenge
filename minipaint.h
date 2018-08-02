#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "mainwindow.h"

#define LIMITE_IMAGENS 30

class MiniPaint : public QWidget
{
    Q_OBJECT

public:
    MiniPaint(QWidget *parent = 0);

    bool abrirImagem(const QString &fileName);
    bool salvarImagem(const QString &fileName, const char *fileFormat);
    void setCorCaneta(const QColor &newColor);
    void setLarguraCaneta(int newWidth);
    void setTtipoDeDesenho(int newFormat);

    bool getModificado() const {
        return modificado;
    }

    QColor getCorCaneta() const {
        return corDaCaneta;
    }

    int getLarguraCaneta() const {
        return larguraDaCaneta;
    }

    int getTipoDesenho() const {
        return tipoDeDesenho;
    }

    //obter lista
    //obter qtdImagens

public slots:
    void limparImagem();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void desenharQuadrado(const QPoint &endPoint);
    void desenharCirculo(const QPoint &endPoint);
    void desenharTriangulo(const QPoint &endPoint);
    void desenhoLivre(const QPoint &endPoint);
    void redimensionarTela(QImage *image, const QSize &newSize);

    //QList<QImage *> imagens;
    //int qtdImagens;

    bool modificado;
    bool desenhando;
    int tipoDeDesenho;

    int larguraDaCaneta;
    QColor corDaCaneta;

    QImage imagem;
    QPoint lastPoint;
};

#endif
