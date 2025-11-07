#include "visualvolumefactory.h"
#include "volume.h"

#include <QMessageBox>

VisualProperties VisualVolumeFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Volume Plugin");
    properties.shortName = "volume";
    properties.hasAbout = true;
    return properties;
}

Visual *VisualVolumeFactory::create(QWidget *parent)
{
    return new Volume(parent);
}

#if (QMMP_VERSION_INT < 0x10700) || (0x20000 <= QMMP_VERSION_INT && QMMP_VERSION_INT < 0x20200)
QDialog *VisualVolumeFactory::createConfigDialog(QWidget *parent)
#else
QDialog *VisualVolumeFactory::createSettings(QWidget *parent)
#endif
{
    Q_UNUSED(parent);
    return nullptr;
}

void VisualVolumeFactory::showAbout(QWidget *parent)
{
    QMessageBox::about(parent, tr("About Volume Visual Plugin"),
                       tr("Qmmp Volume Visual Plugin") + "\n" +
                       tr("This plugin adds Volume visualization") + "\n" +
                       tr("Written by: Greedysky <greedysky@163.com>"));
}

QString VisualVolumeFactory::translation() const
{
    return QString();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(volume, VisualVolumeFactory)
#endif
