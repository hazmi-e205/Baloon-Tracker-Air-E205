#ifndef UTAMA_H
#define UTAMA_H

#include <QMainWindow>

namespace Ui {
class utama;
}

class utama : public QMainWindow
{
    Q_OBJECT

public:
    explicit utama(QWidget *parent = 0);
    ~utama();
    void bukaHalaman();
    void tampilkanKompas();
    void grafik1_init();
    void grafik2_init();



private slots:
    void serialConnect();
    void serialUpdate();
    void serialTerima();
    void terimaAll(QString, int);

    void grafik1_updt(float setpoint, float sekarang);
    void grafik2_updt(float setpoint, float sekarang);
    void suhu_updt(int suhune);
    void rssi_updt(int rssi_L, int rssi_R);
    void tekanan_updt(float press_0, float press_1);
    void tinggi_updt(float tinggiRelatif);
    void lokasi_updt(double lon_updt, double lat_updt, double press_updt);
    void kompas_updt(int sudute);

    void az_set_sp();
    void az_set_p();
    void az_set_i();
    void az_set_d();
    void el_set_sp();
    void el_set_p();
    void el_set_i();
    void el_set_d();
private:
    Ui::utama *ui;
};

#endif // UTAMA_H
