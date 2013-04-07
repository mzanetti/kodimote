/*****************************************************************************
 * Copyright: 2011-2013 Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Xbmcremote                                           *
 *                                                                           *
 * Xbmcremote is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * Xbmcremote is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#ifndef XBMCCONTAINER_H
#define XBMCCONTAINER_H

#include "libxbmcremote/audiolibrary.h"
#include <Plasma/DataContainer>
#include <QDebug>

class ModelContainer;

class XbmcContainer: public Plasma::DataContainer
{
    Q_OBJECT
public:
  XbmcContainer(QObject *parent = 0);

  
private slots:
  
  void volumeChanged();
  void stateChanged();
  void connectedChanged(bool connected);

private:
  ModelContainer *m_container;
};

class ModelContainer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioLibrary* model READ model)
public:
    ModelContainer(AudioLibrary *model = 0, QObject *parent = 0): QObject(parent), m_model(model) {}

    Q_INVOKABLE AudioLibrary* model() { qDebug() << "PopupDialog************************" << m_model; return m_model; }

private:
    AudioLibrary *m_model;
};

Q_DECLARE_METATYPE(ModelContainer*)

#endif
