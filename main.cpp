#include <QDebug>
#include <QString>
#include <QVector>
#include <QPair>
#include <QRegExp>
#include <QFile>
#include <QException>
#include <QMultiMap>
#include <QMap>
#include "BinaryTree.hpp"

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

class LE_OpenSquareBracketKeyWord : public ILexicalExpression
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

class LE_Delimiter : public ILexicalExpression
{
    QString getType() const
    {
        return "Delimiter";
    }

    bool isThis(QString input)
    {
        return input == ";";
    }
};
//

class ParseException : public QException
{
public:
    void raise() const { throw *this; }
    ParseException *clone() const { return new ParseException(*this); }
};

struct SyntaxNode
{
    ILexicalExpression *item;
    QList<SyntaxNode*> next;
    bool is_body = false;
};

QMap<QString, ILexicalExpression*> validExpressions;
QList<QPair<QString, ILexicalExpression*> > parsedExpressions;

void pushValidExpressions()
{
    validExpressions.insert("LE_Delimiter", new LE_Delimiter());
    validExpressions.insert("LE_ForOperator", new LE_ForOperator());
    validExpressions.insert("LE_DoOperator", new LE_DoOperator());
    validExpressions.insert("LE_CompareLessThanOperator", new LE_CompareLessThanOperator());
    validExpressions.insert("LE_CompareGreaterThanOperator", new LE_CompareGreaterThanOperator());
    validExpressions.insert("LE_CompareEqualsOperator", new LE_CompareEqualsOperator());
    validExpressions.insert("LE_AssignOperator", new LE_AssignOperator());
    validExpressions.insert("LE_NumberConstant", new LE_NumberConstant());
    validExpressions.insert("LE_HexNumberConstant", new LE_HexNumberConstant());
    validExpressions.insert("LE_OpenBracketKeyWord", new LE_OpenBracketKeyWord());
    validExpressions.insert("LE_CloseBracketKeyWord", new LE_CloseBracketKeyWord());
    validExpressions.insert("LE_OpenSquareBracketKeyWord", new LE_OpenSquareBracketKeyWord());
    validExpressions.insert("LE_CloseSquareBracketKeyWord", new LE_CloseSquareBracketKeyWord());
    validExpressions.insert("LE_PlusOperator", new LE_PlusOperator());
    validExpressions.insert("LE_MinusOperator", new LE_MinusOperator());
    validExpressions.insert("LE_IntType", new LE_IntType());
    validExpressions.insert("LE_DoubleType", new LE_DoubleType());

    QMapIterator<QString, ILexicalExpression*> iterator(validExpressions);

    // Операторы
    SyntaxNode *node_delimiter = new SyntaxNode();
    node_delimiter->item = validExpressions.value("LE_Delimiter");

    SyntaxNode *node_open_bracket = new SyntaxNode();
    node_open_bracket->item = validExpressions.value("LE_OpenBracketKeyWord");

    SyntaxNode *node_close_bracket = new SyntaxNode();
    node_close_bracket->item = validExpressions.value("LE_CloseBracketKeyWord");

    SyntaxNode *node_assign_operator = new SyntaxNode();
    node_assign_operator->item = validExpressions.value("LE_AssignOperator");

    SyntaxNode *node_open_square_bracket = new SyntaxNode();
    node_open_square_bracket->item = validExpressions.value("LE_OpenSquareKeyWord");

    SyntaxNode *node_close_square_bracket = new SyntaxNode();
    node_close_square_bracket->item = validExpressions.value("LE_CloseSquareBracketKeyWord");

    SyntaxNode *node_do_operator = new SyntaxNode();
    node_do_operator->item = validExpressions.value("LE_DoOperator");

    SyntaxNode *node_subprogram_body = new SyntaxNode(); // Сюда поидее залетят все возможные варианты, но как???
    node_subprogram_body->is_body = true;
    //

    /*for (A; B; C) {
                 * D
                }

                A - строго оператор присвоения
                B - строго оператор условия
                C - строго оператор присвоения
                D - любая последовательность операторов*/

    SyntaxNode *node_for = new SyntaxNode();
    node_for->item = validExpressions.value("LE_ForOperator");

    node_for->next.append(node_open_bracket);
    node_for->next.append(node_assign_operator);
    node_for->next.append(node_delimiter);
    node_for->next.append(node_condition);
    node_for->next.append(node_delimiter);
    node_for->next.append(node_assign_operator);
    node_for->next.append(node_close_bracket);
    node_for->next.append(node_do_operator);
    node_for->next.append(node_open_square_bracket);
    node_for->next.append(node_subprogram_body);
    node_for->next.append(node_close_square_bracket);
    //
}

void checkWord(QString input)
{
    QListIterator<ILexicalExpression*> iterator(validExpressions.values());

    while (iterator.hasNext())
    {
        ILexicalExpression* expression = iterator.next();

        if (expression->isThis(input))
        {
            QPair<QString, ILexicalExpression*> pair(input, expression);
            parsedExpressions.append(pair);

            return;
        }
    }

    // Not found
}

void closeResources()
{
    QListIterator<ILexicalExpression*> iterator(validExpressions.values());

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
                        if (str2.length() != 0) {
                            checkWord(str2);
                            checkWord(Delimiter);
                        }
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

    QListIterator<QPair<QString, ILexicalExpression*> > iterator(parsedExpressions);

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
