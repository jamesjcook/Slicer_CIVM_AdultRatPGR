/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerCIVM_AdultRatPGRModuleWidget.h"
#include "ui_qSlicerCIVM_AdultRatPGRModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCIVM_AdultRatPGRModuleWidgetPrivate: public Ui_qSlicerCIVM_AdultRatPGRModuleWidget
{
public:
  qSlicerCIVM_AdultRatPGRModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerCIVM_AdultRatPGRModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModuleWidgetPrivate::qSlicerCIVM_AdultRatPGRModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerCIVM_AdultRatPGRModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModuleWidget::qSlicerCIVM_AdultRatPGRModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerCIVM_AdultRatPGRModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModuleWidget::~qSlicerCIVM_AdultRatPGRModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::setup()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}

