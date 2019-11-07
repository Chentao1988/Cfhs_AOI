#include "cfhs_combobox.h"
#include <QListView>


Cfhs_ComboBox::Cfhs_ComboBox(QWidget *parent)
    : QComboBox (parent)
{
    this->setView(new QListView());
}
