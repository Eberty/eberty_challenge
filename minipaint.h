/****
*
* Título: minipaint.h
*
* Autor: Eberty Alves
*
* Data de Criação: 31 de julho de 2018
* Última modificação: 03 de agosto de 2018
*
* Descrição: Classe relativa as ações realizadas pelo usuario
*
****/

#ifndef MINIPAINT_H
#define MINIPAINT_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "mainwindow.h"

//#define LIMITE_IMAGENS 30

class MiniPaint : public QWidget
{
    Q_OBJECT

public:
    MiniPaint(QWidget *parent = 0);

    bool abrirImagem(const QString &nomeArquivo);
    bool salvarImagem(const QString &nomeArquivo, const char *fileFormat);
    void addImagem();

    void setCorCaneta(const QColor &newColor);
    void setLarguraCaneta(int newWidth);
    void setTipoDeDesenho(int newFormat);
    void setImagemAntiga();
    void setImagemNova();

    bool getAlterado() const;
    QColor getCorCaneta() const;
    int getLarguraCaneta() const;
    int getTipoDesenho() const;
    QImage getImagem() const;


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
    void desenharTexto(const QPoint &endPoint);
    void desenharTriangulo(const QPoint &endPoint);
    void desenhoLivre(const QPoint &endPoint);
    void redimensionarTela(QImage *image, const QSize &newSize);

    bool alterado;
    bool desenhando;
    int tipoDeDesenho;

    int larguraDaCaneta;
    QColor corDaCaneta;

    QPoint lastPoint;
    QList<QImage *> imagens;
    QImage imagem;
    QImage outraImagem;
    int indiceImagem;
    int indiceUltimoClean;
};

#endif
