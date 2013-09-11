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
#include <QtPlugin>

// CIVM_AdultRatPGR Logic includes
#include <vtkSlicerCIVM_AdultRatPGRLogic.h>

// CIVM_AdultRatPGR includes
#include "qSlicerCIVM_AdultRatPGRModule.h"
#include "qSlicerCIVM_AdultRatPGRModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerCIVM_AdultRatPGRModule, qSlicerCIVM_AdultRatPGRModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCIVM_AdultRatPGRModulePrivate
{
public:
  qSlicerCIVM_AdultRatPGRModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerCIVM_AdultRatPGRModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModulePrivate
::qSlicerCIVM_AdultRatPGRModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerCIVM_AdultRatPGRModule methods

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModule
::qSlicerCIVM_AdultRatPGRModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerCIVM_AdultRatPGRModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerCIVM_AdultRatPGRModule::~qSlicerCIVM_AdultRatPGRModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerCIVM_AdultRatPGRModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerCIVM_AdultRatPGRModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCIVM_AdultRatPGRModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerCIVM_AdultRatPGRModule::icon()const
{
  return QIcon(":/Icons/CIVM_AdultRatPGR.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerCIVM_AdultRatPGRModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerCIVM_AdultRatPGRModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerCIVM_AdultRatPGRModule
::createWidgetRepresentation()
{
  return new qSlicerCIVM_AdultRatPGRModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerCIVM_AdultRatPGRModule::createLogic()
{
  return vtkSlicerCIVM_AdultRatPGRLogic::New();
}
