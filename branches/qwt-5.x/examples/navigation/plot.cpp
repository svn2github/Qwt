#include "plot.h"
#include <qpainter.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_double_interval.h>
#include <qwt_painter.h>
#include <qwt_plot_item.h>
#include <stdlib.h>

class RectItem: public QwtPlotItem
{
public:
    enum Type
    {
        Rect,
        Ellipse
    };

    RectItem(Type type):
        d_type(type)
    {
    }

    void setPen(const QPen &pen)
    {
        if ( pen != d_pen )
        {
            d_pen = pen;
            itemChanged();
        }
    }

    void setBrush(const QBrush &brush)
    {
        if ( brush != d_brush )
        {
            d_brush = brush;
            itemChanged();
        }
    }
    void setRect(const QwtDoubleRect &rect)
    {
        if ( d_rect != rect )
        {
            d_rect = rect;
            itemChanged();
        }
    }

    virtual QwtDoubleRect boundingRect() const
    {
        return d_rect;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRect &) const
    {
        if ( d_rect.isValid() )
        {
            const QRect rect = transform(xMap, yMap, d_rect);
            painter->setPen(d_pen);
            painter->setBrush(d_brush);
            if ( d_type == Ellipse )
                QwtPainter::drawEllipse(painter, rect);
            else
                QwtPainter::drawRect(painter, rect);
        }
    }
private:
    QPen d_pen;
    QBrush d_brush;
    QwtDoubleRect d_rect;
    Type d_type;
};

Plot::Plot(QWidget *parent, const QwtDoubleInterval &interval):
    QwtPlot(parent)
{
    for ( int axis = 0; axis < QwtPlot::axisCnt; axis ++ )
        setAxisScale(axis, interval.minValue(), interval.maxValue());

    setCanvasBackground(QColor(Qt::darkBlue));
    plotLayout()->setAlignCanvasToScales(true);

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    //grid->enableXMin(true);
    grid->setMajPen(QPen(Qt::white, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(this);

    const int numEllipses = 10;

    for ( int i = 0; i < numEllipses; i++ )
    {
        const double x = interval.minValue() + 
            rand() % qRound(interval.width());
        const double y = interval.minValue() + 
            rand() % qRound(interval.width());
        const double r = interval.minValue() + 
            rand() % qRound(interval.width() / 6);

        const QwtDoubleRect area(x - r, y - r , 2 * r, 2 * r);

        RectItem *item = new RectItem(RectItem::Ellipse);
#if QT_VERSION >= 0x040000
        item->setRenderHint(QwtPlotItem::RenderAntialiased, true);
#endif
        item->setRect(area);
        item->setPen(QPen(Qt::yellow));
        item->attach(this);
    }


    d_rectOfInterest = new RectItem(RectItem::Rect);
    d_rectOfInterest->setPen(Qt::NoPen);
    QColor c = Qt::gray;
#if QT_VERSION < 0x040000
    d_rectOfInterest->setBrush(QBrush(c, Qt::Dense5Pattern));
#else
    c.setAlpha(100);
    d_rectOfInterest->setBrush(QBrush(c));
#endif
    d_rectOfInterest->attach(this);
}

void Plot::updateLayout()
{
    QwtPlot::updateLayout();

    const QwtScaleMap xMap = canvasMap(QwtPlot::xBottom);
    const QwtScaleMap yMap = canvasMap(QwtPlot::yLeft);

    const QRect cr = canvas()->contentsRect();
    const double x1 = xMap.invTransform(cr.left());
    const double x2 = xMap.invTransform(cr.right());
    const double y1 = yMap.invTransform(cr.bottom());
    const double y2 = yMap.invTransform(cr.top());

    const double xRatio = (x2 - x1) / cr.width();
    const double yRatio = (y2 - y1) / cr.height();

    emit resized(xRatio, yRatio);
}

void Plot::setRectOfInterest(const QwtDoubleRect &rect)
{
    d_rectOfInterest->setRect(rect);
}
