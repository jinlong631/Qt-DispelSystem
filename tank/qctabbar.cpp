#include "qctabbar.h"
#include "QStylePainter"
#include "QStyleOptionTabV3"
#include "QDebug"
#include "QPixmap"
#include "QApplication"
#include "QToolButton"
#include "qtankpublic.h"

QCTabBar::QCTabBar(QWidget *parent) :
    QTabBar(parent)
{
    setFocusPolicy(Qt::NoFocus);
}

void QCTabBar::tabPixmap(int index, QImage& icon, QImage& iconSel)
{
    if(index < 0 || index + 1 > count() || index + 1 > iconList.size())
        return;

    icon = QImage(iconList[index][BTN_NORMAL]);
    iconSel = QImage(iconList[index][BTN_PRESS]);

    return ;
}

void QCTabBar::setTabPixmap(int index, const QString &icon, const QString& iconSel)
{
    if(index < 0 || index + 1 > count())
        return;

    TBtnIconTable table;
    table[BTN_NORMAL] = icon;
    table[BTN_PRESS] = iconSel;

    iconList.insert(index, table);
}

void QCTabBar::paintEvent(QPaintEvent * e)
{
    QStylePainter p(this);

    for (int index = 0; index < count(); index++)
    {
        QStyleOptionTabV3 opt;
        initStyleOption(&opt, index);
        opt.shape = verticalTabs() ? QTabBar::RoundedWest : QTabBar::RoundedNorth;

        QRect tabRectValue = tabRect(index);

        if( iconList.size() > index )
        {
            int sel = currentIndex() == index ? BTN_PRESS : BTN_NORMAL;
            p.drawItemPixmap(tabRectValue, Qt::AlignCenter, QIcon(iconList[index][sel]).pixmap(rect().size(), QIcon::Normal, QIcon::On));
            //失真不明显，使用以下方法
            //QImage image(iconList[index][sel]);
            //p.drawItemPixmap(tabRectValue, Qt::AlignLeft |Qt::AlignTop, QPixmap::fromImage(image.scaled(tabRectValue.size(), Qt::KeepAspectRatio)));
        }

        opt.palette.setColor(QPalette::Normal, QPalette::Text, QColor(255, 132, 0));
        opt.palette.setCurrentColorGroup(QPalette::Active);
        opt.state |= QStyle::State_Sunken;

        //-rect.height()/20 上移
        verticalTabs() ? tabRectValue.adjust(0, 0, 0, 0) : tabRectValue.adjust(0, 0, 0, 0);

        int flags = verticalTabs() ? Qt::AlignHCenter|Qt::AlignBottom : Qt::AlignCenter;

        //pline() << objectName() << rect;
        if(index == currentIndex())
            p.drawItemText(tabRectValue, flags, opt.palette, true, opt.text, QPalette::Text);
        else
            p.drawItemText(tabRectValue, flags, opt.palette, true, opt.text, QPalette::BrightText);
    }
}

bool QCTabBar::verticalTabs()
{
    return shape() == QTabBar::RoundedWest
            || shape() == QTabBar::RoundedEast
            || shape() == QTabBar::TriangularWest
            || shape() == QTabBar::TriangularEast;
}
