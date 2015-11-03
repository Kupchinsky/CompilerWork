#include <QDebug>
#include <QString>
#include <QVector>
#include <QPair>
#include <QRegExp>
#include <QFile>
#include <QException>

/*
 * 8. Входной язык содержит операторы цикла for (…; …; …) do …, разделённые символом ; (точка с запятой).
 * Операторы цикла содержат идентификаторы, знаки сравнения <, >, =, шестнадцатеричные числа, знак присваивания (:=).
 * Шестнадцатеричными числами считать последовательность цифр и симво-лов a, b, c, d, e, f, начинающуюся с цифры (например, 89, 45ac, 0abc).
*/

class ILexicalExpression
{
public:
    virtual QString getType() const = 0;
    virtual bool isThis(QString) = 0;

    virtual ~ILexicalExpression()
    {
    }
};

// Операторы цикла
class LE_ForOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Cycle initial operator";
    }

    bool isThis(QString input)
    {
        return input == "for";
    }
};

class LE_DoOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Cycle begin operator";
    }

    bool isThis(QString input)
    {
        return input == "do";
    }
};

class LE_CompareLessThanOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Compare less than operator";
    }

    bool isThis(QString input)
    {
        return input == "<";
    }
};

class LE_CompareGreaterThanOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Compare greater than operator";
    }

    bool isThis(QString input)
    {
        return input == ">";
    }
};

class LE_CompareEqualsOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Compare equals operator";
    }

    bool isThis(QString input)
    {
        return input == "=";
    }
};

class LE_AssignOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Assign operator";
    }

    bool isThis(QString input)
    {
        return input == ":=";
    }
};

class LE_PlusOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Plus operator";
    }

    bool isThis(QString input)
    {
        return input == "+";
    }
};

class LE_MinusOperator : public ILexicalExpression
{
    QString getType() const
    {
        return "Minus operator";
    }

    bool isThis(QString input)
    {
        return input == "-";
    }
};
//

// Числа
class LE_NumberConstant : public ILexicalExpression
{
    QString getType() const
    {
        return "Number constant";
    }

    bool isThis(QString input)
    {
        QRegExp pattern("^[0-9.e-]+$");
        return pattern.indexIn(input) != -1;
    }
};

class LE_HexNumberConstant : public ILexicalExpression
{
    QString getType() const
    {
        return "Hex number constant";
    }

    bool isThis(QString input)
    {
        QRegExp pattern("^0x[0-9a-zA-Z]+$");
        return pattern.indexIn(input) != -1;
    }
};
//

// Служебные символы (скобочки, точка с запятой)
class LE_OpenBracketKeyWord : public ILexicalExpression
{
    QString getType() const
    {
        return "Open bracket key word";
    }

    bool isThis(QString input)
    {
        return input == "(";
    }
};

class LE_CloseBracketKeyWord : public ILexicalExpression
{
    QString getType() const
    {
        return "Close bracket key word";
    }

    bool isThis(QString input)
    {
        return input == ")";
    }
};

class LE_OpenSquareKeyWord : public ILexicalExpression
{
    QString getType() const
    {
        return "Open square bracket key word";
    }

    bool isThis(QString input)
    {
        return input == "{";
    }
};

class LE_CloseSquareBracketKeyWord : public ILexicalExpression
{
    QString getType() const
    {
        return "Close square bracket key word";
    }

    bool isThis(QString input)
    {
        return input == "}";
    }
};
//

// Типы
class LE_IntType : public ILexicalExpression
{
    QString getType() const
    {
        return "Integer data type";
    }

    bool isThis(QString input)
    {
        return input == "int";
    }
};

class LE_DoubleType : public ILexicalExpression
{
    QString getType() const
    {
        return "Double data type";
    }

    bool isThis(QString input)
    {
        return input == "double";
    }
};
//

class ParseException : public QException
{
public:
    void raise() const { throw *this; }
    ParseException *clone() const { return new ParseException(*this); }
};

QVector<ILexicalExpression*> validExpressions;
QVector<QPair<QString, ILexicalExpression*> > parsedExpressions;

void pushValidExpressions()
{
    validExpressions.push_back(new LE_ForOperator());
    validExpressions.push_back(new LE_DoOperator());
    validExpressions.push_back(new LE_CompareLessThanOperator());
    validExpressions.push_back(new LE_CompareGreaterThanOperator());
    validExpressions.push_back(new LE_CompareEqualsOperator());
    validExpressions.push_back(new LE_AssignOperator());
    validExpressions.push_back(new LE_NumberConstant());
    validExpressions.push_back(new LE_HexNumberConstant());
    validExpressions.push_back(new LE_OpenBracketKeyWord());
    validExpressions.push_back(new LE_CloseBracketKeyWord());
    validExpressions.push_back(new LE_OpenSquareKeyWord());
    validExpressions.push_back(new LE_CloseSquareBracketKeyWord());
    validExpressions.push_back(new LE_PlusOperator());
    validExpressions.push_back(new LE_MinusOperator());
    validExpressions.push_back(new LE_IntType());
    validExpressions.push_back(new LE_DoubleType());
}

void checkWord(QString input)
{
    QVectorIterator<ILexicalExpression*> iterator(validExpressions);

    while (iterator.hasNext())
    {
        ILexicalExpression* expression = iterator.next();

        if (expression->isThis(input))
        {
            QPair<QString, ILexicalExpression*> pair(input, expression);
            parsedExpressions.push_back(pair);

            return;
        }
    }

    // Not found
}

void closeResources()
{
    QVectorIterator<ILexicalExpression*> iterator(validExpressions);

    while (iterator.hasNext())
        delete iterator.next();
}

#define Delimiter ";"

int main() // int argc, char *argv[]
{
    QFile file("input.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Open file error:" << file.errorString();
        return 1;
    }

    pushValidExpressions();

    QTextStream inStream(&file);

    try
    {
        while (!inStream.atEnd())
        {
            foreach (const QString &str, inStream.readLine().split(" "))
            {
                if (str.length() == 0)
                    continue;

                if (str.indexOf(Delimiter) != -1)
                {
                    foreach (const QString &str2, str.split(Delimiter))
                    {
                        if (str2.length() != 0)
                            checkWord(str2);
                    }
                }
                else
                    checkWord(str);
            }
        }
    }
    catch (ParseException &e)
    {
        file.close();
        qDebug() << "ParseException: " << e.what();
        return 1;
    }

    file.close();

    qDebug() << "Parse complete. Expressions found: " << parsedExpressions.size();

    QVectorIterator<QPair<QString, ILexicalExpression*> > iterator(parsedExpressions);

    int i = 0;
    while (iterator.hasNext())
    {
        i++;
        QPair<QString, ILexicalExpression*> pair = iterator.next();
        qDebug() << i << pair.first << "is" << pair.second->getType();
    }

    closeResources();
    return 0;
}
