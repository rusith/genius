#include "contentviewer.h"
#include "ui_contentviewer.h"

ContentViewer::ContentViewer(ClipboardEntity *entity, QWidget *parent):
  QDialog(parent),ui(new Ui::ContentViewer)
{
  ui->setupUi(this);
  if(entity)
    _entity=entity;
  else
    reject();
  initializeBasicUI();
}

ContentViewer::~ContentViewer()
{
  delete ui;
}

void ContentViewer::initializeBasicUI()
{
  setLayout(ui->rootLayout);
  QString addedTime=_entity->addedTime()->toString("hh.mm.ss.zzz AP");
  int formatsCount=_entity->formats().size();
  double size =(double)_entity->size()/1024;
  QString lblText=QString("<b>Added Time :</b> %1   <b>Formats :</b> %2         <b>Size :</b> %3 kb")
                  .arg(addedTime).arg(formatsCount).arg(size);
  ui->lblBasicInfo->setText(lblText);

  addImageTab();
  addHTMLTab();
  addURLsTab();
  addPlainTextTab();
  if(ui->tvFormats->count()<1)
    replaceTabViewWithFormats();

  QHBoxLayout *buttonSet=new QHBoxLayout();
  buttonSet->addStretch();
  QPushButton *closeButton=new QPushButton(this);
  closeButton->setIcon(QIcon(Resources::cancel16));
  closeButton->setText("close");
  connect(closeButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
  buttonSet->addWidget(closeButton);
  ui->rootLayout->addLayout(buttonSet);
}

void ContentViewer::addPlainTextTab()
{
  if(_entity->hasPlainText())
  {

    QWidget *plainTextTab=new QWidget(ui->tvFormats);
    plainTextTab->setObjectName("Plain Text");
    plainTextTab->setWindowTitle("Plain Text");

    QVBoxLayout *plainTextTabLayout=new QVBoxLayout();

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

    QVBoxLayout *tabLayout=new QVBoxLayout();

    QTabWidget *innerTabWidget=new QTabWidget(tab);
    QWidget *previewTab=new QWidget(innerTabWidget);
    previewTab->setObjectName("preview");
    previewTab->setWindowTitle("preview");

    QVBoxLayout *previewTabLayout=new QVBoxLayout();


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

    QVBoxLayout *codeTabLayout=new QVBoxLayout();
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
      imageTab->setObjectName("imaaddURLsTabge");
      imageTab->setWindowTitle("image");

      QVBoxLayout *imageTabLayout=new QVBoxLayout();
      QTabWidget *innerTabWidget=new QTabWidget(imageTab);

      foreach (QString format, formats)
      {
        QString formatName=imageMimeTypeToText(format);
        if(formatName.isEmpty()) continue;


        QWidget *innerTab=new QWidget(innerTabWidget);
        innerTab->setObjectName(formatName);
        innerTab->setWindowTitle(formatName);

        QVBoxLayout *innerTabLayout=new QVBoxLayout();

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

void ContentViewer::addURLsTab()
{
  if(_entity->hasURLs())
  {
    QWidget *tab=new QWidget(ui->tvFormats);
    tab->setObjectName("URLS");
    tab->setWindowTitle("URLS");

    QVBoxLayout *tabLayout=new QVBoxLayout();
    tabLayout->setMargin(5);

    QTextBrowser *textBrowser=new QTextBrowser(tab);
    textBrowser->setOpenLinks(false);
    textBrowser->setWordWrapMode(QTextOption::NoWrap);
    QList<QUrl> urls=_entity->urls();
    QString text;
    foreach (QUrl url, urls)
    {
      if(url.scheme()=="file")
      {
        QString filename=url.fileName();
        text.append(QString("<p><a href=\""+url.toString()+"\"><span style=\" font-weight:600; text-decoration: none; color:#0000ff;\">(file) "+filename+"</span></a></p>"));
      }
      else
      {
        QString path=url.toString();
        text.append(QString("<p><a href=\""+path+"\"><span style=\" font-weight:600; text-decoration: none; color:#0000ff;\">"+path+"</span></a></p>"));
      }
    }
    textBrowser->setHtml(text);
    connect(textBrowser,&QTextBrowser::anchorClicked,this,[textBrowser](const QUrl &link){
      QDesktopServices::openUrl(link);
    });
    tabLayout->addWidget(textBrowser);
    tab->setLayout(tabLayout);
    ui->tvFormats->addTab(tab,"URLS");
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
void ContentViewer::replaceTabViewWithFormats()
{
  if(ui->tvFormats)
    delete ui->tvFormats;
  QVBoxLayout *formatsLayout=new QVBoxLayout(this);

  QListWidget *lvFormats=new QListWidget(this);
  QStringList formats=_entity->formats();
  if(formats.isEmpty())
    reject();
  else
  {
    foreach (QString format, formats)
    {
      QListWidgetItem *item=new QListWidgetItem(lvFormats);
      item->setText(format);
      lvFormats->addItem(item);
    }
  }

  QPlainTextEdit *rowDataViewer=new QPlainTextEdit(this);
  rowDataViewer->setReadOnly(true);

  QHBoxLayout *detailsLayout=new QHBoxLayout(this);
  QLabel *dlabel=new QLabel(this);
  detailsLayout->addWidget(dlabel);
  detailsLayout->addStretch();
  QPushButton *savebtn=new QPushButton(this);
  savebtn->setText("save");
  savebtn->setIcon(QIcon(Resources::save16));
  connect(savebtn,&QPushButton::clicked,this,[lvFormats,this](){
    if(lvFormats && this->_entity && lvFormats->count()>0)
    {
      int currentIndex=lvFormats->currentRow();
      if(currentIndex<0)
        return;
      QByteArray *data=this->_entity->data(lvFormats->item(currentIndex)->text()); //DELETED
      if(!data)return;
      QFileDialog fileDialog(this);
      fileDialog.setWindowTitle("select a file to save data ");
      fileDialog.setWindowIcon(QIcon(Resources::save16));
      fileDialog.setFileMode(QFileDialog::AnyFile);
      fileDialog.setAcceptMode(QFileDialog::AcceptSave);
      fileDialog.setDefaultSuffix("data");
      while(true)
      {
        if(fileDialog.exec())
        {
          QString fileName=fileDialog.selectedFiles().at(0);
          QFile file(fileName);
          bool opened=file.open(QFile::WriteOnly);
          if(opened)
          {
            file.write(*data);
            delete data;
            return;
          }
          else
          {
            if(QMessageBox::critical(this,"cannot save file","cannot save file as \n"+fileName+". \nchange directory and try again.\ndo you want to try again?",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
              continue;
            else
              break;
          }
        }
        else
          break;
      }
      delete data;
    }

  });

  detailsLayout->addWidget(savebtn);

  formatsLayout->addWidget(lvFormats);

  QLabel *rowDatalbl=new QLabel(this);
  rowDatalbl->setText("Row Data :");

  formatsLayout->addWidget(rowDatalbl);
  formatsLayout->addWidget(rowDataViewer);
  formatsLayout->addLayout(detailsLayout);

  connect(lvFormats,&QListWidget::currentRowChanged,this,[this,rowDataViewer,dlabel,lvFormats](int currentRow){
    if(dlabel && lvFormats && this->_entity && currentRow>-1)
    {

      QListWidgetItem *item=lvFormats->item(currentRow);
      QString name=item->text();
      double size=((double)this->_entity->formatSize(name)/1024);
      QByteArray *ba=this->_entity->data(name);
      if(ba)
      {
        rowDataViewer->setPlainText(QString::fromUtf8(*ba));
        delete ba;
      }
      dlabel->setText(QString("<b>size :</b> %1 kb ").arg(size));
    }
  });


  ui->rootLayout->addLayout(formatsLayout);
}
