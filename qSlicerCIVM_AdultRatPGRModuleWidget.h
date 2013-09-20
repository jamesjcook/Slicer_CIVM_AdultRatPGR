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

#ifndef __qSlicerCIVM_AdultRatPGRModuleWidget_h
#define __qSlicerCIVM_AdultRatPGRModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerCIVM_AdultRatPGRModuleExport.h"

#include <ctkSliderWidget.h>

class qSlicerCIVM_AdultRatPGRModuleWidgetPrivate;
class vtkCollection;
class vtkMRMLNode;
class vtkMRMLSliceNode;



/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_CIVM_ADULTRATPGR_EXPORT qSlicerCIVM_AdultRatPGRModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerCIVM_AdultRatPGRModuleWidget(QWidget *parent=0);
  virtual ~qSlicerCIVM_AdultRatPGRModuleWidget();
  // public functions.
  void PrintMethod(QString);  //print to console with constant text+message
  void PrintText(QString);    //print to console
  QStringList SetLayout();    //defunct?(that needs verification) function to change the layout.
  const char * NodeID(QString); //translates node name to node id
  
  //public data
  QString GalleryName;        // Name of the gallery
  QStringList ImageContrasts; // list of the different contrasts available.
  bool CenterVolumeOnLoad;    // variable to choose when to center on load, might not want that behavior in the future
  bool LoadLabels; 
  QString ViewNodeContrast1;  // First viewers's contrast selection. 
  QString ViewNodeContrast2;  // Second viewers Contrast Selection.
  QStringList SceneNodes;     // list of nodes in the scene, this could probably be a protected variable.
  QString Layout;             //String name of the layout to use.
  QString DataPath;           //path to data set in the setup function, will have to be
  QString LabelPath;          //see datapath.
  QString DataPattern;        //pattern for data file to have parts replacd wit actual contrast and timepoint information.
  QString LabelPattern;       //see datapattern
  double Pos;           //container for position of the axial image slider
  double Angle;         //container for position of the sagital angle slider
  //  vtkMRMLSliceNode * SlicePointer ; // smart pointer to connect our slider to to adjust both our datastes at the same time. 
  

  double GetSliceOffsetValue(vtkMRMLSliceNode *);
  std::vector<double> GetSliderRange(vtkMRMLNode * );
  void SetSliderRange(ctkSliderWidget * , std::vector<double>);

  /*
  ///
  /// Get/Set the current distance from the origin to the slice plane
  double GetSliceOffset();
  void SetSliceOffset(double offset);*/

  public slots:
  //  void IncrimentSlider(ctkSliderWidget * );     // incrimentslider
  //  void DecrimentSlider(ctkSliderWidget * );     // decrimentslider
  void BuildScene();          //sets up mrml nodes loads data
  bool NodeExists(QString);   // checks scene for existing node with given name. Used in the build scene fucntion to avoid reloading data.
  QStringList GetContrasts();    // will look up all the checkboxes in contrasts group and make a qstringlist of values
  void SetViewNodeProperties(QString); //Sets the view properties for a given view node string
  
  void SetSliceOffsetValue(double offset);
  void SetSliceGeometry(double angle);
  //  void trackSliceOffsetValue(double offset);
  
 protected:
  QScopedPointer<qSlicerCIVM_AdultRatPGRModuleWidgetPrivate> d_ptr;
  
  virtual void setup();
  
 private:
  Q_DECLARE_PRIVATE(qSlicerCIVM_AdultRatPGRModuleWidget);
  Q_DISABLE_COPY(qSlicerCIVM_AdultRatPGRModuleWidget);
};

#endif
