#include "basewidget.h"
#include <QGridLayout>

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent),
    m_isPaused(false), m_isDragging(false), m_autoRangeEnabled(true)
{
    this->setWindowTitle("Real-time Dynamic Curve Demo");
    this->resize(800, 600);
    initPlot();
}

BaseWidget::~BaseWidget() = default;

bool BaseWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_customPlot && event->type() == QEvent::Wheel) {
        auto *wheelEvent = dynamic_cast<QWheelEvent *>(event);
        // 禁用自动范围并启动倒计时
        m_autoRangeEnabled = false;
        m_dragTimer->start(5000);   // 重新计时
        return false;               // 保留默认缩放行为
    }

    return QWidget::eventFilter(watched, event);
}

void BaseWidget::initPlot() {
    m_customPlot = new QCustomPlot(this);

    // 配置 QCustomPlot
    m_customPlot->addGraph();                                           // 添加一条曲线
    m_customPlot->graph(0)->setPen(QPen(QBrush(Qt::green), 3));         // 设置曲线颜色和线宽
    m_customPlot->axisRect()->setBackground(QColor(50, 50, 50));        // 设置绘图区域的背景为灰色

    // 设置标题样式
    m_customPlot->xAxis->setLabel("Time (s)");
    m_customPlot->yAxis->setLabel("Value");
    QFont xFont = m_customPlot->xAxis->labelFont();
    xFont.setPointSize(16);
    xFont.setBold(true);
    m_customPlot->xAxis->setLabelFont(xFont);
    m_customPlot->xAxis->setLabelColor(QColor(255, 0, 0));
    QFont yFont = m_customPlot->yAxis->labelFont();
    yFont.setPointSize(16);
    yFont.setBold(true);
    m_customPlot->yAxis->setLabelFont(yFont);
    m_customPlot->yAxis->setLabelColor(QColor(0, 0, 255));

    // 设置坐标轴样式
    m_customPlot->xAxis->setTickLabelFont(QFont("Arial", 15));
    m_customPlot->yAxis->setTickLabelFont(QFont("Arial", 15));
    m_customPlot->xAxis->setRange(0, 10);                               // 初始 X 轴范围为 0-10 秒
    m_customPlot->yAxis->setRange(-1.5, 1.5);                           // 初始 Y 轴范围
    // 设置x轴主刻度线样式
    m_customPlot->xAxis->setTickPen(QPen(Qt::white, 3));                // 设置主刻度颜色和线宽
    m_customPlot->xAxis->setTickLength(12, 0);                          // 设置主刻度长度
    m_customPlot->xAxis->setSubTickPen(QPen(Qt::white, 2));             // 设置子刻度颜色和线宽
    m_customPlot->xAxis->setSubTickLength(5, 0);                        // 设置子刻度长度
    // 设置y轴主刻度线样式
    m_customPlot->yAxis->setTickPen(QPen(Qt::white, 3));                // 设置主刻度颜色和线宽
    m_customPlot->yAxis->setTickLength(12, 0);                          // 设置主刻度长度
    m_customPlot->yAxis->setSubTickPen(QPen(Qt::white, 2));             // 设置子刻度颜色和线宽
    m_customPlot->yAxis->setSubTickLength(5, 0);                        // 设置子刻度长度

    m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);   // 允许拖动和缩放
    m_customPlot->installEventFilter(this);                             // 安装事件过滤器

    connect(m_customPlot, &QCustomPlot::mousePress, this, &BaseWidget::onMousePress);
    connect(m_customPlot, &QCustomPlot::mouseRelease, this, &BaseWidget::onMouseRelease);

    auto *layout = new QGridLayout(this);
    layout->addWidget(m_customPlot);

    // 配置数据定时器
    m_timeData = 0.0;
    m_dataTimer = new QTimer(this);
    connect(m_dataTimer, &QTimer::timeout, this, &BaseWidget::updatePlot);
    m_dataTimer->start(50);   //50ms 更新一次

    // 配置拖动定时器
    m_dragTimer = new QTimer(this);
    connect(m_dragTimer, &QTimer::timeout, this, &BaseWidget::restoreAutoRange);
}

void BaseWidget::updatePlot() {
    if (!m_isPaused) {
        m_timeData += 0.05;
        double value = std::sin(m_timeData);
        m_customPlot->graph(0)->addData(m_timeData, value);
        if (m_autoRangeEnabled) {
            double rangeHalf = 5.0;
            m_customPlot->xAxis->setRange(m_timeData - rangeHalf, m_timeData + rangeHalf);
            m_customPlot->yAxis->setRange(-1.5, 1.5);
        }
        m_customPlot->replot();
    }
}

void BaseWidget::onMousePress(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        m_isDragging = true;
        m_autoRangeEnabled = false;         // 鼠标按下时禁用自动范围调整
        m_dragTimer->start(5000);           // 启动 5 秒倒计时
    }
}

void BaseWidget::onMouseRelease(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        m_isDragging = false;
         if (!m_dragTimer->isActive()) {
             m_autoRangeEnabled = true;     // 仅在倒计时未激活时恢复
         }
    }
}

void BaseWidget::restoreAutoRange() {
    m_autoRangeEnabled = true;              // 5 秒后恢复自动范围调整
    m_dragTimer->stop();                    // 停止倒计时
}

