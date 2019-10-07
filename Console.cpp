#include "Console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>

Console::Console(QWidget *parent) : QTextEdit(parent)
{
}

void Console::clear()
{
    this->clear();
}

void Console::write(QString msg)
{
    this->append(msg);
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
        return;
    if (event->key() == Qt::Key_Delete)
        return;
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        lastLine=lastLine.toUpper();
        if(lastLine.length()==0) {write("Now, it's computer's turn.\n"); emit TurnToComputer();}
        else if(lastLine.length()<4) {write("This word is too short!\n"); return;}
        else
        emit newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}

void Console::ifExist()
{
    write("This word already exists!");

}
void Console::success()
{
    write("Success!");

}
void Console::fail()
{
    write("Can't find!");

}
void Console::illegal()
{
    write("It is not a word!");

}

void Console::end(bool ans)
{
    if(ans) write("Computer wins! You are a loser.");
    else write("Unbelievable! You beat the computer! ");
}

void Console::warn()
{
    write("Please click legally!\n");
}
