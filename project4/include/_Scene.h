#ifndef _SCENE_H
#define _SCENE_H

#include <_Common.h>
class _Scene
{

    public:
        _Scene();
        virtual ~_Scene();

        GLint initGL(); //initializing the scene
        GLint drawScene(); //Render scene
        GLvoid resizeScene(GLsizei, GLsizei); // address window resize
        GLvoid mouseMapping(int,int);
        void advanceLevel();

        int winMsg(HWND, UINT, WPARAM, LPARAM);
        WPARAM wParam;

        bool wireFrame = false;

        float screenWidth, screenHeight;

        GLdouble mouseX;
        GLdouble mouseY;
        GLdouble mouseZ;

        int mouseClicks = 0;


    protected:

    private:
        bool weaponPickedUp = false;
        float weaponInitialX = 50.0f;    // Adjust these coordinates to place the weapon
        float weaponInitialY = -5.0f;    // Slightly above ground level
        float weaponInitialZ = -100.0f;  // Same Z plane as player
        const float PICKUP_DISTANCE = 10.0f;  // Adjust this for pickup radius
};

#endif // _SCENE_H
