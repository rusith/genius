#include "contentviewer.h"
#include "ui_contentviewer.h"

ContentViewer::ContentViewer(ClipboardEntity *entity, QWidget *parent):
  QDialog(parent),ui(new Ui::ContentViewer)
{
  ui->setupUi(this);
  if(entity)
    _entity=entity;

  initializeBasicUI();
}

ContentViewer::~ContentViewer()
{
  delete ui;
}

void ContentViewer::initializeBasicUI()
{
  setLayout(ui->rootLayout);
  if(_entity)
  {
    ui->lbAddedTime->setText("<b>Added Time :</b> "+_entity->addedTime()->toString("hh.mm.ss.zzz AP"));
    addImageTab();
    addHTMLTab();
    addPlainTextTab();
  }
}

void ContentViewer::addPlainTextTab()
{
  if(_entity->hasPlainText())
  {

    QWidget *plainTextTab=new QWidget(ui->tvFormats);
    plainTextTab->setObjectName("Plain Text");
    plainTextTab->setWindowTitle("Plain Text");

    QVBoxLayout *plainTextTabLayout=new QVBoxLayout(plainTextTab);

    QPlainTextEdit *plainTextEdit=new QPlainTextEdit(plainTextTab);
    plainTextEdit->setPlainText(_entity->plainText(false,-1));
    plainTextEdit->setReadOnly(true);
    plainTextTabLayout->addWidget(plainTextEdit);

    QHBoxLayout *plainTextInfoLayout=new QHBoxLayout();
    QLabel *plainTextInfoLabel=new QLabel(plainTextTab);
    plainTextInfoLabel->setText(QString("<b>length :</b> %1").arg(plainTextEdit->toPlainText().count())
                               +"  "+QString("<b>size :</b> %1 KB").arg((double)_entity->formatSize("text/plain")/1024));
    plainTextInfoLayout->addWidget(plainTextInfoLabel);

    QPushButton *btnExport=new QPushButton(plainTextTab);
    connect(btnExport,&QPushButton::clicked,this,[this,plainTextEdit](){
      QFileDialog dialog;
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog.setWindowTitle("select a file to save text");
      dialog.setWindowIcon(QIcon(Resources::Export16));
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setDefaultSuffix("txt");
      for(;;)
      {
        if(dialog.exec())
        {
          int length=dialog.selectedFiles().length();
          if(length>0 && length==1)
          {
            QFile file(dialog.selectedFiles().at(0));
            file.open(QIODevice::WriteOnly);
            if(file.isWritable())
            {
              qint64 i= file.write(plainTextEdit->toPlainText().toUtf8());
              if(i<0)
              {
                int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write to the selected file.nothing saved. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
                if(messageResult==QMessageBox::Ok)
                  continue;
                else break;
              }
              else break;
            }
            else
            {
              int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write selected file. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
              if(messageResult==QMessageBox::Ok)
                continue;
              else break;
            }
          }
        }
        else break;
      }
    });
    btnExport->setIcon(QIcon(Resources::Export16));
    btnExport->setText("export");
    plainTextInfoLayout->addStretch();
    plainTextInfoLayout->addWidget(btnExport);
    plainTextTabLayout->addLayout(plainTextInfoLayout);

    plainTextTab->setLayout(plainTextTabLayout);

    ui->tvFormats->addTab(plainTextTab,"Plain Text");
  }
}

void ContentViewer::addHTMLTab()
{
  if(_entity->hasHTML())
  {
    QWidget *tab=new QWidget(ui->tvFormats);
    tab->setObjectName("HTML");
    tab->setWindowTitle("HTML");

    QVBoxLayout *tabLayout=new QVBoxLayout(tab);

    QTabWidget *innerTabWidget=new QTabWidget(tab);
    QWidget *previewTab=new QWidget(innerTabWidget);
    previewTab->setObjectName("preview");
    previewTab->setWindowTitle("preview");

    QVBoxLayout *previewTabLayout=new QVBoxLayout(previewTab);


    QTextBrowser *previewTextBorowser=new QTextBrowser(tab);
    previewTextBorowser->setText(_entity->HTMLText(false,-1));
    previewTextBorowser->setReadOnly(true);

    QHBoxLayout *previewTabOptionsLayout=new QHBoxLayout();
    previewTabOptionsLayout->addStretch();

    QLabel *previewCBLabel=new QLabel(previewTab);
    previewCBLabel->setText("background color : ");
    previewTabOptionsLayout->addWidget(previewCBLabel);

    QPushButton *previewBCButton=new QPushButton(previewTab);
    previewBCButton->setText("white");
    previewBCButton->setStyleSheet("background-color : white ;");
    connect(previewBCButton,&QPushButton::clicked,this,[this,previewTextBorowser,previewBCButton](){
      QColorDialog CD(this);
      if(CD.exec())
      {
        QColor color=CD.selectedColor();
        previewTextBorowser->setStyleSheet("background-color :  "+color.name()+";");
        previewBCButton->setText(color.name());
        previewBCButton->setStyleSheet("background-color :  "+color.name()+";");
      }
    });

    previewTabOptionsLayout->addWidget(previewBCButton);
    previewTabLayout->addLayout(previewTabOptionsLayout);

    previewTabLayout->addWidget(previewTextBorowser);
    previewTab->setLayout(previewTabLayout);
    innerTabWidget->addTab(previewTab,"preview");

    QWidget *codeTab=new QWidget(innerTabWidget);
    codeTab->setObjectName("code");
    codeTab->setWindowTitle("code");

    QVBoxLayout *codeTabLayout=new QVBoxLayout(codeTab);
    QPlainTextEdit *codeTextEdit=new QPlainTextEdit(codeTab);
    codeTextEdit->setPlainText(_entity->HTMLText(false,-1));
    codeTextEdit->setReadOnly(true);
    codeTabLayout->addWidget(codeTextEdit);

    codeTab->setLayout(codeTabLayout);

    innerTabWidget->addTab(codeTab,"code");


    tabLayout->addWidget(innerTabWidget);

    QHBoxLayout *infoLayout=new QHBoxLayout();
    QLabel *infoLabel=new QLabel(tab);
    infoLabel->setText(QString("<b>length :</b> %1").arg(previewTextBorowser->toPlainText().count())
                               +"  "+QString("<b>size :</b> %1 KB").arg((double)_entity->formatSize("text/html")/1024));
    infoLayout->addWidget(infoLabel);

    QPushButton *btnExport=new QPushButton(tab);
    connect(btnExport,&QPushButton::clicked,this,[this](){
      QFileDialog dialog;
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog.setWindowTitle("select a file to save HTML");
      dialog.setWindowIcon(QIcon(Resources::Export16));
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setDefaultSuffix("html");
      for(;;)
      {
        if(dialog.exec())
        {
          int length=dialog.selectedFiles().length();
          if(length>0 && length==1)
          {
            QFile file(dialog.selectedFiles().at(0));
            file.open(QIODevice::WriteOnly);
            if(file.isWritable())
            {
              qint64 i= file.write(this->_entity->HTMLText(false,-1).toUtf8());
              if(i<0)
              {
                int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write to the selected file.nothing saved. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
                if(messageResult==QMessageBox::Ok)
                  continue;
                else break;
              }
              else break;
            }
            else
            {
              int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write selected file. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
              if(messageResult==QMessageBox::Ok)
                continue;
              else break;
            }
          }
        }
        else break;
      }
    });
    btnExport->setIcon(QIcon(Resources::Export16));
    btnExport->setText("export");
    infoLayout->addStretch();
    infoLayout->addWidget(btnExport);
    tabLayout->addLayout(infoLayout);

    tab->setLayout(tabLayout);

    ui->tvFormats->addTab(tab,"HTML");
  }
}

void ContentViewer::addImageTab()
{
  if(_entity->hasImage())
  {
    QStringList formats=_entity->imageFormats();
    if(formats.isEmpty()==false)
    {
      QWidget *imageTab=new QWidget(ui->tvFormats);
      imageTab->setObjectName("image");
      imageTab->setWindowTitle("image");

      QVBoxLayout *imageTabLayout=new QVBoxLayout(imageTab);
      QTabWidget *innerTabWidget=new QTabWidget(imageTab);

      foreach (QString format, formats)
      {
        QString formatName=imageMimeTypeToText(format);
        if(formatName.isEmpty()) continue;


        QWidget *innerTab=new QWidget(innerTabWidget);
        innerTab->setObjectName(formatName);
        innerTab->setWindowTitle(formatName);

        QVBoxLayout *innerTabLayout=new QVBoxLayout(innerTab);

        QGraphicsScene *graphicScene=new QGraphicsScene(innerTab);
        QGraphicsView *graphicsView=new QGraphicsView(graphicScene);
        QGraphicsPixmapItem *gpixmap=new QGraphicsPixmapItem();
        QByteArray *imageData=_entity->data(format);
        if(!imageData)continue;
        gpixmap->setPixmap(QPixmap::fromImage(QImage::fromData(*imageData)));
        delete imageData;
        graphicScene->addItem(gpixmap);

        innerTabLayout->addWidget(graphicsView);

        QHBoxLayout *innerOptionsLayout=new QHBoxLayout();
        QLabel *innerDetailsLabel=new QLabel(innerTab);
        innerDetailsLabel->setText(QString("<b>size : </b> %1 KB      <b>width : </b>%2 px    <b>height : </b>%3px ").arg((double)_entity->formatSize(format)/1024)
                                   .arg(gpixmap->pixmap().width())
                                   .arg(gpixmap->pixmap().height()));
        innerOptionsLayout->addWidget(innerDetailsLabel);

        QPushButton *innerExportButton=new QPushButton(innerTab);
        innerExportButton->setText("export");
        innerExportButton->setIcon(QIcon(Resources::Export16));
        connect(innerExportButton,&QPushButton::clicked,this,[this,gpixmap,format,formatName](){

          QFileDialog fileDialog(this);
          QStringList mimeTypeFilters;
          mimeTypeFilters.append(format);
          fileDialog.setMimeTypeFilters(mimeTypeFilters);
          fileDialog.setFileMode(QFileDialog::AnyFile);
          fileDialog.setAcceptMode(QFileDialog::AcceptSave);
          fileDialog.setDefaultSuffix(formatName.toLower());
          while(true)
          {
            if(fileDialog.exec())
            {
              QString fileName=fileDialog.selectedFiles().at(0);
               if(!gpixmap->pixmap().toImage().save(fileName))
               {
                 if(QMessageBox::critical(this,"cannot save file","cannot save file as \n"+fileName+". \nchange directory and try again.\ndo you want to try again?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
                 {
                   continue;
                 }
                 else
                   break;
               }
               else
                break;
            }
            else
              break;
          }
        });
        innerOptionsLayout->addStretch();
        innerOptionsLayout->addWidget(innerExportButton);
        innerTabLayout->addLayout(innerOptionsLayout);

        innerTab->setLayout(innerTabLayout);
        innerTabWidget->addTab(innerTab,formatName);
        graphicsView->show();

      }

      imageTabLayout->addWidget(innerTabWidget);
      imageTab->setLayout(imageTabLayout);
      ui->tvFormats->addTab(imageTab,"image");
    }
  }
}



QString ContentViewer::imageMimeTypeToText(const QString &MT)
{

  if(MT=="image/png")
    return "PNG";
  else if(MT=="image/bmp")
    return "BMP";
  else if(MT=="image/x-bmp")
    return "XBMP";
  else if(MT=="image/x-MS-bmp")
    return "MSXBMP";
  else if(MT=="image/jpeg")
    return "JPEG";
  else if(MT=="image/x-icon")
    return "ICON";
  else if(MT=="image/x-ico")
    return "ICO";
  else if(MT=="image/x-win-bitmap")
    return "BITMAP";
  else if(MT=="image/tiff")
    return "TIFF";
  else
    return "";
}
void ContentViewer::on_pushButton_clicked()
{
  reject();
}
