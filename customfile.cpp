#include "customfile.h"

CustomFile::CustomFile()
{

}

CustomFile::CustomFile(int index, QString filePath, QString currentText)
{
    this->index = index;
    this->filePath = filePath;
    setFileNameWithFilePath(filePath);
    this->currentText = currentText;
};

CustomFile::CustomFile(int index, QString filePath, QString fileName, QString currentText)
{
    this->index = index;
    this->filePath = filePath;
    this->fileName = fileName;
    this->currentText = currentText;
};

void CustomFile::setFileNameWithFilePath(QString filePath)
{
    for (int i=filePath.length();i>0;i--) {
        if (filePath[i].toLatin1()=='/') {
            this->fileName = QString::fromStdString(filePath.toStdString().substr(i+1));
            break;
        }
    }
}
