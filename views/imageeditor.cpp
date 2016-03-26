#include "imageeditor.h"
#include "ui_imageeditor.h"

ImageEditor::ImageEditor(ClipboardImageItem *item,QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ImageEditor)
{
  ui->setupUi(this);
  if(item && item->type()==ClipboardItem::Image)
  {
    _item=item;
    QImage *image=item->image();
    if(image)
    {
      _image=new QImage(*image);
      _image_backup=QImage(*image);
    }
    delete image;
  }
}

ImageEditor::~ImageEditor()
{
  delete ui;
}

void ImageEditor::initializeView()
{
  setLayout(ui->rootLayout);
  QGraphicsScene *qc=new QGraphicsScene(ui->graphicsView);
  QPixmap pm=QPixmap::fromImage(*_image);
  qc->addPixmap(pm.scaled(QSize(ui->graphicsView->width(),ui->graphicsView->height()),Qt::KeepAspectRatio,Qt::SmoothTransformation));
  ui->graphicsView->setScene(qc);
  ui->graphicsView->setBackgroundBrush(QBrush(Qt::transparent));
  ui->graphicsView->setStyleSheet("background:transparent;border:none;");
  ui->graphicsView->fitInView(qc->sceneRect(),Qt::KeepAspectRatio);
}

void ImageEditor::showEvent(QShowEvent *event)
{
  event->accept();
  initializeView();
}

void ImageEditor::on_okButton_clicked()
{
  if(_item)
  {
    _item->image(_image);
  }
  accept();
}

void ImageEditor::on_cancelButton_clicked()
{
   *_image=_image_backup;
  reject();
}

void ImageEditor::on_exportButton_clicked()
{
  QFileDialog fileDialog(this);
  QStringList mimeTypeFilters;
  mimeTypeFilters.append("image/jpeg");
  mimeTypeFilters.append("image/png");
  mimeTypeFilters.append("image/bmp");
  fileDialog.setMimeTypeFilters(mimeTypeFilters);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);

  while(true)
  {
    if(fileDialog.exec())
    {
      QString fileName=fileDialog.selectedFiles().at(0);
      fixFileNameExtension(&fileName,fileDialog.selectedNameFilter());
       if(!_image->save(fileName))
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

}

void ImageEditor::fixFileNameExtension(QString *fileName, QString mimeType)
{
  if(fileName->isEmpty()==false && mimeType.isEmpty()==false)
  {
     if(mimeType=="JPEG Image (*.jpeg *.jpg *.jpe *.jfif)")
     {
       if(!(fileName->endsWith(".jpeg")||fileName->endsWith(".jpg")||fileName->endsWith(".jpe")||fileName->endsWith(".jfif")))
         fileName->append(fileName->endsWith(".")?"jpg":".jpg");
     }
     else if(mimeType=="PNG image (*.png)")
     {
       if(!fileName->endsWith(".png"))
         fileName->append(fileName->endsWith(".")?"png":".png");
     }
     else if("Windows BMP image (*.bmp *.dib)")
     {
       if(!(fileName->endsWith(".bmp")||fileName->endsWith(".dib")))
         fileName->append(fileName->endsWith(".")?"bmp":".bmp");
     }
   }
}

void ImageEditor::on_replaceButton_clicked()
{
  QFileDialog fileDialog(this);
  QStringList mimeTypeFilters;
  mimeTypeFilters.append("image/jpeg");
  mimeTypeFilters.append("image/png");
  mimeTypeFilters.append("image/bmp");
  mimeTypeFilters.append("image/ppm");
  mimeTypeFilters.append("image/xbm");
  fileDialog.setMimeTypeFilters(mimeTypeFilters);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);

  while(true)
  {
    if(fileDialog.exec())
    {
      if(fileDialog.selectedFiles().length()>0)
      {
        QString fileName=fileDialog.selectedFiles().at(0);
        QFile file(fileName);
        if(file.exists())
        {
          if(_image->load(fileName))
          {
            initializeView();
            break;
          }
          else
          {
            int res=QMessageBox::critical(this,"cannot read file","cannot read this image file.\ntry another",QMessageBox::Ok,QMessageBox::Cancel);
            if(res==QMessageBox::Ok)
              continue;
            else
              break;
          }
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

}
