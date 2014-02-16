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

// MRMLWidgets includes
#include <qMRMLSliceWidget.h>
#include <qMRMLSliceControllerWidget.h>
#include <qMRMLSliceView.h>
#include <qSlicerViewersToolBar.h>

// SlicerQt includes
#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"
#include "qSlicerIOManager.h"

#include "qSlicerCIVM_AdultRatPGRModuleWidget.h"
#include "ui_qSlicerCIVM_AdultRatPGRModuleWidget.h"

#include "vtkMRMLScene.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLLayoutNode.h"
#include "vtkMRMLViewNode.h"
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLSliceNode.h"
#include "vtkMRMLSliceLogic.h"
#include "vtkMRMLSliceLayerLogic.h"
#include "vtkMRMLScalarVolumeNode.h"
#include <vtkMRMLVolumeDisplayNode.h>
#include <vtkImageData.h>
#include <vtkMRMLColorNode.h>
#include <vtkMRMLCrosshairNode.h>
#include <vtkMRMLCrosshairDisplayableManager.h>
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLLayoutLogic.h"
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorObserver.h>
#include <vtkCollection.h>
#include <vtkSmartPointer.h>

#include <ctkPimpl.h>
#include <ctkSliderWidget.h>
#include <ctkVTKSliceView.h>
#include "ctkMessageBox.h"

#include <vtkTransform.h>
//#include "/Applications/SegmentationSoftware/src/Slicer_multi/modules_comp/Slicer/Modules/Loadable/Markups/Logic/"vtkSliceMarkupsLogic.h
#include <vtkSlicerMarkupsLogic.h>
#include "vtkMRMLApplicationLogic.h"
#include "vtkEventForwarderCommand.h"
#include <vtkMRMLInteractionNode.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkMRMLMarkupsNode.h>
//#include <vtkSlicerMarkupsModule.h>
//#include "vtkMRMLMarkupsNode.h"
#include <vtkMRMLMarkupsNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>
#include <vtkMRMLMarkupsStorageNode.h>
//#include <qSlicerMarkupsModule.h>
// #include <vtkMRMLFiducial.h>
#include <vtkMRMLAnnotationFiducialNode.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCIVM_AdultRatPGRModuleWidgetPrivate: public Ui_qSlicerCIVM_AdultRatPGRModuleWidget
{
public:
  //  void sliceConnector(vtkMRMLNode *n, qSlicerLayoutManager *layoutManager) ;
  typedef std::map<vtkSmartPointer<vtkMRMLNode>, qMRMLViewControllerBar* > ControllerMapType;
  ControllerMapType ControllerMap;
  QList<vtkInteractorObserver*> ObservedInteractorStyles;
  qSlicerCIVM_AdultRatPGRModuleWidgetPrivate();
//  void sliceConnector(vtkMRMLNode *n, qSlicerLayoutManager
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
//  Q_Q(qSlicerCIVM_AdultRatPGRModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
#ifdef WIN32 
  this->ps=('\\');
  this->DataRoot=QString("L:"+ps+"DataLibraries");
#else
  this->ps=('/');
  this->DataRoot=QString(""+ps+"DataLibraries");
#endif
  // this->EventCallbackCommand->SetCallback(qSlicerCIVM_AdultRatPGRModuleWidget::Processevent);
  //this->SliceLogic = 0;
  // insert init and static data definitions here.
  this->Layout=QString("PGR");
  this->GalleryName="CIVM_AdultRatAtlas";
  this->CenterVolumeOnLoad=true;
//  this->Priority=2;
  this->NumberOfFiducials=0;
  //Create the markups list F here before we get below so we know its on and added to the scene.
  double var[3] = {1, 1, 1 };
  this->LastLabelName="";
//  this->cLastRAS=var;
//  this->cLastIJK=var;
//  this->cLastXYZ=var;
  this->Angle=0;
  this->Pos=0;
  this->LoadLabels=true;
  
  /* Load data paths */
  /* hardcode for now*/
  /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
  /* labels are at /DataPath/labels/completed/completed_good_header/nii/           */
  //this->DataPath=QString(""+DataRoot+ps+"timepoint"+ps+"average"+ps+"");
  //this->LabelPath=QString(""+DataRoot+ps+"labels"+ps+"completed"+ps+"completed_good_header"+ps+"nii"+ps+"");
  QString organ="Brain";
  QString species="Rat";
  QString strain="Wistar";
  QString specimenid="Average_SPECCODE";

  this->DataPath=QString(""+DataRoot+ps+organ+ps+species+ps+strain+ps+specimenid+ps+"timepoint"+ps+"");
  this->LabelPath=DataPath;

  //  this->DataPattern  << "p"   << "timepoint" << "_average_" << "contrast" << ".nii";
  //  this->LabelPattern << "pnd" << "timepoint" << "_average_labels.nii";

  // using this mrml to initalize the 3d views for now, and then set to the first gallery setup.
  QString out_path = "";//DataRoot+"/DoubleBlank.mrml";
  // example for replacment to fix the blank not loading.
//   imageFile.replace("timepoint",timepointList[t]);
//   imageFile.replace("contrast",contrastList[c]);
//   //confusingly DataPath is root to data directories
//   imagePath = DataPath+imageFile;
//   imagePath.replace("timepoint",timepointList[t]);

  qSlicerApplication * app = qSlicerApplication::application();
//  app->ioManager()->loadScene(out_path);
  out_path = DataRoot+ps+"Blank.mrml";
  app->ioManager()->loadScene(out_path,false);  

  //this->DataPattern  =QString("ptimepoint_average_contrast.nii");
  //this->LabelPattern =QString("pndtimepoint_average_labels.nii");
  this->DataPattern  =QString("average_timepoint_contrast.nii.gz");
  this->LabelPattern =QString("average_timepoint_labels.nii.gz");
  this->HistologyNodeName=QString("NO_IMAGE");
  d->LabelInformation->setCollapsed(true);
  //Timepoint=80; using the same code as our time/contrast bit so we can switch between timepoints for more rapid testing.
  // see the buildscene function for the list of times allowed.
  // need to create the getcontrasts function for this
  ImageContrasts << "adc" << "b0" << "dwi" << "fa" << "fa_color"  << "freq" << "gre" << "rd";
  
  d->ComboBoxA->insertItems(0, ImageContrasts );
  d->ComboBoxB->insertItems(0, ImageContrasts );

//  d->ComboBoxA->setDefaultText("Select Sagittal/Image1 Contrast");
//  d->ComboBoxB->setDefaultText("Select Axial/Image2 Contrast");

  // need to do better with these names
  d->ComboBoxA->setDefaultText("First Contrast");
  d->ComboBoxB->setDefaultText("Second Contrast");


  d->ComboBoxA->setCurrentIndex(-1);
  d->ComboBoxB->setCurrentIndex(-1);

  //insert button connections here.
  connect(d->HistologySelector,SIGNAL(clicked()),SLOT(HistologySelectionDialog()));
  connect(d->BuildSceneButton,SIGNAL(clicked()),SLOT(BuildScene()));//the restviewer button.
  connect(d->WatchMousePositionCheckBox,SIGNAL(clicked()),SLOT(SetMouseWatcherStatus()));
  //insert drop down connections here.
  connect(d->ComboBoxA,SIGNAL(currentIndexChanged(int)),SLOT(LoadData()));
  connect(d->ComboBoxB,SIGNAL(currentIndexChanged(int)),SLOT(LoadData()));
  //Connect our slice slider here.
  connect(d->SliderSlice, SIGNAL(valueChanged(double)), SLOT(SetSliceOffsetValue(double)), Qt::QueuedConnection);
  connect(d->SliderAngle, SIGNAL(valueChanged(double)), SLOT(SetSliceGeometry(double)), Qt::QueuedConnection);
  // QObject::connect(this->deleteAllMarkupsInListPushButton, SIGNAL(clicked()),
//                    q, SLOT(onDeleteAllMarkupsInListPushButtonClicked()));
   connect(d->ClearFiducialList, SIGNAL(clicked()),SLOT(onDeleteAllMarkupsInListPushButtonClicked()));

  d->SliderSlice->setTracking(true);
}

QStringList qSlicerCIVM_AdultRatPGRModuleWidget::GetContrasts()
{
//  this->PrintMethod(QString("GetContrasts"));
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);

  //forms comaseparated list of contrast strings
  QStringList contrast_list;
  QStringList all_contrasts;
  if ( d->ComboBoxA->currentText() != "" && d->ComboBoxA->currentIndex() !=-1 ) 
    {
    contrast_list << d->ComboBoxA->currentText();
    } else 
    {
    this->PrintText("ComboBoxA undefined");
    }
  if ( d->ComboBoxB->currentText() != "" && d->ComboBoxB->currentIndex() !=-1 )
    {
    contrast_list << d->ComboBoxB->currentText();
    } else 
    { 
    this->PrintText("ComboBoxB undefined");
    }
  
  return contrast_list;
}
const char * qSlicerCIVM_AdultRatPGRModuleWidget::NodeID(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);
  
  // Search the scene for the nodes and return true on match
  currentScene->InitTraversal();
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) 
    {
    if (sn->GetName()==nodeName) 
      { 
      //this->PrintText("Found volume not reloading");
      return sn->GetID();
      } else 
      { 
      //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
      }
    }
  this->PrintText("\tDid not find "+nodeName+" in open nodes.");
  return "";

}
void qSlicerCIVM_AdultRatPGRModuleWidget::SetViewNodeProperties(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);

  // Search othe scene for the nodes set properties
  currentScene->InitTraversal();
  vtkMRMLNode *sn = currentScene->GetNodeByID(nodeName.toStdString());
  vtkMRMLViewNode *vNode = NULL;
  //qSlicerIO::IOProperties *viewNodeProps;
  if ( sn != NULL )
    {
      vNode = vtkMRMLViewNode::SafeDownCast(sn); //Composite
      if ( vNode !=NULL )
	{
	  this->PrintText("SET PROPERTIES FOR VIEWER HERE");
	  //do work
	  //vNode->id("vtkMRMLViewNode1");
	  //vNode->name(View1);
	  //vNode->hideFromEditors(false);
	  //vNode->selectable(true);
	  //vNode->selected(false);
	  //vNode->attributes(MappedInLayout:1);
	  //vNode->layoutLabel(1);
	  //vNode->layoutName(1);
	  //vNode->active(false);
	  //vNode->visibility(false);
	  double bg1[3]={0.0, 0.0, 0.0};//{0.756863, 0.764706, 0.909804};
	  vNode->SetBackgroundColor(bg1);
	  double bg2[3]={0.0, 0.0, 0.0};//{0.454902, 0.470588, 0.745098};
	  vNode->SetBackgroundColor2(bg2);
	  vNode->SetFieldOfView(45);
	  //      vNode->SetLetterSize(0.05);
	  vNode->SetBoxVisible(false);
	  //      vNode->SetFiducialsVisible(true);
	  //      vNode->SetFiducialLabelsVisible(true);
	  vNode->SetAxisLabelsVisible(false);
	  //      vNode->SetAxisLabelsCameraDependent(true);
	  //      vNode->SetAnimationMode("Off");
	  //      vNode->SetNviewAxisMode("LookFrom");
	  //      vNode->SetSpinDegrees(2);
	  //      vNode->SetSpinMs(5);
	  //      vNode->SetSpinDirection("YawLeft");
	  //      vNode->SetRotateDegrees(5);
	  //      vNode->SetRockLength(200);
	  //      vNode->SetRockCount(0);
	  //      vNode->SetStereoType("NoStereo");
	  //      vNode->SetRenderMode("Perspective");



	}
      else
	{
	  this->PrintText("View node did not set properly!");
	  return;
	}


    }
  else
    {
      this->PrintText("Bad view node or not a view node");
      return;
    }
  //   for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) {
  //     if (sn->GetName()==nodeName) {
  //      this->PrintText(""+nodeName +" found.");
  //       return;
  //     } else {
  //       //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
  //     }
  //   }
  this->PrintText(""+nodeName+" Properties Set.");
  return;
}

//-----------------------------------------------------------------------------
// build mrml and load datasests
void qSlicerCIVM_AdultRatPGRModuleWidget::HistologySelectionDialog()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget); 
  QString hist_path = "dummy file";
  qSlicerApplication * app = qSlicerApplication::application();
  bool status=app->ioManager()->openAddVolumeDialog(); //loadScene(out_path);
  QStringList histologySlides;
  QTextStream out(stdout);  
  if ( status )
    {
    vtkMRMLScene* currentScene = this->mrmlScene();
    currentScene->InitTraversal();
    vtkMRMLScalarVolumeNode * hNode =  vtkMRMLScalarVolumeNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode")) ;
    //vtkMRMLNode *sn = currentScene->GetNodeByID(nodeName.toStdString());
    while (hNode !=NULL ) 
      {
      double sliceBounds[6] = {0, -1, 0, -1, 0, -1};//xmin,xmax,ymin,ymax,zmin,zmax
      hNode->GetRASBounds(sliceBounds);
      double zmin=sliceBounds[4];
      double zmax=sliceBounds[5];

      out << "Examining node for Slides list";
      if( zmax-zmin==1 ) 
        {
        histologySlides << hNode->GetName(); //GetID
        out << "\tAdded " <<hNode->GetName();
        } 
      else 
        {
        out << zmin << ":" << zmax << "!=1" ;
        }

      //GetName()
      hNode = vtkMRMLScalarVolumeNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode")) ;

      }
    for(int it=0;it<histologySlides.size();it++) 
      {
      this->HistologyNodeName=histologySlides[it];
      out << histologySlides[it];
      }
    }
  else
    {
    this->PrintText("Histology Load Failed!");
    this->HistologyNodeName=QString("NO_IMAGE");
    }
  out << "\n";  
  //qSlicerApplication::application()->ioManager()->openLoadSceneDialog();// change dialog to just add the scene.
  
  // always buildsecene after selecting file.
  this->BuildScene();
  return ;
}


//-----------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::LoadData()
{
  //todo fix this stupid hack and separte the load_data and buildscene functions.
  this->BuildScene();
  this->BuildScene();
  return;
}

//-----------------------------------------------------------------------------
// build mrml and load datasests
void qSlicerCIVM_AdultRatPGRModuleWidget::BuildScene()
{
  this->PrintMethod(QString("BuildScene"));
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget); //i think the Q_D line connects us back to our parent widget
    
//   // get the index of the current column
//   this->Superclass::mouseMoveEvent(e);
//   QModelIndex index = indexAt(e->pos());
  

  // last minute settings read
  /*
    QStringList timepointList = this->GetTimepoints();
  */

  QStringList timepointList;
  //timepoint=4*24*60*60;//transformtimepoint to new standard measured by seconds.
  timepointList<< "00006912000";
  
  QStringList contrastList  = this->GetContrasts();
  // make sure we have two image contrasts selected, refuse to run if we do not.
  if (contrastList.size()<2 )
    {
    this->PrintText("Not enough contrasts selected");
    return;
    }

  this->SceneNodes=this->SetLayout(); // sets the layout.

  QString     orientation    = "Coronal";
  QString     labelFile;     //singular label file var
  QString     labelPath;     //singular label path var
  QString     imageFile;     //singular image file var
  QString     imagePath;     //singular image path
  QString     nodeName;      //singular var for imagefile on load
  QString     labelNode;     //node name for the label node
  QList <qSlicerIO::IOProperties> imageProperties; // container to hold all the properties(including name,path,nodename) of images to have open
  QList <qSlicerIO::IOProperties> labelProperties; // container to hold all the properties(including name,path,nodename) of labels to have open will have an entry for each image element of imageProperties
  //  QStringList imageFileList; //list of image files to load.
  //  QStringList imagePathList; //list of image file paths to load.
  //  QStringList labelFileList; //list of label files to load.
  //  QStringList labelPathList; //list of label files to load.
  QList <qSlicerIO::IOProperties> unloadedFiles; //container for the parameters of unloadedfiles.
  bool loadMoreFiles=false;
  //DataPath
  //LabelPath
  //DataPattern (replace timepoint with numbers and contrast with abreviation)
  //LabelPattern(replace timepoint with numbers)
  //save previous settings to be applied once data is loaded and positionned.
  this->PosBackup=d->SliderSlice->value();
  this->AngleBackup=d->SliderAngle->value();
  this->Angle=0;
  this->Pos=0;
    {
    QTextStream sout(stdout);
    sout << "\tSaved position" << this->PosBackup << " Saved Angel" << this->AngleBackup <<"\n";
    }
    d->SliderSlice->reset();//setValue(0);//
    d->SliderAngle->reset();//setValue(0);//
    this->ResetTransform();
////
// initialize scene
////
    bool setSliceOrient=true;
    // for set orientations use a cheat and use this array
    QStringList orientationOverride;
    orientationOverride  << "Sagittal" << "Axial" << "Axial" << "Axial"; 
    // These are the correct orientations for PGR, However the data is not oriented correctly,
    // so we'll override the override for now with following
    orientationOverride.clear();
    orientationOverride  << "Coronal" << "Sagittal" << "Sagittal" << "Axial";
  

    vtkMRMLScene* currentScene = this->mrmlScene();
    currentScene->InitTraversal();
    currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout. Will probably fail if more than one layout is allowed. Oh well for now.
    QStringList orthogonalOrientations;
    orthogonalOrientations << "Axial"
                           << "Sagittal"
                           << "Coronal";


    //build list of files to load.
    for (int c=0;c<contrastList.size();c++)
      {
      for (int t=0;t<timepointList.size(); t++)
        {
        qSlicerIO::IOProperties tParams; //temp params
        // add data to file list
        imageFile=DataPattern;
        if(this->GalleryName=="CIVM_AdultRatAtlas")
          {
          imageFile.replace("timepoint",timepointList[t]);
          imageFile.replace("contrast",contrastList[c]);
          //confusingly DataPath is root to data directories
          imagePath = DataPath+imageFile;
          imagePath.replace("timepoint",timepointList[t]);
          nodeName=imageFile;
          nodeName.replace(".nii","");
          tParams["fileName"]       = imagePath;
          tParams["nodeName"]       = nodeName;
          tParams["labelmap"]       = false;
          tParams["center"]         = this->CenterVolumeOnLoad;
          tParams["autoWindowLevel"]= false;
          tParams["fileType"]       = "VolumeFile";
          labelFile=LabelPattern;
          labelFile.replace("timepoint",timepointList[t]);
          //confusingly LabelPath is root to label root
          labelPath = LabelPath+labelFile;
          labelNode=labelFile;
          labelNode.replace(".nii","");
          tParams["labelNode"]      = labelNode; //will end up as duplicate of nodeName in the labelProperties entries, but this is easier.
          imageProperties << tParams;
          this->PrintText("\timageProperties << "+imageFile);
          this->PrintText("\t  Added volume to load list:"+imagePath);
          if( LoadLabels )
            {
            nodeName                   = labelNode;
            tParams["fileName"]        = labelPath;
            tParams["nodeName"]        = nodeName;
            tParams["labelmap"]        = true;
            tParams["center"]          = this->CenterVolumeOnLoad;
            tParams["autoWindowLevel"] = false;
            labelProperties << tParams;
            this->PrintText("\tlabelProperties << "+labelFile);
            this->PrintText("  Added labels to load list:"+labelPath);
            }
          }
        else 
          {
          this->PrintText("Bad or unset GalleryName.");
          return;
          }
        }//timepoint foor loop end
      }//contrast timepoint loop end.
    
    qSlicerIO::IOProperties cTable;
    labelFile="Slicer_LUT_RatDev.txt";
    labelPath = LabelPath+labelFile;
    labelNode=labelFile;
    labelNode.replace(".txt","");
    cTable["fileType"]       = "ColorTableFile";
    imageProperties << cTable;
    nodeName                   = labelNode;
    cTable["fileName"]        = labelPath;
    cTable["nodeName"]        = nodeName;
    labelProperties[0]["colorMapName"] = cTable["nodeName"];

    labelProperties << cTable;



    int snCounter;// this is for scene nodes later but right now its for any image we want loaded.
    //loop for all labels to add only unloaded to load list need to do a unique element on this list... (bleh), the qset might do the trick
    QSet<QString> labelNodes;
    for(snCounter=0;snCounter<labelProperties.size();snCounter++)
      {
      if ( ! NodeExists(labelProperties[snCounter]["nodeName"].toString()) )
        {
        //this->PrintText("-");
        if( labelNodes.isEmpty() ) 
          {
          //this->PrintText(" +");
          unloadedFiles << labelProperties[snCounter];
          labelNodes << labelProperties[snCounter]["nodeName"].toString(); 
          loadMoreFiles=true;
          } 
        else 
          {
          //this->PrintText("-- ");
          if( ! labelNodes.contains(labelProperties[snCounter]["nodeName"].toString()) ) 
            {
            this->PrintText("\tadding "+labelProperties[snCounter]["nodeName"].toString()+"to loadlist");
            unloadedFiles << labelProperties[snCounter];
            labelNodes << labelProperties[snCounter]["nodeName"].toString();
            loadMoreFiles=true;
            }
          else 
            {
            //this->PrintText("  XXXXXX");
            }
          }
        }
      else 
        {
        this->PrintText("\tFound label"+labelProperties[snCounter]["nodeName"].toString());
        }
      }
    //loop for all images to add only unloaded to load list
    for(snCounter=0;snCounter<imageProperties.size();snCounter++)
      {
      if ( ! NodeExists(imageProperties[snCounter]["nodeName"].toString()) )
        {
        unloadedFiles << imageProperties[snCounter];
        loadMoreFiles=true;
        } 
      }

    //load data
    qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
    if ( loadMoreFiles ) 
      {
      s_app_obj->coreIOManager()->loadNodes(unloadedFiles); // images
      }
    if( LoadLabels)
      {
      //get volumedisplaynode for the labelmap
      qSlicerIO::IOProperties temp;
      temp=labelProperties[0];
      QString nodename = temp["nodeName"].toString();
      vtkMRMLVolumeNode * vN=vtkMRMLVolumeNode::SafeDownCast(
        this->mrmlScene()->GetFirstNodeByName(temp["nodeName"].toString().toStdString().c_str()));
      //scNode->SetLabelVolumeID(this->NodeID(labelProperties[imCounter]["nodeName"].toString()));	//set labels to slicecomposite
//may have to loop over the volumedisplaynodes in tehe scene to find the one pointing at the labelnode
      //setcolormap on the volumedisplaynode.
      //labelProperties[0]["colorMapID"] = cTable["nodeName"];
      if ( vN ) 
        {
        vN->GetVolumeDisplayNode()->SetAndObserveColorNodeID(this->NodeID(temp["colorMapName"].toString().toStdString().c_str()) );
        }
      else 
        {
        this->PrintText("could not fetch the VolumeNode of the labels to assign the color table");
        }
 
      }
    
    
//  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout.  

//   if (sliceNode !=NULL && snCounter>0 && snCounter<3) 
//     { 
//     this->PrintText("Setting range");
//     // commented code works, it was just harder to debug.
//     std::vector<double> range=this->GetSliderRange(sliceNode);
//     QTextStream test(stdout);
//     test << range[0] << range[1] << "\n";
//     this->SetSliderRange(d->SliderSlice,range);
//     //this->SetSliderRange(d->SliderSlice,this->GetSliderRange(sliceNode));
//     } else 
//     { 
//     this->PrintText("Skipped Setting range!");
//     }
    ////
    //arrange objects.
    ////
    //assign slices
    //assign 3d windows, 
    //for now just set properties in 3d views, rather than setting the volume settings.
    snCounter=0;
    int imCounter;
    this->PrintText("\tArranging");
//for (snCounter=SceneNodes.size();snCounter>=0;snCounter--) 
    for (snCounter=0;snCounter<SceneNodes.size();snCounter++) 
      {
      imCounter=snCounter/2;
      // pick out the scene node pointer to work on.    
      QString sliceNodeID=QString("vtkMRMLSliceNode")+SceneNodes[snCounter];
      QString viewNodeID =QString("vtkMRMLViewNode") +SceneNodes[snCounter];
      QString sliceCompositeNodeID=QString("vtkMRMLSliceCompositeNode")+SceneNodes[snCounter];
      vtkMRMLNode      *sn        = currentScene->GetNodeByID(sliceNodeID.toStdString());
      vtkMRMLSliceNode *sliceNode = NULL;
      if ( sn != NULL ) 
        {
        sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode
        } 
      else 
        {
        this->PrintText("Bad slice node or no slice node for SceneNode: "+sliceNodeID);
        }
      vtkMRMLNode      *scn       = currentScene->GetNodeByID(sliceCompositeNodeID.toStdString());
      vtkMRMLSliceCompositeNode *scNode = NULL;
      if ( scn != NULL )//node existed try to downcast
        {
        scNode = vtkMRMLSliceCompositeNode::SafeDownCast(scn); //Composite
        } 
      else 
        { 
        this->PrintText("Bad composite node or no composite node for SceneNode"+sliceCompositeNodeID);
        }
      if ( scNode != NULL )  
        //if we have a composinte node sucessfully downcast to assign volumes to
        { //check vtkMRMLSliceCompositeNode good
        //<>//
        this->PrintText("\tSliceComposite ready:"+sliceCompositeNodeID);
        //set default orientations according to slicer rules.
        if(SceneNodes.size()==orthogonalOrientations.size() && snCounter<orthogonalOrientations.size() )
          {
//        this->PrintText("Setting default orientation");
          sliceNode->SetOrientation(orthogonalOrientations[snCounter].toLatin1());
          }
        else 
          {
//        this->PrintText("No default orientaion");
          }
        // if specific orientations are desired clean up by setting them now. 
        if(setSliceOrient) 
          {
//        this->PrintText("\tSetting override orientation");
          sliceNode->SetOrientation(orientationOverride[snCounter].toLatin1());
          sliceNode->SetOrientationToReformat();
          }
        else 
          {
          this->PrintText("\tNo orientation override. Not setting.");
          }
        if( LoadLabels )
          { 
          //this->PrintText("SetLabelVolumeID:"+labelProperties[imCounter]["nodeName"].toString());
          scNode->SetLabelVolumeID(this->NodeID(labelProperties[imCounter]["nodeName"].toString()));	//set labels
          scNode->SetLabelOpacity(1); 
          sliceNode->SetUseLabelOutline(true);
          }
        else 
          {
          this->PrintText("LabelID=NULL");
          scNode->SetLabelVolumeID(NULL);
          }
        //specific viewer settings
        if( snCounter==0 || snCounter==1 || snCounter==2 )
          {  //! strcmp(HistologyNodeName,"NO_IMAGE")
           
          if (snCounter==1||snCounter==2)
            {
            scNode->SetLinkedControl(1);
            }

          if(imCounter<imageProperties.size()) //prevents bad index crash
            {
            //this->PrintText("SetBackgroundVolumeID:"+imageProperties[imCounter]["nodeName"].toString());
            scNode->SetBackgroundVolumeID(this->NodeID(imageProperties[imCounter]["nodeName"].toString())); //set image
            //scNode->SetaAnnotationSpcae();
            }
          //get the range setting from the volumes.
          currentScene->InitTraversal();
          std::vector<double> range (2,0);
          vtkMRMLNode * vNode = currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
          while( vNode != NULL) // traverse volume nodes looking for the node we're currently assigning. 
            {
            if ( vNode->GetName() ==imageProperties[imCounter]["nodeName"] ) 
              {
              //            this->PrintText("Getting range from current node.");//"+imageProperties[imCounter]["nodeName"]+"
              range=this->GetSliderRange(vNode);
              //            this->PrintText("Setting range");
              this->SetSliderRange(d->SliderSlice,range);
//             if ( snCounter !=0 ) 
//               {

//               this->PrintText("Getting Slice FOV");
//               std::vector<double> sliceFov ( 3,0);
//               sliceFov=this->GetSliceFOV(vNode); //gets volume bounds, and uses that to set slice fov.
	      
// 	      double * sliceDims;
// 	      sliceDims = sliceNode->GetDimensions();
	      
	      
//               sliceNode->SetFieldOfView(sliceFov[0],sliceFov[1],sliceFov[2]);
//               std::vector<double> origin (3,0);
//               sliceNode->SetXYZOrigin(origin[0],origin[1],origin[2]);
//               }
              }
            vNode=currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
            }
//         currentScene->InitTraversal();
//         vtkMRMLNode * volNode = currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
//         while( volNode != NULL) // traverse volume nodes looking for the node we're currently assigning. 
//           {
//           if ( volNode->GetName() ==imageProperties[imCounter]["nodeName"] ) 
//             {
//             this->PrintText("Getting Slice FOV");
//             std::vector<double> sliceFov ( 3,0);
//             sliceFov=this->GetSliceFOV(volNode); //gets volume bounds, and uses that to set slice fov.
//             sliceNode->SetFieldOfView(sliceFov[0],sliceFov[1],sliceFov[2]);
//             std::vector<double> origin (3,0);
//             sliceNode->SetXYZOrigin(origin[0],origin[1],origin[2]);
//             }
//           volNode=currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
//           }
          //<<.>>//
          if (snCounter==1 ) 
            { // && snCounter<3) // only turn on visiblilty on slicenode yellow, to give yellow slice indicator in the sagital view
            scNode->SetSliceIntersectionVisibility(1);
            }
          }
        else if( snCounter==3 && HistologyNodeName != "NO_IMAGE" )  //! strcmp(HistologyNodeName,"NO_IMAGE")
          {
//        this->PrintText("HistologyNodeName Good");
          scNode->SetBackgroundVolumeID(this->NodeID(HistologyNodeName)); //set image
          //scNode->SetLinkedControl(0);
//         currentScene->InitTraversal();
//         vtkMRMLNode * vNode = currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
//         while( vNode != NULL) // traverse volume nodes looking for the node we're currently assigning. 
//           {
//           if ( vNode->GetName() ==imageProperties[imCounter]["nodeName"] ) 
//             {
//             this->PrintText("Getting Slice FOV");
//             std::vector<double> sliceFov ( 3,0);
//             sliceFov=this->GetSliceFOV(vNode); //gets volume bounds, and uses that to set slice fov.
//             sliceNode->SetFieldOfView(sliceFov[0],sliceFov[1],sliceFov[2]);
//             std::vector<double> origin (3,0);
//             sliceNode->SetXYZOrigin(origin[0],origin[1],origin[2]);
//             }
          }        
//        this->PrintText("Histology Viewer set!");
        }
      else if( snCounter==3 && HistologyNodeName == "NO_IMAGE" )  //! strcmp(HistologyNodeName,"NO_IMAGE")
        {
        this->PrintText("HistologyNodeName bad");
        this->PrintText(HistologyNodeName);
        }
      //insert fit to background here,(maybe use fov in the per scene node lookup.
      //
      
      //     void qMRMLSliceControllerWidget::setSliceLogic(vtkMRMLSliceLogic * newSliceLogic)
      if (snCounter !=3 )
        {
        //vtkSmartPointer<vtkMRMLSliceLogic>::New();
        //currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
        vtkMRMLNode * sl = NULL;
        vtkMRMLSliceLogic * sliceLogic = NULL;
        //currentScene->InitTraversal();
        // there is no slice logic until we load our histology slice.
        //sl = currentScene->GetNextNodeByClass("vtkMRMLSLiceLogic");

        // qSlicerApplication *        s_app_obj = qSlicerApplication::application(); //set application linking.
        // vtkMRMLApplicationLogic *mrmlAppLogic = s_app_obj->LayoutManager()->GetMRMLApplicationLogic();
        // vtkMRMLScene* currentScene = this->mrmlScene();
      
        vtkMRMLApplicationLogic *mrmlAppLogic = this->logic()->GetMRMLApplicationLogic();
        if (mrmlAppLogic )
          {
          sliceLogic = mrmlAppLogic->GetSliceLogic(sliceNode);
          }
        else 
          {
          this->PrintText("Failed to find slice logic");
          }

        if ( sliceLogic != NULL  ) 
          {
//        this->PrintText("sliceLogic setNode");
          sliceLogic->SetSliceNode(sliceNode);
//        this->PrintText("SliceLogic Interaction");
          sliceLogic->StartSliceNodeInteraction(vtkMRMLSliceNode::ResetFieldOfViewFlag);
//        this->PrintText("SliceLogic FitSlice");

            sliceLogic->FitSliceToAll();
//           if (loadMoreFiles) 
//             {
//             }
//        this->PrintText("Slicenode UpdateMatricies");
          sliceNode->UpdateMatrices();
//        this->PrintText("SliceLogic Interaction end");
          sliceLogic->EndSliceNodeInteraction();
          }
        else 
          {
          this->PrintText("Failed to set slice logic");
          }
        //sliceNode->sliceController->FitSliceToBackground();      
        //fgsliceController->fitSlcieToBackground();% 
        }   
    
      else 
        { //we'll enter this code if we are on orthogonal or dual3d for the 3d volumes
        //this->PrintText("SliceComposite bad:"+sliceCompositeNodeID+" could not downcast!");
        }
      if (sn == NULL ) 
        { 
        vtkMRMLNode     *vn       = currentScene->GetNodeByID(viewNodeID.toStdString());
//    vtkMRMLViewNode *vNode;
        if ( vn != NULL && this->Layout=="Dual3D" )
          {
//      vNode = vtkMRMLViewNode::SafeDownCast(vn); //Composite
          this->SetViewNodeProperties(viewNodeID);
          } 
        else
          {
          this->PrintText("Bad view node or not a view node:"+viewNodeID);
          }
        }
      } // end scenenode set up viewer loop,

// set angel and posotion sliders back, then apply values.
      { 
      QTextStream sout(stdout);
      sout << "Restoring position" << this->PosBackup << " Restoring Angel" << this->AngleBackup <<"\n"; //this->Angle
      }

      this->Pos=0;
      this->Angle=0;
      d->SliderSlice->setValue(this->PosBackup);
      d->SliderAngle->setValue(this->AngleBackup);
        { 
        QTextStream sout(stdout);
        sout << "Restored position" << d->SliderSlice->value() << " Restoring Angle" << d->SliderAngle->value()<<"\n"; //this->Angle
        }
        return;
}

/*
//---------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::setSliceLogic(vtkMRMLSliceLogic * newSliceLogic)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  if (d->SliceLogic == newSliceLogic)
    {
      return;
    }

  d->qvtkReconnect(d->SliceLogic, newSliceLogic, vtkCommand::ModifiedEvent,
                   d, SLOT(onSliceLogicModifiedEvent()));

  d->SliceLogic = newSliceLogic;

  if (d->SliceLogic && d->SliceLogic->mrmlScene())
    {
      this->setMRMLScene(d->SliceLogic->mrmlScene());
    }

  d->onSliceLogicModifiedEvent();
  }
*/


//-------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliceGeometry(double value)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  //this->PrintMethod("SetSliceGeometry");
  // get a reformat widget aimed at the yellow slice, read the current orientation of the yellow slice for the starting point. 
  // read the values for offest and angle
  // apply them to the reformat widget
  // set viewers 1/2 of 0-3 to the reformat widget(just to be sure).
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();


  // terribly specific to our case here, we're skipping the first image viewer.
  //  int snCounter;
//  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout.  
  // Will probably fail if more than one layout is allowed. Oh well for now.
  QString         sliceNodeID = NULL;
  vtkMRMLNode             *sn = NULL;
  vtkMRMLSliceNode *sliceNode = NULL;
//  vtkMRMLSliceLogic* MRMLSliceLogic;

  //double value () const
  //this->Pos;
  //this->Angle;
  QTextStream test(stdout);
  for (int snCounter=1;snCounter<3;snCounter++) //SceneNodes.size()-1
    {
    sliceNodeID = QString("vtkMRMLSliceNode")+SceneNodes[snCounter];
    sn          = currentScene->GetNodeByID(sliceNodeID.toStdString());
    sliceNode   = NULL;
    if ( sn != NULL ) 
      { 
      sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode    
      if ( sliceNode != NULL ) 
        { 
//        QString temp="Setting Angle on slice: "+sliceNodeID+" ";
//        this->PrintText(temp);
        transform->SetMatrix(sliceNode->GetSliceToRAS());
        // Rotate on LR given the angle with the last value reccorded
//         test << "Last:"<< Angle[snCounter]
//              << " next:"<< d->SliderAngle->value() 
//              << " diff:" << Angle[snCounter]-d->SliderAngle->value() << "\n" ;

        transform->RotateX(this->Angle-d->SliderAngle->value()); //rotation-d->LastRotationValues[axisX]
        sliceNode->GetSliceToRAS()->DeepCopy(transform->GetMatrix());
        sliceNode->UpdateMatrices();
        sliceNode->SetOrientationToReformat();
        } //endslicenode null check
      }// end sn null check
    }// end for
  this->Angle=d->SliderAngle->value();
  return;
}
//------------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetMouseWatcherStatus()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);  
  this->PrintMethod("SetMouseWatcherStatus");
  QTextStream test(stdout);  
  bool status=d->WatchMousePositionCheckBox->isChecked();
  qSlicerApplication *        s_app_obj = qSlicerApplication::application(); //set application linking.
  qSlicerLayoutManager *  layoutManager = s_app_obj->layoutManager();
  QList<vtkInteractorObserver*> interactorStyles;
  int crosshairBehavior=0;

  // this finds the interactor styles
  foreach(const QString& sliceViewName, layoutManager->sliceViewNames())
    {
    qMRMLSliceWidget * sliceWidget = layoutManager->sliceWidget(sliceViewName);
//    Q_ASSERT(sliceWidget);
    interactorStyles << sliceWidget->sliceView()->interactorStyle();
    }

  this->mrmlScene()->InitTraversal();
  vtkMRMLCrosshairNode *  cNode = NULL; 
  cNode = vtkMRMLCrosshairNode::SafeDownCast(this->mrmlScene()->GetNextNodeByClass("vtkMRMLCrosshairNode"));
  if ( ! cNode) 
    {
    this->PrintText("failed to retrieve crosshair");
    return;
    }

  if ( ! status ) //if we're not watching the mouse position.
    {
    this->PrintText("Disconnect");
    // Remove observers
    foreach(vtkInteractorObserver * observedInteractorStyle, d->ObservedInteractorStyles)
      {
      //vtkCommand::LeftButtonReleaseEvent:
      foreach(int event, QList<int>()
              << vtkCommand::MouseMoveEvent << vtkCommand::EnterEvent << vtkCommand::LeaveEvent << vtkMRMLMarkupsNode::MarkupAddedEvent) //LeftButtonReleaseEvent
        {
        qvtkDisconnect(observedInteractorStyle, event,
                       this, SLOT(ProcessEvent(vtkObject*,void*,ulong,void*)));
        }
      }
    d->ObservedInteractorStyles.clear();
    if (cNode) // if the crosshair node exists in the scene, turn off its display.
      {
      this->PrintText("Un-setting crosshair");
      cNode->SetCrosshairMode(vtkMRMLCrosshairNode::NoCrosshair);
      vtkMRMLInteractionNode *interactionNode =
        this->logic()->GetMRMLApplicationLogic() ? this->logic()->GetMRMLApplicationLogic()->GetInteractionNode() : 0;
      vtkMRMLSelectionNode *selectionNode = this->logic()->GetMRMLApplicationLogic()->GetSelectionNode();//= d->MRMLAppLogic->GetSelectionNode();
      if (interactionNode&& selectionNode)
        {
        interactionNode->SwitchToSinglePlaceMode();
        selectionNode->SetReferenceActivePlaceNodeClassName(NULL);
        }
      else
        {
        this->PrintText("couldnt get interaction node to set placement mode on");
        } // end interactionnode exists check
      }//end rosshairnode exists check
    }// end turn off visible crosshair and fiducial placement. 
  else // our status is true, therefore we should be watching the mouse position.
    {
    // Add observers
    this->PrintText("Connect");
    QTextStream test(stdout);
    foreach(vtkInteractorObserver * interactorStyle, interactorStyles)
      {
      foreach(int event, QList<int>()
              << vtkCommand::MouseMoveEvent << vtkCommand::EnterEvent << vtkCommand::LeaveEvent << vtkMRMLMarkupsNode::MarkupAddedEvent) //LeftButtonReleaseEvent
        {
        test<<"+";
        qvtkConnect(interactorStyle, event,
                    this, SLOT(ProcessEvent(vtkObject*,void*,ulong,void*)));
        }
      d->ObservedInteractorStyles << interactorStyle;
      }
    test << ".\n";
    if (cNode) // if the crosshair node exists in the scene, turn on its display, and change its settings to our desired settings.
      {
      this->PrintText("Setting Crosshair");
      cNode->SetCrosshairToFine();
      cNode->SetCrosshairMode(vtkMRMLCrosshairNode::ShowBasic);
      vtkMRMLInteractionNode *interactionNode =
        this->logic()->GetMRMLApplicationLogic() ? this->logic()->GetMRMLApplicationLogic()->GetInteractionNode() : 0;
      vtkMRMLSelectionNode *selectionNode = this->logic()->GetMRMLApplicationLogic()->GetSelectionNode();
      if (interactionNode && selectionNode)
        {
        selectionNode->SetReferenceActivePlaceNodeClassName("vtkMRMLMarkupsFiducialNode");
        interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
        }
      else
        this->PrintText("couldnt get interaction node to set placement mode on");
      }  
//     //connect nodeadded event to something...
//      vtkIntArray *events;
//      events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
//      this->logic()->SetAndObserveMRMLSceneEvents(this->mrmlScene(), events);
    }
  // set placement to on fiducial.
  this->setPersistence(status);// sets our persistence to follow our checkbox, 
  vtkMRMLInteractionNode *interactionNode =
    this->logic()->GetMRMLApplicationLogic() ? this->logic()->GetMRMLApplicationLogic()->GetInteractionNode() : 0;
  //interactionNode->SwitchToPersistentPlaceMode();
  if (interactionNode)
    {
    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
    if ( interactionNode->GetPlaceModePersistence())
      {
      this->PrintText("Set interaction node, perstience on");
      }
    else
      this->PrintText("Set interaction node, perstience off");
    }
  this->PrintText("SetMouseWatcherSuccess");
  return;
}
//---------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::OnMRMLSceneNodeAdded(vtkMRMLNode * addedNode)//SetAndObserveMRMLSceneEvents
{// this never gets called, not sure how to set that up, triyng to avoid it for now. 
  vtkMRMLMarkupsFiducialNode* fidNode =
    vtkMRMLMarkupsFiducialNode::SafeDownCast(addedNode);
  if (fidNode)
    {
    // here you write what you need to do when a 
    // fiducial node is added into the scene
    this->PrintText("mrmlsceneadded Added fiducialnode");
    
//     void vtkMRMLMarkupsFiducialNode::SetNthFiducialLabel
    for(int fN=0;fN<this->mrmlScene()->GetNumberOfNodesByClass("vtkMRMLAnnotationiducialNode");fN++)
      {
//       double xyz[4];
//       fidNode->GetNthFiducialPosition(fN,xyz);
// //       fidNode->GetMarkupPointWorld(fN,0,xyz);
//       vtkMRMLSliceLayerLogic * sliceLayerLogic = vtkMRMLSliceLayerLogic::SafeDownCast(this->CurrentSliceLayerLogic);
//       vtkMatrix4x4 * xyToIJK = sliceLayerLogic->GetXYToIJKTransform()->GetMatrix();
//       double xyzw[4] = {xyz[0], xyz[1], xyz[2], 1.0};
//       double ijkw[4] = {0.0, 0.0, 0.0, 0.0};
//       xyToIJK->MultiplyPoint(xyzw, ijkw);
//       //Q_ASSERT(ijkw[3] == 1.0);
//       QList<double> ijk ;
//       ijk << ijkw[0] <<  ijkw[1] <<  ijkw[2];
//       vtkMRMLScalarVolumeNode * scalarVolumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(this->CurrentLabelVolume);
//       vtkImageData * imageData = scalarVolumeNode->GetImageData();
//       double labelIndex = imageData->GetScalarComponentAsDouble(ijkw[0],ijkw[1],ijkw[2],0); //ijkw[3]);
//       vtkMRMLColorNode * colorNode = scalarVolumeNode->GetDisplayNode()->GetColorNode();
//       std::string labelName = colorNode->GetColorName(static_cast<int>(labelIndex));
//       std::string labelName = "test";      
//       fidNode->SetNthFiducialLabel(fN,labelName);
      // fidNode->SetNthMarkupLabel(fN,labelName);
      }
    }
  else
    {
    this->PrintText("OnMRMLScenENodeAdded");
    }
}

// pulled from qSlicerMouseModeToolBar, used in setmousewatcher
//---------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::setPersistence(bool persistent)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
//this->logic()->GetMRMLApplicationLogic()
  vtkMRMLInteractionNode *interactionNode =
    this->logic()->GetMRMLApplicationLogic() ? this->logic()->GetMRMLApplicationLogic()->GetInteractionNode() : 0;

  if (interactionNode)
    {
    interactionNode->SetPlaceModePersistence(persistent ? 1 : 0);
    }
  else
    {
    qWarning() << __FUNCTION__ << ": no interaction node found to toggle.";
    }
}
//------------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetMouseWatcherStatus_old()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);  
  this->PrintMethod("SetMouseWatcherStatus_old");
  QTextStream test(stdout);  
  bool status=d->WatchMousePositionCheckBox->isChecked();

  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  vtkMRMLCrosshairNode  * cNode = NULL;
  vtkMRMLSliceNode      * sliceNode = NULL;
  qMRMLSliceWidget      * sliceWidget = NULL;
  const qMRMLSliceView  * sliceView = NULL;
  vtkMRMLSliceLogic     * sliceLogic = NULL;
  const char            * sliceNodeName = NULL;
  vtkRenderWindowInteractor * interactor = NULL;
  qSlicerApplication    * s_app_obj = qSlicerApplication::application(); //set application linking.
  qSlicerLayoutManager  * layoutManager =s_app_obj->layoutManager();
  int numberCrosshairNodes = 0; 
  numberCrosshairNodes = currentScene->GetNumberOfNodesByClass("vtkMRMLCrosshairNode");
  if (numberCrosshairNodes == 0 ) 
    {
    this->PrintText("No crosshair nodes found");
    d->WatchMousePositionCheckBox->setChecked(false);
    this->SetMouseWatcherStatus();
    return;
    } 
  else
    {
    cNode = vtkMRMLCrosshairNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLCrosshairNode"));
    test << "Number of crosshair nodes :" << numberCrosshairNodes << "\n";
    }
  int numberSliceNodes = 0; 
  numberSliceNodes     = currentScene->GetNumberOfNodesByClass("vtkMRMLSliceNode");
  if(numberSliceNodes == 0) 
    {
    this->PrintText("No slice nodes found");
    d->WatchMousePositionCheckBox->setChecked(false);
    this->SetMouseWatcherStatus();
    return;
    } 
  else
    {
    test << "Number of slice nodes :" << numberSliceNodes << "\n";
    }

  if(status) 
    {  
    if ( cNode ) 
      {
    //   QStringList observeEvents ;
    //   observeEvents << "MouseMoveEvent" << "EnterEvent" <<  "LeaveEvent" <<"ModifiedEvent";
    //   for (int e_idx=0;e_idx<observeEvents.size();e_idx++)
    //     {
    // //   tag = style.AddObserver(event, self.processEvent, self.priority)
    // //     self.styleObserverTags.append([style,tag])
    // //     tag = sliceNode.AddObserver(observeEvents[e_idx], this->LabelAtPosition(), 2)
    //     interactor->AddObserver(observeEvents[e_idx], this->LabelAtPosition(sliceNode,observeEvents[e_idx]), 0.0);
    //     }
    // //
    //  for event in events:
    //     self.styleObserverTags.append([sliceNode,tag])
    //       vtkMRMLCrosshairDisplayableManager *cmgr =
    //         vtkMRMLCrosshairDisplayableManager::SafeDownCast(
    //           d->DisplayableManagerGroup->GetDisplayableManagerByClassName(
    //             "vtkMRMLCrosshairDisplayableManager"));
    //       if (cmgr) 
    //         {
    //      connect(cmgr->OnInteractorEvent(),SIGNAL(),SLOT(LabelAtPosition()))
    //         }
    //    GetNextNodeByClass("vtkMRMLCrosshairDisplayableManager");
    //     this->InitialCrosshairThickness=cNode->GetCrosshairThickness();
      cNode->SetCrosshairToFine();    
//       cNode->SetCrosshairBehaviro("ShowBasic");
    //     connect(cmgr->OnInteractorEvent(),SIGNAL(),SLOT(LabelAtPosition()))
    //    connect(d->OnInteractionEvent(),SIGNAL(convertRASToXYZ(QList<double> ),SLOT(LabelAtPosition());
    //connect(d->WatchMousePositionCheckBox,SIGNAL(toggled(bool)),SLOT(EnabelMouseWatcher(bool)));
    //connect(d->mouse.moved,SIGNAL(mouse.position(pos)),SLOT(LabelAtPosition());
    //cNode->vtkMRMLCrosshairDisplayableManager::AddInteractorObservableEvent(vtkCommand::MouseMoveEvent, -1.0);
      }
    
    for(int sNode=0;sNode<numberSliceNodes;sNode++) 
      {
      sliceNode = vtkMRMLSliceNode::SafeDownCast(currentScene->GetNthNodeByClass(sNode,"vtkMRMLSliceNode"));
      if (! sliceNode) 
        {
        this->PrintText("Failed to get sliceNode from scene");
        return;
        }
      else 
        {
        sliceNodeName = sliceNode->GetLayoutName();
        test <<"sliceNodeName:" << sliceNodeName << "\n";
        //sliceNode->AddObserver("ModifiedEvent",this->LabelAtPosition(sliceNode),2);// this is a copy of the python code yet it doesnt work.
        }
      }
    if ( sliceNode )
      {
      sliceWidget = layoutManager->sliceWidget(sliceNode->GetLayoutName());
      if ( sliceWidget )
        {
        sliceLogic = sliceWidget->sliceLogic();
        //sliceView = sliceWidget->sliceView(); // not used 
        //vtkRenderWindowInteractor * interactor = sliceView->interactor();
        interactor = sliceWidget->sliceView()->interactor();
        //const vtkInteractorObserver * interactorStyle = sliceView->interactorStyle(); //BUGGERS! this code works but that is not what we're looking for.
        // this stuff works now....
        //   vtkIndent  ident = vtkIndent::vtkIndent(0); 
        //   if( interactor ) 
        //     {
        //       interactor->PrintSelf(cout,ident);
        //     }
        //   else
        //     {
        //     QTextStream test(stdout);  
        //     test << "unable to set interactor";
        //     }
        
        if ( interactor) 
          {
          
//          unsigned long movetag = interactor->AddObserver("MouseMoveEvent",this->LabelAtPosition(),2.0);
//           const char * event = "MouseMoveEvent";
//           //vtkCommand::MouseMoveEvent
//           ObserverTags.push_back( interactor->AddObserver(event,this->LabelAtPosition(cNode),2.0));
          //ObserverTags.push_back( );
          unsigned long tag = 0 ;
          const char * event ;
          event="MouseMoveEvent";
//          tag = interactor->AddObserver(event,this->ProcessEvent( ),this->Priority);
          //tag = interactor->AddObserver(vtk->vtkCommand-MouseMoveEvent,LabelAtPosition(),this->Priority);
          int * xy = interactor->GetEventPosition();  
          //   double * xy[2] = interactor->GetEventPosition();
          //    double * xyz[3] = sliceWidget->sliceView()->convertDeviceToXYZ(xy);
          //    double * ras[3] = sliceWidget->sliceView()->convertXYZToRAS(xyz);
	    
          //   double * xyz = sliceWidget->sliceView()->convertDeviceToXYZ(xy);
          //   double * ras = sliceWidget->sliceView()->convertXYZToRAS(xyz);
          //   double * xyz = sliceView->convertDeviceToXYZ(xy);
          //   double * ras = sliceView->convertXYZToRAS(xyz);
	    
          //double * xyz = sliceView->convertDeviceToXYZ(xy);
          //   double * ras = sliceView->convertXYZToRAS(xyz);
          double init [3]= { 0,0,0 };
          double * xyz = init;
          double * ras = init;
          //sliceNode->GetXYtoRAS()->MultiplyPoint(xyzw, rasw);
          vtkMRMLAbstractSliceViewDisplayableManager::ConvertDeviceToXYZ(interactor, sliceNode, xy[0], xy[1], xyz);
          vtkMRMLAbstractSliceViewDisplayableManager::ConvertXYZToRAS(sliceNode, xyz, ras);
          test << "xyz:ras co-ordinates\n"
               << xyz[0] << ":" << ras[0] << "\n"
               << xyz[1] << ":" << ras[1] << "\n"
               << xyz[2] << ":" << ras[2] << "\n";

          //   //vtkRenderWindowInteractor * interactor = sliceView->interactor() ;
          
          }
        else
          {
          this->PrintText("Failed to get interactor");
          }

        }
      else
        {
        this->PrintText("Failed to get sliceWidget");
        }
      }
    else
      {
      this->PrintText("Failed to get sliceNode");
      }
    }
  else
    {
    if ( cNode ) 
      {
      //QObject::disconnect(receiver, SLOT(slot()));
      //disconnect(d->mouse.moved(),'LabelAtPosition()');
      //break connect...
      //virtual void SetCrosshairThickness (int
      //  cNode->SetCrosshairThickness (this->InitialCrosshairThickness);
      }
    }
  return;
}
//-----------------------------------------------------------------------------
qMRMLSliceWidget * qSlicerCIVM_AdultRatPGRModuleWidget::SlicerWidget(vtkInteractorObserver * interactorStyle) //const
{ // simple function to return the SliceWidget assigned to an interactorStyle
  if (!qSlicerApplication::application()->layoutManager()) 
    {
    return 0;
    }
  // this loop runs through all slice views in the layout and stops when we find the interactor style
  foreach(const QString& sliceViewName, qSlicerApplication::application()->layoutManager()->sliceViewNames())
    {
    qMRMLSliceWidget * sliceWidget = qSlicerApplication::application()->layoutManager()->sliceWidget(sliceViewName);
    Q_ASSERT(sliceWidget);
    if (sliceWidget->sliceView()->interactorStyle() == interactorStyle)
      {
      return sliceWidget;
      }
    }
  return 0;
}

//------------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::ProcessEvent(vtkObject* sender, void* callData, unsigned long eventId, void* clientData) 
 {
   Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
//   Q_Q(qSlicerCIVM_AdultRatPGRModuleWidget);
//   this->PrintMethod("ProcessEvent");
   Q_UNUSED(callData);
   Q_UNUSED(clientData);
   QTextStream test(stdout);   

   vtkInteractorObserver * interactorStyle = vtkInteractorObserver::SafeDownCast(sender);
   
   if ( ! interactorStyle ) 
     {
     this->PrintText("Not Interactor");
     if ( eventId == vtkMRMLMarkupsNode::MarkupAddedEvent) 
       {
       this->PrintText("markup added" ); 
       this->ReadMousePosition=false;
       }
     } else {
   this->ReadMousePosition=true;
   }
   Q_ASSERT(d->ObservedInteractorStyles.indexOf(interactorStyle) != -1);
//   vtkRenderWindowInteractor * interactor = interactorStyle->GetInteractor();
   if(eventId == vtkCommand::EnterEvent || eventId == vtkCommand::MouseMoveEvent || vtkMRMLMarkupsNode::MarkupAddedEvent) //LeftButtonReleaseEvent
     {
     if(eventId == vtkCommand::EnterEvent) 
       { 
       this->FiducialPlacement=true;
       this->ReadMousePosition=true;
       this->mrmlScene()->InitTraversal();
       vtkMRMLMarkupsFiducialNode * markupsNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(
         this->mrmlScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
       this->qvtkConnect(markupsNode, vtkMRMLMarkupsNode::MarkupAddedEvent,
                         this, SLOT(ProcessEvent(vtkObject*,void*,ulong,void*)));

//         qvtkConnect(interactoStyle, event,
//                     this, SLOT(ProcessEvent(vtkObject*,void*,ulong,void*)));

       }
     if ( eventId == vtkMRMLMarkupsNode::MarkupAddedEvent) 
       {
       this->PrintText("markup processing" ); 
       this->ReadMousePosition=false;
       }
     // Compute RAS
     if ( this->ReadMousePosition) 
       {
       //vtkInteractorObserver * interactorStyle = vtkInteractorObserver::SafeDownCast(sender);
       Q_ASSERT(d->ObservedInteractorStyles.indexOf(interactorStyle) != -1);
       
       int xy[2] = {-1, -1};
       interactorStyle->GetInteractor()->GetEventPosition(xy); 
       
       qMRMLSliceWidget * sliceWidget = NULL;
       foreach(const QString& sliceViewName, qSlicerApplication::application()->layoutManager()->sliceViewNames())
         {
         sliceWidget = qSlicerApplication::application()->layoutManager()->sliceWidget(sliceViewName);
         if ( !( sliceWidget->sliceView()->interactorStyle() == interactorStyle ))
           {
           sliceWidget = NULL; 
           }
         else 
           {
           //this->PrintText(sliceViewName);
           break;
           }
         }     
       if ( ! sliceWidget) 
         {
         this->PrintText("sliceWidget Fail! Found no slice widgets.");
         return;
         }
       this->mrmlScene()->InitTraversal();
       vtkMRMLCrosshairDisplayableManager *cmgr = 
         vtkMRMLCrosshairDisplayableManager::SafeDownCast(this->mrmlScene()->GetNextNodeByClass("vtkMRMLCrosshairDisplayableManager"));
       this->mrmlScene()->InitTraversal();
       vtkMRMLCrosshairNode * cNode = vtkMRMLCrosshairNode::SafeDownCast(this->mrmlScene()->GetNextNodeByClass("vtkMRMLCrosshairNode"));
       double xyz[3] = { 0,0,0 };
       double ras[3] = { 0,0,0 };
       if ( cmgr ) 
         {
         cmgr->ConvertDeviceToXYZ(xy[0], xy[1], xyz);
         cmgr->ConvertXYZToRAS(xyz,ras);
         }
       else 
         {
         //this->PrintText("no cmgr");
         if ( cNode ) 
           {
           double  * RAS = cNode->GetCrosshairRAS();
           ras[0] = RAS[0];
           ras[1] = RAS[1];
           ras[2] = RAS[2];
           }
         else 
           {
           this->PrintText("Did not find crosshair manager ");
           this->PrintText("Did not find crosshair.");
           return;
           }
         }
       if ( ras[0]+ras[1]+ras[2] == 0) 
         {
         this->PrintText("RAS not set, crosshair failed to start.");
         }
       if ( ! sliceWidget )
         {
         this->PrintText("sliceWidget not set, return.");
         return;       
         }
       vtkMRMLSliceLogic * sliceLogic = sliceWidget->sliceLogic();
       vtkMRMLSliceNode * sliceNode = sliceWidget->mrmlSliceNode();
       if ( ! sliceLogic || ! sliceNode ) 
         {
         this->PrintText("sliceLogic or sliceNode not set");
         return;
         }
       else 
         {
         vtkMatrix4x4 *rasToXYZ = vtkMatrix4x4::New();
         rasToXYZ->DeepCopy(sliceNode->GetXYToRAS());
         rasToXYZ->Invert();
         double rasw[4], xyzw[4];
         rasw[0] = ras[0];
         rasw[1] = ras[1];
         rasw[2] = ras[2];
         rasw[3] = 1.0;
         rasToXYZ->MultiplyPoint(rasw, xyzw);
         xyz[0] = xyzw[0]/xyzw[3];
         xyz[1] = xyzw[1]/xyzw[3];
         xyz[2] = xyzw[2]/xyzw[3];
         rasToXYZ->Delete();
         }
       // Layer name, ijk and value
       std::string labelName;
       typedef QPair<QString, vtkMRMLSliceLayerLogic*> LayerIdAndLogicType;
       foreach(LayerIdAndLogicType layerIdAndLogic,
               (QList<LayerIdAndLogicType>()
                << LayerIdAndLogicType("L", sliceLogic->GetLabelLayer())
//               << LayerIdAndLogicType("B", sliceLogic->GetBackgroundLayer())
//               << LayerIdAndLogicType("F", sliceLogic->GetForegroundLayer())
               ) )
         {
         
         QString sliceLayerId = layerIdAndLogic.first;
         vtkMRMLSliceLayerLogic * sliceLayerLogic = layerIdAndLogic.second;
         if (! sliceLayerLogic) 
           {
           this->PrintText("Failed to get slicelayer logic");
           return;
           }
         vtkMRMLVolumeNode * volumeNode = sliceLayerLogic->GetVolumeNode();
         this->CurrentLabelVolume = volumeNode;
         QString layerName = "None";
         // QString ijkAsString;
         QString valueAsString;
         if (volumeNode)
           {
           layerName = volumeNode->GetName();
           
           vtkMatrix4x4 * xyToIJK = sliceLayerLogic->GetXYToIJKTransform()->GetMatrix();
           double xyzw[4] = {xyz[0], xyz[1], xyz[2], 1.0};
           double ijkw[4] = {0.0, 0.0, 0.0, 0.0};
           xyToIJK->MultiplyPoint(xyzw, ijkw);
           //Q_ASSERT(ijkw[3] == 1.0);
           QList<double> ijk ;
           ijk << ijkw[0] <<  ijkw[1] <<  ijkw[2];
//            double * i_j_k;
//            i_j_k[0]=ijk[0];
//            i_j_k[1]=ijk[1];
//           i_j_k[2]=ijk[2];
//           this->cLastIJK=i_j_k;
          
//          ijkAsString = QString("(%1, %2, %3)").arg(qRound(ijk[0])).arg(qRound(ijk[1])).arg(qRound(ijk[2]));
         
           // from vtkSlicerDataProbeLogic::ProbePixel
           vtkMRMLScalarVolumeNode * scalarVolumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(volumeNode);
           vtkImageData * imageData = scalarVolumeNode->GetImageData();
           double labelIndex = imageData->GetScalarComponentAsDouble(ijkw[0],ijkw[1],ijkw[2],0); //ijkw[3]);
           vtkMRMLColorNode * colorNode = scalarVolumeNode->GetDisplayNode()->GetColorNode();
           labelName = colorNode->GetColorName(static_cast<int>(labelIndex));
           this->LastLabelName=labelName;
           // highly verbose debug print. Disabled for produciton.
           //this->PrintText(QString("LastLabelName is "+QString::fromStdString(labelName)));
            
           QString valueAsString;
           QStringList valueAsStrings;
           
           valueAsString = QString::fromStdString(labelName);
           valueAsStrings << valueAsString;
           valueAsStrings << " ("+QString::number(labelIndex)+")";
           
           valueAsString = valueAsStrings.join(", ");
           //QString lIdx = QString("%1").arg(labelIndex, /* fieldWidth= */ 0, /* format = */ 'g', /* precision= */ 4);
           
           d->StructureName->setText(valueAsString);
           /*
             if(d->DataProbeLogic)
             {
             int probeStatus = d->DataProbeLogic->ProbePixel(volumeNode, ijk[0], ijk[1], ijk[2]);
             if (probeStatus & vtkSlicerDataProbeLogic::PROBE_SUCCESS)
             {
             if (d->DataProbeLogic->GetPixelNumberOfComponents() > 3)
             {
             valueAsString = QString("%1 components").arg(d->DataProbeLogic->GetPixelNumberOfComponents());
             }
             else
             {
             QStringList valueAsStrings;
             for(int pixelValueIdx = 0; pixelValueIdx < d->DataProbeLogic->GetNumberOfPixelValues(); ++pixelValueIdx)
             {
             valueAsStrings << QString("%1").
             arg(d->DataProbeLogic->GetPixelValue(pixelValueIdx), / * fieldWidth= * / 0, / * format = * / 'g', / * precision= * / 4);
             }
             valueAsString = valueAsStrings.join(", ");
             }
             QString pixelDescription = QString::fromStdString(d->DataProbeLogic->GetPixelDescription());
             if (!pixelDescription.isEmpty())
             {
             pixelDescription.append(" ");
             if (probeStatus == vtkSlicerDataProbeLogic::PROBE_SUCCESS_LABEL_VOLUME ||
             probeStatus == vtkSlicerDataProbeLogic::PROBE_SUCCESS_LABEL_VOLUME_UNKNOWN_LABELNAME)
             {
             valueAsString = QString("(%1)").arg(valueAsString);
             }
             valueAsString.prepend(pixelDescription);
             }
             }
             else
             {
             valueAsString = QString::fromStdString(d->DataProbeLogic->GetPixelProbeStatusAsString());
             }
             }*/
           } // end if volumenode valid statement
         else
           {
           this->PrintText("Could not get volulmenode");
           }
         }
       }
       if ( FiducialPlacement && eventId == vtkMRMLMarkupsNode::MarkupAddedEvent) //LeftButtonReleaseEvent
         {
         this->PrintText("Added fiducial, now setting text labels");
         this->mrmlScene()->InitTraversal();
         vtkMRMLMarkupsFiducialNode * fiducialListNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(
           this->mrmlScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
         int numberMarkupsNodes = this->mrmlScene()->GetNumberOfNodesByClass("vtkMRMLMarkupsFiducialNode");
         if (numberMarkupsNodes == 0 ) 
           {
           this->PrintText("No markups nodes found");
           } 
         else
           {
           if ( ! fiducialListNode) 
             {
             test << " failed to get fiduciallist.\n";
             return;
             }
//              this->mrmlScene()->InitTraversal();
//              cNode = vtkMRMLMarkupsNode::SafeDownCast(this->mrmlScene()->GetNextNodeByClass("vtkMRMLMarkupsNode"));
           
//           test << "Number of markups nodes :" << numberMarkupsNodes << "\n";
           
           //int nFiducials=0;
           //nFiducials = fiducialListNode->GetNumberOfFiducials();//slicer crash, i think due to race condition between left click event and make fiducial
           this->NumberOfFiducials = fiducialListNode->GetNumberOfFiducials();//slicer crash, i think due to race condition between left click event and make fiducial
//           nFiducials = fiducialListNode->GetNumberOfMarkups(); //slicer crash
//           vtkMRMLMarkupsDisplayNode * disp = fiducialListNode->GetMarkupsDisplayNode();
           test << "Label setting "<< QString::fromStdString(this->LastLabelName) <<"\n"; 
           test << "nFiducials" << NumberOfFiducials<<"\n";
           if ( this->NumberOfFiducials>=1 ) 
             {
             fiducialListNode->SetNthFiducialLabel(this->NumberOfFiducials-1,this->LastLabelName); // fail no symbol
             }
           }
         
         for ( int fN=0;fN<0;fN++) //this->NumberOfFiducials;fN++ )
           {
//              test<< "fiducial "<<fN<<"\n";
//              vtkMRMLAnnotationFiducialNode * fNode =vtkMRMLAnnotationFiducialNode::SafeDownCast(this->mrmlScene()->GetNthNodeByClass(fN,"vtkMRMLAnnotationFiducialNode"));
//              std::string labelName = "test";      
//              fidNode->SetNthFiducialLabel(fN,labelName);
//              fNode->SetNthMarkupLabel(fN,labelName);
//              fNode->SetFiducialLabel("Test");

//       double xyz[4];
//       fidNode->GetNthFiducialPosition(fN,xyz);
// //       fidNode->;GetMarkupPointWorld(fN,0,xyz);
//        vtkMRMLSliceLayerLogic * sliceLayerLogic = vtkMRMLSliceLayerLogic::SafeDownCast(this->CurrentSliceLayerLogic);
//        vtkMatrix4x4 * xyToIJK = sliceLayerLogic->GetXYToIJKTransform()->GetMatrix();
//        double xyzw[4] = {xyz[0], xyz[1], xyz[2], 1.0};
//        double ijkw[4] = {0.0, 0.0, 0.0, 0.0};
//        xyToIJK->MultiplyPoint(xyzw, ijkw);
//        //Q_ASSERT(ijkw[3] == 1.0);
//        QList<double> ijk ;
//        ijk << ijkw[0] <<  ijkw[1] <<  ijkw[2];
//        vtkMRMLScalarVolumeNode * scalarVolumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(this->CurrentLabelVolume);
//        vtkImageData * imageData = scalarVolumeNode->GetImageData();
//        double labelIndex = imageData->GetScalarComponentAsDouble(ijkw[0],ijkw[1],ijkw[2],0); //ijkw[3]);
//        vtkMRMLColorNode * colorNode = scalarVolumeNode->GetDisplayNode()->GetColorNode();
//        std::String labelName = colorNode->GetColorName(static_cast<int>(labelIndex));
           
           fiducialListNode->SetNthFiducialLabel(fN,this->LastLabelName); // fail no symbol
           //labelName=fiducialListNode->GetNthFiducialLabel(fN);// fail no symbol
           
           }
         
       //        d->RowsOfLayerLabels[sliceLayerId].at(0)->setText(QString("<b>%1</b>").arg(layerName));
//        d->RowsOfLayerLabels[sliceLayerId].at(1)->setText(ijkAsString);
//        d->RowsOfLayerLabels[sliceLayerId].at(2)->setText(QString("<b>%1</b>").arg(valueAsString));

         }// end foreach layer loop
     
     }// end if event match statement
   else if(eventId == vtkCommand::LeaveEvent)
     {
     this->FiducialPlacement=false;
     this->ReadMousePosition=false;
     this->mrmlScene()->InitTraversal();
     vtkMRMLMarkupsFiducialNode * markupsNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(
       this->mrmlScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
     this->qvtkConnect(markupsNode, vtkMRMLMarkupsNode::MarkupAddedEvent,
                       this, SLOT(ProcessEvent(vtkObject*,void*,ulong,void*)));
     }
   else
     {
     this->PrintText("Unknown event");
     }
   
//   vtkCommand * out = new vtkCommand::MouseMoveEvent;
   
   return ;//out;
 } // end processevent


 
//------------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::LabelAtPosition() //vtkMRMLNode * node)// ,QString event)
//void qSlicerCIVM_AdultRatPGRModuleWidget::Processevent(vtkObject* object, unsigned long event, void* clientdata, void* calldata) //vtkMRMLNode * node)// ,QString event)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);  
  
//   if ( event == "LeaveEvent" ) 
//     {
//     d->StructureName->setText("");
//     this->PrintText("LeaveEvent");
//     } 
//   else if ( event == "EnterEvent" )
//     {
//     this->PrintText("EnterEvent");
//     }
//   else if ( event == "MouseMoveEvent" ) 
//     {
//     d->StructureName->setText("TEST");  
//     }
//   else
//     {
    
//     }
  //cNode=vtkMRMLCrosshairNode::SafeDownCast(node);
    

  
 
  /*

      layerLogicCalls = (('L', sliceLogic.GetLabelLayer),
                         ('F', sliceLogic.GetForegroundLayer),
                         ('B', sliceLogic.GetBackgroundLayer))
      for layer,logicCall in layerLogicCalls:
        layerLogic = logicCall()
        volumeNode = layerLogic.GetVolumeNode()
        
        layerLogic.GetXYToIJKTransform().GetMatrix()
    
      colorNode = volumeNode.GetDisplayNode().GetColorNode()
      if colorNode:
        labelValue = colorNode.GetColorName(labelIndex)
      return "%s (%d)" % (labelValue, labelIndex)

  */
  //sliceview.interactor.
  //vtkSliceViewInteractorinside sliceview.
    //qMRMLSliceView::convertRAStoXYZ()
  vtkMRMLSliceNode * sliceNode = NULL;
  vtkMRMLSliceLogic * sliceLogic = NULL;
  vtkMRMLApplicationLogic *mrmlAppLogic = this->logic()->GetMRMLApplicationLogic();
  vtkMRMLScene* currentScene = this->mrmlScene();
//  vtkMRMLNode * node = NULL;
  //vtkMRMLSelectionNode * selectionNode = NULL; 
  //selectionNode = mrmlAppLogic->GetSelectionNode();
  //currentScene->GetNodeByID(selectionNode->GetActiveViewID());//toStdString()
  //sliceNode=currentScene->GetNodeByID(mrmlAppLogic->GetSelectionNode->GetActivewViewID);//
  //sliceNode=currentScene->GetNodeByID(mrmlAppLogic->GetSelectionNode->GetActivewViewID);//
  
  //selectionNode=mrmlAppLogic->GetSelectionNode();
  //selectionNode->PrintSelf();
  
  //->GetActiveViewID();;
  //char * ID = selectionNode->GetActiveViewID();
  
  //sliceNode=vtkMRMLSliceNode::SafeDownCast(selectionNode->GetActiveViewID());
  //sliceNode = vtkMRMLSliceNode::SafeDownCast()  
  if (mrmlAppLogic && sliceNode)
    {
    sliceLogic = mrmlAppLogic->GetSliceLogic(sliceNode);

    if ( sliceLogic != NULL  ) 
      {
      vtkMRMLVolumeNode * volumeNode = NULL;
      //volumeNode = sliceLogic->GetBackgroundLayer();
      //qvolumeNode = sliceLogic->GetLabelLayer();
      //getxytoijktransform.getmatrix
//sliceLogic->GetLabelLayer;
//sliceLogic->GetLabelLayer;
      
    }

    }
  else 
    {
    this->PrintText("Failed to find slice logic");
    }

/*
        if ( sliceLogic != NULL  ) 
          {
//        this->PrintText("sliceLogic setNode");
          sliceLogic->SetSliceNode(sliceNode);
//        this->PrintText("SliceLogic Interaction");
          sliceLogic->StartSliceNodeInteraction(vtkMRMLSliceNode::ResetFieldOfViewFlag);
//        this->PrintText("SliceLogic FitSlice");

            sliceLogic->FitSliceToAll();
//           if (loadMoreFiles) 
//             {
//             }
//        this->PrintText("Slicenode UpdateMatricies");
          sliceNode->UpdateMatrices();
//        this->PrintText("SliceLogic Interaction end");
          sliceLogic->EndSliceNodeInteraction();
          }
        else 
          {
          this->PrintText("Failed to set slice logic");
          }

   */


  return ;
}

//------------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::ResetTransform()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);  
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout.  
  // Will probably fail if more than one layout is allowed. Oh well for now.
  QString         sliceNodeID = NULL;
  vtkMRMLNode             *sn = NULL;
  vtkMRMLSliceNode *sliceNode = NULL;


//  for (int snCounter=1;snCounter<3;snCounter++) //SceneNodes.size()-1
  sliceNode = vtkMRMLSliceNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLSliceNode"));

  while(sliceNode)
    {
//     sliceNodeID = QString("vtkMRMLSliceNode")+SceneNodes[snCounter];
//     sn          = currentScene->GetNodeByID(sliceNodeID.toStdString());
//     sliceNode   = NULL;
//     if ( sn != NULL ) 
//       { 
      sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode    
      if ( sliceNode != NULL ) 
        {
//        transform->SetMatrix(sliceNode->GetSliceToRAS());
//        transform->RotateX(this->Angle-d->SliderAngle->value()); //rotation-d->LastRotationValues[axisX]
        transform->Identity();
        sliceNode->GetSliceToRAS()->DeepCopy(transform->GetMatrix());
        sliceNode->UpdateMatrices();
        sliceNode->SetOrientationToReformat();
        }
//       }
      sliceNode = vtkMRMLSliceNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLSliceNode"));
    }

  return;
}
//-------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliceOffsetValue(double offset)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  //this->PrintMethod("SetSliceOffsetValue");
  // we're going to hard set this to the SceneNodes 1-2 in the list from list of 0-3
  // that should correspond to scene nodes Yellow and 1.
  
  // terribly specific to our case here, we're skipping the first image viewer.
  //  int snCounter;
//  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
  vtkMRMLScene* currentScene = this->mrmlScene();
  currentScene->InitTraversal();
  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout.
  // Will probably fail if more than one layout is allowed. Oh well for now.
  QString         sliceNodeID = NULL;
  vtkMRMLNode             *sn = NULL;
  vtkMRMLSliceNode *sliceNode = NULL;
  for (int snCounter=1;snCounter<SceneNodes.size()-1;snCounter++) 
    {
    sliceNodeID = QString("vtkMRMLSliceNode")+SceneNodes[snCounter];
    sn          = currentScene->GetNodeByID(sliceNodeID.toStdString());
    sliceNode   = NULL;
    if ( sn != NULL ) 
      { 
      sliceNode = vtkMRMLSliceNode::SafeDownCast(sn); //SliceNode    
      if ( sliceNode != NULL ) 
        { 
        sliceNode->SetSliceOffset(offset);
        } //endslicenode null check
      }// end sn null check
    }// end for
//   if (!d->SliceLogic)
//     {
//       return;
//     }
//   //qDebug() << "qSlicerCIVM_AdultRatPGRModuleWidget::setSliceOffsetValue:" << offset;
//   d->SliceLogic->StartSliceOffsetInteraction();
//   d->SliceLogic->SetSliceOffset(offset);
//   d->SliceLogic->EndSliceOffsetInteraction();
  return;
}


//-------------------------------------------------------------------------
// double qSlicerCIVM_AdultRatPGRModuleWidget::GetSliceOffsetValue(vtkMRMLNode * n)
// {
//   double offset;
//   vtkMRMLSliceNode sn = vtkMRMLSliceNode::SafeDownCast(n);
//   offset=sn->GetSliceOffset();
//   return offset;
// }`

//-------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliderRange(ctkSliderWidget * widge, std::vector<double> range) //vtkMRMLSliceMode
{
//  this->PrintMethod("SetSliderRange");
  if ( !widge ) 
    {
    this->PrintText("bad widge, skipping set range.");
    return; 
    }
  if ( range.size()==2 ) 
    {
    //this->PrintText("setMin");
    //widge->setMinimum(range[0]);
    //  this->PrintText("setMax");
    //widge->setMaximum=(range[1]);
//    this->PrintText("setRan");
    widge->setRange(range[0],range[1]);
//   this->PrintText("setTic");
//nope   widge->setTickInterval((range[1]-range[0])/256);
//   this->PrintText("setPStep");
//nope   widge->setPageStep((range[1]-range[0])/256);
//   this->PrintText("setTicPos");
//have to use position type.   widge->setTickPosition((range[1]-range[0])/2);
//    this->PrintText("setSStep");
    widge->setSingleStep((range[1]-range[0])/256);
    }
  return ;
} // */
//-------------------------------------------------------------------------
//std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRange(vtkImageData * node)
std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliceFOV(vtkMRMLNode * node)
{
  this->PrintMethod("GetSliceFOV");
  QTextStream test(stdout);  
  std::vector<double> fov (3,0);
  test << "\tSliceBounds init\n";
  double sliceBounds[6] = {0, -1, 0, -1, 0, -1};//xmin,xmax,ymin,ymax,zmin,zmax
  //double spacing[3] = {1, 1, 1 };

  vtkMRMLScalarVolumeNode *  localNode = NULL;
  test << "\tlocalNode init\n";
  localNode = vtkMRMLScalarVolumeNode::SafeDownCast(node);
  if ( localNode  == NULL ) 
    this->PrintText("Error getting the volume node");
  test << "\tlocalNode GetRASBounds\n";
  localNode->GetRASBounds(sliceBounds);
//  localNode->GetSpacing
  fov[0]=abs(sliceBounds[0]-sliceBounds[1]);
  fov[1]=abs(sliceBounds[2]-sliceBounds[3]);
  fov[2]=abs(sliceBounds[4]-sliceBounds[5]);
  test << "Returning fov \t";
  for(int it=0;it<fov.size(); it++)
    {
    test<< fov[it] << " \t";
    }
  test << "\n";
  return fov;
}
//-------------------------------------------------------------------------
//std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRange(vtkImageData * node)
std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRange(vtkMRMLNode * node)
{
  //this->PrintMethod("GetSliderRange");
  std::vector<double> range (2,0);
  range[0] = 0;
  range[1] = 1;
//   std::vector<double> xrange (2,0);
//   std::vector<double> yrange (2,0);
//   std::vector<double> zrange (2,0);
  double sliceBounds[6] = {0, -1, 0, -1, 0, -1};//xmin,xmax,ymin,ymax,zmin,zmax
  vtkMRMLScalarVolumeNode *  localNode = NULL;
  localNode = vtkMRMLScalarVolumeNode::SafeDownCast(node);
  localNode->GetRASBounds(sliceBounds);
  range[0]=sliceBounds[0];
  range[1]=sliceBounds[1];
//  QTextStream test(stdout);  
//  test << "Returning range \t";
//   for(int it=0;it<range.size(); it++)
//     {
//     test<< range[it] << " \t";
//     }
//   test << "\n";
//   double * fov;
//   double * spacing;
//   int * dims ;
//   double * origin; 

//   origin=node->GetOrigin();    //gets the image origin, not the centered bits...
//   test<< "\tgetorigin\n";
//   test << "\tX," << origin[0] << "Y," << origin[1] << "Z," << origin[2]
//        << "\n";
//   dims=node->GetDimensions();
// //   xrange[1]=dims[0]-origin[0];
// //   yrange[1]=dims[1]-origin[1];
// //   zrange[1]=dims[2]-origin[2];
//   spacing=node->GetSpacing();     // this is in the correct bits
//   test<< "\tGetfov\n";
//   for(int it=0; it<3;it++) 
//     {
//     fov[it]=dims[it]*spacing[it];
//     }
//   test << "\tX," << fov[0] << "Y," << fov[1] << "Z," << origin[2]
//        << "\n";
//   if ( this->CenterVolumeOnLoad ) 
//     {
//     test << "\tsetcentered\n";
//     origin[0]=fov[0]/2;
//     origin[1]=fov[1]/2;
//     origin[2]=fov[2]/2;
//     }
//   test<< "\tset min\n";
//   xrange[0]=floor(-origin[0]);
//   yrange[0]=floor(-origin[1]);
//   zrange[0]=floor(-origin[2]);
//   test<< "\tset max\n";
//   xrange[1]=ceil(fov[0]-origin[0]);
//   yrange[1]=ceil(fov[1]-origin[1]);
//   zrange[1]=ceil(fov[2]-origin[2]);

//   test << "X," << origin[0] << "Y," << origin[1]  << "Z," << origin[2]
//        << "\n"
//        << "X," << xrange[0]<< ":" << xrange[1] << " "
//        << "Y," << yrange[0]<< ":" << yrange[1] << " "
//        << "Z," << zrange[0]<< ":" << zrange[1] << " "
//        << "\n"
//        << "default," << range[0]<< ":"<< range[1] << " \n";
//   range=yrange;
//   test << "Returning range \t";
//   for(int it=0;it<range.size(); it++)
//     {
//     test<< range[it] << " \t";
//     }
//   test << "\n";
  return range;
}

//-------------------------------------------------------------------------
std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRangeSlice(vtkMRMLNode * n)
{

  this->PrintMethod("GetSliderRange");
  QTextStream test(stdout);

  std::vector<double> xrange (2,0);
  std::vector<double> yrange (2,0);
  std::vector<double> zrange (2,0);
  std::vector<double> range (2,0);
  double * fov;
  int * dims ;
  double * origin; 
  double sliceBounds[6] = {0, -1, 0, -1, 0, -1};
  range[0]=0;
  range[1]=1;
  vtkMRMLSliceNode * sn=NULL; 
  if ( n != NULL ) 
    { 
    sn = vtkMRMLSliceNode::SafeDownCast(n); //SliceNode
   }
  if ( sn != NULL ) 
    { 
    this->PrintText("Node good, proceding");
    //THis slice logic stuff causes a timebomb crash, i cant figure out why
    //vtkMRMLSliceLogic * sliceLogic = 0;
    //sliceLogic=vtkMRMLSliceLogic::New();
    //sliceLogic->SetSliceNode(sn);
    //sliceLogic->GetLowestVolumeSliceBounds(sliceBounds);
    //ex   Q_ASSERT(sliceBounds[4] <= sliceBounds[5]);
    //ex   q->setSliceOffsetRange(sliceBounds[4], sliceBounds[5]);
    //   range[0]=sliceBounds[4];
    //   range[1]=sliceBounds[5];

    origin=sn->GetXYZOrigin();    //gets the volume origin, not the centered bits...
    

    
    test<< "\tGetfov\n";
    fov=sn->GetFieldOfView();     // this is in the correct bits
    test << "\tX," << fov[0] << "Y," << fov[1] << "Z," << origin[2]
         << "\n";
    test<< "\tgetorigin\n";

    test << "\tX," << origin[0] << "Y," << origin[1] << "Z," << origin[2]
         << "\n";
    if ( this->CenterVolumeOnLoad ) 
      {
      test << "\tsetcentered\n";
      origin[0]=fov[0]/2;
      origin[1]=fov[1]/2;
      origin[2]=fov[2]/2;
      }
    test<< "\tset min\n";
    xrange[0]=floor(-origin[0]);
    yrange[0]=floor(-origin[1]);
    zrange[0]=floor(-origin[2]);
    test<< "\tset max\n";
    xrange[1]=ceil(fov[0]-origin[0]);
    yrange[1]=ceil(fov[1]-origin[1]);
    zrange[1]=ceil(fov[2]-origin[2]);
    


    //dims=sn->GetUVWDimensions();// these are 256x256x1? why
    //xrange[1]=dims[0]-origin[0];    yrange[1]=dims[1]-origin[1];    zrange[1]=dims[2]-origin[2];

    //  test << sliceBounds[0] << " " << sliceBounds[1] << " " << sliceBounds[2] << " " 
    //       << sliceBounds[3] << " " << sliceBounds[4] << " " << sliceBounds[5] << " \n" ;
    test << "X," << origin[0] << "Y," << origin[1]  << "Z," << origin[2]
         << "\n"
         << "X," << xrange[0]<< ":" << xrange[1] << " "
         << "Y," << yrange[0]<< ":" << yrange[1] << " "
         << "Z," << zrange[0]<< ":" << zrange[1] << " "
         << "\n"
         << "default," << range[0]<< ":"<< range[1] << " \n";
    range=yrange;
    }else { 
  this->PrintText("Node bad failed to get range");
  }
  
//*/
  test << "Returning range \t";
  for(int it=0;it<range.size(); it++) {
  test<< range[it] << " \t";
  }
  test << "\n";
  return range;
}

//-----------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::onDeleteAllMarkupsInListPushButtonClicked()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  // get the active node
  this->mrmlScene()->InitTraversal();
  vtkMRMLMarkupsFiducialNode * mrmlNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(
    this->mrmlScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
//  vtkMRMLNode *mrmlNode = d->activeMarkupMRMLNodeComboBox->currentNode();
  vtkMRMLMarkupsNode *listNode = NULL;
  if (mrmlNode)
    {
    listNode = vtkMRMLMarkupsNode::SafeDownCast(mrmlNode);
    }
  if (listNode)
    {
    // qDebug() << "Removing markups from list " << listNode->GetName();
    ctkMessageBox deleteAllMsgBox;
    deleteAllMsgBox.setWindowTitle("Delete All Markups in this list?");
    QString labelText = QString("Delete all ")
      + QString::number(listNode->GetNumberOfMarkups())
      + QString(" Markups in this list?");
    // don't show again check box conflicts with informative text, so use
    // a long text
    deleteAllMsgBox.setText(labelText);
    deleteAllMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    deleteAllMsgBox.setDefaultButton(QMessageBox::Yes);
    deleteAllMsgBox.setDontShowAgainVisible(true);
    deleteAllMsgBox.setDontShowAgainSettingsKey("Markups/AlwaysDeleteAllMarkups");
    int ret = deleteAllMsgBox.exec();
    if (ret == QMessageBox::Yes)
      {
      listNode->RemoveAllMarkups();
      }
    }
}


/*
void qSlicerCIVM_AdultRatPGRModuleWidget::IncrimentSlider(ctkSliderWidget * )
{
  return;
}
void qSlicerCIVM_AdultRatPGRModuleWidget::DecrimentSlider(ctkSliderWidget * )
{
  return;
}

//-------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::getSliceOffsetValue(double offset)
{
    Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  if (!d->SliceLogic)
    {
      return;
    }
  //qDebug() << "qSlicerCIVM_AdultRatPGRModuleWidget::trackSliceOffsetValue";
  d->SliceLogic->StartSliceOffsetInteraction();
  d->SliceLogic->SetSliceOffset(offset);
}*/

// simple function to check if a node is part of the scene already.
bool qSlicerCIVM_AdultRatPGRModuleWidget::NodeExists(QString nodeName)
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->Superclass::setMRMLScene(currentScene);

  // Search the scene for the nodes and return true on match
  currentScene->InitTraversal();
  for (vtkMRMLNode *sn = NULL; (sn=currentScene->GetNextNode());) {
    if (sn->GetName()==nodeName) {
      this->PrintText(""+nodeName +" found.");
      return true;
    } else {
      //this->PrintText("examined node "+nameTemp+"not the same as "+nodeName);
    }
  }
  this->PrintText(""+nodeName+" not in open nodes.");
  return false;
}

//---------------------------------------------------------------------------
QStringList qSlicerCIVM_AdultRatPGRModuleWidget::SetLayout()  //QString layout
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->PrintMethod(QString("SetLayout"));
  bool setSliceOrient=true;
  //  qSlicerAppMainWindow main=
  //  vtkMRMLNode currentScene->GetNextNodeByClass("vtkMRMLLayoutNode")
  currentScene->InitTraversal();
  vtkMRMLLayoutNode* sceneLayoutNode = vtkMRMLLayoutNode::SafeDownCast(currentScene->GetNextNodeByClass("vtkMRMLLayoutNode") );
  QStringList sceneNodes;
   if ( this->Layout=="PGR")
    {
    //Two over Two
    sceneNodes << "Red"
    << "Yellow"
    << "Slice4"
    << "Green";
    //vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView
    this->PrintText("\tLayout should have 2 volumes");
    sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView);
    }
   else if ( this->Layout=="Dual3D")
    {
      sceneNodes << "Red"
		 << "Green"  
		 << "Yellow"
		 << "1"
		 << "2";
      setSliceOrient=true;
      //vtkMRMLLayoutNode::SlicerLayoutFourUpView
      sceneLayoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutDual3DView);
      this->PrintText("\tLayout should have 2 volumes");
    }  
    else
    {
      this->PrintText(QString("Bad layout selection "+Layout));
    }
   return sceneNodes;
}

void qSlicerCIVM_AdultRatPGRModuleWidget::PrintMethod(const QString text)
{
  QString pass="qSlicerCIVM_AdultRatPGRModuleModuleWidget method:"+text;
  this->PrintText(pass);
  return;
}


void qSlicerCIVM_AdultRatPGRModuleWidget::PrintText(const QString text)
{
  QTextStream out(stdout);
  out << text<<"\n";
  return;
}

