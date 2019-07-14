#include <QFile>
#include <QDebug>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>

#include "sintacticanalyser/lexicalanalyser/token.h"
#include "sintacticanalyser/lexicalanalyser/lexicalanalyser.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    QCoreApplication a(argc, argv);

    qInfo() << "Parameters : " << a.arguments() ;
    qInfo() << "\n";

    if (argc > 1){
        QFile file(a.arguments()[1]);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QTextStream stream(&file);
            stream.setCodec("UTF-8"); //ISO 8859-1
            while(!stream.atEnd()){
                QString line = stream.readLine();
                qInfo() << "Line : " << line;
                LexicalAnalyser lLine(line);
                qInfo() << "Token count : " << lLine.tokenCount() << "\n";

                for(int i = 0; i < lLine.tokenCount(); ++i){
                    Token currentToken = lLine.nextToken();
                    qInfo() << "Token atual : " << currentToken.word()
                            << " : " << currentToken.type();
                }
                qInfo() << "------------------------------\n";
            }
            file.close();
        }
        else qInfo() << "Não foi possível abrir o arquivo.";
    }
    else qInfo() << "Nenhum arquivo especificado.";

    return 0; //a.exec()
}
