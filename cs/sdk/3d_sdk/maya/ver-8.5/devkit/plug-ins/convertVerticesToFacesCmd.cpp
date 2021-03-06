//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+

// DISCLAIMER: THIS PLUGIN IS PROVIDED AS IS.  IT IS NOT SUPPORTED BY

//            AUTODESK, SO PLEASE USE AND MODIFY AT YOUR OWN RISK.

//

// PLUGIN NAME: Convert Vertices To Contained Faces Command v1.0

// FILE: convertVerticesToFacesCmd.cpp

// DESCRIPTION: o MEL command that converts a selection of vertices into a selection of faces that interconnect

//                the original vertices (i.e. only faces whose composite vertices are contained in the original

//                vertex selection).  The command's return value is a string array that contains the names of all

//                of the new contained faces.

//              o This MEL command has no flags, returns a string array, and operates on selected vertices.

//              o Example MEL usage:

//                   select -r pCube1.vtx[0:5];

//                   string $convertedFaces[] = `convertVerticesToFaces`;

//                   // Result: pCube1.f[0:1] //

// AUTHOR: QT

// LAST UPDATED: Oct. 11th, 2001.

// TESTED ON: Maya 4.0 on Windows

// STEPS TO INSTALL: 1) Unzip the file convertVerticesToFaces_v1_0_Maya4_Windows.zip to any temporary directory.

//                   2) Copy the file convertVerticesToFacesCmd.mll to: /AW/Maya4.0/bin/plug-ins/

//                   3) Launch Maya and load the plugin from Windows->Setting/Preferences->Plugin Manager.





// BEGIN PLUGIN:

// =============





// HEADER FILES:

#include <maya/MFnPlugin.h>

#include <maya/MPxCommand.h>

#include <maya/MGlobal.h>

#include <maya/MSelectionList.h>

#include <maya/MItSelectionList.h>

#include <maya/MObject.h>

#include <maya/MDagPath.h>

#include <maya/MItMeshVertex.h>

#include <maya/MItMeshPolygon.h>





// MAIN CLASS FOR THE MEL COMMAND:

class convertVerticesToContainedFacesCommand : public MPxCommand

{

   public:

      convertVerticesToContainedFacesCommand();

      virtual ~convertVerticesToContainedFacesCommand();

      static void* creator();

      bool isUndoable() const;



      MStatus doIt(const MArgList&);

      MStatus redoIt();

      MStatus undoIt();



   private:

      MSelectionList previousSelectionList;

};





// CONSTRUCTOR:

convertVerticesToContainedFacesCommand::convertVerticesToContainedFacesCommand()

{

}





// DESTRUCTOR CLEARS, AND FREES MEMORY OF THE PREVIOUS SELECTION LIST:

convertVerticesToContainedFacesCommand::~convertVerticesToContainedFacesCommand()

{

   previousSelectionList.clear();

}





// FOR CREATING AN INSTANCE OF THIS COMMAND:

void* convertVerticesToContainedFacesCommand::creator()

{

   return new convertVerticesToContainedFacesCommand;

}





// MAKE THIS COMMAND UNDOABLE:

bool convertVerticesToContainedFacesCommand::isUndoable() const

{

   return true;

}





// SAVE THE PREVIOUS SELECTION, AND THEN CALL redoIt(), WHICH DOES MOST OF THE WORK:

MStatus convertVerticesToContainedFacesCommand::doIt(const MArgList& args)

{

   MGlobal::getActiveSelectionList(previousSelectionList);

   return redoIt();

}





// CONVERTS THE SELECTED VERTICES TO "CONTAINED FACES", AND RETURN FROM THE MEL COMMAND, A STRING ARRAY OF THE NEW FACES:

MStatus convertVerticesToContainedFacesCommand::redoIt()

{

   MSelectionList finalFacesSelection;

   MDagPath meshDagPath;

   MObject multiVertexComponent, singleVertexComponent;

   int dummyIndex;



   // ITERATE THROUGH EACH "VERTEX COMPONENT" THAT IS CURRENTLY SELECTED:

   for (MItSelectionList vertexComponentIter(previousSelectionList, MFn::kMeshVertComponent); !vertexComponentIter.isDone(); vertexComponentIter.next())

   {

      // STORE THE DAGPATH, COMPONENT OBJECT AND MESH NAME OF THE CURRENT VERTEX COMPONENT:

      vertexComponentIter.getDagPath(meshDagPath, multiVertexComponent);

      MString meshName = meshDagPath.fullPathName();



      // VERTEX COMPONENT HAS TO CONTAIN AT LEAST ONE VERTEX:

      if (!multiVertexComponent.isNull())

      {

         // ITERATE THROUGH EACH "VERTEX" IN THE CURRENT VERTEX COMPONENT:

         for (MItMeshVertex vertexIter(meshDagPath, multiVertexComponent); !vertexIter.isDone(); vertexIter.next())

         {

            // FOR STORING THE FACES CONNECTED TO EACH VERTEX:

            MIntArray connectedFacesIndices;

            vertexIter.getConnectedFaces(connectedFacesIndices);



            // ITERATE THROUGH EACH FACE CONNECTED TO THE CURRENT VERTEX:

            MItMeshPolygon faceIter(meshDagPath);

            for (unsigned i=0; i<connectedFacesIndices.length(); i++)

            {

               // GET THE VERTEX INDICES FOR CURRENT FACE:

               MIntArray faceVerticesIndices;

               faceIter.setIndex(connectedFacesIndices[i], dummyIndex);

               faceIter.getVertices(faceVerticesIndices);



               // ITERATE THROUGH EACH FACE VERTEX TO SEE IF EACH ONE BELONGS TO THE ORIGINAL VERTEX SELECTION:

               bool faceIsContained=1;

               for (unsigned j=0; j<faceVerticesIndices.length(); j++)

               {

                  // CREATE A SELECTION LIST FOR EACH VERTEX TO USE IN THE ::hasItem() METHOD:

                  MSelectionList singleVertexList;

                  singleVertexList.clear();

                  MString vertexName = meshName;

                  vertexName += ".vtx[";

                  vertexName += faceVerticesIndices[j];

                  vertexName += "]";

                  singleVertexList.add(vertexName);

                  singleVertexList.getDagPath(0, meshDagPath, singleVertexComponent);



                  // SEE WHETHER VERTEX BELONGS TO ORIGINAL SELECTION, AND IF IT DOESN'T, THEN THE WHOLE FACE IS NOT CONTAINED:

                  if (!previousSelectionList.hasItem(meshDagPath, singleVertexComponent))

                  {

                     faceIsContained = 0;

                     break;

                  }

               }



               // IF FACE IS "CONTAINED", ADD IT TO THE FINAL CONTAINED FACES LIST:

               if (faceIsContained)

               {

                  MString faceName = meshName;

                  faceName += ".f[";

                  faceName += connectedFacesIndices[i];

                  faceName += "]";

                  finalFacesSelection.add(faceName);

               }

            }

         }

      }

   }



   // FINALLY, MAKE THE NEW "CONTAINED FACES", THE CURRENT SELECTION:

   MGlobal::setActiveSelectionList(finalFacesSelection, MGlobal::kReplaceList);



   // RETURN NEW CONTAINED FACES LIST FROM THE MEL COMMAND, AS AN ARRAY OF STRINGS:

   MStringArray containedFacesArray;

   finalFacesSelection.getSelectionStrings(containedFacesArray);

   MPxCommand::setResult(containedFacesArray);



   return MS::kSuccess;

}





// TO UNDO THE COMMAND, SIMPLY RESTORE THE ORIGINAL SELECTION BEFORE THE COMMAND WAS INVOKED:

MStatus convertVerticesToContainedFacesCommand::undoIt()

{

   MGlobal::setActiveSelectionList(previousSelectionList, MGlobal::kReplaceList);

   return MS::kSuccess;

}





// INITIALIZES THE PLUGIN:

MStatus initializePlugin(MObject obj)

{

   MStatus status;

   MFnPlugin plugin(obj, PLUGIN_COMPANY, "4.0", "Any");



   status = plugin.registerCommand("convertVerticesToFaces", convertVerticesToContainedFacesCommand::creator);

   if (!status)

   {

      status.perror("registerCommand");

      return status;

   }



   return status;

}





// UNINITIALIZES THE PLUGIN:

MStatus uninitializePlugin(MObject obj)

{

   MStatus status;

   MFnPlugin plugin(obj);



   status = plugin.deregisterCommand("convertVerticesToFaces");

   if (!status)

   {

      status.perror("deregisterCommand");

      return status;

   }



   return status;

}





// ============================================================================================================== END PLUGIN.

