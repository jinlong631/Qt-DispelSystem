#ifndef QCLOUDTREEWIDGET_H
#define QCLOUDTREEWIDGET_H

#include "qcloudtreeview.h"
#include "qcloudmodel.h"

namespace Ui {
class QCloudTreeWidget;
}

class QCloudTreeWidget : public QCloudTreeView
{
    Q_OBJECT

public:
    explicit QCloudTreeWidget(QWidget *parent = 0);
    ~QCloudTreeWidget();

    QString currentDownloadingFile();
    QString currentDownloadingFilelocalName();

private slots:
    void currentRowChanged();
    void downFile();
    void delFile();

private:
    Ui::QCloudTreeWidget *ui;
    QCloudModel* model;
    QString m_tmpfile, m_localfile;
};

#endif // QCLOUDTREEWIDGET_H
