#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include <cr3version.h>
#include <QDesktopServices>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AboutDialog)
{
    m_ui->setupUi(this);
    m_ui->lblVersion->setText(QString("Cool Reader v") + QString(CR_ENGINE_VERSION));
    m_ui->lblDate->setText(tr("Build time: ") + QString(CR_ENGINE_BUILD_DATE));
}

AboutDialog::~AboutDialog()
{
    delete m_ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool AboutDialog::showDlg( QWidget * parent )
{
    AboutDialog * dlg = new AboutDialog( parent );
    dlg->setModal( true );
    dlg->setWindowTitle(tr("About Cool Reader"));
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    return true;
}


void AboutDialog::on_buttonBox_accepted()
{
    close();
}

void AboutDialog::on_btnSite_clicked()
{
    QUrl qturl( "http://sourceforge.net/projects/crengine" );
    QDesktopServices::openUrl( qturl );
}
