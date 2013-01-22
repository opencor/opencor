/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SLIDER_H
#define QWT_SLIDER_H

#include "qwt_global.h"
#include "qwt_abstract_slider.h"

class QwtScaleDraw;

/*!
  \brief The Slider Widget

  QwtSlider is a slider widget which operates on an interval
  of type double. QwtSlider supports different layouts as
  well as a scale.

  \image html sliders.png
*/

class QWT_EXPORT QwtSlider: public QwtAbstractSlider
{
    Q_OBJECT

    Q_ENUMS( ScalePosition BackgroundStyle )

    Q_PROPERTY( Qt::Orientation orientation
                READ orientation WRITE setOrientation )
    Q_PROPERTY( ScalePosition scalePosition READ scalePosition
        WRITE setScalePosition )
    Q_PROPERTY( BackgroundStyles backgroundStyle 
        READ backgroundStyle WRITE setBackgroundStyle )
    Q_PROPERTY( QSize handleSize READ handleSize WRITE setHandleSize )
    Q_PROPERTY( int borderWidth READ borderWidth WRITE setBorderWidth )
    Q_PROPERTY( int spacing READ spacing WRITE setSpacing )

public:

    /*!
      Scale position. 

      \sa QwtSlider(), setOrientation()
     */
    enum ScalePosition
    {
        //! The slider has no scale
        NoScale,

        //! The scale is right of a vertical or below a horizontal slider
        LeadingScale,

        //! The scale is left of a vertical or above a horizontal slider
        TrailingScale
    };

    /*!
      Background style.
      \sa QwtSlider()
     */
    enum BackgroundStyle
    {
        //! Trough background
        Trough = 0x01,

        //! Groove
        Groove = 0x02,
    };

    //! Background styles
    typedef QFlags<BackgroundStyle> BackgroundStyles;

    explicit QwtSlider( QWidget *parent = NULL );
    explicit QwtSlider( Qt::Orientation, QWidget *parent = NULL );

    virtual ~QwtSlider();

    void setOrientation( Qt::Orientation );
    Qt::Orientation orientation() const;

    void setScalePosition( ScalePosition );
    ScalePosition scalePosition() const;

    void setBackgroundStyle( BackgroundStyles );
    BackgroundStyles backgroundStyle() const;

    void setHandleSize( const QSize & );
    QSize handleSize() const;

    void setBorderWidth( int bw );
    int borderWidth() const;

    void setSpacing( int );
    int spacing() const;

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setScaleDraw( QwtScaleDraw * );
    const QwtScaleDraw *scaleDraw() const;

    void setUpdateInterval( int );
    int updateInterval() const;

protected:
    virtual double scrolledTo( const QPoint & ) const;
    virtual bool isScrollPosition( const QPoint & ) const;

    virtual void drawSlider ( QPainter *, const QRect & ) const;
    virtual void drawHandle( QPainter *, const QRect &, int pos ) const;

    virtual void mousePressEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void paintEvent ( QPaintEvent * );
    virtual void changeEvent( QEvent * );
    virtual void timerEvent( QTimerEvent * );

    virtual void scaleChange();

    QRect sliderRect() const;
    QRect handleRect() const;

private:
    QwtScaleDraw *scaleDraw();

    void layoutSlider( bool );
    void initSlider( Qt::Orientation );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtSlider::BackgroundStyles )

#endif
