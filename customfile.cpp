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

void CustomFile::updateFilenameFromFilepath()
{
    setFileNameWithFilePath(filePath);
}

void CustomFile::setFileNameWithFilePath(QString path)
{
    for (int i=path.length();i>0;i--) {
        if (path[i].toLatin1()=='/') {
            this->fileName = QString::fromStdString(path.toStdString().substr(i+1));
            break;
        }
    }
}

QString CustomFile::getFileNameWithFilePath(QString path)
{
    for (int i=path.length();i>0;i--) {
        if (path[i].toLatin1()=='/') {
            return QString::fromStdString(path.toStdString().substr(i+1));
        }
    }
}

QString CustomFile::getEnding(QString fileName)
{
    int i;
    for (i=fileName.length();i>0;i--) {
        if (fileName[i] == '.') {
            break;
        }
    }
    return QString::fromStdString(fileName.toStdString().substr(i));
}
