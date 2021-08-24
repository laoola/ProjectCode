#ifndef WINDOWSTYLE_H
#define WINDOWSTYLE_H

#include <QObject>
#include <QMouseEvent>

class CursorStyle : public QObject
{
public:
    enum ShapeType
    {
        ShapeType_Origin,
        ShapeType_Left,
        ShapeType_Right,
        ShapeType_Top,
        ShapeType_Bottom,
        ShapeType_Topleft,
        ShapeType_Bottomleft,
        ShapeType_Topright,
        ShapeType_Bottomright,
    };

public:
    explicit CursorStyle(QObject *parent = 0);
    ~CursorStyle();

    /**
     * @brief 更新鼠标显示样式
     * @param 鼠标得全局坐标
     * @param 窗口
     */
    void updateCursorStyle(const QPoint &point, QWidget *widget);
    /**
     * @brief 获得鼠标当前样式
     * @return 返回鼠标样式
     */
    ShapeType CursorType(){return m_type;}

private:
    /**
     * @brief 计算鼠标的位置，更新m_type
     * @param 鼠标得全局坐标
     * @param 窗口的区域
     */
    void updateCursorPos(const QPoint &point, const QRect &rect);

private:
    ShapeType m_type;   //鼠标当前的样式
};

class WindowStyle :public QObject
{
    Q_OBJECT
    public:
        explicit WindowStyle(QObject *parent = 0);
        ~WindowStyle();
        /**
         * @brief 窗口注册
         * @param 注册窗口
         */
        void activateOn(QWidget *widget);
        /**
         * @brief 设置是否支持窗口移动
         * @param true or false
         */
        void setMovable(bool isMovable){m_isMovable = isMovable;}
        /**
         * @brief 设置是否支持窗口拉伸
         * @param true or false
         */
        void setResizable(bool isResizable){m_isResizable = isResizable;}

        void setCur(){m_isChangeCur = false;}

    private:
        bool eventFilter(QObject *watched, QEvent *event);
        void mousePressHandle(QMouseEvent *event);
        void mouseReleaseHandle(QMouseEvent *event);
        void mouseMoveHandle(QMouseEvent *event);
        void mouseDBClickedHandle(QMouseEvent *event);
        void mouseHoverHandle(QHoverEvent *event);

        /**
         * @brief 拉伸窗口
         * @param 鼠标的全局坐标
         */
        void resizeWidget(const QPoint &point);

    private:
        QPoint   m_distance;            //记录鼠标点击位置与m_wnd左上角距离
        QWidget *m_widget;              //鼠标可以操作的窗口，一般是标题栏
        QWidget *m_wnd;                 //进行拉伸，移动的窗口，一般为整个窗口

        bool     m_isLeftPressed;       //鼠标左键是否按下
        bool     m_isCursorOnWidget;    //鼠标按下时是否在m_widget上
        bool     m_isMovable;           //窗口是否可移动
        bool     m_isResizable;         //窗口是否可拉伸
        bool     m_isChangeCur;

        CursorStyle m_cursorStyle;
};

#endif // WINDOWSTYLE_H
