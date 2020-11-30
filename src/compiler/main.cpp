/*
 *     This file is part of PIFC (Compiler).
 *
 *    PIFC (Compiler) is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Foobar is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QtCore>
#include <QCoreApplication>

#include "pifcompiler.h"

int main(int argc, char *argv[])
{
    setbuf(stdout, nullptr);
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    QCoreApplication a(argc, argv);

    PIFCompiler *compiler = new PIFCompiler();
    QTimer::singleShot(0, compiler, &PIFCompiler::execute);

    return a.exec();
}
