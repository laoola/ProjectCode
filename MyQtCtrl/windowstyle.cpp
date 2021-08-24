#include "windowstyle.h"

#include <QWidget>
#include <QDebug>

#define BORDER 10

CursorStyle::CursorStyle(QObject *parent)
    : QObject(parent)
    , m_type(ShapeType_Origin)
{

}

CursorStyle::~CursorStyle()
{

}

void CursorStyle::updateCursorPos(const QPoint &point, const QRect &rect)
{
    int cursorX = point.x();
    int cursorY = point.y();

    int widgetX = rect.x();
    int widgetY = rect.y();

    int widgetW = rect.width();
    int widgetH = rect.height();

    bool isLeftEdge = cursorX >= widgetX && cursorX <= widgetX + BORDER;
    bool isRightEdge = cursorX <= widgetX + widgetW && cursorX >= widgetX + widgetW - BORDER;
    bool isTopEdge = cursorY >= widgetY && cursorY <= widgetY + BORDER;
    bool isBottomEdge = cursorY <= widgetY + widgetH && cursorY >= widgetY + widgetH - BORDER;
    bool isTopLeftEdge = isTopEdge && isLeftEdge;
    bool isBottomLeftEdge = isBottomEdge && isLeftEdge;
    bool isTopRightEdge = isTopEdge && isRightEdge;
    bool isBottomRightEdge = isBottomEdge && isRightEdge;

    if (isTopLeftEdge)
    {
        m_type = ShapeType_Topleft;
    }
    else if (isBottomLeftEdge)
    {
        m_type = ShapeType_Bottomleft;
    }
    else if (isTopRightEdge)
    {
        m_type = ShapeType_Topright;
    }
    else if (isBottomRightEdge)
    {
        m_type = ShapeType_Bottomright;
    }
    else if (isLeftEdge)
    {
        m_type = ShapeType_Left;
    }
    else if (isRightEdge)
    {
        m_type = ShapeType_Right;
    }
    else if (isTopEdge)
    {
        m_type = ShapeType_Top;
    }
    else if (isBottomEdge)
    {
        m_type = ShapeType_Bottom;
    }
    else
    {
        m_type = ShapeType_Origin;
    }
}

void CursorStyle::updateCursorStyle(const QPoint &point, QWidget *widget)
{
    updateCursorPos(point, widget->geometry());

    if (m_type == ShapeType_Topleft || m_type == ShapeType_Bottomright)
    {
        widget->setCursor(Qt::SizeFDiagCursor);
    }
    else if (m_type == ShapeType_Topright || m_type == ShapeType_Bottomleft)
    {
        widget->setCursor(Qt::SizeBDiagCursor);
    }
    else if (m_type == ShapeType_Left || m_type == ShapeType_Right)
    {
        widget->setCursor(Qt::SizeHorCursor);
    }
    else if (m_type == ShapeType_Top || m_type == ShapeType_Bottom)
    {
        widget->setCursor(Qt::SizeVerCursor);
    }
    else
    {
         widget->unsetCursor();
    }
}


WindowStyle::WindowStyle(QObject *parent)
    : QObject(parent)
    , m_widget(NULL)
    , m_wnd(NULL)
    , m_isLeftPressed(false)
    , m_isCursorOnWidget(false)
    , m_isMovable(true)
    , m_isResizable(false)
    , m_isChangeCur(true)
{
}

WindowStyle::~WindowStyle()
{

}

void WindowStyle::activateOn(QWidget *widget)
{
    if (widget == NULL)
    {
        return;
    }

    m_widget = widget;
    m_wnd = widget->window();

    m_wnd->setWindowFlags(Qt::FramelessWindowHint | m_wnd->windowFlags());
    m_wnd->setAttribute(Qt::WA_Hover);
    m_wnd->installEventFilter(this);
}

bool WindowStyle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_wnd)
    {
        QEvent::Type type = event->type();

        switch(type)
        {
        case QEvent::MouseButtonPress:
            mousePressHandle(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseHandle(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            mouseMoveHandle(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonDblClick:
            //mouseDBClickedHandle(dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::HoverMove:
            mouseHoverHandle(dynamic_cast<QHoverEvent*>(event));
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}

void WindowStyle::mousePressHandle(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isLeftPressed = true;

        //若m_wnd等于m_widget使用全局坐标，否则使用相对坐标
        QPoint pos = m_widget == m_wnd ? event->globalPos() : event->pos();
        if (m_widget->geometry().contains(pos))
        {
            m_isCursorOnWidget = true;
            m_distance = event->globalPos() - m_wnd->mapToGlobal(QPoint(0,0));
        }
    }
}

void WindowStyle::mouseReleaseHandle(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isLeftPressed = false;
        m_isCursorOnWidget = false;
    }
}

void WindowStyle::mouseMoveHandle(QMouseEvent *event)
{
    if (m_isLeftPressed)
    {
        if (m_cursorStyle.CursorType() != CursorStyle::ShapeType_Origin)
        {
            resizeWidget(event->globalPos());
            return;
        }

        if (m_isMovable &&  m_isCursorOnWidget)
        {
            m_wnd->move(event->globalPos() - m_distance);
        }
    }
}

void WindowStyle::mouseDBClickedHandle(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        m_widget->geometry().contains(event->pos()))
    {
        if (m_wnd->isMaximized())
        {
            m_wnd->showNormal();
        }
        else
        {
            m_wnd->showMaximized();
        }
    }
}

void WindowStyle::mouseHoverHandle(QHoverEvent *event)
{
    if (m_isResizable && !m_isLeftPressed && m_isChangeCur)
    {
        m_cursorStyle.updateCursorStyle(m_wnd->mapToGlobal(event->pos()), m_wnd);
    }
}

void WindowStyle::resizeWidget(const QPoint &point)
{
    QRect originRect = m_wnd->geometry();

    //原始窗口四个点得坐标
    int left = originRect.left();
    int right = originRect.right();
    int top = originRect.top();
    int bottom = originRect.bottom();

    //窗口最小宽度和高度
    int minWidth = m_wnd->minimumWidth();
    int minHeight = m_wnd->minimumHeight();

    //根据鼠标得位置更新四个点得坐标
    switch (m_cursorStyle.CursorType())
    {
    case CursorStyle::ShapeType_Topleft:
        top = point.y();
        left = point.x();
        break;
    case CursorStyle::ShapeType_Bottomleft:
        bottom = point.y();
        left = point.x();
        break;
    case CursorStyle::ShapeType_Topright:
        top = point.y();
        right = point.x();
        break;
    case CursorStyle::ShapeType_Bottomright:
        bottom = point.y();
        right = point.x();
        break;
    case CursorStyle::ShapeType_Left:
        left = point.x();
        break;
    case CursorStyle::ShapeType_Right:
        right = point.x();
        break;
    case CursorStyle::ShapeType_Top:
        top = point.y();
        break;
    case CursorStyle::ShapeType_Bottom:
        bottom = point.y();
        break;
    default:
        break;
    }

    //窗口得拉伸后得区域
    QRect newRect(QPoint(left, top), QPoint(right, bottom));

    //防止窗口到达最小宽度和高度之后，开始平移
    if ( newRect.isValid() )
    {
      if ( minWidth > newRect.width() )
      {
        if( left != originRect.left() )
          newRect.setLeft( originRect.left() );
        else
          newRect.setRight( originRect.right() );
      }
      if ( minHeight > newRect.height() )
      {
        if ( top != originRect.top() )
          newRect.setTop( originRect.top() );
        else
          newRect.setBottom( originRect.bottom() );
      }

      m_wnd->setGeometry(newRect);
    }
}
