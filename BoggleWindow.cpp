#include "BoggleWindow.h"
#include "lexicon.h"
#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    connect(console,SIGNAL(newLineWritten(QString)),me,SLOT(ifNew(QString)));
    connect(me,SIGNAL(exist()),console,SLOT(ifExist()));
    connect(this,SIGNAL(legalword(QString)),board,SLOT(if_CanFind(QString)));
    connect(console,SIGNAL(TurnToComputer()),board,SLOT(computerTurn()));
    connect(board,SIGNAL(find(QString)),me,SLOT(add(QString)));
    connect(board,SIGNAL(find(QString)),console,SLOT(success()));
    connect(board,SIGNAL(notfind()),console,SLOT(fail()));
    connect(this,SIGNAL(illegalword()),console,SLOT(illegal()));
    connect(board,SIGNAL(newLineWritten(QString)),me,SLOT(ifrepeat(QString)));
    connect(me,SIGNAL(ifnew(QString)),computer,SLOT(ifNew(QString)));
    connect(computer,SIGNAL(exist()),board,SLOT(ifExist()));
    connect(computer,SIGNAL(notexist(QString)),computer,SLOT(add(QString)));
    connect(board,SIGNAL(CompareTheResult()),this,SLOT(compare()));
    connect(this,SIGNAL(result(bool)),console,SLOT(end(bool)));
    connect(board,SIGNAL(illegalclick()),console,SLOT(warn()));
    connect(board,SIGNAL(newoneword(QString)),me,SLOT(ifnewword(QString)));
    connect(me,SIGNAL(existflag()),board,SLOT(ifExist()));
    connect(&button,SIGNAL(clicked()),me,SLOT(resettable()));
    connect(&button,SIGNAL(clicked()),computer,SLOT(resettable()));
    connect(&button,SIGNAL(clicked()),console,SLOT(restart()));
    connect(&button,SIGNAL(clicked()),board,SLOT(reshake()));
    //connect(me,SIGNAL(notexist(QString)),me,SLOT(add(QString)));


    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320, 740, 260);

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
   lex =new Lexicon (qFile);
   connect(me,SIGNAL(notexist(QString)),this,SLOT(check(QString)));

    /*
    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
    */
    console->write("Welcome to the game Boggle!\n");

    button.setText("RESTART");
    button.setParent(this);
    button.resize(80,30);
    button.move(720,570);
    button.show();
}

BoggleWindow::~BoggleWindow()
{
}
void BoggleWindow::check(QString word)
{
    std::string str;
    str=word.toStdString();
    if(lex->contains(str))
        emit legalword(word);
    else emit illegalword();

}

void BoggleWindow::compare()
{
    emit result(computer->score > me->score);
}

/*void BoggleWindow::restart()
{
    me->resettable();
    computer->resettable();
    console->clear();
    board->shake();
}*/

/*void BoggleWindow::AIcheck(QString word)
{
    std::string str;
    str=word.toStdString();
    if(lex->containsPrefix(str))
        emit AIlegal();
   // else emit illegalword();

}*/
