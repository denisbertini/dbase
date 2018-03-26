#ifndef FAIRDBWTFILEEDITDIALOG_H
#define FAIRDBWTFILEEDITDIALOG_H

#include <fstream>

#include <WComboBox>
#include <WContainerWidget>
#include <WDatePicker>
#include <WDateValidator>
#include <WDialog>
#include <WIntValidator>
#include <WItemDelegate>
#include <WLabel>
#include <WLineEdit>
#include <WMessageBox>
#include <WPushButton>
#include <WRegExpValidator>
#include <WGridLayout>
#include <WPopupMenu>
#include <WSortFilterProxyModel>
#include <WStandardItem>
#include <WStandardItemModel>
#include <WTableView>
#include <WTreeView>
#include <WText>
#include <WVBoxLayout>


#include "CsvUtil.h"
#include "FolderView.h"


using namespace Wt;


class FairDbWtFileEditDialog : public WDialog
{
public:
  FairDbWtFileEditDialog(WAbstractItemModel *model, const WModelIndex& item);

private:
  WAbstractItemModel  *model_;
  WModelIndex          item_;

  WLineEdit *nameEdit_, *sizeEdit_;
  WComboBox *typeEdit_;
  WDatePicker *createdPicker_, *modifiedPicker_;

  void handleFinish(DialogCode result);

};







#endif //FAIRDBWTFILEEDITDIALOG_H
