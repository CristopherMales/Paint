#include "principal.h"
#include "ui_principal.h"

#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    connect(ui->actionLineas, SIGNAL(triggered(bool)),this,SLOT(on_actionLineas()));
    connect(ui->actionLibre,SIGNAL(triggered(bool)),this,SLOT(on_actionLibre()));
    connect(ui->actionRect_nculos, SIGNAL(triggered(bool)),this,SLOT(on_actionRectangulos()));
    connect(ui->actionCircunferencias,SIGNAL(triggered(bool)),this,SLOT(on_actionCircuferencias()));
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}
void Principal::on_actionLineas()
{
    type_figure = 0;
}
void Principal::on_actionLibre()
{
    type_figure = 1;
}
void Principal::on_actionRectangulos()
{
    type_figure = 2;
}
void Principal::on_actionCircuferencias()
{
    type_figure = 3;
}
void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    mPuedeDibujar = true;
    mInicial = event->pos();
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        event->accept();
        return;
    }
    // Capturar el punto donde se suelta el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    //Tipo de figura
    if (type_figure == 1){
        mPainter->drawLine(mInicial, mFinal);
        // Mostrar el número de líneas en la barra de estado
        ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));


    // Actualizar la interfaz
    update();
    // actualizar el punto inicial
    mInicial = mFinal;
    }
}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    //Tipo de figura
    if (type_figure == 0){
        mPainter->drawLine(mInicial, mFinal);
    }else if (type_figure == 2){
        mPainter->drawRect(QRect(QPoint(mInicial),QPoint(mFinal)));
    }else if(type_figure == 3)
    {
        int w = mFinal.x() - mInicial.x();
        mPainter->drawArc(mInicial.x()-w,mInicial.y()-w,w*2,w*2,-90*16,360*16);
    }
    update();
    mInicial = mFinal;
    mPuedeDibujar = false;
    // Aceptar el vento
    event->accept();

}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes (*.png)");
    if ( !nombreArchivo.isEmpty() ){
        if (mImagen->save(nombreArchivo))
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}
