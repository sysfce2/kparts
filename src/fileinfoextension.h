/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2010 Dawit Alemayehu <adawit@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_FILEINFOEXTENSION_H
#define KPARTS_FILEINFOEXTENSION_H

#include <kparts/kparts_export.h>

#include <KFileItem>
#include <QObject>
#include <memory>

class KFileItemList;

namespace KParts
{
class ReadOnlyPart;
class FileInfoExtensionPrivate;

/*!
 * \class KParts::FileInfoExtension
 * \inheaderfile KParts/FileInfoExtension
 * \inmodule KParts
 *
 * \brief An extension for obtaining file information from the part.
 *
 * This extension provides information about file and directory resources
 * that are present in the part the implements it.
 *
 * The main purpose of for this extension is to provide information about
 * files and directories located on remote servers so that download managers
 * such as kget can easily retrieve these resources.
 *
 * \since 4.6
 */
class KPARTS_EXPORT FileInfoExtension : public QObject
{
    Q_OBJECT

public:
    /*!
     * Supported file information retrieval modes.
     *
     * \value None Querying for file information is NOT possible
     * \value AllItems Retrieve or can retrieve file information for all items
     * \value SelectedItems Retrieve or can retrieve file information for selected items
     */
    enum QueryMode {
        None = 0x00,
        AllItems = 0x01,
        SelectedItems = 0x02,
    };
    Q_DECLARE_FLAGS(QueryModes, QueryMode)

    /*!
     *
     */
    explicit FileInfoExtension(KParts::ReadOnlyPart *parent);

    ~FileInfoExtension() override;

    /*!
     * Queries \a obj for a child object which inherits from this class.
     */
    static FileInfoExtension *childObject(QObject *obj);

    /*!
     * Returns \c true if any of the items in the current view of the part that
     * implements this extension are selected.
     *
     * By default this function returns \c false.
     */
    virtual bool hasSelection() const;

    /*!
     * Returns the file information retrieve modes supported by the part
     * that implements this extension.
     *
     * By default this function returns None.
     */
    virtual QueryModes supportedQueryModes() const;

    /*!
     * Returns a information for files that match the specified query \a mode.
     *
     * If the mode specified by \a mode is not supported or cannot be
     * handled, then an empty list is returned.
     */
    virtual KFileItemList queryFor(QueryMode mode) const = 0;

private:
    std::unique_ptr<FileInfoExtensionPrivate> const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FileInfoExtension::QueryModes)

}

#endif /* KPARTS_FILEINFOEXTENSION_H */
