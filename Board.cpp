#include "Board.h"
#include "Cube.h"
#include <ctime>
#include <QGridLayout>
#include "lexicon.h"
#include <QFile>


const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    qsrand(time(NULL));
    this->size = size;
    this->cubes = new Cube*[size * size];

    this->letters = new QString[size * size];
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];

    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);
    layout->setSpacing(10);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);
        }
    }
    setLayout(layout);

    shake();


    for(int i=0;i<size*size;i++)
    {
        connect(cubes[i],SIGNAL(click(int)),this,SLOT(check(int)));
        cubes[i]->numberOfThisCube=i;
    }
    // this->setStyleSheet("background-color:grey; border: 3px solid");
    h[0].x=-1;h[0].y=-1;
    h[1].x=0;h[1].y=-1;
    h[2].x=1;h[2].y=-1;
    h[3].x=1;h[3].y=0;
    h[4].x=1;h[4].y=1;
    h[5].x=0;h[5].y=1;
    h[6].x=-1;h[6].y=1;
    h[7].x=-1;h[7].y=0;//各个方向（逆时针）

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
   lex =new Lexicon (qFile);
}

Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
}

void Board::shake()
{

        for(int i=0;i<size*size;i++)
           swap(letters[i],letters[rand()%(i+1)]);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int x=rand()%6;
                if(this->letters[index(i, j)].at(x)=="Q")
                    this->cubes[index(i, j)]->setLetter("QU");
                else
                this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(x));
            }
        }// Shake Cubes
}
void Board::reshake()
{

        for(int i=0;i<size*size;i++)
           swap(letters[i],letters[rand()%(i+1)]);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int x=rand()%6;
                if(this->letters[index(i, j)].at(x)=="Q")
                    this->cubes[index(i, j)]->setLetter("QU");
                else
                this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(x));
            }
        }// Shake Cubes
}

//人工输入的单词能否在盘上找到
void Board::if_CanFind(QString word)
{
   // int count=0;
    //int length=word.length();
    bool ans=false;

    for(int x=0;x<size*size;x++)
        cubes[x]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
    for(int i=0;i<25;i++)
        flag[i]=0;//初始化

    for(int i=0;i<size*size;i++)
    {
        if(cubes[i]->getLetter()==word[0])
        {
            flag[i]=1;
            if(word[0]=='Q'&&word[1]=='U')
                ans=search(word,2,i/size,i%size);
           // count++;
            ans=search(word,1,i/size,i%size);
            flag[i]=0;
            if(ans) {
                cubes[index(i/size,i%size)]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
                emit find(word);//发给console和me
                break;
            }

        }

    }
    if(!ans) emit notfind();//发给console
}

//检查人工输入单词的递归
bool Board::search(QString word,int count,int x,int y)
{
    if (count==word.length())
        return true;//找到即成功
    int x1,y1;
    for(int i=0;i<8;i++)
    {       
        x1=x+h[i].x;
        y1=y+h[i].y;
        if(x1>=0&&x1<5&&y1>=0&&y1<5&&!flag[index(x1,y1)]&&cubes[index(x1,y1)]->getLetter()==word[count])
        {
            flag[index(x1,y1)]=1;//标记走过
            if(word[count]=='Q'&&word[count+1]=='U')
            {
                if(search(word,count+2,x1,y1))
                {
                    cubes[index(x1,y1)]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
                    return true;
                }
            }
            else if(search(word,count+1,x1,y1))
            {
                cubes[index(x1,y1)]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
                return true;
            }

        }

    }
    flag[index(x,y)]=0;
    return false;
}

void Board::computerTurn()
{
    int count=0;
    int x,y,x1,y1;
    QString word;
    for(int i=0;i<size*size;i++)
    {
        for(int j=0;j<25;j++)
            flag[j]=0;
        word=word.left(0);
        count=0;
        word+=cubes[i]->getLetter();
        if(word[count]=='Q') {count++;}
        x=i/size;
        y=i%size;
        flag[i]=1;//初始化


        for(int j=0;j<8;j++)
        {
            word=word.left(count+1);
            x1=x+h[j].x;
            y1=y+h[j].y;
            if(!(x1>=0&&x1<5&&y1>=0&&y1<5&&!flag[index(x1,y1)])) continue;

            //count++;

            word+=cubes[index(x1,y1)]->getLetter();
            //if(word[count]=='Q') { count++;}
            if( checkAIpre(word))
                consearch(word,x1,y1);
        }
    }

    emit CompareTheResult();
}

//检查电脑搜索出的是不是单词
bool Board::checkAI(QString word)
{
    std::string str;
    str=word.toStdString();
    if(lex->contains(str)&&word.length()>3)
    {
       emit newLineWritten(word);
       return true;}
    else return false;
}

//检查电脑搜出的是不是合法前缀
bool Board::checkAIpre(QString word)
{
    std::string str;
    str=word.toStdString();
    if(lex->containsPrefix(str))
       return true;
    else return false;
}

//是否已经在清单中存在
void Board::ifExist()
{
    ifExistflag=true;
}

void Board::consearch(QString word,int x,int y)
{
    flag[index(x,y)]=1;
    checkAI(word);//是单词就加入清单
    int x1,y1;
    for(int i=0;i<8;i++)
    {
        x1=x+h[i].x;
        y1=y+h[i].y;
        QString word1;
        if(x1>=0&&x1<5&&y1>=0&&y1<5&&!flag[index(x1,y1)])
            word1=word+cubes[index(x1,y1)]->getLetter();

        else continue;
        if(checkAIpre(word1))
           consearch(word1,x1,y1);
    }
    flag[index(x,y)]=0;
}

//检查用鼠标点击出的单词
void Board::check(int numofnow)
{
    QString letter=cubes[numofnow]->getLetter();
    bool legalclick=false;
    int x=numOflastLetter/size;
    int y=numOflastLetter%size;
    int x1,y1;
    x1=numofnow/size;
    y1=numofnow%size;
    ifExistflag=false;

    if(numOflastLetter==-1) {
        for(int i=0;i<size*size;i++)
        {cubes[i]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
            flag[i]=0;}//初始化
        cubes[numofnow]->setStyleSheet("background-color: yellow; border-radius: 15px; border: 2px solid");
        numOflastLetter=numofnow;legalclick=true;
    }
    else {
        for(int i=0;i<8;i++)
        {
            if(((x1+h[i].x)==x&&(y1+h[i].y)==y)||flag[index(x1,y1)])//判断是否遵循路线规则
            {legalclick=true;break;}
        }
    }
    if(!legalclick) {
        cubes[numofnow]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
        emit illegalclick(); }//不合理且不重复的点击不强调
    else if(flag[index(x1,y1)]) emit illegalclick();//重复点击同样不合理
    else{
    clickword+=letter;
    numOflastLetter=numofnow;
    flag[numofnow]=1;

    if(!checkAIpre(clickword)||checkauto(clickword))
    {
        for(int i=0;i<size*size;i++)
        {cubes[i]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
        flag[i]=0;}
        clickword=clickword.left(0);
        numOflastLetter=-1;
    }
    }
}

//检查鼠标点出的单词是否正确且不存在于清单中
bool Board::checkauto(QString word)
{
    std::string str;
    str=word.toStdString();
    if(lex->contains(str)&&word.length()>3)
    {
       emit newoneword(word);
        if(!ifExistflag)
        return true;
        else return false;
    }
    else return false;
}
