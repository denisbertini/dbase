#ifndef FAIRDBWTFILEMODEL_H
#define FAIRDBWTFILEMODEL_H

#include <fstream>

#include <WStandardItem>
#include <WStandardItemModel>


#include "FolderView.h"


using namespace Wt;


class FairDbWtFileModel : public WStandardItemModel
{
public:
  FairDbWtFileModel(WObject *parent)
    : WStandardItemModel(parent) { }

  virtual std::string mimeType() const;

  static WString dateDisplayFormat;
  static WString dateEditFormat;
};



#endif //FAIRDBWTFILEMODEL_H
