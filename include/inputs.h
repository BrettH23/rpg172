#ifndef INPUTS_H
#define INPUTS_H

#include <windows.h>
#include <model.h>
#include <parallax.h>
#include <player.h>

class Inputs
{
    public:
        Inputs();
        virtual ~Inputs();

        void keyPressed(Model *);
        void keyPlayer(player *);
        void keyPlayerUp(player *);
        void keyUp();

        void mouseBtnDown(Model *, double, double);
        void mouseBtnUp();
        void mouseWheel(Model *,double);
        void mouseMove(Model *,double, double);
        void keyEnv(parallax*,float);

        WPARAM wParam;

        double prev_Mx;
        double prev_My;

        bool Mouse_translate = false;
        bool Mouse_rotate = false;

        Model *mdl = new Model();

    protected:

    private:
};

#endif // INPUTS_H
