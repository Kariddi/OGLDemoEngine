#include <GLFWEngine.h>
#include <Mesh.h>
#include <Node.h>
#include <Scene.h>
#include <Shader.h>
#include <ObjLoader.h>
#include <Camera.h>
#include <Constants.h>
#include <iostream>
#include <GUI/GUI.h>
#include <GUI/GUILabel.h>
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
  GUI *gui = Eng.GetGUI();
  GUILabel *lab = new GUILabel(gui);
  lab->SetXPos(100);
  lab->SetYPos(100);
  lab->SetText("Testo di prova!!");
  gui->AddElement(lab);
  Loader::ObjLoader<UBE_LOADER_BYTE, UBE_LOADER_BYTE> load("./");
  mesh = load.loadMesh("Torus.obj", false);
  
  //Creating a scene
  Scene *SC = Eng.CreateNewScene();
  //Creating nodes
  Node *MyNode = new Node;
  Node *MyNode2 = new Node;
  Node *MyNode3 = new Node;
 
  SC->AttachNodeToScene(*MyNode);
  SC->AttachNodeToScene(*MyNode3);
  SC->AttachNodeToParent(*MyNode2, *MyNode);
  //Creating the camera
  Camera *Cam = new Camera();
  //Set perspective parameters
  Cam->SetPerspectiveCamera(60.0f, (float)4/3, 0.1f, 1000.0f);
  //Adding camera to the scene
  SC->SetCamera(Cam);
  //Set mesh on the node (Torus mesh)
  mesh2 = load.loadMesh("Spiderman.obj", false);
  
  MyNode->SetMesh(mesh);
  //Set mesh on the second node (Spiderman mesh)
  MyNode2->SetMesh(mesh2);
  //Adding a light
  Light *MyLight = new Light(Light::LightType::DIRECTIONAL, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f);
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
  MyNode3->SetTransform(0.0f, -4.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f);
  Cam->LookAt(0.0f, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  float xm = 0.0f, ym = 0.0f, zm = 0.0f;
  //Main loop
  int framecount = 0;
  float cum_time = 0.0f;
  
  PhysicsManager *PyMan = Eng.GetPhysicsManager();
  CollisionShape *CircleShape = PyMan->GetSphereShape(2);
  CollisionShape *BoxShape = PyMan->GetBoxShape(100,1,100);
  CollisionShape *CircleShape2 = PyMan->GetSphereShape(10);
  CollisionShape *BoxShape2 = PyMan->GetBoxShape(1,1,1);
  CollisionShape *CircleShape3 = PyMan->GetSphereShape(11);
  CollisionShape *BoxShape3 = PyMan->GetBoxShape(10,21,5);
  RigidBody *NewBody = new RigidBody(0.001f, CircleShape);
  SC->SetRigidBodyForNode(NewBody, MyNode);
  NewBody = new RigidBody(0.0f, BoxShape);
  SC->SetRigidBodyForNode(NewBody, MyNode3);
  MyNode->SetKinematic(true);
  SC->SetGravity(0,-10,0);

  std::cout << "Sphere Shape = " << CircleShape << std::endl;
  std::cout << "Box Shape = " << BoxShape << std::endl;
  std::cout << "Sphere Shape = " << CircleShape2 << std::endl;
  std::cout << "Box Shape = " << BoxShape2 << std::endl;
  std::cout << "Sphere Shape = " << CircleShape3 << std::endl;
  std::cout << "Box Shape = " << BoxShape3 << std::endl;
  bool shift_press = false;

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
    bool pressed = Eng.CheckPressedKey(GLFW_KEY_LSHIFT);
    if (!shift_press && pressed)
      shift_press = true;
    else if (shift_press && !pressed) {
      MyNode->SetKinematic(!MyNode->IsKinematic());
      shift_press = false;
    }
    x = x -320;
    y = y - 240;
    float fx = x / 320.0f, fy = y / 240.0f;
   
    MyNode->SetTransform(0.0f,0.0f,zm, (fx*fx + fy*fy)*PI, fy,-fx,1.0f);
    Eng.StepSingleFrame();
    
    cum_time += Eng.GetFrameTimeDelta();
    ++framecount;
    if (cum_time > 5.0f) {
      std::stringstream ss;
      ss << "FPS: " << framecount / cum_time << " Passed time: " << cum_time << std::endl;
      framecount = 0;
      cum_time = 0.0f;
      lab->SetText(ss.str().c_str());
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
