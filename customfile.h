#ifndef CUSTOMFILE_H
#define CUSTOMFILE_H

#include <QString>
#include <QFile>

class CustomFile
{
public:
    CustomFile();
    CustomFile(int index, QString filePath, QByteArray currentText);
    CustomFile(int index, QString filePath, QString fileName, QByteArray currentText);

    void updateFilenameFromFilepath();

    void setFileNameWithFilePath(QString filePath);
    static QString getFileNameWithFilePath(QString filePath);
    static QString getEnding(QString fileName);

    int index;
    QString fileName;
    QString filePath;
    QByteArray currentText;
    bool isSaved = true;
};

#endif // CUSTOMFILE_H
