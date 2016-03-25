#ifndef HNTABLEVIEW_H
#define HNTABLEVIEW_H

#include <QTableView>
#include "hnheaderview.h"

namespace Ui {
class HNTableView;
}

class HNTableView : public QTableView
{
    Q_OBJECT

public:
    explicit HNTableView(QWidget *parent = 0);
    ~HNTableView();

    HNHeaderView* chorizontalHeader() { return m_header; }
    HNHeaderView* cverticalHeader() { return m_vheader; }

    void setObjectName(const QString &name);
private:
    Ui::HNTableView *ui;

    HNHeaderView* m_vheader;
    HNHeaderView* m_header;
};

#endif // HNTABLEVIEW_H
