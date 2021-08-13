#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <dbw_mkz_msgs/SteeringReport.h>
#include <std_msgs/Float64.h>
#include <iostream>
#include <cmath>

class detect_radius
{
	private: 
	double speed;
	double yaw_rate;
	double radius;

	ros::NodeHandle n;
	ros::Subscriber speed_sub;
	ros::Subscriber imu_sub;
	ros::Publisher radius_pub;

	void speed_callback(const dbw_mkz_msgs::SteeringReport::ConstPtr& speed_msg);
	void gyro_callback(const sensor_msgs::Imu::ConstPtr& imu_msg);

	public:
	void print_radius();
	detect_radius();
	~detect_radius();
};

detect_radius::detect_radius()
{
	speed_sub = n.subscribe("/vehicle/steering_report", 1, &detect_radius::speed_callback, this);
	imu_sub = n.subscribe("/vehicle/imu/data_raw", 1, &detect_radius::gyro_callback, this);

	radius_pub = n.advertise<std_msgs::Float64>("/current_radius", 1);
	speed = 0.0;
	yaw_rate = 0.0;
	radius = 0.0;
}

detect_radius::~detect_radius()
{
	std::cout<<"Radius Detector Shutdown"<<std::endl;
}

void detect_radius::speed_callback(const dbw_mkz_msgs::SteeringReport::ConstPtr& speed_msg)
{
	speed = speed_msg->speed;
}

void detect_radius::gyro_callback(const sensor_msgs::Imu::ConstPtr& imu_msg)
{
	yaw_rate = imu_msg->angular_velocity.z;
}

void detect_radius::print_radius()
{
	if (speed==0.0)
	{
		radius = 0.0;
	}
	else
	{
		radius = speed/yaw_rate;
	}
	std::cout<<radius<<std::endl;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "detect_radius");
	detect_radius detector;
	ros::Rate loop_rate(1);
	while (ros::ok())
	{
		detector.print_radius();
		loop_rate.sleep();
		ros::spinOnce();
	}

}