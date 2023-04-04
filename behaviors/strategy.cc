#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle)
{
    beamX = -HALF_FIELD_X + worldModel->getUNum();
    beamY = -HALF_FIELD_Y + 2 * worldModel->getUNum();
    beamAngle = 0;
}

SkillType NaoBehavior::selectSkill()
{
    // My position and angle
    // cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    // cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    // return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    // return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    // return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    // return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    // return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    // return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    // return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    // return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    // return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    // return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    // return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    // return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    // return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    // return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    // return demoKickingCircle();
    return gogogo();
}

/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */

SkillType NaoBehavior::gogogo()
{
    VecPosition Posball = worldModel->getBall();
    VecPosition target[6] = {
        Posball + VecPosition(-0.5, 0, 0),
        VecPosition(-HALF_FIELD_X, Posball.getY() * GOAL_Y / FIELD_Y, 0),
        Posball + VecPosition(9 * (HALF_FIELD_X - Posball.getX()) / HALF_FIELD_X, 4 * abs(HALF_FIELD_Y - Posball.getY()) / HALF_FIELD_Y, 0),
        Posball + VecPosition(9 * (HALF_FIELD_X - Posball.getX()) / HALF_FIELD_X, -4 * abs(HALF_FIELD_Y - Posball.getY()) / HALF_FIELD_Y, 0),
        VecPosition((Posball.getX() - HALF_FIELD_X) / 2, Posball.getY() + 5, 0),
        VecPosition((Posball.getX() - HALF_FIELD_X) / 2, Posball.getY() - 5, 0)};

    for (int i = 0; i < 6; i++)
    {
        if (target[i].getX() > 15)
        {
            target[i].setX(15);
        }
        if (target[i].getX() < -15)
        {
            target[i].setX(-15);
        }
        if (target[i].getY() > 10)
        {
            target[i].setY(10);
        }
        if (target[i].getY() < -10)
        {
            target[i].setY(-10);
        }
    }
    vector<vector<double>> dis(6, vector<double>(6, 0));
    int BotForTarget[6] = {0, 0, 0, 0, 0, 0};
    VecPosition Pos;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            int playerNum = WO_TEAMMATE1 + j;
            WorldObject *tem = worldModel->getWorldObject(playerNum);
            if (worldModel->getUNum() == playerNum)
            {
                Pos = me;
                dis[i][j] = target[i].getDistanceTo(Pos) + (double)worldModel->isFallen();
            }

            else
            {
                tem = worldModel->getWorldObject(WO_TEAMMATE1 + j);
                Pos = tem->pos;
                dis[i][j] = target[i].getDistanceTo(Pos) + (double)worldModel->getFallenTeammate(playerNum);
            }
            // if(!worldModel->getWorldObject(playerNum)->validPosition){
            //     dis[i][j]=10000;
            // } bu zhi dao wei shen me yi jia zhe ge jiu bao cuo
        }
    }

    for (int i = 0; i < 6; i++)
    {
        int robot = min_element(dis[i].begin(), dis[i].begin() + 6) - dis[i].begin();
        BotForTarget[i] = robot;
        for (int j = 0; j < 6; j++)
            dis[j][robot] = 10000;
    }

    VecPosition closestmatePos = VecPosition(100, 100, 0);
    double closestmateDistance = 10000;

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE6; i++)
    {
        WorldObject *teammate = worldModel->getWorldObject(i);
        if (worldModel->getUNum() == i - WO_TEAMMATE1 + 1)
        {
            continue;
        }
        else
        {
            if (me.getDistanceTo(teammate->pos) < closestmateDistance && teammate->validPosition == true)
            {
                closestmatePos == teammate->pos;
                closestmateDistance == me.getDistanceTo(teammate->pos);
            }
        }
    }

    for (int i = 0; i < 6; i++)
    {
        worldModel->getRVSender()->clearStaticDrawings(); // 清屏
        worldModel->getRVSender()->drawPoint("ball" /*代号*/, target[i].getX() /*x坐标*/,
                                             target[i].getY() /*y坐标*/, 10.0f /*半径*/, RVSender::MAGENTA);
    }

    for (int i = 0; i < 6; i++)
    {
        if (worldModel->getUNum() == WO_TEAMMATE1 + BotForTarget[i])
        {
            if (i == 0)
            {
                if (me.getDistanceTo(target[0]) > 1)
                {
                    return goToTarget(collisionAvoidance(true, true, false, 1,
                                                         .5, target[0], true));
                }
                else
                {
                    if (Posball.getDistanceTo(target[2]) > 3 && Posball.getDistanceTo(target[3]) > 3)
                    {
                        if (me.getX() >= -15 && me.getX() < -15 + 1.8 && (me.getY() > -3 || me.getY() < 3))
                        {
                            return kickBall(KICK_FORWARD, closestmatePos);
                        }
                        else
                        // if(me.getX()>=-15+1.8&&(me.getY()<=-3||me.getY()>=3)){
                        {
                            if (me.getDistanceTo(target[2]) <= me.getDistanceTo(target[3]))
                            {
                                return kickBall(KICK_FORWARD, target[2]);
                            }
                            else
                            {
                                return kickBall(KICK_FORWARD, target[3]);
                            }
                        }
                    }
                    else
                    {
                        if (me.getDistanceTo(VecPosition(15, 1.05, 0)) < me.getDistanceTo(VecPosition(15, -1.05, 0)))
                        {
                            return kickBall(KICK_FORWARD, VecPosition(15, -1.05, 0));
                        }
                        else
                        {
                            return kickBall(KICK_FORWARD, VecPosition(15, 1.05, 0));
                        }
                    }
                }
            }
            else if (i != 0)
            {
                return goToTarget(collisionAvoidance(true, true, false, 1, .5,
                                                     target[i], true));
            }
        }
    }

    // for(int i=0;i<6;i++)
    // {
    // if (worldModel->getUNum() == WO_TEAMMATE1+BotForTarget[i])
    // return goToTarget(collisionAvoidance(true, false, false, 1,
    // .5,target[i], true));//就跑向对应的点位
    // }
}

SkillType NaoBehavior::demoKickingCircle()
{
    // Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X / 2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i)
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum)
        {
            // This is us
            temp = worldModel->getMyPosition();
        }
        else
        {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition)
            {
                temp = teammate->pos;
            }
            else
            {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall)
        {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel->getUNum())
    {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    }
    else
    {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);
        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

        // Our desired target position on the circle
        // Compute target based on uniform number, rotate rate, and time
        VecPosition target = center + VecPosition(circleRadius, 0, 0).rotateAboutZ(360.0 / (NUM_AGENTS - 1) * (worldModel->getUNum() - (worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime() * rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false /*opponent*/, true /*ball*/, 1 /*proximity thresh*/, .5 /*collision thresh*/, target, true /*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10)
        {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        }
        else if (me.getDistanceTo(target) < .5)
        {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        }
        else
        {
            // Move toward target location
            return goToTarget(target);
        }
    }
}
