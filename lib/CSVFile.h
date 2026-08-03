#pragma once

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

// Qt replacement for MFC's CStdioFileEx-based CCSVFile
class CCSVFile
{
public:
    CCSVFile();
    ~CCSVFile();

    // Opens the underlying text file and attaches a stream.
    // The original class was derived from CStdioFileEx; this method
    // provides the same "open‑then‑use" workflow.
    bool Open(const QString &fileName,
              QIODevice::OpenMode mode = QIODevice::ReadWrite | QIODevice::Text);
    void Close();
    void SetCodePage(int codePage) { m_codePage = codePage; }

    // Read one CSV line into a QStringList.
    // Returns true if a line was read (even if empty), false at EOF.
    bool ReadData(QStringList &arr);

    // Write a QStringList as a CSV line.
    void WriteData(const QStringList &arr);

private:
    QFile *m_file;
    QTextStream *m_stream;
    int m_codePage = -1;
};
