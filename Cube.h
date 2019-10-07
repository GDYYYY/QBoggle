#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>

class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);
    QString getLetter(){return label->text();}
    void mousePressEvent(QMouseEvent *event) override;
    int numberOfThisCube;

signals:
    void click(int);

public slots:

private:
    QLabel *label;
};

#endif // CUBE_H
