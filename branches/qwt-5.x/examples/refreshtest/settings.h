#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <qpen.h>
#include <qbrush.h>

class Settings
{
public:
    enum FunctionType
    {
        NoFunction = -1,

        Wave,
        Noise
    };

    enum UpdateType
    {
        UpdateCanvas,
        RepaintCanvas,
        Replot
    };

    Settings()
    {
        grid.pen = Qt::NoPen;

        curve.brush = Qt::NoBrush;
        curve.numPoints = 1000;
        curve.functionType = Wave;
        curve.paintAttributes = 0;
        curve.renderHint = 0;
        curve.lineSplitting = true;
        
        canvas.cached = false;
#if QT_VERSION >= 0x040000
        canvas.paintOnScreen = false;
#endif
        canvas.deviceClipping = false;

        updateType = RepaintCanvas;
        updateInterval = 20;
    }

    struct
    {
        QPen pen;
    } grid;

    struct 
    {
        QPen pen;
        QBrush brush;
        uint numPoints;
        FunctionType functionType;
        int paintAttributes;
        int renderHint;
        bool lineSplitting;
    } curve;

    struct
    {
        bool deviceClipping;
        bool cached;
#if QT_VERSION >= 0x040000
        bool paintOnScreen;
#endif
    } canvas;

    UpdateType updateType;
    int updateInterval;
};

#endif
