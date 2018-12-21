#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
#include "settings.h"
#include "cr3widget.h"
#include "searchdlg.h"

#include <QLabel>
#include <QMenu>
#include <QEvent>
#include <QTimer>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow, public PropsChangeCallback
{
    Q_OBJECT

    QLabel *labelTitle;
    QLabel *labelProgress;

    SearchDialog *searchDialog;

public:
    virtual void onPropsChange( PropsRef props );
    MainWindow(QWidget *parent = 0);
    void disableCr3Widget();
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    QString _filenameToOpen;
    void toggleProperty( const char * name );

    QColor getColor( PropsRef props, const char * optionName, unsigned def );
    void setColor( PropsRef props, const char * optionName, QColor cl );
    void changeColor(QColor &cl, bool increase);
    void changeBrightness(bool increase, bool font);
protected:
    virtual void showEvent ( QShowEvent * event );
    virtual void focusInEvent ( QFocusEvent * event );
    virtual void closeEvent ( QCloseEvent * event );
    virtual bool event(QEvent *e);
public slots:
    void contextMenu( QPoint pos );
    void on_actionFindText_triggered();
    void setStatusBarInfo(const QString &left, const QString &right);
    void setWinTitInfo(const QString &info);
    void fontBrightnessAdapter(bool inc);
    void backBrightnessAdapter(bool inc);
    void fontSizeAdapter(bool inc);
private slots:
    void on_actionNextPage3_triggered();
    void on_actionPrevPage3_triggered();
    void on_actionToggleEditMode_triggered();
    void on_actionRotate_triggered();
    void on_actionFileProperties_triggered();
    void on_actionShowBookmarksList_triggered();
    void on_actionAddBookmark_triggered();
    void on_actionAboutCoolReader_triggered();
    void on_actionAboutQT_triggered();
    void on_actionCopy2_triggered();
    void on_actionCopy_triggered();
    void on_actionSettings_triggered();
    void on_actionRecentBooks_triggered();
    void on_actionTOC_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionZoom_In_triggered();
    void on_actionToggle_Full_Screen_triggered();
    void on_actionToggle_Pages_Scroll_triggered();
    void on_actionPrevChapter_triggered();
    void on_actionNextChapter_triggered();
    void on_actionForward_triggered();
    void on_actionBack_triggered();
    void on_actionLastPage_triggered();
    void on_actionFirstPage_triggered();
    void on_actionPrevLine_triggered();
    void on_actionNextLine_triggered();
    void on_actionPrevPage_triggered();
    void on_actionNextPage_triggered();
    void on_actionPrevPage2_triggered();
    void on_actionNextPage2_triggered();
    void on_actionClose_triggered();
    void on_actionMinimize_triggered();
    void on_actionOpen_triggered();
    void on_actionExport_triggered();
    void on_view_destroyed();
    void on_actionNextSentence_triggered();
    void on_actionPrevSentence_triggered();
    void on_actionMove_Window_to_0_0_triggered();
    void on_actionIncrease_Brightness_triggered();
    void on_actionDecrease_Brightness_triggered();
    void on_actionIncrease_Font_Brightness_triggered();
    void on_actionDecrease_Font_Brightness_triggered();
    void on_actionIncrease_Image_Brightness_triggered();
    void on_actionDecrease_Image_Brightness_triggered();
    void on_actionReset_Brightness_triggered();
    void on_actionInvert_Brightness_triggered();
    void enableCr3Widget();
};

class ContextMenu : public QMenu {
    Q_OBJECT

    MainWindow *mainWindow;

protected:
    void closeEvent(QCloseEvent *) {
        QTimer::singleShot(300, mainWindow, SLOT(enableCr3Widget())); // 300 ms
    }

    void showEvent(QShowEvent *) {
        mainWindow->disableCr3Widget();
    }

public:
    ContextMenu(QWidget *parent, MainWindow *mainWindow, const QString &title) : QMenu(title, parent) {
        this->mainWindow = mainWindow;
    }
};

#endif // MAINWINDOW_H
