#include "signaldata.h"
#include <qvector.h>
#include <qmutex.h>
#include <qreadwritelock.h>

class SignalData::PrivateData
{
public:
    PrivateData():
        boundingRect(1.0, 1.0, -2.0, -2.0) // invalid
    {
        values.reserve(1000);
    }

    inline void append(const QwtDoublePoint &sample)
    {
        values.append(sample);

        // adjust the bounding rectangle

        if ( !boundingRect.isValid() )
        {
            boundingRect.setRect(sample.x(), sample.y(), 0.0, 0.0);
        }
        else
        {
            boundingRect.setRight(sample.x());

            if ( sample.y() > boundingRect.bottom() )
                boundingRect.setBottom(sample.y());

            if ( sample.y() < boundingRect.top() )
                boundingRect.setBottom(sample.y());
        }
    }

    QReadWriteLock lock;

    QVector<QwtDoublePoint> values;
    QwtDoubleRect boundingRect;

    QMutex mutex; // protecting pendingValues
    QVector<QwtDoublePoint> pendingValues;
};

SignalData::SignalData()
{
    d_data = new PrivateData();
}

SignalData::~SignalData()
{
    delete d_data;
}

int SignalData::size() const
{
    return d_data->values.size();
}   

QwtDoublePoint SignalData::value(int index) const
{
    return d_data->values[index];
}   

QwtDoubleRect SignalData::boundingRect() const
{
    return d_data->boundingRect;
}

void SignalData::lock()
{
    d_data->lock.lockForRead();
}

void SignalData::unlock()
{
    d_data->lock.unlock();
}

void SignalData::append(const QwtDoublePoint &sample)
{
    d_data->mutex.lock();
    d_data->pendingValues += sample;

    const bool isLocked = d_data->lock.tryLockForWrite();
    if ( isLocked )
    {
        const int numValues = d_data->pendingValues.size();
        const QwtDoublePoint *pendingValues = d_data->pendingValues.data();

        for ( int i = 0; i < numValues; i++ )
            d_data->append(pendingValues[i]);

        d_data->pendingValues.clear();

        d_data->lock.unlock();
    }

    d_data->mutex.unlock();
}

void SignalData::clearStaleValues(double limit)
{
    d_data->lock.lockForWrite();

    d_data->boundingRect = QwtDoubleRect(1.0, 1.0, -2.0, -2.0); // invalid

    const QVector<QwtDoublePoint> values = d_data->values;
    d_data->values.clear();
    d_data->values.reserve(values.size());

    int index;
    for ( index = values.size() - 1; index >= 0; index-- )
    {
        if ( values[index].x() < limit )
            break;
    }

    if ( index > 0 )
        d_data->append(values[index++]);

    while ( index < values.size() - 1 )
        d_data->append(values[index++]);

    d_data->lock.unlock();
}

SignalData &SignalData::instance()
{
    static SignalData valueVector;
    return valueVector;
}
