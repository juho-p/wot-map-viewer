#include "util.h"

#include "simple-dds-image-reader/ddsreader.hpp"

QImage readDDS(QByteArray& data) {
    std::vector<uint8_t> inputdata(data.begin(), data.end());
    Image ddsimage = read_dds(inputdata);
    QImage img(ddsimage.width, ddsimage.height, QImage::Format_RGB32);
    memcpy(img.bits(), &ddsimage.data[0], ddsimage.data.size());

    return img;
}
