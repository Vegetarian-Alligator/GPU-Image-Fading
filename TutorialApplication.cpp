/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
//--------------------------------------------------------------

///This was fully functional for single image cycling however
/// This was not the goal!  This project has since been broken
/// In order to the next phase.

#include <OgreStringConverter.h>
using namespace std;
using namespace Ogre;
#include "TutorialApplication.h"
    #include <stdlib.h>
    #include <iostream>
#include <sstream>

//#include <OgreMaterialSerializer.h>

short cycles_total; // Keeps track of the number of images in the animation across funcs
string stages;      // Refers to the specific images from the loading subroutine to the frame_created routine
Ogre::MaterialPtr yourMaterial;

//Vector3 wow;
//Vector3 size;
Ogre::Vector3 wow;
Ogre::Vector3 size;
Ogre::Timer* whymust=new Ogre::Timer;
Ogre::Entity *ent;
Ogre::SceneNode *node;
    Ogre::Image image;
    Ogre::Image image2;
    Ogre::TextureUnitState* yourTexUnitState;
Ogre::TextureUnitState* TexOne;
Ogre::TextureUnitState* TexTwo;

TutorialApplication::TutorialApplication(void)
{
    //mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
Ogre::TexturePtr wowagain;
void TutorialApplication::createScene(void)
{
    ent = mSceneMgr->createEntity("Cube", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube");
    ent->setMaterialName("shader/digesttest");

    ifstream myfile ("victim.txt");
    cycles_total=0;
     //MessageBox(NULL, "This RAN!", NULL, NULL);
    Ogre::Image* imagememory;
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            //MessageBox(NULL, "This RAN!!!!", NULL, NULL);
            string buffer;
            getline (myfile,buffer);
            stages+=buffer+'\n';
            Ogre::String Translate;
            Translate = buffer;
            //MessageBox(NULL,buffer.c_str(),NULL,NULL);
          //  imagememory=image.load(Translate, "General");
           // wowagain = Ogre::TextureManager::getSingleton().loadImage(Translate,"General",image);
            cycles_total++;
        }
        myfile.close();
    }
    else exit(-5);
    //ent->setMaterialName("digesther5.bmp", "General");
    //MessageBox(NULL, "This RAN!2", NULL, NULL);


    yourMaterial = Ogre::MaterialManager::getSingleton().getByName("shader/digesttest") ;
    TexOne = yourMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("digesther1.bmp",0);
    TexTwo = yourMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("digesther8.bmp",1);



int const nb_textures=2;

   
    node->attachObject(ent);

    node->scale(Ogre::Vector3(1.333,.705,1));

}


bool TutorialApplication::frameStarted(const Ogre::FrameEvent& evt)
{
    //static bool initilized=true;
    static bool ran = true;
    static short cycles_current=-1;
    static short lastinstance=0;
    static istringstream imagestream(stages);
    static short derivitave;
    static string imagelist[100]; ///Ugly
    static short PICTUREITERATION=-1;

    PICTUREITERATION++;

//std::ostringstream oss;
//oss << PICTUREITERATION;
string buffer99;
char ugly[150];
itoa(PICTUREITERATION, ugly, 10);
buffer99="";buffer99=ugly;
//itoa (PICTUREITERATION,buffer99,10);

Ogre::String translatecount; // = _itoa(PICTUREITERATION);
translatecount = buffer99;
translatecount = "image" + translatecount + ".jpg";
//translatecount="Image"+buffer99;
mWindow->writeContentsToFile(translatecount);

if (ran){
        int i;
        i=0;
        for (i =0; i != cycles_total;i++) imagestream>>imagelist[i];
        imagelist[i+1]=imagelist[i];
        ran = false;
}


    if ((cycles_current==-1)) {

        whymust->reset();
        cycles_current++;
        imagestream.~istringstream();
        new ( (void *) &imagestream ) std::istringstream( stages);
    }

    if (whymust->getMilliseconds() >= 5000 || cycles_current==0)
    {

        TexOne->setTextureName(imagelist[cycles_current], Ogre::TEX_TYPE_2D);
        TexTwo->setTextureName(imagelist[cycles_current+1], Ogre::TEX_TYPE_2D);
        cycles_current++;
        whymust->reset();
    }

    ///The code below is needed in the actual animation sequence and NOT this test loader
    Ogre::Pass* pPass =  yourMaterial->getTechnique(0)->getPass(0);
            if (pPass->hasFragmentProgram()) {
                Ogre::GpuProgramParametersSharedPtr pParams = pPass->getFragmentProgramParameters();
                if (pParams->_findNamedConstantDefinition("timediff")) {
                    pParams->setNamedConstant("timediff",(float)whymust->getMilliseconds());
                }else exit(1);
            }else exit(2);


    if (cycles_current>=(cycles_total*2))
    {
        cycles_current=-1;
        whymust->reset();
        ran=true;
        /// Method Published at http://bytes.com/topic/c/answers/63254-std-stringstream-reset
        //imagestream.~istringstream();
        //new ( (void *) &imagestream ) std::istringstream( stages);
        ///More reliable than istringstream::str(); in some circumstances
    }
    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try
        {
            app.go();
        }
        catch ( Ogre::Exception& e )
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
