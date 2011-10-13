#ifndef XBMCLIBRARY_H
#define XBMCLIBRARY_H

#include "xbmcmodel.h"

class XbmcLibrary : public XbmcModel
{
    Q_OBJECT
public:
    XbmcLibrary(XbmcModel *parent = 0);

    Q_INVOKABLE virtual XbmcModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual XbmcModel *exit();

    Q_INVOKABLE virtual void playItem(int index) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;

    Q_INVOKABLE QVariant get(int index, const QString &roleName);

};

#endif // XBMCLIBRARY_H
