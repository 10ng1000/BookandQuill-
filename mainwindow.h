#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Location
{
    double x,z;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
    void fontIni();
    void shadowIni();
    void pagePaintingIni();
    void readSettings();
    void sideIni();
    void sideIniOpenFile(QLabel* list[],QString setting,int begin, int end);
    void readAllDoc(const QString &path);
    Location calStrongHold(Location loc[]);
    Location calIntersection(double k1, double k2, double b1, double b2);
    static void writeLocation(QString texts[], QFile &file);
    static void writeBook(QString texts[], QFile &file);
    void saveDoc(QString texts[],const QString &dir,const QString &path,bool isCover,void(*writeMode) (QString texts[], QFile &file));
    void setSaveFolder(const QString &key, QLineEdit *le);
    void getFolder(const QString &folder,QString &fileNameToBeWrite);
    void lineEditini();

    int lastClickPage;

private slots:
    void on_btnPainting_clicked();
    void on_btnMap_clicked();

    void on_btnBookAndQuill_clicked();

    void on_btnBook_clicked();

    void on_btnCompass_clicked();

    void on_btnEndPortal_clicked();

    void on_btnAnvil_clicked();
    void on_btnFile_1_clicked();

    void on_btnFile_2_clicked();

    void on_btnFile_3_clicked();

    void on_btnMapSave_clicked();

    void on_btnLandStoneSave_clicked();

    void on_btnMapFile_clicked();

    void on_btnHomeFile_clicked();

    void on_btnWiki_clicked();

    void on_btnInk_clicked();

    void on_btnPaper_clicked();

    void on_btnPortalHelp_clicked();

    void on_btnEyeOfEnder_clicked();

    void on_btnBack_clicked();

    void on_btnStart_clicked();

    void on_btnFile_4_clicked();

    void on_btnPaper_2_clicked();

    void on_btnBookShelf_clicked();

    void on_btnFile_5_clicked();

    void on_btnBAQBook_clicked();

    void on_btnSign_clicked();

private:
    Ui::MainWindow *ui;
    QPoint mousePos;
};
#endif // MAINWINDOW_H
