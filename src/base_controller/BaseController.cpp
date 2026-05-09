#include "BaseController.h"

BaseController::BaseController() : engine(FastAccelStepperEngine()) {
    for (int i = 0; i < NB_MOTOR; i++) {
        motors[i] = nullptr;
    }
}

void BaseController::init() {
    // Utilisation des index 0-3 pour NB_MOTOR 4
    motors[0] = new MotorStepper(engine, M2_DRIVER_ADDRESS, M2_STP_PIN, M2_DIR_PIN, M2_EN_PIN);
    motors[1] = new MotorStepper(engine, M3_DRIVER_ADDRESS, M3_STP_PIN, M3_DIR_PIN, M3_EN_PIN);
    motors[2] = nullptr;
    motors[3] = nullptr;

    if (motors[0]) motors[0]->begin();
    if (motors[1]) motors[1]->begin();
}

void BaseController::setSpeedBase(double joints_speed[4]) {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        if (motors[i]) {
            motors[i]->setSpeed(joints_speed[i]);
        }
    }
}

void BaseController::enableBase() {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        if (motors[i]) {
            motors[i]->enable();
        }
    }
}

void BaseController::disableBase() {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        if (motors[i]) {
            motors[i]->disable();
        }
    }
}

void BaseController::setCurrent(double current_ma) {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        if (motors[i]) {
            motors[i]->set_current(current_ma);
        }
    }
}

void BaseController::getSpeeds(double out_speeds[4]) {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        out_speeds[i] = motors[i] ? motors[i]->getCurrentSpeed() : NAN;
    }
}

void BaseController::getPositions(double out_positions[4]) {
    for (size_t i = 0; i < NB_MOTOR; i++) {
        out_positions[i] = motors[i] ? motors[i]->getPosition() : NAN;
    }
}

void BaseController::updateSpeed(const robot_messages__msg__WheelsCommand* wheels_cmd_msg) {
    double speeds[4] = {
        wheels_cmd_msg->lb_joint, 
        wheels_cmd_msg->lf_joint, 
        wheels_cmd_msg->rb_joint, 
        wheels_cmd_msg->rf_joint
    };
    setSpeedBase(speeds);
}

BaseState BaseController::get_current_state() {
    BaseState bs = {}; // Initialise tout à 0 par défaut
    
    if (motors[0]) {
        bs.w1 = motors[0]->getCurrentSpeed();
        bs.p1 = motors[0]->getPosition();
    }
    if (motors[1]) {
        bs.w2 = motors[1]->getCurrentSpeed();
        bs.p2 = motors[1]->getPosition();
    }
    if (motors[2]) {
        bs.w3 = motors[2]->getCurrentSpeed();
        bs.p3 = motors[2]->getPosition();
    }
    if (motors[3]) {
        bs.w4 = motors[3]->getCurrentSpeed();
        bs.p4 = motors[3]->getPosition();
    }

    return bs;
}