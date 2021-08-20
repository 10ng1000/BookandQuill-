#include <QDebug>
#include <QApplication>
#include <QFontDatabase>
#include <QFont>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QFileDevice>
#include <QProcess>
#include <QDesktopServices>
#include <QValidator>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lastClickPage = 0;
    readSettings();
    ui->stackedWidget->setCurrentIndex(0);
    setMaximumSize(800,575);
    setMinimumSize(800,575);
    setWindowFlags(Qt::FramelessWindowHint);
    fontIni();
    sideIni();
    lineEditini();
    pagePaintingIni();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mousePos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPosition().toPoint()-(this->mousePos));
}

void MainWindow::lineEditini()
{
    QList<QLineEdit *> l = {ui->leMapX,ui->leMapY,ui->leMapZ,ui->leHomeX,ui->leHomeY,ui->leHomeZ,ui->le0,ui->le1,
                         ui->le2,ui->le3,ui->le4,ui->le5,ui->le6,ui->le7,ui->le8,ui->le9,ui->le10,ui->le11};
    QDoubleValidator  *doubleValidator = new QDoubleValidator();
    for (auto it = l.begin(); it != l.end(); ++it) {
        (*it)->setValidator(doubleValidator);
    }
}

void MainWindow::fontIni()
{
    int fontId0 = QFontDatabase::addApplicationFont(":/new/fonts/G-OTF-GRyuminPro-Regular.otf");
    int fontId1 = QFontDatabase::addApplicationFont(":/new/fonts/minecraft.ttf");
    QString fontAlice = QFontDatabase::applicationFontFamilies(fontId0).at(0);
    QString fontMinecraft = QFontDatabase::applicationFontFamilies(fontId1).at(0);
    QFont iconFont(fontAlice);
    QFont labelFont(fontMinecraft);

    labelFont.setPixelSize(14);

    auto listWidget = findChildren<QWidget*>();
    for(auto &widget : listWidget)
    {
        widget->setFont(labelFont);
    }

    iconFont.setWeight(QFont::DemiBold);
    iconFont.setPixelSize(24);
    iconFont.setLetterSpacing(QFont::PercentageSpacing,QFont::SemiExpanded);
    iconFont.setStyleStrategy(QFont::PreferAntialias);
    ui->labelIconWord->setFont(iconFont);
}

void MainWindow::sideIni()
{
    QSettings settings;
    QLabel *list[8] = {ui->labelId1,ui->labelLocation1,ui->labelId2,ui->labelLocation2,ui->labelId3,ui->labelLocation3,ui->labelId4,ui->labelHome4};
    sideIniOpenFile(list,"file/location",0,6);
    sideIniOpenFile(list,"file/home",6,8);
}

void MainWindow::sideIniOpenFile(QLabel **list, QString setting,int begin, int end)
{
    QSettings settings;
    if(settings.contains(setting)){
        QFile file;
        QVector<QString> words;
        file.setFileName(settings.value(setting).toString());
        if(file.open(QIODevice::ExistingOnly|QIODevice
                  ::ReadOnly|QIODevice::Text)){
            QTextStream in(&file);
            QString temp;
            while(!in.atEnd()){
                in >> temp;
                words.append(temp);
                temp.clear();
                in >> temp;
                words.append(temp);
                temp.clear();
            }
            for(auto it = words.begin(); it != words.end();){
                if(it->isEmpty()){
                    it = words.erase(it);
                }
                else{
                    ++it;
                }
            }
            if(words.empty()){
                return;
            }
            for(int i = end - 1; i >= begin; --i,--i){
                list[i]->setText(words.back());
                words.removeLast();
                list[i-1]->setText(words.back());
                words.removeLast();
            }
        }
    }
}

void MainWindow::pagePaintingIni()
{
    QSettings settings;
    QDir paintingDir(settings.value("dir/paintingDir").toString());
    QStringList filter;
    filter << "*.png";
    paintingDir.setNameFilters(filter);
    QList<QFileInfo> *paintingFileInfo = new QList<QFileInfo>(paintingDir.entryInfoList(filter));
    QPixmap *paintings[4];
    QList<QLabel *> labelpictures = {ui->labelRecentPicture1, ui->labelRecentPicture2, ui->labelRecentPicture3, ui->labelRecentPicture4};
    for (int i = 0; i < 4; ++i){
        if(i == paintingFileInfo->length()){
            break;
        }
        //qDebug() << paintingFileInfo->at(i);
        paintings[i] = new QPixmap((*paintingFileInfo).at(i).filePath());
        *paintings[i] = paintings[i]->scaled(261,141,Qt::KeepAspectRatio);
        labelpictures[i]->setPixmap(*paintings[i]);
    }
}

void MainWindow::readSettings()
{
    QCoreApplication::setApplicationName("BookAndQuill++");
    QCoreApplication::setOrganizationName("l0ng");
    QSettings settings;
    settings.beginGroup("dir");
    ui->lePicture->setText(settings.value("paintingDir").toString());
    ui->leLocation->setText(settings.value("locationDir").toString());
    ui->leBookAndQuill->setText(settings.value("bookAndQuillDir").toString());
    ui->leStart->setText(settings.value("startDir").toString());
    ui->leBookShelf->setText(settings.value("bookShelfDir").toString());
    settings.endGroup();
    settings.beginGroup("doc");
    if(settings.contains("book")){
        QTextDocument *text = new QTextDocument(this);
        QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(text);
        text->setDocumentLayout(layout);
        text->setPlainText(settings.value("book").toString());
        ui->ptxBookAndQuill->setDocument(text);
    }
    settings.endGroup();
}

void MainWindow::readAllDoc(const QString &path)
{
    QFile file(path);
    QString words;
    if(file.open(QIODevice::ExistingOnly|QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        words = file.readAll();
    }
    if(words.isEmpty()){
        return;
    }
    ui->tbBook->setMarkdown(words);
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::writeLocation(QString texts[], QFile &file)
{
    QTextStream out(&file);
    out << '\n' <<texts[0] << ": " << texts[1] << ',' << texts[2] << ',' << texts[3] << '\n' ;
}

void MainWindow::writeBook(QString texts[], QFile &file)
{
    QTextStream out(&file);
    out << texts[0];
}

void MainWindow::saveDoc(QString texts[],const QString &dir, const QString &path, bool isCover, void (*writeMode) (QString texts[], QFile &file))
{
    QSettings settings;
    QFile file;
    if(isCover && settings.contains(path)){
        //qDebug() << settings.value(path).toString();
        file.setFileName(settings.value(path).toString());
        if(file.open(QIODevice::ExistingOnly|QIODevice
                  ::WriteOnly|QIODevice::Append|QIODevice::Text)){
            writeMode(texts,file);
            return;
        }
    }

    QString fileName;
    getFolder(dir,fileName);
    settings.setValue(path,fileName);
    file.setFileName(fileName);
    file.open(QIODevice::NewOnly|QIODevice
              ::WriteOnly|QIODevice::Append|QIODevice::Text);
    writeMode(texts,file);
}

void MainWindow::setSaveFolder(const QString &key,QLineEdit *le)
{
    QSettings settings;
    settings.beginGroup("dir");
    QString dir = QFileDialog::getExistingDirectory(this,tr("获得一个目录"),"./");
    if(!dir.isEmpty()){
        le->setText(dir);
        settings.setValue(key,dir);
        settings.sync();
    }
    pagePaintingIni();
    settings.endGroup();
}

Location MainWindow::calIntersection(double k1, double k2, double b1, double b2)
{
    if (k1 == k2) {
       k2 -= (k2/100);
    }
    Location ans;
    ans.x = (b2 - b1) / (k1 - k2);
    ans.z = k1 * ans.x + b1;
    return ans;
}

Location MainWindow::calStrongHold(Location loc[])
{
    double k[3],b[3];
    for(int i = 0; i < 6 ; ++i,++i){
        if(loc[i+1].x == loc[i].x){
            loc[i].x -= (loc[i].x / 100);
        }
        k[i/2] = (loc[i+1].z - loc[i].z) / (loc[i+1].x - loc[i].x);
        b[i/2] = -k[i/2] * loc[i].x + loc[i].z;
    }
    /*for(int i = 0; i < 3; ++i){
        qDebug() << "k:" << k[i] << "  b:" << b[i] <<'\n';
    }*/
    Location ans[3];
    ans[0] = calIntersection(k[0],k[1],b[0],b[1]);
    ans[1] = calIntersection(k[0],k[2],b[0],b[2]);
    ans[2] = calIntersection(k[1],k[2],b[1],b[2]);
    /*for(int i = 0; i < 3; ++i){
        qDebug() << "x:" << ans[i].x << "  y:" << ans[i].z <<'\n';
    }*/
    return {(ans[0].x + ans[1].x + ans[2].x) / 3, (ans[0].z + ans[1].z + ans[2].z) / 3};
}

void MainWindow::getFolder(const QString &folder, QString &fileNameToBeWrite)
{
    QSettings settings;
    settings.beginGroup("dir");
    if(settings.contains(folder)){
        fileNameToBeWrite = QFileDialog::getSaveFileName(this,tr("保存文件"),settings.value(folder).toString(),"*.txt");
    }
    else{
        fileNameToBeWrite = QFileDialog::getSaveFileName(this,tr("保存文件"),"./", "*.txt");
    }
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    QString text = ui->ptxBookAndQuill->toPlainText();
    settings.setValue("doc/book",text);
}

void MainWindow::on_btnPainting_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    lastClickPage = 0;
}

void MainWindow::on_btnMap_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    lastClickPage = 1;
}

void MainWindow::on_btnBookAndQuill_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    lastClickPage = 2;
}

void MainWindow::on_btnBook_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnCompass_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    lastClickPage = 4;
}

void MainWindow::on_btnEndPortal_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    lastClickPage = 5;
}

void MainWindow::on_btnAnvil_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    lastClickPage = 6;
}


void MainWindow::on_btnFile_1_clicked()
{
    setSaveFolder("paintingDir",ui->lePicture);
}

void MainWindow::on_btnFile_2_clicked()
{
    setSaveFolder("locationDir",ui->leLocation);
}

void MainWindow::on_btnFile_3_clicked()
{
    setSaveFolder("bookAndQuillDir",ui->leBookAndQuill);
}

void MainWindow::on_btnFile_4_clicked()
{
    QSettings settings;
    settings.beginGroup("dir");
    QString dir = QFileDialog::getOpenFileName(this,tr("获得一个目录"),"./","*.exe");
    if(!dir.isEmpty()){
        ui->leStart->setText(dir);
        settings.setValue("startDir",dir);
        settings.sync();
    }
    pagePaintingIni();
    settings.endGroup();
}

void MainWindow::on_btnFile_5_clicked()
{
    setSaveFolder("bookShelfDir",ui->leBookShelf);
}


void MainWindow::on_btnMapSave_clicked()
{
    if(ui->leMapId->text().isEmpty()||ui->leMapX->text().isEmpty()||ui->leMapY->text().isEmpty()||ui->leMapZ->text().isEmpty()){
        return;
    }
    QString texts[4] = {ui->leMapId->text(),ui->leMapX->text(),ui->leMapY->text(),ui->leMapZ->text()};
    saveDoc(texts,"locationDir","file/location",1,writeLocation);
    sideIni();
}

void MainWindow::on_btnLandStoneSave_clicked()
{
    if(ui->leHomeId->text().isEmpty()||ui->leHomeX->text().isEmpty()||ui->leHomeY->text().isEmpty()||ui->leHomeZ->text().isEmpty()){
        return;
    }
    QString texts[4] = {ui->leHomeId->text(),ui->leHomeX->text(),ui->leHomeY->text(),ui->leHomeZ->text()};
    saveDoc(texts,"locationDir","file/home",1,writeLocation);
    sideIni();
}

void MainWindow::on_btnSign_clicked()
{
    if(ui->ptxBookAndQuill->toPlainText().isEmpty()){
        return;
    }
    QString text = ui->ptxBookAndQuill->toPlainText();
    saveDoc(&text,"bookAndQuillDir","file/bookAndQuill",0,writeBook);
}

void MainWindow::on_btnMapFile_clicked()
{
    QSettings settings;
    if(settings.contains("file/location")){
        readAllDoc(settings.value("file/location").toString());
    }
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnHomeFile_clicked()
{
    QSettings settings;
    if(settings.contains("file/home")){
        readAllDoc(settings.value("file/home").toString());
    }
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btnWiki_clicked()
{
    QUrl url("https://wiki.biligame.com/mc");
    QDesktopServices::openUrl(url);
}

void MainWindow::on_btnBAQBook_clicked()
{
    QSettings settings;
    if(settings.contains("dir/bookAndQuillDir")){
        QString fileName = QFileDialog::getOpenFileName(this,tr("打开一个文件"),
                                                        settings.value("dir/bookAndQuillDir").toString(),
                                                        "*.txt;;*.markdown;;*.html");
        readAllDoc(fileName);
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_btnInk_clicked()
{
    readAllDoc(":/new/document/ink.md");
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnPaper_clicked()
{
    ui->tbBook->clear();
}


void MainWindow::on_btnPortalHelp_clicked()
{
    readAllDoc(":/new/document/stronghold.md");
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_btnEyeOfEnder_clicked()
{
    bool ok[12];
    Location loc[6] = {{ui->le0->text().toDouble(&ok[0]),ui->le1->text().toDouble(&ok[1])},
                       {ui->le2->text().toDouble(&ok[2]),ui->le3->text().toDouble(&ok[3])},
                       {ui->le4->text().toDouble(&ok[4]),ui->le5->text().toDouble(&ok[5])},
                       {ui->le6->text().toDouble(&ok[6]),ui->le7->text().toDouble(&ok[7])},
                       {ui->le8->text().toDouble(&ok[8]),ui->le9->text().toDouble(&ok[9])},
                       {ui->le10->text().toDouble(&ok[10]),ui->le11->text().toDouble(&ok[11])}};
    for (int i = 0; i < 12; ++i) {
        if(ok[i] == false){
            return;
        }
        //qDebug() << loc[i].x << loc[i].z << '\n';
    }
    Location strongHoldLoc = calStrongHold(loc);
    QString locx,locz;
    locx.setNum(strongHoldLoc.x);
    locz.setNum(strongHoldLoc.z);
    ui->leStrongHoldX->setText(locx);
    ui->leStrongHoldZ->setText(locz);
}


void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(lastClickPage);
}


void MainWindow::on_btnStart_clicked()
{
    QSettings settings;
    if(!settings.contains("dir/startDir")){
        return;
    }
    QProcess *pro = new QProcess(this);
    qDebug() << settings.value("dir/startDir").toString();
    pro->start(settings.value("dir/startDir").toString());
}


void MainWindow::on_btnPaper_2_clicked()
{
    ui->le0->clear();ui->le1->clear();
    ui->le2->clear();ui->le3->clear();
    ui->le4->clear();ui->le5->clear();
    ui->le6->clear();ui->le7->clear();
    ui->le8->clear();ui->le9->clear();
    ui->le10->clear();ui->le11->clear();
    ui->leStrongHoldX->clear();ui->leStrongHoldZ->clear();
}


void MainWindow::on_btnBookShelf_clicked()
{
    QSettings settings;
    if(!settings.contains("dir/bookShelfDir")){
        return;
    }
    qDebug() << settings.value("dir/bookShelfDir").toString();
    QUrl *url = new QUrl(settings.value("dir/bookShelfDir").toString());
    QDesktopServices::openUrl(*url);
}



