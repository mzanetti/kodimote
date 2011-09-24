#include "xbmclibrary.h"

XbmcLibrary::XbmcLibrary(XbmcModel *parent) :XbmcModel(parent)
{
}

XbmcModel* XbmcLibrary::exit()
{
    deleteLater();
    return m_parentModel;
}
