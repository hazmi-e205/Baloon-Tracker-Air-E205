#include "utama.h"
#include "ui_utama.h"
#include "serialthread.h"
#include <QUrl>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <QtQuick>
#include <QSerialPortInfo>
#include <QtWebEngineWidgets>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "haversinethread.h"
#include "vincentythread.h"
#include <QDebug>

QSerialPort Serial1;
serialThread serial1;
QObject *kompasnya;

QwtPlotCurve *az_sp_crv;
QwtPlotCurve *az_now_crv;
QwtPlotCurve *el_sp_crv;
QwtPlotCurve *el_now_crv;

unsigned long tambah=0;

double lon_baru, lat_baru;
double tinggiR=0;

const int ukurane = 100;
haversineThread hav;
vincentyThread vin;


utama::utama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::utama)
{
    ui->setupUi(this);
    setWindowTitle("Atmosphere Balloon Project :: by hazmi.e205");
    setFixedHeight(height()-12);
    setFixedWidth(width());


    //bukaHalaman();
    tampilkanKompas();
    grafik1_init();
    grafik2_init();
    serial1.config();
    serialUpdate();
    hav.config();
    vin.config();

    connect(ui->koneksi,SIGNAL(pressed()),this,SLOT(serialConnect()));
    connect(ui->updateUSB,SIGNAL(pressed()),this,SLOT(serialUpdate()));
    connect(ui->set_sp_az,SIGNAL(returnPressed()),this,SLOT(az_set_sp()));
    connect(ui->set_p_az,SIGNAL(returnPressed()),this,SLOT(az_set_p()));
    connect(ui->set_i_az,SIGNAL(returnPressed()),this,SLOT(az_set_i()));
    connect(ui->set_d_az,SIGNAL(returnPressed()),this,SLOT(az_set_d()));
    connect(ui->set_sp_el,SIGNAL(returnPressed()),this,SLOT(el_set_sp()));
    connect(ui->set_p_el,SIGNAL(returnPressed()),this,SLOT(el_set_p()));
    connect(ui->set_i_el,SIGNAL(returnPressed()),this,SLOT(el_set_i()));
    connect(ui->set_d_el,SIGNAL(returnPressed()),this,SLOT(el_set_d()));


    connect(&hav,SIGNAL(selesai(int)),this,SLOT(kompas_updt(int)));

    qDebug()<< "Main Thread ID is " <<QThread::currentThreadId();
}

utama::~utama()
{
    delete ui;
}

void utama::bukaHalaman()
{

    /*
    QWebEngineView *peta=new QWebEngineView(ui->maps);

    peta->resize(ui->maps->width(),ui->maps->height());
    peta->settings()->setAttribute(QWebEngineSettings::WebGLEnabled,true);
    peta->settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled,true);
    peta->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    peta->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled,true);
    peta->load(QUrl("http://127.0.0.1/cesiumjs/Apps/Sandcastle/tracker/index3.html"));
    */
    //QUrl source("qrc:/qml/track.qml");
    //ui->track->setSource(source);
    //QObject *objt = ui->track->rootObject();
    //kompasnya=objt->findChild<QObject*>("rect");
    //kompasnya->setProperty("url", "http://127.0.0.1/cesiumjs/Apps/Sandcastle/tracker/index2.html");
}

void utama::tampilkanKompas()
{
    QUrl source("qrc:/qml/kompas.qml");
    ui->kompas_show->setSource(source);
    QObject *objt = ui->kompas_show->rootObject();
    kompasnya=objt->findChild<QObject*>("rect");
    kompasnya->setProperty("angle", 0);
}

void utama::grafik1_init()
{
    ui->qwtPlot->setAxisScale(0,-10,370);
    az_sp_crv = new QwtPlotCurve("Set Point");
    az_sp_crv->setPen(QPen(Qt::darkBlue));
    az_now_crv = new QwtPlotCurve("Azimuth");
    az_now_crv->setPen(QPen(Qt::darkGreen));
}

void utama::grafik2_init()
{
    ui->qwtPlot_2->setAxisScale(0,-10,100);
    el_sp_crv = new QwtPlotCurve("Set Point");
    el_sp_crv->setPen(QPen(Qt::darkBlue));
    el_now_crv = new QwtPlotCurve("Elevasi");
    el_now_crv->setPen(QPen(Qt::darkGreen));
}

void utama::grafik1_updt(float setpoint, float sekarang)
{
    static double tik = 0;
    static double ydata1[ukurane];
    static double ydata2[ukurane];
    static double xdata[ukurane];

    for(unsigned long int i=ukurane-1;i>0;i--){
        xdata[i] = xdata[i-1];
        ydata1[i] = ydata1[i-1];
        ydata2[i] = ydata2[i-1];
    }

    xdata[0]=tik;
    ydata1[0]=setpoint;
    ydata2[0]=sekarang;

    az_sp_crv->setRawSamples(xdata,ydata1,ukurane);
    az_now_crv->setRawSamples(xdata,ydata2,ukurane);

    az_sp_crv->attach(ui->qwtPlot);
    az_now_crv->attach(ui->qwtPlot);

    ui->qwtPlot->replot();
    tik++;
}

void utama::grafik2_updt(float setpoint, float sekarang)
{
    static double tik = 0;
    static double ydata1[ukurane];
    static double ydata2[ukurane];
    static double xdata[ukurane];

    for(unsigned long int i=ukurane-1;i>0;i--){
        xdata[i] = xdata[i-1];
        ydata1[i] = ydata1[i-1];
        ydata2[i] = ydata2[i-1];
    }

    xdata[0]=tik;
    ydata1[0]=setpoint;
    ydata2[0]=sekarang;

    el_sp_crv->setRawSamples(xdata,ydata1,ukurane);
    el_now_crv->setRawSamples(xdata,ydata2,ukurane);

    el_sp_crv->attach(ui->qwtPlot_2);
    el_now_crv->attach(ui->qwtPlot_2);

    ui->qwtPlot_2->replot();
    tik++;
}

void utama::suhu_updt(int suhune)
{
    if (suhune>=0){
        if (suhune>53) ui->suhuBar->setStyleSheet("QProgressBar::chunk {background-color: rgb(170, 0, 0);} QProgressBar{color: rgb(255, 255, 255);}");
        else ui->suhuBar->setStyleSheet("QProgressBar::chunk {background-color: rgb(170, 0, 0);} QProgressBar{color: rgb(0, 0, 0);}");
        ui->suhuBar->setInvertedAppearance(false);
        ui->suhuBar->setValue(suhune);
    } else {
        if (suhune<(-53)) ui->suhuBar->setStyleSheet("QProgressBar::chunk {background-color: rgb(162, 162, 162);} QProgressBar{color: rgb(255, 255, 255);}");
        else ui->suhuBar->setStyleSheet("QProgressBar::chunk {background-color: rgb(162, 162, 162);} QProgressBar{color: rgb(0, 0, 0);}");
        ui->suhuBar->setInvertedAppearance(true);
        ui->suhuBar->setValue(suhune*(-1));
    }
}

void utama::rssi_updt(int rssi_L, int rssi_R)
{
    ui->rssiBar_L->setValue(abs(rssi_L));
    ui->rssiBar_R->setValue(abs(rssi_R));
}

void utama::tekanan_updt(float press_0, float press_1)
{
    ui->tekanan0_teks->setText(QString::number(press_0) + " Pa");
    ui->tekanan1_teks->setText(QString::number(press_1) + " Pa");
    ui->pressure_1->setText(QString::number(press_1));
}

void utama::tinggi_updt(float tinggiRelatif)
{
    ui->ketinggian_teks->setText(QString::number(tinggiRelatif,'f',2) + " meter");
}

double lon_lama, lat_lama;
void utama::lokasi_updt(double lon_updt, double lat_updt, double press_updt)
{
    ui->longitude_1->setText(QString::number(lon_updt));
    ui->latitude_1->setText(QString::number(lat_updt));
    ui->pressure_1->setText(QString::number(press_updt));

    tinggiR = (44330.0f * (1.0f - pow((float)press_updt / ui->pressure_0->text().toFloat(), 0.1902949f)));
    hav.begin(lon_lama,lat_lama,lon_updt,lat_updt);
    vin.begin(lon_lama,lat_lama,lon_updt,lat_updt,tinggiR);

    tinggi_updt(tinggiR);
    tekanan_updt(ui->pressure_0->text().toFloat(),press_updt);

    lon_lama=lon_updt;
    lat_lama=lat_updt;
}

void utama::kompas_updt(int sudute)
{
    kompasnya->setProperty("angle", sudute);
}

void utama::az_set_sp()
{
    //serial1.serialOne.write("z" + ui->set_sp_az->text().toLatin1() + "#");
    Serial1.write("z" + ui->set_sp_az->text().toLatin1() + "#");
}

void utama::az_set_p()
{
    //serial1.serialOne.write("a" + ui->set_p_az->text().toLatin1() + "#");
    Serial1.write("a" + ui->set_p_az->text().toLatin1() + "#");
}

void utama::az_set_i()
{
    //serial1.serialOne.write("k" + ui->set_i_az->text().toLatin1() + "#");
    Serial1.write("k" + ui->set_i_az->text().toLatin1() + "#");
}

void utama::az_set_d()
{
    //serial1.serialOne.write("y" + ui->set_d_az->text().toLatin1() + "#");
    Serial1.write("y" + ui->set_d_az->text().toLatin1() + "#");
}

void utama::el_set_sp()
{
    //serial1.serialOne.write("s" + ui->set_sp_az->text().toLatin1() + "#");
    Serial1.write("s" + ui->set_sp_az->text().toLatin1() + "#");
}

void utama::el_set_p()
{
    //serial1.serialOne.write("p" + ui->set_p_az->text().toLatin1() + "#");
    Serial1.write("p" + ui->set_p_az->text().toLatin1() + "#");
}

void utama::el_set_i()
{
    //serial1.serialOne.write("i" + ui->set_i_az->text().toLatin1() + "#");
    Serial1.write("i" + ui->set_i_az->text().toLatin1() + "#");
}

void utama::el_set_d()
{
    //serial1.serialOne.write("d" + ui->set_d_az->text().toLatin1() + "#");
    Serial1.write("d" + ui->set_d_az->text().toLatin1() + "#");
}

void utama::serialConnect()
{
    /*
    if (!serial1.serialOne.isOpen()){
        serial1.begin(ui->serial_list->currentText());
        QThread::msleep(300);
        if (serial1.serialOne.isOpen()){
            ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/conect.png);");
            connect(&serial1,SIGNAL(dataPenuh(QString,int)),this,SLOT(terimaAll(QString,int)));
        } else {
            ui->statusbar_e205->setText("Koneksi serial bermasalah, mohon dicek lagi...");
        }
    } else {
        serial1.stopAll();
        QThread::msleep(300);
        if (!serial1.serialOne.isOpen()){
            ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/disconect.png);");
            disconnect(&serial1,SIGNAL(dataPenuh(QString,int)),0,0);
        }
    }
    */


    if (!Serial1.isOpen()){
        Serial1.setPortName(ui->serial_list->currentText());
        Serial1.setBaudRate(QSerialPort::Baud115200);
        Serial1.setDataBits(QSerialPort::Data8);
        Serial1.setParity(QSerialPort::NoParity);
        Serial1.setStopBits(QSerialPort::OneStop);
        Serial1.setFlowControl(QSerialPort::NoFlowControl);
        Serial1.open(QIODevice::ReadWrite);

        QThread::msleep(300);
        if (Serial1.isOpen()){
            ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/conect.png);");
            connect(&Serial1,SIGNAL(readyRead()),this,SLOT(serialTerima()));
        } else {
            ui->statusbar_e205->setText("Koneksi serial bermasalah, mohon dicek lagi...");
        }
    } else {
        Serial1.close();
        QThread::msleep(300);
        if (!Serial1.isOpen()){
            ui->koneksi->setStyleSheet("border-image: url(:/konten/gambar/tombol/disconect.png);");
            disconnect(&Serial1,SIGNAL(readyRead()),0,0);
        }
    }

}

void utama::serialUpdate()
{
    ui->serial_list->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->serial_list->addItem(info.portName());
    }
}

QString dataAll;
bool bacaFlag=false;
double hit=0;

void utama::serialTerima()
{
    QString terima = Serial1.readLine();
    QStringList terima1 = terima.split('@');
    if (terima1.length()==2){
        QStringList terima2 = terima1[1].split('#');
        if (terima2.length()==2){
            ui->terima->setPlainText(terima2[0]);
            QStringList pisah = terima2[0].split(' ');

            switch (pisah.length()) {
            case 10:
                ui->skr_az->setText(pisah[0]);
                ui->sp_az->setText(pisah[1]);
                ui->p_az->setText(pisah[2]);
                ui->i_az->setText(pisah[3]);
                ui->d_az->setText(pisah[4]);

                ui->skr_el->setText(pisah[5]);
                ui->sp_el->setText(pisah[6]);
                ui->p_el->setText(pisah[7]);
                ui->i_el->setText(pisah[8]);
                ui->d_el->setText(pisah[9]);

                //grafik1_updt(pisah[1].toFloat(),pisah[0].toFloat());
                //grafik2_updt(pisah[6].toFloat(),pisah[5].toFloat());
                break;

            case 13:

                break;
            }
            qDebug()<< "Serial Thread ID is " <<QThread::currentThreadId();
        }
    }
}


void utama::terimaAll(QString semuanya, int perdetik)
{
    /*
    ui->terima->setPlainText(semuanya + " | " + QString::number(perdetik));
    QStringList pisah = semuanya.split(' ');

    switch (pisah.length()) {
    case 10:
        ui->skr_az->setText(pisah[0]);
        ui->sp_az->setText(pisah[1]);
        ui->p_az->setText(pisah[2]);
        ui->i_az->setText(pisah[3]);
        ui->d_az->setText(pisah[4]);

        ui->skr_el->setText(pisah[5]);
        ui->sp_el->setText(pisah[6]);
        ui->p_el->setText(pisah[7]);
        ui->i_el->setText(pisah[8]);
        ui->d_el->setText(pisah[9]);

        grafik1_updt(pisah[1].toFloat(),pisah[0].toFloat());
        grafik2_updt(pisah[6].toFloat(),pisah[5].toFloat());
        break;

    case 13:

        break;
    }
    */
}
