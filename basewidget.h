#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <qcustomplot.h>

class BaseWidget : public QWidget {
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    ~BaseWidget() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initPlot();
    void updatePlot();
    void onMousePress(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);
    void restoreAutoRange();                    //恢复自动范围调整

private:
    QCustomPlot *m_customPlot = nullptr;
    QTimer *m_dataTimer = nullptr;          // 声明实时更新定时器指针
    QTimer *m_dragTimer = nullptr;          // 声明倒计时定时器指针
    double m_timeData = 0.0;                // 用于跟踪时间
    bool m_isPaused;                        // 暂停状态标志
    bool m_isDragging;                      // 拖动状态标志
    bool m_autoRangeEnabled;                // 自动范围调整标志
};
#endif // BASEWIDGET_H
