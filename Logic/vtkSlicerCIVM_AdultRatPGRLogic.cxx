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

// CIVM_AdultRatPGR Logic includes
#include "vtkSlicerCIVM_AdultRatPGRLogic.h"

// MRML includes

// VTK includes
#include <vtkNew.h>
#include <vtkMRMLMarkupsFiducialNode.h>
// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerCIVM_AdultRatPGRLogic);

//----------------------------------------------------------------------------
vtkSlicerCIVM_AdultRatPGRLogic::vtkSlicerCIVM_AdultRatPGRLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerCIVM_AdultRatPGRLogic::~vtkSlicerCIVM_AdultRatPGRLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  //  events->InsertNextValue(vtkMRMLScene::SceneEditedEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
  //  vtkErrorMacro("Logic:UpdateFromMRMLScene");
}

//---------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{

//   int nFiducials=0;
//   vtkErrorMacro("Logic:gotnodeadded");
//   this->GetMRMLScene()->InitTraversal();
//   vtkMRMLMarkupsFiducialNode * fiducialListNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(this->GetMRMLScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
//   //Superclass::NumberOfFiducials = fiducialListNode->GetNumberOfFiducials();
//   if ( fiducialListNode) 
//     {
//     nFiducials=fiducialListNode->GetNumberOfFiducials();
//     }
  //cout << "fiducials after node addition " << nFiducials;// pretty sure this causes a crash
}
// //---------------------------------------------------------------------------
// void vtkSlicerCIVM_AdultRatPGRLogic::OnMRMLSceneEdited(vtkMRMLNode* vtkNotUsed(node))
// {
//   int nFiducials=0;
//   vtkErrorMacro("Logic:SceneEdited");
//   this->GetMRMLScene()->InitTraversal();
//   vtkMRMLMarkupsFiducialNode * fiducialListNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(this->GetMRMLScene()->GetNextNodeByClass("vtkMRMLMarkupsFiducialNode"));
//   //Superclass::NumberOfFiducials = fiducialListNode->GetNumberOfFiducials();
//   if ( fiducialListNode) 
//     {
//     nFiducials=fiducialListNode->GetNumberOfFiducials();
//     }
//   //cout << "fiducials after node addition " << nFiducials;
// }

//---------------------------------------------------------------------------
void vtkSlicerCIVM_AdultRatPGRLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

