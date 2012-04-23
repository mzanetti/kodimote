#ifndef XBMCLIBRARY_H
#define XBMCLIBRARY_H

#include "xbmcmodel.h"

class LibraryItem;
class XbmcDownload;

class XbmcLibrary : public XbmcModel
{
    Q_OBJECT
public:
    XbmcLibrary(XbmcModel *parent = 0);
    ~XbmcLibrary();

    Q_INVOKABLE virtual XbmcModel *enterItem(int index) = 0;
    Q_INVOKABLE virtual XbmcModel *exit();

    Q_INVOKABLE virtual void playItem(int index) = 0;
    Q_INVOKABLE virtual void addToPlaylist(int index) = 0;

    virtual QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QVariant get(int index, const QString &roleName);
    Q_INVOKABLE XbmcModelItem *getItem(int index);

    Q_INVOKABLE virtual void fetchItemDetails(int index) { Q_UNUSED(index) }
    Q_INVOKABLE virtual bool hasDetails() { return false; }

    Q_INVOKABLE virtual void download(int index, const QString &path);
    Q_INVOKABLE void setDeleteAfterDownload(bool deleteAfterDownload);
    Q_INVOKABLE bool deleteAfterDownload() const;

protected:
    void startDownload(int index, XbmcDownload *download);

private slots:
    void slotResponseReceived(int, const QVariantMap &rsp);

private:
    enum Request {
        RequestDownload
    };
    QMap<int, Request> m_requestMap;
    QMap<int, XbmcDownload*> m_downloadMap;
    bool m_deleteAfterDownload;

};

#endif // XBMCLIBRARY_H
