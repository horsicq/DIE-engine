#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "binary.h"

class TextFile : public Binary
{
    Q_OBJECT
public:
    explicit TextFile(QObject *parent = 0);
    
public slots:
    bool isValid();
};

#endif // TEXTFILE_H
