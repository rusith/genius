#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QString *str,QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TextEditor)
{
  ui->setupUi(this);
  setLayout(ui->rootLayout);

  if(str)
  {
    _str=str;
    ui->editor->setPlainText(*str);
  }
}

TextEditor::~TextEditor()
{
  delete ui;
}

void TextEditor::on_cancelButton_clicked()
{
    reject();
}

void TextEditor::on_okButton_clicked()
{
  if(_str)
  {
    *_str=ui->editor->toPlainText();
    accept();
  }
}

void TextEditor::on_importButton_clicked()
{
  QFileDialog dialog;
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setWindowTitle("select a file to import as text ");
  dialog.setWindowIcon(QIcon(Resources::import16));
  dialog.setFileMode(QFileDialog::AnyFile);

  for(;;)
  {
    if(dialog.exec())
    {
      int length=dialog.selectedFiles().length();
      if(length>0 && length==1)
      {
        QFile file(dialog.selectedFiles().at(0));
        if(file.exists())
        {
          file.open(QIODevice::ReadOnly);
          if(file.isReadable())
          {
            if(file.bytesAvailable()<=((1024*1024)*2))
            {
              QByteArray ba=file.readAll();
              if(!ba.isEmpty())
              {
                QString data=QTextCodec::codecForMib(106)->toUnicode(ba);
                if(_str)
                {
                  *_str=data;
                  ui->editor->setPlainText(*_str);
                  break;
                }
                else
                {
                  break;
                }
              }
              else
              {
                int messageResult= QMessageBox::critical(this,"slected file is empty","the file you selected is empty. \nnothing to read.\ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
                if(messageResult==QMessageBox::Ok)
                {
                  continue;
                }
                else
                {
                  break;
                }
              }
            }
            else
            {
              int messageResult= QMessageBox::critical(this,"maximum file size is 2 mb","maximum file size is 2 megabytes.\ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
              if(messageResult==QMessageBox::Ok)
              {
                continue;
              }
              else
              {
                break;
              }
            }

          }
          else
          {
            int messageResult= QMessageBox::critical(this,"cannot read selected file","cannot read selected file \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
            if(messageResult==QMessageBox::Ok)
            {
              continue;
            }
            else
            {
              break;
            }
          }
        }
        else
        {
          int messageResult= QMessageBox::critical(this,"file not exist","the file you selected is not exist. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
          if(messageResult==QMessageBox::Ok)
          {
            continue;
          }
          else
          {
            break;
          }
        }
      }
    }
    else
    {
      break;
    }
  }


}


void TextEditor::on_exportButton_clicked()
{
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
          qint64 i= file.write(_str->toUtf8());
          if(i<0)
          {
            int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write to the selected file.nothing saved. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
            if(messageResult==QMessageBox::Ok)
            {
              continue;
            }
            else
            {
              break;
            }
          }
          else
          {
            break;
          }
        }
        else
        {
          int messageResult= QMessageBox::critical(this,"cannot write to the file.","cannot write selected file. \ntry another file.",QMessageBox::Ok,QMessageBox::Cancel);
          if(messageResult==QMessageBox::Ok)
          {
            continue;
          }
          else
          {
            break;
          }
        }

      }
    }
    else
    {
      break;
    }
  }

}
