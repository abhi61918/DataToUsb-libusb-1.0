#include "usbexample.h"
#include<cassert>
#include <QStorageInfo>
#include <QDebug>
#include <QPdfWriter>
#include<QtPrintSupport/QPrinter>
#include<QPainter>
#include<QFile>
#include <QPrintDialog>
#include<QDialog>
#include <QImage>


UsbExample::UsbExample(QObject *parent) :
    QObject(parent)
{
    this->setupDevice();

    QByteArray send, recv;

    send.append((char)0xAB);

#ifdef interface
#undef interface
#endif

    rc = libusb_init(&context);
    assert(rc == 0);

    count = libusb_get_device_list(context, &list);
    assert(count > 0);

    for (size_t idx = 0; idx < count; ++idx) {
        libusb_device *device = list[idx];
        libusb_device_descriptor desc = {0};

        rc = libusb_get_device_descriptor(device, &desc);
        assert(rc == 0);

        qDebug("Vendor:Device = %04x:%04x\n", desc.idVendor, desc.idProduct);

        //QStorageInfo storage = QStorageInfo::root();

        //       qDebug() << storage.rootPath();
        //              if (storage.isReadOnly())
        //                  qDebug() << "isReadOnly:" << storage.isReadOnly();

        //qDebug() << "name:" << storage.name();


        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("writable.pdf");
//        printer.setPageSize(QPrinter::A4);
//        printer.setOrientation(QPrinter::Portrait);
//        printer.setPageMargins (15,15,15,15,QPrinter::Millimeter);
//        printer.setFullPage(false);

        QPainter painter(&printer);
        //            if (! painter.begin(&printer)) { // failed to open file
        //                qWarning("failed to open file, is it writable?");
        //                return 0;
        //            }
        painter.drawText(00, 80, "Testing the automaization of this printer");

        painter.drawText(00, 100, "Test 2");
        QString str="Pdf is ready============== to print";

        painter.drawText(40,80,str);

        qDebug()<<"printer     --------------------";
        painter.end();





        QString location;

        QString path1= "/Report/1.pdf";
        QString locationoffolder="/Report";

        foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
            if (storage.isValid() && storage.isReady()) {
                if (!storage.isReadOnly()) {

                    qDebug() << "path:" << storage.rootPath();
                    location = storage.rootPath();
                    QString srcPath = "writable.pdf";
                    QString destPath = location+path1;
                    QString folderdir = location+locationoffolder;

                    QDir dir(folderdir);
                    if(!dir.exists()){
                        dir.mkpath(".");
                    }

                    qDebug() << "path:" <<destPath;
                    if (QFile::exists(destPath)) QFile::remove(destPath);
                    QFile::copy(srcPath,destPath);
                    qDebug("copied");

                }else{
                    qDebug()<<"error in storage.isReadOnly()";
                }
            }else{
                qDebug()<<"error in storage.isValid() && storage.isReady() && (!(storage.name()).isEmpty()";
            }
        }
    }
    //    if (this->openDevice())
    //    {
    //        qDebug("Device open!");
    //        this->write(&send);
    //        this->read(&recv);
    //    }
}

UsbExample::~UsbExample()
{
    delete mUsbDev;
}

void UsbExample::setupDevice()
{
    mUsbDev = new QUsbDevice();
    mUsbDev->setDebug(true);

    // Linux
    mFilter.pid = 0x3748;
    mFilter.vid = 0x0483;

    mConfig.alternate = 0;
    mConfig.config = 0;
    mConfig.interface = 1;
    mConfig.readEp = 0x81;
    mConfig.writeEp = 0x02;
}

//bool UsbExample::openDevice()
//{
//    qDebug("Opening");

//    QtUsb::DeviceStatus ds;
//    ds = mUsbManager.openDevice(mUsbDev, mFilter, mConfig);

//    if (ds == QtUsb::deviceOK) {
//        // Device is open
//        return true;
//    }
//    return false;
//}

//bool UsbExample::closeDevice()
//{
//    qDebug("Closing");
//    mUsbManager.closeDevice(mUsbDev);
//    return false;
//}

//void UsbExample::read(QByteArray *buf)
//{
//    mUsbDev->read(buf, 1);
//}

//void UsbExample::write(QByteArray *buf)
//{
//    mUsbDev->write(buf, buf->size());
//}
