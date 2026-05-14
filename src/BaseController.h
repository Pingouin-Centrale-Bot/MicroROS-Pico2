#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

#include "MotorStepper.h"
#include <sensor_msgs/msg/joint_state.h>
#include <robot_messages/msg/wheels_command.h>
#include <cmath>
#include "config.h"



struct BaseState {
    double w1, w2;//, w3, w4; 
    double p1, p2;//, p3, p4;
};

class BaseController {
public:
    BaseController();
    
    void init();
    void setSpeedBase(double joints_speed[NB_MOT_CARD]);
    void enableBase();
    void disableBase();
    void setCurrent(double current_ma);
    
    // On passe le tableau en paramètre pour la sécurité mémoire
    void getSpeeds(double out_speeds[NB_MOT_CARD]);
    void getPositions(double out_positions[NB_MOT_CARD]);
    
    void updateSpeed(const robot_messages__msg__WheelsCommand* wheels_cmd_msg);
    BaseState get_current_state();

private:
    FastAccelStepperEngine engine;
    MotorStepper* motors[NB_MOT_CARD];
};

#endif