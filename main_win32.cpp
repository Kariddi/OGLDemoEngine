#include <GLFWEngine.h>
#include <Mesh.h>
#include <Node.h>
#include <Scene.h>
#include <Shader.h>
#include <ObjLoader.h>
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
  Loader::ObjLoader<UBE_LOADER_BYTE, UBE_LOADER_BYTE> load("E:\\DemoData\\");
  mesh = load.loadMesh("Torus.obj", false);
  
  //Creating a scene
  Scene *SC = Eng.CreateNewScene();
  //Creating nodes
  Node *MyNode = new Node(&Eng, SC);
  Node *MyNode2 = new Node(&Eng, MyNode);
  //Creating the camera
  Camera *Cam = new Camera(&Eng, SC);
  //Set perspective parameters
  Cam->SetPerspectiveCamera(60.0f, 4/3, 1.0f, 1000.0f);
  //Adding camera to the scene
  SC->SetCamera(Cam);
  //Set mesh on the node (Torus mesh)
  MyNode->SetMesh(mesh);
  mesh2 = load.loadMesh("Spiderman.obj", false);
  //Set mesh on the second node (Spiderman mesh)
  MyNode2->SetMesh(mesh2);
  //Adding a light
  Light *MyLight = new Light(Light::DIRECTIONAL, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f);
/*  //Adding the node to the scene
  SC->AddChildNode(MyNode);
  //Adding the second node as child of the first
  MyNode->AddChildNode(MyNode2);
*/  //Adding the light to the scene
  SC->AddLight(MyLight);
  //Load the scene
  Eng.LoadScene(0);
  //Creating and adding shaders to the nodes
  Shader *shad = new Shader("E:\\DemoData\\test.vert", "E:\\DemoData\\no_tex_test.frag");
  MyNode->SetShader(shad);
  Shader *shad2 = new Shader("E:\\DemoData\\test.vert", "E:\\DemoData\\test.frag");
 MyNode2->SetShader(shad2);
  //Setting node and camera transformations
  MyNode2->SetTransform(0.0f, 0.0f, -0.0f, 0.0f, 0.0f,0.0f,0.0f);
  Cam->LookAt(0.0f, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  float xm = 0.0f, ym = 0.0f, zm = 0.0f;
  //Main loop
  while(1) {
    int x, y;
    Eng.GetMousePosition(&x, &y);
    float time_diff = Eng.GetFrameTimeDelta();
    //Move of a predefined amount ... should be substituted with something relative to absolute time, but for now it is enough
    zm += (Eng.CheckPressedKey(GLFW_KEY_UP) ? 1.0f : 0.0f)*time_diff;
    zm += (Eng.CheckPressedKey(GLFW_KEY_DOWN) ? -1.0f : 0.0f)*time_diff;
    if (Eng.CheckPressedKey(GLFW_KEY_ESC))
      break;
    x = x -320;
    y = y - 240;
    float fx = x / 320.0f, fy = y / 240.0f;
   
    MyNode->SetTransform(0.0f,0.0f,zm, (fx*fx + fy*fy)*PI, fy,-fx,1.0f);
    Eng.UpdateScene();
    Eng.RenderScene();

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
