/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_grid.h"
#include "qwt_painter.h"
#include "qwt_text.h"
#include "qwt_scale_map.h"
#include "qwt_scale_div.h"
#include "qwt_math.h"
#include <qpainter.h>
#include <qpen.h>

class QwtPlotGrid::PrivateData
{
public:
    PrivateData():
        xEnabled( true ),
        yEnabled( true ),
        xMinEnabled( false ),
        yMinEnabled( false )
    {
    }

    bool xEnabled;
    bool yEnabled;
    bool xMinEnabled;
    bool yMinEnabled;

    QwtScaleDiv xScaleDiv;
    QwtScaleDiv yScaleDiv;

    QPen majPen;
    QPen minPen;
};

//! Enables major grid, disables minor grid
QwtPlotGrid::QwtPlotGrid():
    QwtPlotItem( QwtText( "Grid" ) )
{
    d_data = new PrivateData;
    setZ( 10.0 );
}

//! Destructor
QwtPlotGrid::~QwtPlotGrid()
{
    delete d_data;
}

//! \return QwtPlotItem::Rtti_PlotGrid
int QwtPlotGrid::rtti() const
{
    return QwtPlotItem::Rtti_PlotGrid;
}

/*!
  \brief Enable or disable vertical grid lines
  \param on Enable (true) or disable

  \sa Minor grid lines can be enabled or disabled with
      enableXMin()
*/
void QwtPlotGrid::enableX( bool on )
{
    if ( d_data->xEnabled != on )
    {
        d_data->xEnabled = on;
        itemChanged();
    }
}

/*!
  \brief Enable or disable horizontal grid lines
  \param on Enable (true) or disable
  \sa Minor grid lines can be enabled or disabled with enableYMin()
*/
void QwtPlotGrid::enableY( bool on )
{
    if ( d_data->yEnabled != on )
    {
        d_data->yEnabled = on;
        itemChanged();
    }
}

/*!
  \brief Enable or disable  minor vertical grid lines.
  \param on Enable (true) or disable
  \sa enableX()
*/
void QwtPlotGrid::enableXMin( bool on )
{
    if ( d_data->xMinEnabled != on )
    {
        d_data->xMinEnabled = on;
        itemChanged();
    }
}

/*!
  \brief Enable or disable minor horizontal grid lines
  \param on Enable (true) or disable
  \sa enableY()
*/
void QwtPlotGrid::enableYMin( bool on )
{
    if ( d_data->yMinEnabled != on )
    {
        d_data->yMinEnabled = on;
        itemChanged();
    }
}

/*!
  Assign an x axis scale division

  \param scaleDiv Scale division
*/
void QwtPlotGrid::setXDiv( const QwtScaleDiv &scaleDiv )
{
    if ( d_data->xScaleDiv != scaleDiv )
    {
        d_data->xScaleDiv = scaleDiv;
        itemChanged();
    }
}

/*!
  Assign a y axis division

  \param scaleDiv Scale division
*/
void QwtPlotGrid::setYDiv( const QwtScaleDiv &scaleDiv )
{
    if ( d_data->yScaleDiv != scaleDiv )
    {
        d_data->yScaleDiv = scaleDiv;
        itemChanged();
    }
}

/*!
  Assign a pen for both major and minor grid lines

  \param pen Pen
  \sa setMajPen(), setMinPen()
*/
void QwtPlotGrid::setPen( const QPen &pen )
{
    if ( d_data->majPen != pen || d_data->minPen != pen )
    {
        d_data->majPen = pen;
        d_data->minPen = pen;
        itemChanged();
    }
}

/*!
  Assign a pen for the major grid lines

  \param pen Pen
  \sa majPen(), setMinPen(), setPen()
*/
void QwtPlotGrid::setMajPen( const QPen &pen )
{
    if ( d_data->majPen != pen )
    {
        d_data->majPen = pen;
        itemChanged();
    }
}

/*!
  Assign a pen for the minor grid lines

  \param pen Pen
  \sa minPen(), setMajPen(), setPen()
*/
void QwtPlotGrid::setMinPen( const QPen &pen )
{
    if ( d_data->minPen != pen )
    {
        d_data->minPen = pen;
        itemChanged();
    }
}

/*!
  \brief Draw the grid

  The grid is drawn into the bounding rectangle such that
  grid lines begin and end at the rectangle's borders. The X and Y
  maps are used to map the scale divisions into the drawing region
  screen.

  \param painter  Painter
  \param xMap X axis map
  \param yMap Y axis
  \param canvasRect Contents rectangle of the plot canvas
*/
void QwtPlotGrid::draw( QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRectF &canvasRect ) const
{
    //  draw minor grid lines
    QPen minPen = d_data->minPen;
    minPen.setCapStyle( Qt::FlatCap );

    painter->setPen( minPen );

    if ( d_data->xEnabled && d_data->xMinEnabled )
    {
        drawLines( painter, canvasRect, Qt::Vertical, xMap,
            d_data->xScaleDiv.ticks( QwtScaleDiv::MinorTick ) );
        drawLines( painter, canvasRect, Qt::Vertical, xMap,
            d_data->xScaleDiv.ticks( QwtScaleDiv::MediumTick ) );
    }

    if ( d_data->yEnabled && d_data->yMinEnabled )
    {
        drawLines( painter, canvasRect, Qt::Horizontal, yMap,
            d_data->yScaleDiv.ticks( QwtScaleDiv::MinorTick ) );
        drawLines( painter, canvasRect, Qt::Horizontal, yMap,
            d_data->yScaleDiv.ticks( QwtScaleDiv::MediumTick ) );
    }

    //  draw major grid lines
    QPen majPen = d_data->majPen;
    majPen.setCapStyle( Qt::FlatCap );

    painter->setPen( majPen );

    if ( d_data->xEnabled )
    {
        drawLines( painter, canvasRect, Qt::Vertical, xMap,
            d_data->xScaleDiv.ticks( QwtScaleDiv::MajorTick ) );
    }

    if ( d_data->yEnabled )
    {
        drawLines( painter, canvasRect, Qt::Horizontal, yMap,
            d_data->yScaleDiv.ticks( QwtScaleDiv::MajorTick ) );
    }
}

void QwtPlotGrid::drawLines( QPainter *painter, const QRectF &canvasRect,
    Qt::Orientation orientation, const QwtScaleMap &scaleMap,
    const QList<double> &values ) const
{
    const double x1 = canvasRect.left();
    const double x2 = canvasRect.right() - 1.0;
    const double y1 = canvasRect.top();
    const double y2 = canvasRect.bottom() - 1.0;

    const bool doAlign = QwtPainter::roundingAlignment( painter );

    for ( int i = 0; i < values.count(); i++ )
    {
        double value = scaleMap.transform( values[i] );
        if ( doAlign )
            value = qRound( value );

        if ( orientation == Qt::Horizontal )
        {
            if ( qwtFuzzyGreaterOrEqual( value, y1 ) &&
                qwtFuzzyLessOrEqual( value, y2 ) )
            {
                QwtPainter::drawLine( painter, x1, value, x2, value );
            }
        }
        else
        {
            if ( qwtFuzzyGreaterOrEqual( value, x1 ) &&
                qwtFuzzyLessOrEqual( value, x2 ) )
            {
                QwtPainter::drawLine( painter, value, y1, value, y2 );
            }
        }
    }
}

/*!
  \return the pen for the major grid lines
  \sa setMajPen(), setMinPen(), setPen()
*/
const QPen &QwtPlotGrid::majPen() const
{
    return d_data->majPen;
}

/*!
  \return the pen for the minor grid lines
  \sa setMinPen(), setMajPen(), setPen()
*/
const QPen &QwtPlotGrid::minPen() const
{
    return d_data->minPen;
}

/*!
  \return true if vertical grid lines are enabled
  \sa enableX()
*/
bool QwtPlotGrid::xEnabled() const
{
    return d_data->xEnabled;
}

/*!
  \return true if minor vertical grid lines are enabled
  \sa enableXMin()
*/
bool QwtPlotGrid::xMinEnabled() const
{
    return d_data->xMinEnabled;
}

/*!
  \return true if horizontal grid lines are enabled
  \sa enableY()
*/
bool QwtPlotGrid::yEnabled() const
{
    return d_data->yEnabled;
}

/*!
  \return true if minor horizontal grid lines are enabled
  \sa enableYMin()
*/
bool QwtPlotGrid::yMinEnabled() const
{
    return d_data->yMinEnabled;
}


/*! \return the scale division of the x axis */
const QwtScaleDiv &QwtPlotGrid::xScaleDiv() const
{
    return d_data->xScaleDiv;
}

/*! \return the scale division of the y axis */
const QwtScaleDiv &QwtPlotGrid::yScaleDiv() const
{
    return d_data->yScaleDiv;
}

/*!
   Update the grid to changes of the axes scale division

   \param xScaleDiv Scale division of the x-axis
   \param yScaleDiv Scale division of the y-axis

   \sa QwtPlot::updateAxes()
*/
void QwtPlotGrid::updateScaleDiv( const QwtScaleDiv& xScaleDiv,
    const QwtScaleDiv& yScaleDiv )
{
    setXDiv( xScaleDiv );
    setYDiv( yScaleDiv );
}
