#include "volume.h"
#include "inlines.h"

#include <cmath>
#include <QMenu>
#include <QTimer>
#include <QPainter>

static constexpr int CHANNELS = 2;

Volume::Volume(QWidget *parent)
    : Visual(parent)
{
    setWindowTitle(tr("Volume Widget"));
    setMinimumSize(2 * 300 - 30, 105);

    m_timer = new QTimer(this);
    m_timer->setInterval(40);
    connect(m_timer, SIGNAL(timeout()), SLOT(updateVisual()));

    m_screenAction = new QAction(tr("Fullscreen"), this);
    m_screenAction->setCheckable(true);
    connect(m_screenAction, SIGNAL(triggered(bool)), this, SLOT(setFullScreen(bool)));
}

Volume::~Volume()
{
    delete[] m_visualData;
    delete[] m_xscale;
}

void Volume::start()
{
    if(isVisible())
    {
        m_timer->start();
    }
}

void Volume::stop()
{
    m_timer->stop();
}

void Volume::updateVisual()
{
    if(takeData(m_left, m_right))
    {
        process(m_left, m_right);
        update();
    }
}

void Volume::setFullScreen(bool yes)
{
    if(yes)
    {
        setWindowState(windowState() | Qt::WindowFullScreen);
    }
    else
    {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}

void Volume::hideEvent(QHideEvent *)
{
    m_timer->stop();
}

void Volume::showEvent(QShowEvent *)
{
    m_timer->start();
}

void Volume::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.fillRect(rect(), Qt::black);

    QLinearGradient line(0, 0, width(), 0);
    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));
    painter.fillRect(0, 0, width(), height(), line);
    painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 88));

    line.setColorAt(0.0f, QColor(0, 0xff, 0));
    line.setColorAt(0.65f, QColor(0xff, 0xff, 0));
    line.setColorAt(1.0f, QColor(0xff, 0, 0));

    if(m_visualData)
    {
        const int wid = std::ceil(m_rows / 2);
        painter.fillRect(0, 0, m_visualData[0] * m_cols / m_rows, wid, line);
        painter.fillRect(0, wid, m_visualData[1] * m_cols / m_rows, wid, line);
    }

    painter.setPen(Qt::white);
    painter.drawText(10, height() / 4, "L");
    painter.drawText(10, height() * 3 / 4, "R");
}

void Volume::contextMenuEvent(QContextMenuEvent *)
{
    QMenu menu(this);
    menu.addAction(m_screenAction);
    menu.exec(QCursor::pos());
}

void Volume::process(float *left, float *right)
{
    const int rows = height();
    const int cols = width();

    if(m_rows != rows || m_cols != cols)
    {
        m_rows = rows;
        m_cols = cols;

        delete[] m_visualData;
        delete[] m_xscale;

        m_visualData = new int[CHANNELS]{0};
        m_xscale = new int[CHANNELS]{0};

        for(int i = 0; i < CHANNELS; ++i)
        {
            m_xscale[i] = std::pow(255.0, float(i) / m_cols);
        }
    }

    short destl[256], destr[256];
    calc_freq(destl, left);
    calc_freq(destr, right);

    short yl = 0, yr = 0;
    int i = 0, magnitudel = 0, magnituder = 0;
    const double yscale = (double)1.25 * m_rows / std::log(256);

    if(m_xscale[i] == m_xscale[i + 1])
    {
        yl = (i >= 256 ? 0 : (destl[i] >> 7)); //128
        yr = (i >= 256 ? 0 : (destr[i] >> 7)); //128
    }

    for(int k = m_xscale[i]; k < m_xscale[i + 1]; ++k)
    {
        yl = (k >= 256 ? 0 : qMax(short(destl[k] >> 7), yl));
        yr = (k >= 256 ? 0 : qMax(short(destr[k] >> 7), yr));
    }

    if(yl > 0)
    {
        magnitudel = qBound(0, int(std::log(yl) * yscale), m_rows);
    }

    if(yr > 0)
    {
        magnituder = qBound(0, int(std::log(yr) * yscale), m_rows);
    }

    m_visualData[0] -= m_analyzerSize * m_rows / 15;
    m_visualData[0] = magnitudel > m_visualData[0] ? magnitudel : m_visualData[0];

    m_visualData[1] -= m_analyzerSize * m_rows / 15;
    m_visualData[1] = magnituder > m_visualData[1] ? magnituder : m_visualData[1];
}
