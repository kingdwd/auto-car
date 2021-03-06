#include "control/control.h"
#include "yaml-cpp/yaml.h"
#define CONTROL_CONF_DIR "/home/gyl/my-code/auto-car/ros/src/control/src/conf/control.yaml"

namespace control {
using namespace std;

void Control::Init(void){
    YAML::Node control_conf = YAML::LoadFile(CONTROL_CONF_DIR);
    LonControllerConf lon_controller_conf;
    LatControllerConf lat_controller_conf;

//配置纵向控制参数
    lon_controller_conf.ts                                           = control_conf["lon_controller_conf"]["ts"].as<double>();
    lon_controller_conf.station_pid_conf.integrator_enable           = control_conf["lon_controller_conf"]["station_pid_conf"]["integrator_enable"].as<bool>();
    lon_controller_conf.station_pid_conf.integrator_saturation_level = control_conf["lon_controller_conf"]["station_pid_conf"]["integrator_saturation_level"].as<double>();
    lon_controller_conf.station_pid_conf.kp                          = control_conf["lon_controller_conf"]["station_pid_conf"]["kp"].as<double>();
    lon_controller_conf.station_pid_conf.ki                          = control_conf["lon_controller_conf"]["station_pid_conf"]["ki"].as<double>();
    lon_controller_conf.station_pid_conf.kd                          = control_conf["lon_controller_conf"]["station_pid_conf"]["kd"].as<double>();
    lon_controller_conf.station_pid_conf.kaw                         = control_conf["lon_controller_conf"]["station_pid_conf"]["kaw"].as<double>();
    lon_controller_conf.station_pid_conf.output_saturation_level     = control_conf["lon_controller_conf"]["station_pid_conf"]["output_saturation_level"].as<double>();

    lon_controller_conf.speed_pid_conf.integrator_enable           = control_conf["lon_controller_conf"]["speed_pid_conf"]["integrator_enable"].as<bool>();
    lon_controller_conf.speed_pid_conf.integrator_saturation_level = control_conf["lon_controller_conf"]["speed_pid_conf"]["integrator_saturation_level"].as<double>();
    lon_controller_conf.speed_pid_conf.kp                          = control_conf["lon_controller_conf"]["speed_pid_conf"]["kp"].as<double>();
    lon_controller_conf.speed_pid_conf.ki                          = control_conf["lon_controller_conf"]["speed_pid_conf"]["ki"].as<double>();
    lon_controller_conf.speed_pid_conf.kd                          = control_conf["lon_controller_conf"]["speed_pid_conf"]["kd"].as<double>();
    lon_controller_conf.speed_pid_conf.kaw                         = control_conf["lon_controller_conf"]["speed_pid_conf"]["kaw"].as<double>();
    lon_controller_conf.speed_pid_conf.output_saturation_level     = control_conf["lon_controller_conf"]["speed_pid_conf"]["output_saturation_level"].as<double>();
    
    lon_controller_.Init(&lon_controller_conf);
//配置横向控制参数
    lat_controller_conf.query_relative_time      = control_conf["query_relative_time"].as<double>();
    lat_controller_conf.minimum_speed_protection = control_conf["minimum_speed_protection"].as<double>();

    lat_controller_conf.ts                       = control_conf["lat_controller_conf"]["ts"].as<double>();
    lat_controller_conf.preview_window           = control_conf["lat_controller_conf"]["preview_window"].as<int>();
    lat_controller_conf.cf                       = control_conf["lat_controller_conf"]["cf"].as<double>();
    lat_controller_conf.cr                       = control_conf["lat_controller_conf"]["cr"].as<double>();
    lat_controller_conf.mass_fl                  = control_conf["lat_controller_conf"]["mass_fl"].as<int>();
    lat_controller_conf.mass_fr                  = control_conf["lat_controller_conf"]["mass_fr"].as<int>();
    lat_controller_conf.mass_rl                  = control_conf["lat_controller_conf"]["mass_rl"].as<int>();
    lat_controller_conf.mass_rr                  = control_conf["lat_controller_conf"]["mass_rr"].as<int>();
    lat_controller_conf.eps                      = control_conf["lat_controller_conf"]["eps"].as<double>();
    lat_controller_conf.matrix_q1                = control_conf["lat_controller_conf"]["matrix_q1"].as<double>();
    lat_controller_conf.matrix_q2                = control_conf["lat_controller_conf"]["matrix_q2"].as<double>();
    lat_controller_conf.matrix_q3                = control_conf["lat_controller_conf"]["matrix_q3"].as<double>();
    lat_controller_conf.matrix_q4                = control_conf["lat_controller_conf"]["matrix_q4"].as<double>();
    lat_controller_conf.cutoff_freq              = control_conf["lat_controller_conf"]["cutoff_freq"].as<int>();
    lat_controller_conf.mean_filter_window_size  = control_conf["lat_controller_conf"]["mean_filter_window_size"].as<int>();
    lat_controller_conf.max_iteration            = control_conf["lat_controller_conf"]["max_iteration"].as<int>();
    lat_controller_conf.max_lateral_acceleration = control_conf["lat_controller_conf"]["max_lateral_acceleration"].as<double>();

    lat_controller_conf.front_edge_to_center = control_conf["vehicle_param"]["front_edge_to_center"].as<double>();
    lat_controller_conf.back_edge_to_center  = control_conf["vehicle_param"]["back_edge_to_center"].as<double>();
    lat_controller_conf.left_edge_to_center  = control_conf["vehicle_param"]["left_edge_to_center"].as<double>();
    lat_controller_conf.right_edge_to_center = control_conf["vehicle_param"]["right_edge_to_center"].as<double>();
    lat_controller_conf.length               = control_conf["vehicle_param"]["length"].as<double>();
    lat_controller_conf.width                = control_conf["vehicle_param"]["width"].as<double>();
    lat_controller_conf.height               = control_conf["vehicle_param"]["height"].as<double>();
    lat_controller_conf.min_turn_radius      = control_conf["vehicle_param"]["min_turn_radius"].as<double>();
    lat_controller_conf.max_acceleration     = control_conf["vehicle_param"]["max_acceleration"].as<double>();
    lat_controller_conf.max_deceleration     = control_conf["vehicle_param"]["max_deceleration"].as<double>();
    lat_controller_conf.max_steer_angle      = control_conf["vehicle_param"]["max_steer_angle"].as<double>();
    lat_controller_conf.max_steer_angle_rate = control_conf["vehicle_param"]["max_steer_angle_rate"].as<double>();
    // lat_controller_conf.min_steer_angle_rate = control_conf["vehicle_param"]["min_steer_angle_rate"].as<double>();
    lat_controller_conf.steer_ratio          = control_conf["vehicle_param"]["steer_ratio"].as<double>();
    lat_controller_conf.wheel_base           = control_conf["vehicle_param"]["wheel_base"].as<double>();
    lat_controller_conf.wheel_rolling_radius = control_conf["vehicle_param"]["wheel_rolling_radius"].as<double>();
    //lat_controller_conf.max_abs_speed_when_stopped = control_conf["vehicle_param"]["max_abs_speed_when_stopped"].as<float>();

    lat_controller_.Init(&lat_controller_conf);
//发布控制节点
    chassisCommand_publisher = control_NodeHandle.advertise<car_msgs::control_cmd>("prius", 1);
}



void Control::ProduceControlCommand(car_msgs::control_cmd *control_cmd){

    lon_controller_.ComputeControlCommand(&trajectory_path_,&vehicle_state_,control_cmd,&lon_debug_);

    lat_controller_.ComputeControlCommand(&trajectory_path_,&vehicle_state_,control_cmd,&lat_debug_);
}

void Control::CheckInput(void){

    vehicle_state_.x                = localization_.position.x;
    vehicle_state_.y                = localization_.position.y;
    vehicle_state_.z                = localization_.position.z;
    vehicle_state_.roll             = localization_.angle.x;
    vehicle_state_.pitch            = localization_.angle.y;
    vehicle_state_.yaw              = localization_.angle.z;
    vehicle_state_.heading          = localization_.angle.z;
    vehicle_state_.angular_velocity = localization_.angular_velocity.z;
    vehicle_state_.linear_velocity  = chassis_.speed.x;


    if(trajectory_path_.trajectory_path.size() == 0){
        car_msgs:: trajectory_point trajectory_point;
        trajectory_path_.absolute_time = ros::Time::now().toSec();

        trajectory_point.relative_time = ros::Time::now().toSec()+5;
        trajectory_point.x             = 20;
        trajectory_point.y             = -12;
        trajectory_point.speed         = 1;
        trajectory_point.accel         = 0;
        trajectory_path_.trajectory_path.push_back(trajectory_point);

        trajectory_point.relative_time = ros::Time::now().toSec() + 20;
        trajectory_point.x             = 20;
        trajectory_point.y             = 50;
        trajectory_point.speed         = 1;
        trajectory_point.accel         = 0;
        trajectory_path_.trajectory_path.push_back(trajectory_point);
        ROS_INFO("trajectory_path is null");
        return;
    }
}

void Control::SendCmd(car_msgs::control_cmd *control_cmd){
    control_cmd->header = localization_.header;
    chassisCommand_publisher.publish(*control_cmd);
}

void Control::OnTimer(const ros::TimerEvent&){
    car_msgs:: control_cmd control_cmd;

    CheckInput();
    ProduceControlCommand(&control_cmd);
    SendCmd(&control_cmd);
}

void Control::localization_topic_callback(const car_msgs::localization &localization){
    localization_ = localization;
}
void Control::chassis_topic_callback(const car_msgs::chassis &chassis){
    chassis_ = chassis;
}

void Control::path_topic_callback(const car_msgs::trajectory &trajectory_path){
    trajectory_path_ = trajectory_path;
}
}
