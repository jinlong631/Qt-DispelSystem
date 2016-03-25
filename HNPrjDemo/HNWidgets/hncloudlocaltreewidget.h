#ifndef HNCLOUDLOCALTREEWIDGET_H
#define HNCLOUDLOCALTREEWIDGET_H

#include "hncloudtreeview.h"
#include "hncloudlocalmodel.h"

namespace Ui {
class HNCloudLocalTreeWidget;
}

class HNCloudLocalTreeWidget : public HNCloudTreeView
{
    Q_OBJECT

public:
    explicit HNCloudLocalTreeWidget(QWidget *parent = 0);
    ~HNCloudLocalTreeWidget();

    QString currentUploadingFile();
public slots:
private slots:
    void currentRowChanged();
    void printFile();
    void uploadFile();

private:
    QString m_uploadFileName;
    QString m_localPath;
    Ui::HNCloudLocalTreeWidget *ui;
    HNCloudLocalModel* m_model;
};

#endif // HNCLOUDLOCALTREEWIDGET_H
