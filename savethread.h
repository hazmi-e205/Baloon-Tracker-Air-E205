#ifndef SAVETHREAD_H
#define SAVETHREAD_H

#include <QObject>

class saveThread : public QObject
{
    Q_OBJECT
public:
    explicit saveThread(QObject *parent = 0);
    void getSaveLocation();

signals:

public slots:
    void begin(QString data4save);
    void config();
    void doWork();
};

#endif // SAVETHREAD_H
