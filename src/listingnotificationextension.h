/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2012 Dawit Alemayehu <adawit@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPARTS_LISTINGNOTIFICATIONEXTENSION_H
#define KPARTS_LISTINGNOTIFICATIONEXTENSION_H

#include <kparts/kparts_export.h>

#include <QObject>
#include <memory>

class KFileItemList;

namespace KParts
{
class ReadOnlyPart;
class ListingNotificationExtensionPrivate;

/*!
 * \class KParts::ListingNotificationExtension
 * \inheaderfile KParts/ListingNotificationExtension
 * \inmodule KParts
 *
 * \brief An extension for receiving listing change notification.
 *
 * This extension is intended for implementation by parts that provide listing
 * services, e.g. file management and is intended to notify about changes to
 * a given listing. For example, if file management part implemented this extension
 * it would emit itemsDeleted and itemsAdded signal whenever new files
 * or folders are deleted and added to a directory respectively.
 *
 * \since 4.9.2
 */
class KPARTS_EXPORT ListingNotificationExtension : public QObject
{
    Q_OBJECT

public:
    /*!
     * Supported notification event types.
     *
     * \value None
     * \value ItemsAdded New items added to the listing.
     * \value ItemsDeleted Items deleted from the listing.
     *
     */
    enum NotificationEventType {
        None = 0x00,
        ItemsAdded = 0x01,
        ItemsDeleted = 0x02,
    };

    Q_DECLARE_FLAGS(NotificationEventTypes, NotificationEventType)

    /*! Constructor */
    ListingNotificationExtension(KParts::ReadOnlyPart *parent);

    ~ListingNotificationExtension() override;

    /*!
     * Returns the OR'ed value of the notification types supported by the part
     * that implements this extension.
     *
     * By default this function returns None.
     */
    virtual NotificationEventTypes supportedNotificationEventTypes() const;

    /*!
     * Queries \a obj for a child object which inherits from this class.
     */
    static ListingNotificationExtension *childObject(QObject *obj);

Q_SIGNALS:
    /*!
     * This signal is emitted when one of the notification events listed
     * in NotificationEventType occur.
     */
    void listingEvent(KParts::ListingNotificationExtension::NotificationEventType, const KFileItemList &);

private:
    std::unique_ptr<ListingNotificationExtension> const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ListingNotificationExtension::NotificationEventTypes)

}

#endif /* KPARTS_LISTINGNOTIFICATIONEXTENSION_H */
