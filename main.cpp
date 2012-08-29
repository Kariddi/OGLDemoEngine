#include <GLFWEngine.h>
#include <Mesh.h>
#include <Node.h>
#include <Scene.h>
#include <Shader.h>
#include <Loader.h>
#include <Camera.h>
#include <Constants.h>
#include <iostream>
#include <GL/glfw.h>
#include <cstring>
#include <cmath>

using namespace Uberngine;

int main(int argc, char **argv)
{
  Engine<UBE_GLFW> Eng;
  
  //Initializing engine
  Eng.Init("Baluba", 640,480, 24, 16, 0, false);
  //Loading Meshes
  Mesh *mesh = NULL;
  Mesh *mesh2 = NULL;
  ObjLoader::Loader load("./");
  mesh = load.loadMesh<UBE_LOADER_BYTE, UBE_LOADER_BYTE>("Torus.obj", false);
  
  //Creating a scene
  Scene *SC = Eng.CreateNewScene();
  //Creating nodes
  Node *MyNode = new Node(&Eng, SC);
  Node *MyNode2 = new Node(&Eng, MyNode);
  //Creating the camera
  Camera *Cam = new Camera(&Eng, SC);
  //Set perspective parameters
  Cam->SetPerspectiveCamera(60.0f, (float)4/3, 0.1f, 1000.0f);
  //Adding camera to the scene
  SC->SetCamera(Cam);
  //Set mesh on the node (Torus mesh)
  mesh2 = load.loadMesh<UBE_LOADER_SHORT, UBE_LOADER_SHORT>("Spiderman.obj", false);
  
  MyNode->SetMesh(mesh);
  //Set mesh on the second node (Spiderman mesh)
  MyNode2->SetMesh(mesh2);
  //Adding a light
  Light *MyLight = new Light(Light::DIRECTIONAL, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f);
  //Adding the node to the scene
//  SC->AddChildNode(MyNode);
  //Adding the second node as child of the first
//  MyNode->AddChildNode(MyNode2);
  //Adding the light to the scene
  SC->AddLight(MyLight);
  //Load the scene
  Eng.LoadScene(0);
  //Creating and adding shaders to the nodes
  Shader *shad = new Shader("./test.vert", "./no_tex_test.frag");
  Shader *shad2 = new Shader("./test.vert", "./test.frag");
  MyNode->SetShader(shad);
  MyNode2->SetShader(shad2);
  //Setting node and camera transformations
  MyNode2->SetTransform(0.0f, 0.0f, -0.0f, 0.0f, 0.0f,0.0f,0.0f);
  Cam->LookAt(0.0f, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  float xm = 0.0f, ym = 0.0f, zm = 0.0f;
  //Main loop
  int framecount = 0;
  float cum_time = 0.0f;
  while(1) {
    int x, y;
    Eng.GetMousePosition(&x, &y);
    float time_diff = Eng.GetFrameTimeDelta();
    //Move of a predefined amount ... should be substituted with something relative to absolute time, but for now it is enough
    zm += (Eng.CheckPressedKey(GLFW_KEY_UP) ? 1.0f : 0.0f)*time_diff;
    zm += (Eng.CheckPressedKey(GLFW_KEY_DOWN) ? -1.0f : 0.0f)*time_diff;
//    std::cout << time_diff << std::endl;
    if (Eng.CheckPressedKey(GLFW_KEY_ESC))
      break;
    x = x -320;
    y = y - 240;
    float fx = x / 320.0f, fy = y / 240.0f;
   
    MyNode->SetTransform(0.0f,0.0f,zm, (fx*fx + fy*fy)*PI, fy,-fx,1.0f);
    Eng.StepSingleFrame();
    
    cum_time += Eng.GetFrameTimeDelta();
    ++framecount;
    if (cum_time > 5.0f) {
      std::cout << "FPS: " << framecount / cum_time << " Passed time: " << cum_time << std::endl;
      framecount = 0;
      cum_time = 0.0f;
    }
  }
  //delete SC;
  //delete MyNode;
  //delete MyNode2;
  delete shad;
  delete shad2;
  delete mesh;
  delete mesh2;
  return 0;
}
