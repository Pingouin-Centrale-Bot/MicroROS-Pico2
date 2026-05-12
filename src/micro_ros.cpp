#define DEBUG

#define RICRO_ROS

#ifdef RICRO_ROS


#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <hardware/watchdog.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <sensor_msgs/msg/joint_state.h>
#include <rosidl_runtime_c/string_functions.h>
#include <robot_messages/msg/wheels_command.h>

#include "BaseController.h"

//#define SIMU

#define DBG Serial1

// ── Timings ───────────────────────────────────────────────────────────────────
const unsigned int HEALTH_PERIOD_MS = 1000;
const unsigned int WHEELS_PERIOD_MS = 100;

// ── Handles micro-ROS ─────────────────────────────────────────────────────────
rclc_support_t     support;
rcl_allocator_t    allocator;
rcl_node_t         node;
rclc_executor_t    executor;

rcl_publisher_t    health_publisher;
rcl_publisher_t    wheels_publisher;
rcl_subscription_t wheels_subscriber;
rcl_timer_t        health_timer;
rcl_timer_t        wheels_timer;

// ── Messages ──────────────────────────────────────────────────────────────────
std_msgs__msg__Int32               health_msg;
robot_messages__msg__WheelsCommand wheels_cmd_msg;
sensor_msgs__msg__JointState       wheels_state_msg;

rosidl_runtime_c__String joint_names_buf[4];
double joint_position_buf[4];
double joint_velocity_buf[4];


// ── Hardware ──────────────────────────────────────────────────────────────────
BaseController base;

// ── Macros ────────────────────────────────────────────────────────────────────
#define RCCHECK(fn) do { \
  rcl_ret_t rc = (fn); \
  if (rc != RCL_RET_OK) { \
    DBG.printf("[FATAL] %s:%d  err=%d\r\n", __FILE__, __LINE__, (int)rc); \
    DBG.flush(); delay(100); \
    watchdog_reboot(0, 0, 0); \
    while(1); \
  } \
} while(0)

#define RCSOFTCHECK(fn) do { \
  rcl_ret_t rc = (fn); \
  if (rc != RCL_RET_OK) \
    DBG.printf("[WARN]  %s:%d  err=%d\r\n", __FILE__, __LINE__, (int)rc); \
} while(0)

// ── Init JointState ───────────────────────────────────────────────────────────
void init_joint_state_msg()
{
  sensor_msgs__msg__JointState__init(&wheels_state_msg);

  wheels_state_msg.name.data     = joint_names_buf;
  wheels_state_msg.name.size     = 4;
  wheels_state_msg.name.capacity = 4;
  rosidl_runtime_c__String__assign(&wheels_state_msg.name.data[0], "lf_joint");
  rosidl_runtime_c__String__assign(&wheels_state_msg.name.data[1], "rf_joint");
  rosidl_runtime_c__String__assign(&wheels_state_msg.name.data[2], "rb_joint");
  rosidl_runtime_c__String__assign(&wheels_state_msg.name.data[3], "lb_joint");

  wheels_state_msg.velocity.data     = joint_velocity_buf;
  wheels_state_msg.velocity.size     = 4;
  wheels_state_msg.velocity.capacity = 4;

  wheels_state_msg.position.data     = joint_position_buf;
  wheels_state_msg.position.size     = 4;
  wheels_state_msg.position.capacity = 4;

  wheels_state_msg.effort.data     = NULL;
  wheels_state_msg.effort.size     = 0;
  wheels_state_msg.effort.capacity = 0;

  rosidl_runtime_c__String__assign(&wheels_state_msg.header.frame_id, "base_link");
}

// ── Callbacks ─────────────────────────────────────────────────────────────────
void health_timer_cb(rcl_timer_t *timer, int64_t)
{
  if (!timer) return;
  health_msg.data++;
  DBG.printf("[HEALTH] tick=%ld\r\n", (long)health_msg.data);
  RCSOFTCHECK(rcl_publish(&health_publisher, &health_msg, NULL));
}

void joints_state_timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    uint64_t now_ns = rmw_uros_epoch_nanos();
    BaseState current_state = base.get_current_state();

    wheels_state_msg.header.stamp.sec = (int32_t)(now_ns / 1000000000LL);
    wheels_state_msg.header.stamp.nanosec = (uint32_t)(now_ns % 1000000000LL);

    wheels_state_msg.velocity.data[0] = current_state.w1;
    wheels_state_msg.velocity.data[1] = current_state.w2;
    wheels_state_msg.velocity.data[2] = current_state.w3;
    wheels_state_msg.velocity.data[3] = current_state.w4;

    wheels_state_msg.position.data[0] = current_state.p1;
    wheels_state_msg.position.data[1] = current_state.p2;
    wheels_state_msg.position.data[2] = current_state.p3;
    wheels_state_msg.position.data[3] = current_state.p4;

    RCSOFTCHECK(rcl_publish(&wheels_publisher, &wheels_state_msg, NULL));
  }
}

void wheels_cmd_cb(const void *msgin)
{
  const robot_messages__msg__WheelsCommand *cmd =
    (const robot_messages__msg__WheelsCommand *)msgin;
  if (!cmd) return;

  DBG.printf("[CMD] lf=%.3f rf=%.3f rb=%.3f lb=%.3f\r\n",
             cmd->lf_joint, cmd->rf_joint,
             cmd->rb_joint, cmd->lb_joint);

  base.updateSpeed(cmd);
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup()
{
  Serial.begin(115200);
  unsigned long start = millis();
  while (!Serial && (millis() - start < 2000));

  Serial1.setTX(0);
  Serial1.setRX(1);
  DBG.begin(115200);
  delay(200);
  DBG.println("\r\n=== BOOT ===");

  health_msg.data = 0;
  init_joint_state_msg();

  base.init();
  DBG.println("[INIT] BaseController OK");

  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  DBG.println("[INIT] transport OK");

  DBG.println("[INIT] ping agent...");
  uint32_t n = 0;
  while (rmw_uros_ping_agent(1000, 1) != RMW_RET_OK)
    DBG.printf("       tentative %lu\r\n", ++n);
  DBG.println("[INIT] agent trouvé");

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "micro_ros_node", "", &support));
  DBG.println("[INIT] node OK");

  RCCHECK(rclc_publisher_init_default(
    &health_publisher, &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_node_health"));
  RCCHECK(rclc_publisher_init_default(
    &wheels_publisher, &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
    "/micro_controller/joint_states"));
  DBG.println("[INIT] publishers OK");

  RCCHECK(rclc_subscription_init_default(
    &wheels_subscriber, &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(robot_messages, msg, WheelsCommand),
    "/wheel_commands"));
  DBG.println("[INIT] subscriber OK");

  RCCHECK(rclc_timer_init_default2(
    &health_timer, &support,
    RCL_MS_TO_NS(HEALTH_PERIOD_MS),
    health_timer_cb, true));
  RCCHECK(rclc_timer_init_default2(
    &wheels_timer, &support,
    RCL_MS_TO_NS(WHEELS_PERIOD_MS),
    joints_state_timer_callback, true));
  DBG.println("[INIT] timers OK");

  // N = 2 timers + 1 subscriber
  RCCHECK(rclc_executor_init(&executor, &support.context, 3, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &health_timer));
  RCCHECK(rclc_executor_add_timer(&executor, &wheels_timer));
  RCCHECK(rclc_executor_add_subscription(
    &executor, &wheels_subscriber,
    &wheels_cmd_msg, &wheels_cmd_cb, ON_NEW_DATA));
  DBG.println("[INIT] executor OK");

  DBG.println("[INIT] sync horloge...");
  while (rmw_uros_sync_session(1000) != RMW_RET_OK)
    DBG.println("[INIT] retry sync...");
  DBG.println("[INIT] horloge synced");

  DBG.println("[INIT] done — spinning\r\n");
  base.enableBase();

}

// ── Loop ──────────────────────────────────────────────────────────────────────
void loop()
{
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}

#endif