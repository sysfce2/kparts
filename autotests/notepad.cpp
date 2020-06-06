/*
    Copyright (c) 1999, 2000 David Faure <faure@kde.org>
    Copyright (c) 1999, 2000 Simon Hausmann <hausmann@kde.org>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License or ( at
    your option ) version 3 or, at the discretion of KDE e.V. ( which shall
    act as a proxy as in section 14 of the GPLv3 ), any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "notepad.h"
#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>

#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>

#include <kaboutdata.h>
#include <QDebug>
#include <kactioncollection.h>
#include <klocalizedstring.h>
#include <kpluginfactory.h>

K_PLUGIN_FACTORY_WITH_JSON(NotepadFactory,
                 "notepad.json",
                 registerPlugin<NotepadPart>();
                )

NotepadPart::NotepadPart(QWidget *parentWidget,
                         QObject *parent,
                         const QVariantList &)
    : KParts::ReadWritePart(parent)
{
    KAboutData aboutData(QStringLiteral("notepadpart"), QStringLiteral("Notepad Part"), QStringLiteral("0.1"));
    setComponentData(aboutData, false);

    m_edit = new QTextEdit(parentWidget);
    m_edit->setPlainText(QStringLiteral("NotepadPart's multiline edit"));
    setWidget(m_edit);

    QAction *searchReplace = new QAction(QStringLiteral("Search and replace"), this);
    actionCollection()->addAction(QStringLiteral("searchreplace"), searchReplace);
    connect(searchReplace, SIGNAL(triggered()), this, SLOT(slotSearchReplace()));

    setXMLFile(QStringLiteral("notepadpart.rc")); // will be found in the qrc resource

    setReadWrite(true);

    // Always write this as the last line of your constructor
    loadPlugins();
}

NotepadPart::~NotepadPart()
{
}

void NotepadPart::setReadWrite(bool rw)
{
    m_edit->setReadOnly(!rw);
    if (rw) {
        connect(m_edit, SIGNAL(textChanged()), this, SLOT(setModified()));
    } else {
        disconnect(m_edit, SIGNAL(textChanged()), this, SLOT(setModified()));
    }

    ReadWritePart::setReadWrite(rw);
}

KAboutData *NotepadPart::createAboutData()
{
    return new KAboutData(QStringLiteral("notepadpart"), i18n("Notepad"), QStringLiteral("2.0"));
}

bool NotepadPart::openFile()
{
    // qDebug() << "NotepadPart: opening " << localFilePath();
    QFile f(localFilePath());
    QString s;
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream t(&f);
        t.setCodec("UTF-8");
        s = t.readAll();
        f.close();
    }
    m_edit->setPlainText(s);

    emit setStatusBarText(url().toString());

    return true;
}

bool NotepadPart::saveFile()
{
    if (!isReadWrite()) {
        return false;
    }
    QFile f(localFilePath());
    if (f.open(QIODevice::WriteOnly)) {
        QTextStream t(&f);
        t.setCodec("UTF-8");
        t << m_edit->toPlainText();
        f.close();
        return true;
    } else {
        return false;
    }
}

void NotepadPart::slotSearchReplace()
{
}

#include "notepad.moc"