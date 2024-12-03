#include "_Scene.h"
#include <_lightSetup.h>
#include <_Models.h>
#include <_KbMs.h>
#include <_parallax.h>
#include <_SkyBox.h>
#include <_Skybox2.h>
#include <_Camera.h>
#include <_2DPlayer.h>
#include <_Bullets.h>
#include <_Collision.h>
#include <_ModelLoaderMD2.h>
#include <_OBJLoader.h>
#include "EnemyManager.h"
#include <_TextRenderer.h>





_Camera *cam = new _Camera();
_lightSetup *myLight = new _lightSetup();
//_Models *teapot = new _Models();
_KbMs *KbMs = new _KbMs();
_parallax *plx = new _parallax();
_SkyBox *sky = new _SkyBox();
_Skybox2 *sky2 = new _Skybox2();
//_2DPlayer *pl = new _2DPlayer();
_Bullets b[20];
_Collision *hit = new _Collision();
_ModelLoaderMD2 *mdl3D = new _ModelLoaderMD2();
_ModelLoaderMD2 *mdl3DWeapon = new _ModelLoaderMD2();
EnemyManager* enemies = new EnemyManager();
_parallax *forest = new _parallax();
_parallax *mountain = new _parallax();
_parallax *desert = new _parallax();
_parallax *forest_path = new _parallax();
_parallax *mountain_path = new _parallax();
_parallax *desert_path = new _parallax();



// Add new variables for jump mechanics
float jumpVelocity = 0.0f;
float gravity = 0.15f;
bool isJumping = false;
const float JUMP_FORCE = 5.0f;   // Increased initial jump force
const float INITIAL_X = -70.0f;    // Starting X position
const float INITIAL_Y = 4.0f;    // Starting Y position (ground level)
const float INITIAL_Z = -100.0f; // Starting Z position (distance from camera)

//other vars
int gameScore = 0;
int current_level = 1;

_Scene::_Scene()
{

    //ctor
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);


}

_Scene::~_Scene()
{
    //dtor
}
GLint _Scene::initGL()
{

    // Initialize OpenGL extension function pointers
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

    if (!glGenBuffers || !glBindBuffer || !glBufferData || !glDeleteBuffers) {
        std::cout << "OpenGL extensions not supported!" << std::endl;
        return false;
    }

    glShadeModel(GL_SMOOTH); //for smooth render
    glClearColor(1,1,1,0); //background color
    glClearDepth(2.0f); // test on Depth layer
    glEnable(GL_DEPTH_TEST); //activate depth Test
    glDepthFunc(GL_LEQUAL); //Depth function type
    glEnable(GL_COLOR_MATERIAL);
    myLight->setLight(GL_LIGHT0); // setting my first lights

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cam->camInit();

    glEnable(GL_TEXTURE_2D);
    //mdl3D->initModel("models/Tekk/tris.md2");
    //mdl3D->initModel("models/MarvinMartian/tris.md2");
    //teapot->inintModel("images/tex.jpg");
    plx->parallaxInit("images/bck.png");
    //sky->skyBoxInit();
    sky2->skyBoxinit("images/bk.png");

    forest->parallaxInit("images/forest_para.jpg");
    mountain->parallaxInit("images/mountain_para.jpg");
    desert->parallaxInit("images/desert_para.jpg");

    forest_path->parallaxInit("images/forest_para_walkway.jpg");
    mountain_path->parallaxInit("images/mountain_para_walkway.jpg");
    desert_path->parallaxInit("images/desert_para_walkway.jpg");

    mdl3D->initModel("models/Tekk/tris.md2"); //this is the main character
    mdl3D->setAnimationState(_ModelLoaderMD2::ANIM_STAND); // Start with running
    mdl3D->dirAngleZ = 0; // Face right

    // Set fixed position
    mdl3D->pos.x = INITIAL_X;
    mdl3D->pos.y = INITIAL_Y;
    mdl3D->pos.z = INITIAL_Z;

    mdl3DWeapon->initModel("models/Tekk/weapon.md2"); //this is the main characters weapon
    mdl3DWeapon->setAnimationState(_ModelLoaderMD2::ANIM_STAND); // Start with running
    mdl3DWeapon->dirAngleZ = 0; // Face right

    mdl3DWeapon->pos.x = weaponInitialX;
    mdl3DWeapon->pos.y = weaponInitialY;
    mdl3DWeapon->pos.z = weaponInitialZ;

    //pl->initPlayer(6,4,"images/player.png");

    for(int i = 0; i < 20; ++i){
        b[i].initBullet("no images");
    }

    // In _Scene.cpp, in initGL()
    enemies->initEnemy("models/carrot/tris.md2");  // The carrot model should have its texture path in the md2 file
    enemies->spawnEnemy(50, -30, 100); // Spawns enemy to the right
    //mdl3D->generateUVTemplate("carrot_template.png", &mdl3D->md2file);


    enemies->initEnemy("models/carrot/tris.md2");  // The carrot model should have its texture path in the md2 file
    enemies->spawnEnemy(-200, -30, -100); // Spawns enemy to the right

    enemies->initEnemy("models/carrot/tris.md2");  // The carrot model should have its texture path in the md2 file
    enemies->spawnEnemy(150, -30, -200); // Spawns enemy to the right


    return true;

}

GLint _Scene::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam->setUpCam();

    wireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPushMatrix();
    myLight->movingLight(GL_LIGHT0);
    glPopMatrix();


     /*
    //this part controls the parallax background
    glPushMatrix();
    glTranslatef(0,-10,1);
    glRotatef(90,1,0,0);
    glScalef(100,50,1.0);
    plx->parallaxScroll(true, "right", 0.005);
    plx->parallaxDraw(screenWidth,screenHeight);
    glPopMatrix();
    //*/

    //this part controls the parallax background
    if(current_level == 1){

        //floor
        glPushMatrix();
        glTranslatef(0,-10,1);
        glRotatef(90,1,0,0);
        glScalef(100,50,1.0);
        //forest_path->parallaxScroll(true, "right", 0.005);
        forest_path->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();

        //side
        glPushMatrix();
        glTranslatef(0,10,-20);
        glScalef(30,20,1.0);
        //forest->parallaxScroll(true, "right", 0.005);
        forest->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();

    }else if(current_level == 2)
    {
        //floor
        glPushMatrix();
        glTranslatef(0,-10,1);
        glRotatef(90,1,0,0);
        glScalef(100,50,1.0);
        //desert_path->parallaxScroll(true, "right", 0.005);
        desert_path->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();

        //side
        glPushMatrix();
        glTranslatef(0,10,-20);
        glScalef(30,20,1.0);
        //desert->parallaxScroll(true, "right", 0.005);
        desert->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();
    }else
    {
        //floor
        glPushMatrix();
        glTranslatef(0,-10,1);
        glRotatef(90,1,0,0);
        glScalef(100,50,1.0);
        //mountain_path->parallaxScroll(true, "right", 0.005);
        mountain_path->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();

        //side
        glPushMatrix();
        glTranslatef(0,10,-20);
        glScalef(30,20,1.0);
        //mountain->parallaxScroll(true, "right", 0.005);
        mountain->parallaxDraw(screenWidth,screenHeight);
        glPopMatrix();
    }



    /*
    glPushMatrix();
    sky2->skyBoxDraw();
    glPopMatrix();
    */

    float deltaTime = 1.0f/60.0f;


    // Handle jumping physics
    if (isJumping) {
        mdl3D->pos.y += jumpVelocity * deltaTime * 30.0f;
        jumpVelocity -= gravity;

        // Check if we've landed
        if (mdl3D->pos.y <= INITIAL_Y) {
            mdl3D->pos.y = INITIAL_Y;
            isJumping = false;
            jumpVelocity = 0.0f;
            mdl3D->setAnimationState(_ModelLoaderMD2::ANIM_STAND);  // Go back to running
        }
    }


    glPushMatrix();
    if (!weaponPickedUp) {
        // Weapon is on the ground, check for pickup
        float distanceToWeapon = sqrt(
            pow(mdl3D->pos.x - mdl3DWeapon->pos.x, 2) +
            pow(mdl3D->pos.y - mdl3DWeapon->pos.y, 2) +
            pow(mdl3D->pos.z - mdl3DWeapon->pos.z, 2)
        );

        if (distanceToWeapon < PICKUP_DISTANCE) {
            weaponPickedUp = true;
        }

        // Render weapon on ground with default animation
        mdl3DWeapon->setAnimationState(_ModelLoaderMD2::ANIM_STAND);
        mdl3DWeapon->updateAnimation(deltaTime);
        mdl3DWeapon->Draw();
    } else {
        // Weapon follows player
        mdl3DWeapon->pos.x = mdl3D->pos.x + 5.0f;
        mdl3DWeapon->pos.y = mdl3D->pos.y + 2.0f;
        mdl3DWeapon->pos.z = mdl3D->pos.z;

        // Match player's direction
        mdl3DWeapon->dirAngleZ = mdl3D->dirAngleZ;

        // Sync animation with player
        mdl3DWeapon->syncAnimationWith(mdl3D);

        // Draw the weapon
        mdl3DWeapon->Draw();
    }
    glPopMatrix();

    glPushMatrix();
    mdl3D->updateAnimation(deltaTime);
    mdl3D->Draw();
    glPopMatrix();

    glPushMatrix();
    mdl3DWeapon->updateAnimation(deltaTime);
    mdl3DWeapon->Draw();
    glPopMatrix();

    glPushMatrix();
    enemies->updateEnemies();
    enemies->checkCollisions(mdl3D, isJumping, gameScore);  // Now passing all 3 required arguments
    advanceLevel();
    enemies->drawEnemies();
    glPopMatrix();


    std::string scoreText = "Score: " + std::to_string(gameScore);
    _TextRenderer::renderText(scoreText.c_str(), 10, 550);  // You might need to adjust these y-values
    _TextRenderer::renderText("Press SPACE to jump", 10, 500);  // Increased spacing for larger font


    return true;
}

int _Scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    KbMs->wParam = wParam; // Store the key press

    switch(uMsg) {
    case WM_KEYDOWN:
        // Handle jumping separately since it uses physics
        if(wParam == VK_SPACE) {
            if (!isJumping) {
                isJumping = true;
                jumpVelocity = JUMP_FORCE;
                mdl3D->setAnimationState(_ModelLoaderMD2::ANIM_JUMP);
            }
        }
        // Route all other keypresses to the KbMs handler
        KbMs->keyPress(mdl3D);
        break;

    case WM_KEYUP:
        // Handle key release
        KbMs->keyUp(mdl3D);
        break;
    }
    return 0;
}

GLvoid _Scene::resizeScene(GLsizei width, GLsizei height)
{
    GLfloat aspectRatio = (GLfloat)width/height;

    glViewport(0,0,width,height); // helps set the view port and normalized device coordinates to window coordinates
    glMatrixMode(GL_PROJECTION); // specifies which matrix stack is targeted for the subsequent matrix operations (GL_Projection: applies subsequent matrix operations to the projection matrix stack)
    glLoadIdentity(); // replaces the current matrix with a identity matrix
    gluPerspective(45, aspectRatio, 0.1,3000); // set the aspect ratio to determine the field of view in the x direction as well as changing the z axis an Field of View
    glMatrixMode(GL_MODELVIEW); // Model view applies matrix operations to the model view matrix stack
    glLoadIdentity();

}

GLvoid _Scene::mouseMapping(int x, int y)
{
     screenWidth = GetSystemMetrics(SM_CXSCREEN);
     screenHeight = GetSystemMetrics(SM_CYSCREEN);

     GLint viewPort[4];
     GLdouble modelViewM[16];
     GLdouble projectionM[16];
     GLfloat winX, winY, winZ;


     float scale = 60*(screenWidth/screenHeight);

     glGetDoublev(GL_MODELVIEW_MATRIX,modelViewM);
     glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
     glGetIntegerv(GL_VIEWPORT,viewPort);

     winX = (GLfloat)x;
     winY = (GLfloat)(viewPort[3]-y);
     glReadPixels(x,int(winY),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);

     mouseX = (GLfloat)(x-screenWidth/2.0)/scale;
     mouseY = (GLfloat)(screenHeight/2.0-y)/scale;
     mouseZ = -100;



     gluUnProject(winX,winY,winZ,modelViewM,projectionM,viewPort,&mouseX,&mouseY,&mouseZ);

}

void _Scene::advanceLevel() {
    if (gameScore >= 60) {
        gameScore = 0;
        current_level++;
        if (current_level > 3) {
            current_level = 1;  // Loop back to first level
        }
    }
}
