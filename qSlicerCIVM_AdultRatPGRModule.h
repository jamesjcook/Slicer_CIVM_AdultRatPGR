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

#ifndef __qSlicerCIVM_AdultRatPGRModule_h
#define __qSlicerCIVM_AdultRatPGRModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerCIVM_AdultRatPGRModuleExport.h"

class qSlicerCIVM_AdultRatPGRModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_CIVM_ADULTRATPGR_EXPORT
qSlicerCIVM_AdultRatPGRModule
  : public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerCIVM_AdultRatPGRModule(QObject *parent=0);
  virtual ~qSlicerCIVM_AdultRatPGRModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  virtual QIcon icon()const;

  virtual QStringList categories()const;
  virtual QStringList dependencies() const;

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerCIVM_AdultRatPGRModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerCIVM_AdultRatPGRModule);
  Q_DISABLE_COPY(qSlicerCIVM_AdultRatPGRModule);

};

#endif
