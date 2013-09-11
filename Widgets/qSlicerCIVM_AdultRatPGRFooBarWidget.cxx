/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerCIVM_AdultRatPGRFooBarWidget.h"
#include "ui_qSlicerCIVM_AdultRatPGRFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_CIVM_AdultRatPGR
class qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate
  : public Ui_qSlicerCIVM_AdultRatPGRFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerCIVM_AdultRatPGRFooBarWidget);
protected:
  qSlicerCIVM_AdultRatPGRFooBarWidget* const q_ptr;

public:
  qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate(
    qSlicerCIVM_AdultRatPGRFooBarWidget& object);
  virtual void setupUi(qSlicerCIVM_AdultRatPGRFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate
::qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate(
  qSlicerCIVM_AdultRatPGRFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate
::setupUi(qSlicerCIVM_AdultRatPGRFooBarWidget* widget)
{
  this->Ui_qSlicerCIVM_AdultRatPGRFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerCIVM_AdultRatPGRFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRFooBarWidget
::qSlicerCIVM_AdultRatPGRFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerCIVM_AdultRatPGRFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerCIVM_AdultRatPGRFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRFooBarWidget
::~qSlicerCIVM_AdultRatPGRFooBarWidget()
{
}
