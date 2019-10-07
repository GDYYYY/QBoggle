#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr);

signals:
    void newLineWritten(QString newline);
    void TurnToComputer();

public slots:
    void clear();
    void write(QString msg);
    void ifExist();
    void success();
    void fail();
    void illegal();
    void end(bool);
    void warn();
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_H
