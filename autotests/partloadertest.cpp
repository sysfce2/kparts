/*
    SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include <KParts/PartLoader>
#include <KParts/ReadOnlyPart>
#include <QTest>

#include <KPluginMetaData>
#include <QDebug>
#include <QStandardPaths>

class PartLoaderTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);

        // Ensure notepadpart is preferred over other installed parts.
        // This also tests the mimeapps.list parsing in PartLoader
        const QByteArray contents =
            "[Added KDE Service Associations]\n"
            "text/plain=notepad.desktop;\n";
        const QString configDir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
        QDir().mkpath(configDir);
        const QString mimeAppsPath = configDir + QLatin1String("/mimeapps.list");
        QFile mimeAppsFile(mimeAppsPath);
        QVERIFY(mimeAppsFile.open(QIODevice::WriteOnly));
        mimeAppsFile.write(contents);
    }

    void shouldListParts()
    {
        // GIVEN
        const QString mimeType = QStringLiteral("text/plain");

        // WHEN
        const QList<KPluginMetaData> plugins = KParts::PartLoader::partsForMimeType(mimeType);

        // THEN
        QVERIFY(!plugins.isEmpty());
        //        for (const KPluginMetaData &metaData : plugins) {
        //            qDebug() << metaData.fileName() << metaData.pluginId();
        //        }
        QCOMPARE(plugins.at(0).pluginId(), QStringLiteral("notepadpart")); // basename of plugin
        const QString fileName = plugins.at(0).fileName();
        QVERIFY2(fileName.contains(QLatin1String("notepadpart")), qPrintable(fileName));
    }

    void shouldLoadPlainTextPart()
    {
        // GIVEN
        const QString mimeType = QStringLiteral("text/plain");
        const QString testFile = QFINDTESTDATA("partloadertest.cpp");
        QVERIFY(!testFile.isEmpty());
        QWidget parentWidget;
        QString errorString;

        // WHEN
        auto res = KParts::PartLoader::instantiatePartForMimeType<KParts::ReadOnlyPart>(mimeType, &parentWidget, this);

        // THEN
        QVERIFY(res);
        QCOMPARE(errorString, QString());
        QCOMPARE(res.plugin->metaObject()->className(), "NotepadPart");
        QVERIFY(res.plugin->openUrl(QUrl::fromLocalFile(testFile)));
    }

    void shouldHandleNoPartError()
    {
        // GIVEN
        // can't use an unlikely mimetype here, okteta is associated with application/octet-stream :-)
        const QString mimeType = QStringLiteral("does/not/exist");
        QWidget parentWidget;

        // WHEN
        const KPluginFactory::Result result = KParts::PartLoader::instantiatePartForMimeType<KParts::ReadOnlyPart>(mimeType, &parentWidget, this, {});
        KParts::ReadOnlyPart *part = result.plugin;
        QString errorString = result.errorString;

        // THEN
        QVERIFY2(!part, part ? part->metaObject()->className() : nullptr);
        QCOMPARE(errorString, QStringLiteral("No part was found for mimeType does/not/exist"));
    }

    void shouldInstantiatePart()
    {
        // GIVEN
        const KPluginMetaData md(QStringLiteral("kf6/parts/notepadpart"));
        QVERIFY(md.isValid());

        QWidget parentWidget;

        // WHEN
        const KPluginFactory::Result result = KParts::PartLoader::instantiatePart<KParts::ReadOnlyPart>(md, &parentWidget, this, {});

        // THEN
        QVERIFY(result);
        QVERIFY(result.plugin);
        QCOMPARE(result.plugin->metaObject()->className(), "NotepadPart");
    }
};

QTEST_MAIN(PartLoaderTest)

#include "partloadertest.moc"
