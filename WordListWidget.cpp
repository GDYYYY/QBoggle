#include "WordListWidget.h"
#include "WordTable.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>

WordListWidget::WordListWidget(QWidget *parent, QString label) : QWidget(parent)
{
    reset();

    QVBoxLayout *layout = new QVBoxLayout();
    headLayout = new QHBoxLayout();

    QLabel *nameLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPointSize(20);
    nameLabel->setFont(font);
    scoreLabel->setFont(font);
    nameLabel->setText(label);
    scoreLabel->setText(QString::number(score));

    headLayout->addWidget(nameLabel, 0, Qt::AlignmentFlag::AlignLeft);
    headLayout->addWidget(scoreLabel, 0, Qt::AlignmentFlag::AlignRight);
    layout->addLayout(headLayout);

    QFrame *hline = new QFrame();
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hline);

    wordTable = new WordTable();
    layout->addWidget(wordTable);

    setLayout(layout);
}

void WordListWidget::addScore(int score)
{
    this->score += score;
}
void WordListWidget::addWord(QString word)
{
    this->words.append(word);
    this->wordTable->addWord(word);
}
void WordListWidget::reset()
{
    this->score = 0;
    this->words.clear();
}
void WordListWidget::ifNew(QString word)
{
    bool flag=true;
    for(int i=0;i<words.length();i++)
        if(words[i]==word) {emit exist(); flag=false; break;}
    if(flag) emit notexist(word);

}
void WordListWidget::ifrepeat(QString word)
{
    bool flag=true;
    for(int i=0;i<words.length();i++)
        if(words[i]==word) {flag=false; break;}
    if(flag) emit ifnew(word);

}
void WordListWidget::add(QString word)
{
    addWord(word);
    addScore(word.length()-3);
    scoreLabel->setText(QString::number(score));
    headLayout->addWidget(scoreLabel, 0, Qt::AlignmentFlag::AlignRight);
}

void WordListWidget::ifnewword(QString word)
{
    bool flag=true;
    for(int i=0;i<words.length();i++)
        if(words[i]==word) {emit existflag(); flag=false; break;}
    if(flag) add(word);

}
