#ifndef XBMCLIBRARY_H
#define XBMCLIBRARY_H

#include "xbmcmodel.h"
#include "libraryitem.h"

class XbmcLibrary : public XbmcModel
{
    Q_OBJECT
public:
    XbmcLibrary(XbmcModel *parent = 0);

    Q_INVOKABLE virtual XbmcModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual XbmcModel *exit();

    Q_INVOKABLE virtual void playItem(int index) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QVariant get(int index, const QString &roleName);
    Q_INVOKABLE XbmcModelItem *getItem(int index);

    Q_INVOKABLE virtual void fetchItemDetails(int index) { Q_UNUSED(index) }
    Q_INVOKABLE virtual bool hasDetails() { return false; }

};

#endif // XBMCLIBRARY_H
