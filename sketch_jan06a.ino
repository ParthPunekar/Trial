#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Vector3Stamped.h>

#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define RIGHT_SPEED 9
#define LEFT_SPEED 10
#define LOOPTIME 100

int spd = 0;
float x;
float z;

ros::NodeHandle nh;

void velCallback (const geometry_msgs::Twist& vel) {
  x = vel.linear.x;
  z = vel.angular.z;
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", velCallback);
geometry_msgs::Vector3Stamped speed_msg;
ros::Publisher speed_pub("speed", &speed_msg);
void setup() {

  nh.initNode();
  nh.getHardware() -> setBaud(57600);
  nh.subscribe(sub);
  nh.advertise(speed_pub);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
}

void loop() {

  if (x > 0) {            // Front
    
    int temp = (int)(x * 100);
    spd = map(temp, 1, 26, 1, 255);
    analogWrite(RIGHT_SPEED, spd);
    analogWrite(LEFT_SPEED, spd);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (x < 0) {     // Back

    int temp = (int)(x * 100);
    spd = map(temp, -1, -26, 1, 255);
    analogWrite(RIGHT_SPEED, spd);
    analogWrite(LEFT_SPEED, spd);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (z < 0) {     // Right

    int temp = (int)(z * 100);
    spd = map(temp, -10, -182, 1, 255);
    analogWrite(RIGHT_SPEED, spd);
    analogWrite(LEFT_SPEED, spd);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (z > 0) {     // Left

    int temp = (int)(z * 100);
    spd = map(temp, 10, 182, 1, 255);
    analogWrite(RIGHT_SPEED, spd);
    analogWrite(LEFT_SPEED, spd);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else {                // Stop

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
  publishSpeed(LOOPTIME);
}

void publishSpeed(double time) {

  speed_msg.header.stamp = nh.now();
  speed_msg.vector.x = x;
  speed_msg.vector.y = x;
  speed_msg.vector.z = time/1000;
  speed_pub.publish(&speed_msg);
  nh.spinOnce();
  nh.loginfo("Publishing Odometry");
}
