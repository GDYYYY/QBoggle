#ifndef WORDLISTWIDGET_H
#define WORDLISTWIDGET_H

#include "WordTable.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class WordListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordListWidget(QWidget *parent = nullptr, QString label = "");
    void addScore(int s);
    void addWord(QString word);
    void reset();
    int score;
signals:
    void exist();
    void existflag();
    void notexist(QString newword);
    void ifnew(QString newword);

public slots:
    void ifNew(QString newword);
    void ifrepeat(QString newword);
    void add(QString newword);
    void ifnewword(QString newword);


private:
    QString label;
    QList<QString> words;

    WordTable *wordTable;
    QHBoxLayout *headLayout;
    QLabel *scoreLabel;
};

#endif // WORDLISTWIDGET_H
