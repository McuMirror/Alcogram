#include "utils.h"

namespace Utils {
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight)
    {
        QFont font(family);
        font.setPixelSize(pixelSize);
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        font.setWeight(weight);

        return font;
    }

    /*cv::Mat qImageToCvMat( const QImage &inImage, bool inCloneImageData)
    {
      switch ( inImage.format() )
      {
         // 8-bit, 4 channel
         case QImage::Format_ARGB32:
         case QImage::Format_ARGB32_Premultiplied:
         {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
         }

         // 8-bit, 3 channel
         case QImage::Format_RGB32:
         case QImage::Format_RGB888:
         {
            if ( !inCloneImageData )
            {
               //qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage   swapped = inImage;

            if ( inImage.format() == QImage::Format_RGB32 )
            {
               swapped = swapped.convertToFormat( QImage::Format_RGB888 );
            }

            swapped = swapped.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(),
                            CV_8UC3,
                            const_cast<uchar*>(swapped.bits()),
                            static_cast<size_t>(swapped.bytesPerLine())
                            ).clone();
         }

         // 8-bit, 1 channel
         case QImage::Format_Indexed8:
         {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC1,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
         }

         default:
            //qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
      }

      return cv::Mat();
    }*/

    QString getStateNameNumber(StateName stateName)
    {
        // TODO: use static array instead
        switch (stateName) {
            case SPLASH_SCREEN:
                return "0.1";
            case START:
                return "0.2";
            case SPLASH_SCREEN_ETERNAL_SLEEP:
                return "0.3";
            case SPLASH_SCREEN_NONCRITICAL_ERROR:
                return "0.4";
            case PREPARING_FOR_PHOTO:
                return "1.1";
            case PHOTO_TIMER:
                return "1.2";
            case PHOTO_CONFIRMATION:
                return "1.3";
            case PAY:
                return "2.1";
            case NOT_ENOUGH_MONEY:
                return "2.1.1";
            case MORE_MONEY_THAN_NEED:
                return "2.1.2";
            case PAYMENT_CONFIRMED:
                return "2.2";
            case ALCOTEST:
                return "3.1";
            case DRUNKENESS_NOT_RECOGNIZED:
                return "3.1.1";
            case ALCOTEST_INACTION:
                return "3.1.2";
            case FINAL_PHOTO:
                return "4.1";
            case PHOTO_PRINT:
                return "4.2";
            case CRITICAL_ERROR:
                return "5.1";
            case NON_CRITICAL_ERROR:
                return "5.2";
        }

        return "";
    }

    QPoint rotatePoint(float cx, float cy, float angle, const QPoint& p)
    {
        float a = (angle * M_PI) / 180;
        float s = sin(a);
        float c = cos(a);

        int x = p.x();
        int y = p.y();

        // translate point back to origin:
        x -= cx;
        y -= cy;

        // rotate point
        float xnew = x * c - y * s;
        float ynew = x * s + y * c;

        // translate point back:
        x = xnew + cx;
        y = ynew + cy;

        return QPoint(x, y);
    }

    QPoint movePointInRect(const QRect& rect, const QPoint& point, const QPoint& center, float radius)
    {
        QPoint p(point);
        QRect pRect(p.x() - radius, p.y() - radius, 2 * radius, 2 * radius);

        while (!rect.contains(pRect)) {
            if (p.x() + radius > rect.right()) {
                p = rotatePoint(center.x(), center.y(), -5, p);
            }

            pRect = QRect(p.x() - radius, p.y() - radius, 2 * radius, 2 * radius);
        }

        return p;
    }
}
