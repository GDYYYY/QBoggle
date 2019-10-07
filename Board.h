#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"
#include "lexicon.h"
#include <QWidget>
struct direct
{ int x;int y;};
class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    bool search(QString,int,int,int);
    bool checkAI(QString);
    bool checkAIpre(QString);
    bool consearch(QString,int,int);

signals:
    void find(QString);
    void notfind();
    void newLineWritten(QString newline);
    void CompareTheResult();

public slots:
    void if_CanFind(QString);
    void ifExist();
    void computerTurn();

private:
    int size;
    direct h[8];
    bool flag[25]={0};
    Cube **cubes;
    QString *letters;
    Lexicon *lex;
    bool ifExistflag=false;
    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
};

#endif // BOARD_H
