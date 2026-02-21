//
// Created by Igor NIedbała on 04/02/2026.
//

#pragma once
#include <Arduino.h>


struct Vec2 {
    float x;
    float y;
};

class Ball{
public:
    float x;
    float y;
    Vec2 vel;
    int radius;
    float mass;
    int colorR;
    int colorG;
    int colorB;

    Ball(float x, float y, Vec2 vel, int radius, float mass, int colorR, int colorG, int colorB) {
        this->x = x;
        this->y = y;
        this -> vel = vel;
        this->radius = radius;
        this->mass = mass;
        this->colorR = colorR;
        this->colorG = colorG;
        this->colorB = colorB;
    }
};
float dotProduct(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}
float calcOverlap(int r1, int r2, float x1, float y1, float x2, float y2) {
    return r1 + r2 - sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void calcEllasticCollsion(Ball& ball, Ball& ball2, float damp) {
    const float dx = ball.x - ball2.x;
    const float dy = ball.y - ball2.y;

    Vec2 vel1 = ball.vel;
    Vec2 vel2 = ball2.vel;

    if (sqrt((ball.x - ball2.x) * (ball.x - ball2.x) + (ball.y - ball2.y) * (ball.y - ball2.y)) == 0) {

    } else {
        const Vec2 normalVector = {dx/sqrt(dx * dx + dy *dy), dy/sqrt(dx * dx + dy *dy)};

        const float nVel1 = vel1.x * normalVector.x + vel1.y * normalVector.y;
        const float nVel2 = vel2.x * normalVector.x + vel2.y * normalVector.y;

        //nVel1p = ((ball.mass - ball.mass) * nVel1 + 2*ball.mass * nVel2)/(ball.mass + ball.mass)
        const float nVel1p = ((ball.mass - ball2.mass) * nVel1 + 2*ball2.mass * nVel2)/(ball2.mass + ball.mass);
        const float nVel2p = ((ball2.mass - ball.mass) * nVel2 + 2*ball.mass * nVel1)/(ball2.mass + ball.mass);

        vel1 = {vel1.x + (nVel1p - nVel1) * normalVector.x, vel1.y + (nVel1p - nVel1) * normalVector.y};
        vel2 = {vel2.x + (nVel2p - nVel2) * normalVector.x, vel2.y + (nVel2p - nVel2) * normalVector.y};

        const float overlap = calcOverlap(ball.radius, ball2.radius, ball.x, ball.y, ball2.x, ball2.y);
        ball.x += normalVector.x * (overlap / 2) - 1;
        ball.y += normalVector.y * (overlap / 2) - 1;
        ball2.x -= normalVector.x * (overlap / 2) + 1;
        ball2.y -= normalVector.y * (overlap / 2) + 1;

        ball.vel = vel1;
        ball2.vel = vel2;

        ball.vel.x *= damp;
        ball.vel.y *= damp;
        ball2.vel.x *= damp;
        ball2.vel.y *= damp;
    }
}