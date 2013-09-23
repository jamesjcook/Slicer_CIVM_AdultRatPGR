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
#include "vtkMRMLScalarVolumeNode.h"
//#include <vtkImageData.h>

#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLLayoutLogic.h"

#include <vtkCollection.h>
#include <vtkSmartPointer.h>

#include <ctkSliderWidget.h>
#include <vtkTransform.h>
#include "vtkMRMLApplicationLogic.h"


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerCIVM_AdultRatPGRModuleWidgetPrivate: public Ui_qSlicerCIVM_AdultRatPGRModuleWidget
{
public:
//  void sliceConnector(vtkMRMLNode *n, qSlicerLayoutManager *lm);
//qSlicerCIVM_AdultRatPRGModuleWidget::
//  void sliceConnector(vtkMRMLNode *n, qSlicerLayoutManager *layoutManager) ;
  typedef std::map<vtkSmartPointer<vtkMRMLNode>, qMRMLViewControllerBar* > ControllerMapType;
  ControllerMapType ControllerMap;
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
//  Q_Q(qSlicerCIVM_AdultRatPGRModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  //this->SliceLogic = 0;
  // insert init and static data definitions here.
  this->Layout=QString("PGR");
  this->GalleryName="CIVM_AdultRatAtlas";
  this->CenterVolumeOnLoad=true;
//   double sliderPos[2];
//   sliderPos[0]=0;
//   sliderPos[1]=0;
//   this->Pos=sliderPos;

//   double SliderAngle[4];
//   this->Angle=SliderAngle;
//   Angle[0]=0;
//   Angle[1]=0;
//   Angle[2]=0;
//   Angle[3]=0;

//  this->SlicePointer=0;
   this->Angle=0;
   this->Pos=0;
  //  this->LoadLabels=false;
  
  /* Load data paths */
  /* hardcode for now*/
  /* data is at /DataPath/{timepoint}/average/p{timepoint}_average_{contrast}.nii  */
  /* labels are at /DataPath/labels/completed/completed_good_header/nii/           */
  
  this->DataPath=QString("/ratdevatlas/timepoint/average/");
  this->LabelPath=QString("/ratdevatlas/labels/completed/completed_good_header/nii/");
  //  this->DataPattern  << "p"   << "timepoint" << "_average_" << "contrast" << ".nii";
  //  this->LabelPattern << "pnd" << "timepoint" << "_average_labels.nii";

  //this->DataPattern  =QString("ptimepoint_average_contrast.nii");
  this->DataPattern  =QString("ptimepoint_average_contrast.nii");
  this->LabelPattern =QString("pndtimepoint_average_labels.nii");
  //Timepoint=80; using the same code as our time/contrast bit so we can switch between timepoints for more rapid testing.
  // see the buildscene function for the list of times allowed.
  ImageContrasts << "ad" << "adc" << "b0" << "dwi" << "fa" << "fa_color"  << "freq_16" << "gre_16" << "rd";
  
  //  void QComboBox::setItemText ( int index, const QString & text )
  //  void QComboBox::insertItems ( int index, const QStringList & list )
  d->ComboBoxA->insertItems(0, ImageContrasts );
  d->ComboBoxB->insertItems(0, ImageContrasts );

  d->ComboBoxA->setDefaultText("Select Sagittal/Image1 Contrast");
  d->ComboBoxB->setDefaultText("Select Axial/Image2 Contrast");

  d->ComboBoxA->setCurrentIndex(-1);
  d->ComboBoxB->setCurrentIndex(-1);

  //insert button connections here.
  connect(d->HistologySelector,SIGNAL(clicked()),SLOT(HistologySelectionDialog()));
  //insert drop down connections here.
  connect(d->ComboBoxA,SIGNAL(currentIndexChanged(int)),SLOT(BuildScene()));
  connect(d->ComboBoxB,SIGNAL(currentIndexChanged(int)),SLOT(BuildScene()));
  //Connect our slice slider here.
  connect(d->SliderSlice, SIGNAL(valueChanged(double)), SLOT(SetSliceOffsetValue(double)), Qt::QueuedConnection);
  connect(d->SliderAngle, SIGNAL(valueChanged(double)), SLOT(SetSliceGeometry(double)), Qt::QueuedConnection);

  //  connect(d, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)), d->SliderSlice, SLOT(setMRMLScene(vtkMRMLScene*)));
  //  connect(d->SettingsPanel,SIGNAL(initalized()),SLOT(BuildScene())); // there is no initalized settings! :p
  //  this->BuildScene(); cannot simply call functions in here befcause the class is not set up yet.

  //this->SliceOffsetSlider = new qMRMLSliderWidget(q);
  d->SliderSlice->setTracking(true);
}

QStringList qSlicerCIVM_AdultRatPGRModuleWidget::GetContrasts()
{
  this->PrintMethod(QString("GetContrasts"));
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);

  //forms comaseparated list of contrast strings
  QStringList contrast_list;
  QStringList all_contrasts;
  if ( d->ComboBoxA->currentText() != "" && d->ComboBoxA->currentIndex() !=-1 ) 
    {
    contrast_list << d->ComboBoxA->currentText();
    } else 
    {
    this->PrintText("ComboBoxA undefined\n");
    }
  if ( d->ComboBoxB->currentText() != "" && d->ComboBoxB->currentIndex() !=-1 )
    {
    contrast_list << d->ComboBoxB->currentText();
    } else 
    { 
    this->PrintText("ComboBoxB undefined\n");
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
  this->PrintText("Did not find "+nodeName+" in open nodes.");
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
QString qSlicerCIVM_AdultRatPGRModuleWidget::HistologySelectionDialog()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget); 
  QString hist_path = "dummy file";
  return hist_path;
}
//-----------------------------------------------------------------------------
// build mrml and load datasests
void qSlicerCIVM_AdultRatPGRModuleWidget::BuildScene()
{
  this->PrintMethod(QString("BuildScene"));
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget); //i think the Q_D line connects us back to our parent widget
    
  // last minute settings read
  /*
    QStringList timepointList = this->GetTimepoints();
  */

  QStringList timepointList;
  timepointList<< "80";
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
  //DataPath
  //LabelPath
  //DataPattern (replace timepoint with numbers and contrast with abreviation)
  //LabelPattern(replace timepoint with numbers)

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
  orientationOverride  << "Coronal" << "Sagittal" << "Sagittal" << "Sagittal";
  
  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
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
        this->PrintText("imageProperties << "+imageFile);
        this->PrintText("Added volume to load list:"+imagePath);
        /*if( LoadLabels )
          {
          nodeName                   = labelNode;
          tParams["fileName"]        = labelPath;
          tParams["nodeName"]        = nodeName;
          tParams["labelmap"]        = true;
          tParams["center"]          = this->CenterVolumeOnLoad;
          tParams["autoWindowLevel"] = false;
          labelProperties << tParams;
          this->PrintText("labelProperties << "+labelFile);
          this->PrintText("Added labels to load list:"+labelPath);
          }*/
        }
      else 
        {
        this->PrintText("Bad or unset GalleryName.");
        return;
        }
      }//timepoint foor loop end
    }//contrast timepoint loop end.

  int snCounter;// this is for scene nodes later but right now its for any image we want loaded.
  //loop for all images to add only unloaded to load list
  for(snCounter=0;snCounter<imageProperties.size();snCounter++)
    {
    if ( ! NodeExists(imageProperties[snCounter]["nodeName"].toString()) )
      {
      unloadedFiles << imageProperties[snCounter];
      } 
    }
  //loop for all labels to add only unloaded to load list need to do a unique element on this list... (bleh), the qset might do the trick
  QSet<QString> labelNodes;
  for(snCounter=0;snCounter<labelProperties.size();snCounter++)
    {
    if ( ! NodeExists(labelProperties[snCounter]["nodeName"].toString()) )
      {
      this->PrintText("-");
      if( labelNodes.isEmpty() ) 
        {
        this->PrintText(" +");
        unloadedFiles << labelProperties[snCounter];
        labelNodes << labelProperties[snCounter]["nodeName"].toString(); 
        } 
      else 
        {
        this->PrintText("-- ");
        if( ! labelNodes.contains(labelProperties[snCounter]["nodeName"].toString()) ) 
          {
          this->PrintText("adding "+labelProperties[snCounter]["nodeName"].toString()+"to loadlist");
          unloadedFiles << labelProperties[snCounter];
          labelNodes << labelProperties[snCounter]["nodeName"].toString();
          }
        else 
          {
          this->PrintText("  XXXXXX");
          }
        }
      }
    else 
      {
      this->PrintText("Found label"+labelProperties[snCounter]["nodeName"].toString());
      }
    }

  //load data
  d->SliderSlice->reset();//setValue(0);//
  d->SliderAngle->reset();//setValue(0);//
  this->ResetTransform();
  s_app_obj->coreIOManager()->loadNodes(unloadedFiles); // images
  currentScene->InitTraversal();
  currentScene->GetNextNodeByClass("vtkMRMLLayoutNode"); // correct method it would appear to get the layout.  
//  vtkImageData * imageData = NULL;
//  imageData=vtkImageData::SafeDownCast(currentScene->GetNextNodeByClass("vtkImageData"));
  std::vector<double> range (2,0);
  vtkMRMLNode * vNode = currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
//   scalarvolumenode=currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode");//vtkMRMLScalarVolumeNode::SafeDownCast();
  
   while( vNode != NULL)
     {
     this->PrintText("Getting range\n");
     range=this->GetSliderRange(vNode);
     vNode=currentScene->GetNextNodeByClass("vtkMRMLScalarVolumeNode"); 
     }
   this->PrintText("Setting range\n");
   this->SetSliderRange(d->SliderSlice,range);
//   if (sliceNode !=NULL && snCounter>0 && snCounter<3) 
//     { 
//     this->PrintText("Setting range\n");
//     // commented code works, it was just harder to debug.
//     std::vector<double> range=this->GetSliderRange(sliceNode);
//     QTextStream test(stdout);
//     test << range[0] << range[1] << "\n";
//     this->SetSliderRange(d->SliderSlice,range);
//     //this->SetSliderRange(d->SliderSlice,this->GetSliderRange(sliceNode));
//     } else 
//     { 
//     this->PrintText("Skipped Setting range!\n");
//     }
  
  ////
  //arrange objects.
  ////
  //assign slices
  //assign 3d windows, 
  //for now just set properties in 3d views, rather than setting the volume settings.
  snCounter=0;
  int imCounter;
  for (snCounter=0;snCounter<SceneNodes.size();snCounter++) 
    {
    imCounter=floor(snCounter/2);
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
    if (scNode !=NULL && snCounter==1 ) // && snCounter<3) 
    {
    scNode->SetSliceIntersectionVisibility(1);
    }
    //<>//
    if ( scNode != NULL )  
    //if we have a composinte node sucessfully downcast to assign volumes to
      { //vtkMRMLSliceCompositeNode
      this->PrintText("SliceComposite ready:"+sliceCompositeNodeID);
      //set default orientations according to slicer rules.
      if(SceneNodes.size()==orthogonalOrientations.size() && snCounter<orthogonalOrientations.size() )
        {
        this->PrintText("Setting default orientation\n");
        sliceNode->SetOrientation(orthogonalOrientations[snCounter].toLatin1());
        }
      else 
        {
        this->PrintText("No default orientaion");
        }
      // if specific orientations are desired clean up by setting them now. 
      if(setSliceOrient) 
        {
        this->PrintText("Setting override orientation");
        scNode->SetLinkedControl(1);
        sliceNode->SetOrientation(orientationOverride[snCounter].toLatin1());
	sliceNode->SetOrientationToReformat();
        }
      else 
        {
        this->PrintText("No orientation override. Not setting.");
        }
      //<<.>>//
      if(imCounter<imageProperties.size()) //prevents bad index crash
        {
        this->PrintText("SetBackgroundVolumeID:"+imageProperties[imCounter]["nodeName"].toString());
        scNode->SetBackgroundVolumeID(this->NodeID(imageProperties[imCounter]["nodeName"].toString())); //set image
        if( LoadLabels )
          { 
          this->PrintText("SetLabelVolumeID:"+labelProperties[imCounter]["nodeName"].toString());
          scNode->SetLabelVolumeID(this->NodeID(labelProperties[imCounter]["nodeName"].toString()));	//set labels
          /* if(GalleryName=="Multiple Sclerosis") //MSSingle" || GalleryName=="MSDual" || GalleryName=="MSComparison" )
             {
             scNode->SetLabelOpacity(1.0); 
             sliceNode->SetUseLabelOutline(true);
             if (this-> Layout=="MSComparison")
             {
             if(SceneNodes[snCounter]=="Red"||SceneNodes[snCounter]=="Green")
             {
             this->PrintText("turning off labels for this viewer");
             scNode->SetLinkedControl(0);
             scNode->SetLabelOpacity(0); 
             scNode->SetLinkedControl(1);
             }
             }
             }
             else 
             {// */
          scNode->SetLabelOpacity(0.25); 
          sliceNode->SetUseLabelOutline(false);
          //}
          }
        else 
          {
          this->PrintText("LabelID=NULL");
          scNode->SetLabelVolumeID(NULL);
          }
        }
      else 
        { //there are more scene nodes than there are volumes. 
        //ifnodis yellow, 
        /*if ( SceneNodes[snCounter]=="yellow" ) 
          {//should only run on dual3d
          scNode->SetForegroundVolumeID(this->NodeID(imageProperties[1]["nodeName"].toString())); // set overlay
          scNode->SetForegroundOpacity(0.50);
          }// */
        
        }
      //end check for good scNode
      }
    else 
      { //we'll enter this code if we are on orthogonal or dual3d for the 3d volumes
      this->PrintText("SliceComposite bad:"+sliceCompositeNodeID+" could not downcast!");
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

  if (d->SliceLogic && d->SliceLogic->GetMRMLScene())
    {
      this->setMRMLScene(d->SliceLogic->GetMRMLScene());
    }

  d->onSliceLogicModifiedEvent();
  }
*/


// --------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliceGeometry(double value)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
//  this->PrintMethod("SetSliceGeometry\n");
  // get a reformat widget aimed at the yellow slice, read the current orientation of the yellow slice for the starting point. 
  // read the values for offest and angle
  // apply them to the reformat widget
  // set viewers 1/2 of 0-3 to the reformat widget(just to be sure).
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();


  // terribly specific to our case here, we're skipping the first image viewer.
  //  int snCounter;
  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
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
//         transform->RotateX(this->Angle[snCounter]-d->SliderAngle->value()); //rotation-d->LastRotationValues[axisX]
//         Angle[snCounter]=d->SliderAngle->value();

        transform->RotateX(this->Angle-d->SliderAngle->value()); //rotation-d->LastRotationValues[axisX]
        sliceNode->GetSliceToRAS()->DeepCopy(transform->GetMatrix());
        sliceNode->UpdateMatrices();
        sliceNode->SetOrientationToReformat();
        } //endslicenode null check
      }// end sn null check
    }// end for
  Angle=d->SliderAngle->value();
  return;
}

void qSlicerCIVM_AdultRatPGRModuleWidget::ResetTransform()
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);  
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
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
// --------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliceOffsetValue(double offset)
{
  Q_D(qSlicerCIVM_AdultRatPGRModuleWidget);
  // we're going to hard set this to the SceneNodes 1-2 in the list from list of 0-3
  // that should correspond to scene nodes Yellow and 1.
  
  // terribly specific to our case here, we're skipping the first image viewer.
  //  int snCounter;
  qSlicerApplication * s_app_obj = qSlicerApplication::application(); //set application linking.
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


// --------------------------------------------------------------------------
// double qSlicerCIVM_AdultRatPGRModuleWidget::GetSliceOffsetValue(vtkMRMLNode * n)
// {
//   double offset;
//   vtkMRMLSliceNode sn = vtkMRMLSliceNode::SafeDownCast(n);
//   offset=sn->GetSliceOffset();
//   return offset;
// }`

// --------------------------------------------------------------------------
void qSlicerCIVM_AdultRatPGRModuleWidget::SetSliderRange(ctkSliderWidget * widge, std::vector<double> range) //vtkMRMLSliceMode
{
  this->PrintMethod("SetSliderRange");
  if ( !widge ) 
    {
    this->PrintText("bad widge, skipping set range.\n");
    return; 
    }
  if ( range.size()==2 ) 
    {
    //this->PrintText("setMin\n");
    //widge->setMinimum(range[0]);
    //  this->PrintText("setMax");
    //widge->setMaximum=(range[1]);
    this->PrintText("setRan\n");
    widge->setRange(range[0],range[1]);
//   this->PrintText("setTic\n");
//nope   widge->setTickInterval((range[1]-range[0])/256);
//   this->PrintText("setPStep\n");
//nope   widge->setPageStep((range[1]-range[0])/256);
//   this->PrintText("setTicPos\n");
//have to use position type.   widge->setTickPosition((range[1]-range[0])/2);
    this->PrintText("setSStep\n");
    widge->setSingleStep((range[1]-range[0])/256);
    }
  return ;
} // */
// --------------------------------------------------------------------------
//std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRange(vtkImageData * node)
std::vector<double> qSlicerCIVM_AdultRatPGRModuleWidget::GetSliderRange(vtkMRMLNode * node)
{
  this->PrintMethod("GetSliderRange");
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
    QTextStream test(stdout);  
    test << "Returning range \t";
    for(int it=0;it<range.size(); it++)
      {
      test<< range[it] << " \t";
      }
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

// --------------------------------------------------------------------------
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
    this->PrintText("Node good, proceding\n");
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
  this->PrintText("Node bad failed to get range\n");
  }
  
//*/
  test << "Returning range \t";
  for(int it=0;it<range.size(); it++) {
  test<< range[it] << " \t";
  }
  test << "\n";
  return range;
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

// --------------------------------------------------------------------------
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



QStringList qSlicerCIVM_AdultRatPGRModuleWidget::SetLayout()  //QString layout
{
  vtkMRMLScene* currentScene = this->mrmlScene();
  this->PrintMethod(QString("SetLayout"));
  bool setSliceOrient=true;
  //  qSlicerAppMainWindow main=
  //  vtkMRMLNode currentScene->GetNextNodeByClass("vtkMRMLLayoutNode")
  currentScene->InitTraversal();
  vtkMRMLLayoutNode* sceneLayoutNode = vtkMRMLLayoutNode::SafeDownCast(
								       currentScene->GetNextNodeByClass("vtkMRMLLayoutNode") );
  QStringList sceneNodes;
   if ( this->Layout=="PGR")
    {
    //Two over Two
    sceneNodes << "Red"
    << "Yellow"
    << "Slice1"
    << "Green";
    //vtkMRMLLayoutNode::SlicerLayoutTwoOverTwoView
    this->PrintText("Layout should have 2 volumes");
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
      this->PrintText("Layout should have 2 volumes");
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

