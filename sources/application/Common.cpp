#include "Common.h"

namespace Common 
{
    QImage  cvMatToQImage(const cv::Mat &inMat)
    {
        switch (inMat.type())
        {
            // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_ARGB32);

            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image(inMat.data,
                inMat.cols, inMat.rows,
                static_cast<int>(inMat.step),
                QImage::Format_RGB888);

            return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:
        {
            QImage image((const unsigned char*)inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Grayscale8);

            return image;
        }

        default:
            qWarning() << "cv::Mat image type not handled in switch:" << inMat.type();
            break;
        }

        return QImage();
    }

    QPixmap cvMatToQPixmap(const cv::Mat &inMat)
    {
        return QPixmap::fromImage(cvMatToQImage(inMat));
    }

    // cloneData false if inPixmap exists for the lifetime of the resulting cv::Mat
    cv::Mat qImageToCvMat(const QImage &inImage, bool inCloneImageData)
    {
        switch (inImage.format())
        {
            // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
            cv::Mat  mat(inImage.height(), inImage.width(),
                CV_8UC4,
                const_cast<uchar*>(inImage.bits()),
                static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB32:
        {
            if (!inCloneImageData)
            {
                qWarning() << "conversion requires cloning so we don't modify the original QImage data";
            }

            cv::Mat  mat(inImage.height(), inImage.width(),
                CV_8UC4,
                const_cast<uchar*>(inImage.bits()),
                static_cast<size_t>(inImage.bytesPerLine())
            );

            cv::Mat  matNoAlpha;

            cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

            return matNoAlpha;
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB888:
        {
            if (!inCloneImageData)
            {
                qWarning() << "conversion requires cloning so we don't modify the original QImage data";
            }

            QImage   swapped = inImage.rgbSwapped();

            return cv::Mat(swapped.height(), swapped.width(),
                CV_8UC3,
                const_cast<uchar*>(swapped.bits()),
                static_cast<size_t>(swapped.bytesPerLine())
            ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        case QImage::Format::Format_Grayscale8:
        {
            cv::Mat  mat(inImage.height(), inImage.width(),
                CV_8UC1,
                const_cast<uchar*>(inImage.bits()),
                static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
            qWarning() << "QImage format not handled in switch:" << inImage.format();
            break;
        }

        return cv::Mat();
    }

    // cloneData false if inPixmap exists for the lifetime of the resulting cv::Mat
    cv::Mat qPixmapToCvMat(const QPixmap &inPixmap, bool cloneData)
    {
        return qImageToCvMat(inPixmap.toImage(), cloneData);
    }
};
