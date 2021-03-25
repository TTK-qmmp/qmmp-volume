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

QDialog *VisualVolumeFactory::createConfigDialog(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

void VisualVolumeFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About Volume Visual Plugin"),
                        tr("Qmmp Volume Visual Plugin")+"\n"+
                        tr("This plugin adds Volume visualization")+"\n"+
                        tr("Written by: Greedysky <greedysky@163.com>"));
}

QString VisualVolumeFactory::translation() const
{
    return QString();
}
