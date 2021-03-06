/**
 * @file app/main.cpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <qtapp/mainwindow.h>
#include <QApplication>
#include <QDebug>
#include <QDate>

#include <artis/kernel/Simulator.hpp>
#include <artis/utils/DateTime.hpp>
#include <artis/utils/Trace.hpp>

#include <defines.hpp>
#include <observer/PlantView.hpp>
#include <plant/PlantModel.hpp>
#include <utils/ParametersReader.hpp>

using namespace artis::kernel;

typedef artis::kernel::Simulator < PlantModel,
                                   artis::utils::DoubleTime,
                                   ecomeristem::ModelParameters,
                                   GlobalParameters > EcomeristemSimulator;

typedef artis::context::Context < artis::utils::DoubleTime > EcomeristemContext;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    GlobalParameters globalParameters;
    std::string dirName = "D:/PAMStudio_dev/data/ecomeristem/sample";
    ecomeristem::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromFiles(dirName, parameters);

    QDate start = QDate::fromString(QString::fromStdString(parameters.get < std::string >("BeginDate")),
                                    "dd/MM/yyyy");
    QDate end = QDate::fromString(QString::fromStdString(parameters.get < std::string >("EndDate")),
                                    "dd/MM/yyyy");
    parameters.beginDate = start.toJulianDay();
    qDebug() << parameters.beginDate << end.toJulianDay();
    EcomeristemContext context( start.toJulianDay(), end.toJulianDay());

    ::Trace::trace().clear();
    EcomeristemSimulator simulator(new PlantModel, globalParameters);
    observer::PlantView * view = new observer::PlantView();
    simulator.attachView("plant", view);
    simulator.init(start.toJulianDay(), parameters);
    simulator.run(context);
    w.show_trace();
    w.displayData(view, QString::fromStdString(dirName), &parameters,
                  QString::fromStdString(parameters.get < std::string >("BeginDate")),
                  QString::fromStdString(parameters.get < std::string >("EndDate")));

    return a.exec();
}
