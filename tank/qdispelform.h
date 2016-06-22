#ifndef QDISPELFORM_H
#define QDISPELFORM_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include "qtankdefine.h"
#include "qtankpublic.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QItemSelection>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "qcustomplot.h"
#include "qrunplot.h"
#include "qopenlibrary.h"
#include "qmethodwidget.h"
#include "qmethodform.h"
#include "qcpdocumentobject.h"
#include "qreportviewdialog.h"
#include "qtankpeerport.h"
#include "qdebugwidget.h"
#include "qtankserialport.h"
#include "qtankexception.h"

namespace Ui {
class QDispelForm;
}

class QDispelForm : public QWidget
{
    Q_OBJECT

public:
    explicit QDispelForm(QWidget *parent = 0);
    ~QDispelForm();

    void initAll();
    void initLanguage();

signals:
    void signalUpdateLabReport();
    void signalLockPage(bool block);

public slots:
    void showDebugWindow(int);
private slots:
    void prepareRunning(QString, int);
    void prepareExtractRunning(QString, int);

    void timeNewData();
    void timeNewData2();

    void on_btn_open_clicked();

    void on_btn_play_clicked();

    void startHeating();
    void pauseHeating();
    void continueHeating();
    void stopHeating();
    void saveLabReport();
    void on_btn_stop_clicked();

    void on_btn_trans_clicked();

    void on_btn_open_2_clicked();

    void on_btn_play_2_clicked();

    void startHeatingExtract();
    void pauseHeatingExtract();

    void on_btn_stop_2_clicked();

    void stopHeatingExtract();

    void on_btn_trans_2_clicked();

    void on_btnStir_clicked();

    void slotException(quint16);

    void slotStat(quint16,quint16,quint8);
private:
    Ui::QDispelForm *ui;

    quint8 m_currentStage;
    quint16 m_totalStageRamp;
    quint16 m_totalStageRamp2;
    double m_initPointKey;
    double m_lastPointKey;
    double m_initPointKey2;
    double m_lastPointKey2;

    QTankException* excp;

    QString serialNo;
    QReportViewDialog* m_dlg;

    QUserLibraryDialog * userlibdialog;
    enum {
        eStop,
        ePause,
        ePlay,
    };
    int bRunning;

    QTimer* timer;
    QTimer* timer2;

    quint32 m_pauseTime;
    quint32 m_pauseTime2;

    QMethodForm* methodForm;
    QMethodForm* methodForm2;
    QCPDocumentObject* m_plotObjectHandler;
    QTextEdit* m_text;
    QTankPeerPort* com0;
    QTankSerialPort* s2;
    QDebugWidget* m_debug;

    quint16 m_curRamp;
    quint16 m_curRamp2;

    // QObject interface
public:
    bool eventFilter(QObject *, QEvent *);
};

#endif // QDISPELFORM_H
