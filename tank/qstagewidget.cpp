#include "qstagewidget.h"
#include "ui_qstagewidget.h"

QStageWidget::QStageWidget(QWidget *parent) :
    QStageView(parent),
    ui(new Ui::QStageWidget)
{
    ui->setupUi(this);
    setting = new QSettings(QApplication::organizationName(), QApplication::applicationName(), this);
    m_db = newDatabaseConn();
    m_model = new QStageModel(this, m_db);
    setModel(m_model);
    //m_mapper = new QDataWidgetMapper(this);
    //m_mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //m_mapper->setModel(m_model);
    //m_mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    //connect(this->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), m_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    setItemDelegate(new QSqlRelationalDelegate(this));
}

QStageWidget::~QStageWidget()
{
    delete ui;
}

void QStageWidget::refresh(QString filter)
{
    m_model->setFilter(filter);
    m_model->select();
    setColumnHidden(Stage_Id, true);
    setColumnHidden(Stage_MethodId, true);
    m_model->setHeaderData(Stage_Index, Qt::Horizontal, tr("Stage"));
    m_model->setHeaderData(Stage_Vessel, Qt::Horizontal, tr("Vessel"));
    m_model->setHeaderData(Stage_Timeramp, Qt::Horizontal, tr("TimeRAMP"));
    m_model->setHeaderData(Stage_Presspsi, Qt::Horizontal, tr("PressPSI"));
    m_model->setHeaderData(Stage_Tempture, Qt::Horizontal, tr("Tempture"));
    m_model->setHeaderData(Stage_Hold, Qt::Horizontal, tr("Hold"));
}

void QStageWidget::refresh(int methodid, int type)
{
    m_model->setFilter(QString("M_Id = %1").arg(methodid));
    //异步的操作，不会立即返回选择好的数据
    m_model->select();

    m_model->setHeaderData(Stage_Index, Qt::Horizontal, tr("Stage"));
    m_model->setHeaderData(Stage_Vessel, Qt::Horizontal, tr("Vessel"));
    m_model->setHeaderData(Stage_Timeramp, Qt::Horizontal, tr("TimeRAMP"));
    m_model->setHeaderData(Stage_Presspsi, Qt::Horizontal, tr("PressPSI"));
    m_model->setHeaderData(Stage_Tempture, Qt::Horizontal, tr("Tempture"));
    m_model->setHeaderData(Stage_Hold, Qt::Horizontal, tr("Hold"));

    setColumnHidden(Stage_Id, true);
    setColumnHidden(Stage_MethodId, true);
    setColumnHidden(Stage_Vessel, true);

    setColumnHidden(Stage_Timeramp, false);
    setColumnHidden(Stage_Presspsi, false);
    setColumnHidden(Stage_Tempture, false);
    setColumnHidden(Stage_Hold, false);

    switch(type)
    {
    case Type_Standard:
        setColumnHidden(Stage_Timeramp, true);
        setColumnHidden(Stage_Presspsi, true);
        break;
    case Type_Stressure:
        setColumnHidden(Stage_Timeramp, true);
        setColumnHidden(Stage_Tempture, true);
        break;
    case Type_Extract:
        setColumnHidden(Stage_Vessel, true);
        setColumnHidden(Stage_Timeramp, true);
        setColumnHidden(Stage_Presspsi, true);
        break;
    case Type_Temprature:
    default:
        break;
    }

    if(Type_Stressure == type)
    {
        m_model->setHeaderData(Stage_Presspsi, Qt::Horizontal, tr("gonglv"));
    }
    else
    {
        m_model->setHeaderData(Stage_Presspsi, Qt::Horizontal, tr("press"));
    }

}

void QStageWidget::initdb(QString dbName, bool save)
{
    setDatabaseName(m_db, QString("Method/%1").arg(dbName));
    m_model->setTable(TABLE_STAGE);
    //pline() << "curdb" << gUserName << m_lastDB << "opened" << m_db.isOpen();

    if(!save)
        return;

    m_lastDB = setting->value(QString("%1/lastDB").arg(gUserName), gUserName).toString();
    //pline() << "predb" << gUserName << m_lastDB << "opened" << m_db.isOpen();

    if(!dbName.isEmpty())
        m_lastDB = dbName;

    setting->setValue(QString("%1/lastDB").arg(gUserName), m_lastDB);
    setting->sync();
}

void QStageWidget::selectStage(int row)
{
    selectRow(row);
    setFocus(Qt::ActiveWindowFocusReason);
    //m_mapper->setCurrentIndex(row);
}

int QStageWidget::totalStageTimeRamp()
{
    quint16 ramp = 0;
    for(int row = 0; row < m_model->rowCount(); row++)
    {
        ramp += m_model->index(row, Stage_Timeramp).data().toUInt();
    }
    return ramp;
}

int QStageWidget::totalStageHold()
{
    quint16 ramp = 0;
    for(int row = 0; row < m_model->rowCount(); row++)
    {
        ramp += m_model->index(row, Stage_Hold).data().toUInt();
    }
    return ramp;
}

void QStageWidget::setRamp(qint16 ramp)
{
    //当切换到下一个阶段，设置ramp出现跳空的现象，不选中了。
    //pline() << currentIndex() << ramp;
    int row = currentIndex().row();
    m_model->setData(m_model->index(row, Stage_Timeramp), ramp);
}

void QStageWidget::setHold(qint16 hold)
{
    pline() << currentIndex() << hold;
    int row = currentIndex().row();
    m_model->setData(m_model->index(row, Stage_Hold), hold);
}

void QStageWidget::setStageRamp(qint8 stage, qint16 ramp)
{
    int row = stage;
    m_model->setData(m_model->index(row, Stage_Timeramp), ramp);
}

void QStageWidget::setStageHold(qint8 stage, qint16 hold)
{
    int row = stage;
    m_model->setData(m_model->index(row, Stage_Hold), hold);
}

void QStageWidget::stageParam(qint8 stage, qint8 &vessel, qint16 &ramp, qint16 &press, qint16 &tempture, qint16 &hold)
{
    int row = stage;
    //stage = m_model->index(row, Stage_Index).data().toInt();
    vessel = m_model->index(row, Stage_Vessel).data().toInt();
    ramp = m_model->index(row, Stage_Timeramp).data().toInt();
    press = m_model->index(row, Stage_Presspsi).data().toInt();
    tempture = m_model->index(row, Stage_Tempture).data().toInt();
    hold = m_model->index(row, Stage_Hold).data().toInt();
}

void QStageWidget::currentStageParam(qint8 &stage, qint8 &vessel, qint16 &ramp, qint16 &press, qint16 &tempture, qint16 &hold)
{
    QModelIndex index = currentIndex();
    stage = m_model->index(index.row(), Stage_Index).data().toInt();
    vessel = m_model->index(index.row(), Stage_Vessel).data().toInt();
    ramp = m_model->index(index.row(), Stage_Timeramp).data().toInt();
    press = m_model->index(index.row(), Stage_Presspsi).data().toInt();
    tempture = m_model->index(index.row(), Stage_Tempture).data().toInt();
    hold = m_model->index(index.row(), Stage_Hold).data().toInt();
}

void QStageWidget::openTransaction()
{
    m_db.transaction();
}

void QStageWidget::rollbackTransaction()
{
    m_db.rollback();
}

void QStageWidget::commitTransaction()
{
    m_db.commit();
}

void QStageWidget::newStage()
{
    int row = 0;
    row = m_model->rowCount();
    m_model->insertRow(row);
    m_model->setData(m_model->index(row, Stage_Index), row+1);
    m_model->setData(m_model->index(row, Stage_Vessel), 1);
    m_model->setData(m_model->index(row, Stage_Timeramp), 0);
    m_model->setData(m_model->index(row, Stage_Presspsi), 0);
    m_model->setData(m_model->index(row, Stage_Tempture), 0);
    m_model->setData(m_model->index(row, Stage_Hold), 0);
    m_model->setData(m_model->index(row, Stage_MethodId), m_methodid);
    m_model->submit();
}

void QStageWidget::delStage()
{
    int row = 0;
    row = m_model->rowCount();

    if(row <= 1)
        return;

    m_model->removeRow(row-1);
    m_model->submit();
}

void QStageWidget::delAllStage()
{
    int row = 0;
    row  = m_model->rowCount();

    m_model->removeRows(0, row);
    m_model->submit();
}

void QStageWidget::cleanStage()
{
    int row = 0;

    m_model->setData(m_model->index(row, Stage_Index), row+1);
    m_model->setData(m_model->index(row, Stage_Vessel), 12);
    m_model->setData(m_model->index(row, Stage_Timeramp), 0);
    m_model->setData(m_model->index(row, Stage_Presspsi), 0);
    m_model->setData(m_model->index(row, Stage_Tempture), 0);
    m_model->setData(m_model->index(row, Stage_Hold), 0);
    m_model->setData(m_model->index(row, Stage_MethodId), m_methodid);

    row  = m_model->rowCount();
    m_model->removeRows(1, row);
    m_model->submit();
}

void QStageWidget::saveStage(quint8 stage, quint8 vessel, quint16 ramp,
                             quint16 press, quint16 tempture, quint16 hold)
{
    int row = stage - 1;

    m_model->setData(m_model->index(row, Stage_Vessel), vessel);
    m_model->setData(m_model->index(row, Stage_Timeramp), ramp);
    m_model->setData(m_model->index(row, Stage_Presspsi), press);
    m_model->setData(m_model->index(row, Stage_Tempture), tempture);
    m_model->setData(m_model->index(row, Stage_Hold), hold);
    m_model->setData(m_model->index(row, Stage_MethodId), m_methodid);
    pline() << m_model->submit();
    //m_mapper->submit();

    //selectStage(row);
}

void QStageWidget::prev()
{
    int row = currentIndex().row()-1;
    selectStage(row);
    //pline() << currentIndex();
}

void QStageWidget::next()
{
    int row = currentIndex().row()+1;
    selectStage(row);
    //pline() << currentIndex();
}
