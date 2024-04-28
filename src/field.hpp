/*
 * Field locations for auto
 * Units are inches and degrees
 * Field reference is in the engineering notebook
 * Blue/Red colors refer to the side of the field
 * All positions are relative to the origin
 * All orientations are relative to facing the red offense side(0 deg);
 * Starting positions defined for red only
 */

struct Pose2d
{
    float x;
    float y;
    float angle;

    Pose2d(float xVal, float yVal, float angleVal)
    {
        x = xVal;
        y = yVal;
        angle = angleVal;
    }
};

struct Translation2d
{
    float x;
    float y;

    Translation2d(float xVal, float yVal)
    {
        x = xVal;
        y = yVal;
    }
};

float tile = 24;
float fieldX = 144;
float fieldY = 144;
float ringOD = 7;
float ringID = 3;
float ringThick = 2;
float stakeOD = 10;
float stakeHeight = 14.5;

// Field red defense left side defined as (0, 0)
Translation2d origin = Translation2d(0, 0);
Translation2d fieldCenter = Translation2d(fieldX / 2.0, fieldY / 2.0);
