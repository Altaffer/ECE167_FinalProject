function [DCM] = Euler2DCM(yaw_deg, pitch_deg, roll_deg)
% function takes yaw pitch and roll values and returns a 3x3 DCM matrix
% amgles in degrees
yaw = yaw_deg * 0.0174533; pitch = pitch_deg * 0.0174533; roll = roll_deg * 0.0174533;

DCM = [(cos(pitch) * cos(yaw)) (cos(pitch) * sin(yaw)) (-1*(sin(pitch)));
       ((sin(roll) * sin(pitch) * cos(yaw)) - (cos(roll) * sin(yaw))) ((sin(roll) * sin(pitch) * sin(yaw)) + (cos(roll) * cos(yaw))) (sin(roll) * cos(pitch));
       ((cos(roll) * sin(pitch) * cos(yaw)) + (sin(roll) * sin(yaw))) ((cos(roll) * sin(pitch) * sin(yaw)) - (sin(roll) * cos(yaw))) (cos(roll) * cos(pitch))
      ];