#pragma once

#include <vector>
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QDebug>

#include <opencv2/opencv.hpp>



/*

   Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.
   Andy Maloney <asmaloney@gmail.com>
   https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap

    Endianness
    ---
    Although not totally clear from the docs, some of QImage's formats we use here are
    endian-dependent. For example:
    Little Endian
    QImage::Format_ARGB32 the bytes are ordered:    B G R A
    QImage::Format_RGB32 the bytes are ordered:     B G R (255)
    QImage::Format_RGB888 the bytes are ordered:    R G B
    Big Endian
    QImage::Format_ARGB32 the bytes are ordered:    A R G B
    QImage::Format_RGB32 the bytes are ordered:     (255) R G B
    QImage::Format_RGB888 the bytes are ordered:    R G B
    Notice that Format_RGB888 is the same regardless of endianness. Since OpenCV
    expects (B G R) we need to swap the channels for this format.
    This is why some conversions here swap red and blue and others do not.
    This code assumes little endian. It would be possible to add conversions for
    big endian machines though. If you are using such a machine, please feel free
to submit a pull request on the GitHub page.
*/
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#error Some of QImage's formats are endian-dependant. This file assumes little endian. See comment at top of header.
#endif

namespace Common 
{

    /**
        Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.
        Andy Maloney <asmaloney@gmail.com>
        https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
    **/
    QImage  cvMatToQImage(const cv::Mat &inMat);

    QPixmap cvMatToQPixmap(const cv::Mat &inMat);

    // cloneData false if inPixmap exists for the lifetime of the resulting cv::Mat
    cv::Mat qImageToCvMat(const QImage &inImage, bool inCloneImageData = true);

    // cloneData false if inPixmap exists for the lifetime of the resulting cv::Mat
    cv::Mat qPixmapToCvMat(const QPixmap &inPixmap, bool cloneData = true);

    static QMap<QImage::Format, QString>   sImageFormatEnumMap = {
                                                                    { QImage::Format::Format_Invalid, "Format_Invalid" },
                                                                    { QImage::Format::Format_Mono, "Format_Mono" },
                                                                    { QImage::Format::Format_MonoLSB, "Format_MonoLSB" },
                                                                    { QImage::Format::Format_Indexed8, "Format_Indexed8" },
                                                                    { QImage::Format::Format_RGB32, "Format_RGB32" },
                                                                    { QImage::Format::Format_ARGB32, "Format_ARGB32" },
                                                                    { QImage::Format::Format_ARGB32_Premultiplied, "Format_ARGB32_Premultiplied" },
                                                                    { QImage::Format::Format_RGB16, "Format_RGB16" },
                                                                    { QImage::Format::Format_ARGB8565_Premultiplied, "Format_ARGB8565_Premultiplied" },
                                                                    { QImage::Format::Format_RGB666, "Format_RGB666" },
                                                                    { QImage::Format::Format_ARGB6666_Premultiplied, "Format_ARGB6666_Premultiplied" },
                                                                    { QImage::Format::Format_RGB555, "Format_RGB555" },
                                                                    { QImage::Format::Format_ARGB8555_Premultiplied, "Format_ARGB8555_Premultiplied" },
                                                                    { QImage::Format::Format_RGB888, "Format_RGB888" },
                                                                    { QImage::Format::Format_RGB444, "Format_RGB444" },
                                                                    { QImage::Format::Format_ARGB4444_Premultiplied, "Format_ARGB4444_Premultiplied" },
                                                                    { QImage::Format::Format_RGBX8888, "Format_RGBX8888" },
                                                                    { QImage::Format::Format_RGBA8888, "Format_RGBA8888" },
                                                                    { QImage::Format::Format_RGBA8888_Premultiplied, "Format_RGBA8888_Premultiplied" },
                                                                    { QImage::Format::Format_BGR30, "Format_BGR30" },
                                                                    { QImage::Format::Format_A2BGR30_Premultiplied, "Format_A2BGR30_Premultiplied" },
                                                                    { QImage::Format::Format_RGB30, "Format_RGB30" },
                                                                    { QImage::Format::Format_A2RGB30_Premultiplied, "Format_A2RGB30_Premultiplied" },
                                                                    { QImage::Format::Format_Alpha8, "Format_Alpha8" },
                                                                    { QImage::Format::Format_Grayscale8, "Format_Grayscale8" }
                                                                };

    static QString sQImageFormatToStr(QImage::Format inFormat)
    {
        return sImageFormatEnumMap[inFormat];
    }

    static QString sCVTypeToStr(int inType)
    {
        QString str("CV_");

        switch (CV_MAT_DEPTH(inType))
        {
        case CV_8U:    str += "8U"; break;
        case CV_8S:    str += "8S"; break;
        case CV_16U:   str += "16U"; break;
        case CV_16S:   str += "16S"; break;
        case CV_32S:   str += "32S"; break;
        case CV_32F:   str += "32F"; break;
        case CV_64F:   str += "64F"; break;
        default:       str += "User"; break;
        }

        str += QStringLiteral("C%1").arg(QString::number(CV_MAT_CN(inType)));

        return str;
    }
};
