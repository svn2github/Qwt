#ifndef DIAL_BOX_H
#define DIAL_BOX_H

#include <qwidget.h>

class QLabel;
class QwtDial;

class DialBox: public QWidget
{
    Q_OBJECT
public:
    DialBox( QWidget *parent, int type );

private Q_SLOTS:
    void setNum( double v );

private:
    QwtDial *createDial( int type ) const;

    QwtDial *d_dial;
    QLabel *d_label;
};

#endif
