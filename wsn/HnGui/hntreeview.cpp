#include "hntreeview.h"
#include "ui_hntreeview.h"
#include <QFile>

HNTreeView::HNTreeView(QWidget *parent) :
    QTreeView(parent),
    ui(new Ui::HNTreeView)
{
    ui->setupUi(this);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    header()->setResizeMode(QHeaderView::ResizeToContents);

#ifdef __MIPS_LINUX__
    setFocusPolicy(Qt::NoFocus);
#endif

    QFont fnt(font());
    fnt.setPointSize(16);
    setFont(fnt);
}

HNTreeView::~HNTreeView()
{
    delete ui;
}


void HNTreeView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    return QTreeView::drawRow(painter, options, index);
}

void HNTreeView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    return QTreeView::drawBranches(painter, rect, index);
}
