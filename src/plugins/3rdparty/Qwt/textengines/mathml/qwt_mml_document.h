#ifndef _QWT_MML_DOCUMENT_H_
#define _QWT_MML_DOCUMENT_H_

#include <qwt_global.h>
#include <qcolor.h>
#include <qstring.h>
#include <qsize.h>

class QPainter;
class QPointF;

class QwtMmlDocument;

class QWT_EXPORT QwtMathMLDocument
{
public:
    enum MmlFont
    {
        NormalFont,
        FrakturFont,
        SansSerifFont,
        ScriptFont,
        MonospaceFont,
        DoublestruckFont
    };

    QwtMathMLDocument();
    ~QwtMathMLDocument();

    void clear();

    bool setContent( QString text, QString *errorMsg = 0,
                     int *errorLine = 0, int *errorColumn = 0 );
    void paint( QPainter *, const QPointF &pos ) const;
    QSizeF size() const;

    QString fontName( MmlFont type ) const;
    void setFontName( MmlFont type, const QString &name );

    qreal baseFontPointSize() const;
    void setBaseFontPointSize( qreal size );

    QColor foregroundColor() const;
    void setForegroundColor( const QColor &color );

    QColor backgroundColor() const;
    void setBackgroundColor( const QColor &color );

    bool drawFrames() const;
    void setDrawFrames( const bool &drawFrames );

private:
    QwtMmlDocument *m_doc;
};

#endif
