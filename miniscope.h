#ifndef MINISCOPE_H
#define MINISCOPE_H

#include <QObject>

class Miniscope : public QObject
{
    Q_OBJECT
public:
    explicit Miniscope(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MINISCOPE_H
