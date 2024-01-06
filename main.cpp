#include <cstdio>
#include "raylib.h"

// Canvas attributes.
const int X_BOUNDS = 1500;
const int Y_BOUNDS = 1500;

// Target attributes.
const int TARGET_RADIUS = 100;
const int TARGET_VELOCITY = 15;

// Axe attributes;
const int AXE_LENGTH = 50;
const int AXE_VELOCITY = 50;

// Functions.
void handleAxe(Vector2, Vector2 &, bool &);
void handleTarget(Vector2 &, int &);
bool handleCollision(Vector2, Vector2);
// Vector2 moveRect(int &, Vector2);
// Vector2 moveTarget(Vector2);

int main()
{
    // Canvas components.
    InitWindow(X_BOUNDS, Y_BOUNDS, "Hello, world!");
    SetTargetFPS(60);

    // Target components.
    Vector2 targetPos{X_BOUNDS / 2, Y_BOUNDS / 4};
    int targetDirection{TARGET_VELOCITY};

    // Axe components.
    Vector2 mPos{};
    Vector2 axePos{};
    bool hasBeenThrown{false};

    while (!WindowShouldClose())
    {
        BeginDrawing();

        handleTarget(targetPos, targetDirection);
        handleAxe(mPos, axePos, hasBeenThrown);
        handleCollision(axePos, targetPos);

        ClearBackground(RED);
        EndDrawing();
    }
    printf("Close window has been triggered.\n");
}

void handleTarget(Vector2 &targetPos, int &targetDirection)
{
    if ((targetPos.x + TARGET_RADIUS) >= X_BOUNDS || (targetPos.x - TARGET_RADIUS) <= 0)
    {
        targetDirection *= -1;
        // printf("I have hit an edge, changing direction: %d\n", targetDirection);
    }

    targetPos.x += targetDirection;

    // printf("moving x by %d, curr pos: %f\n", targetDirection, targetPos.x);
    DrawCircle(targetPos.x, targetPos.y, TARGET_RADIUS, BLUE);
}

void handleAxe(Vector2 mPos, Vector2 &axePos, bool &hasBeenThrown)
{
    /*
     * axe should follow the mouse until pressed                             [COMPLETED]
     * once pressed the axe should shoot forward
     * if the axe shoots past the bounds, then reset the axe to the mouse
     */
    bool isOutOfBounds{};
    if (axePos.y <= 0) {
        isOutOfBounds = true;
        hasBeenThrown = false;
    }

    mPos = GetMousePosition();

    if (!hasBeenThrown)
    {
        DrawRectangle(mPos.x - (AXE_LENGTH / 2), mPos.y - AXE_LENGTH, AXE_LENGTH, AXE_LENGTH, BLACK);
    }
    else if (hasBeenThrown && !isOutOfBounds)
    {
        axePos.y -= AXE_VELOCITY;
        DrawRectangle(axePos.x, axePos.y, AXE_LENGTH, AXE_LENGTH, BLACK);
    }

    if (IsMouseButtonPressed(0) && isOutOfBounds)
    {
        puts("Mouse button has been pressed!");
        hasBeenThrown = true;
        axePos.x = mPos.x;
        axePos.y = mPos.y;
    }
}

bool handleCollision(Vector2 axePos, Vector2 targetPos)
{
    /*
     * collision will occur if the bounding box of the target and the bounding
     * box of the axe are within eachother.
     */
    int targetXLowerBound = targetPos.x - TARGET_RADIUS;
    int targetXUppderBound = targetPos.x + TARGET_RADIUS;

    int targetYUppderBound = targetPos.y - TARGET_RADIUS;
    int targetYLowerBound = targetPos.y + TARGET_RADIUS;

    if ((axePos.x >= targetXLowerBound || axePos.x+AXE_LENGTH >= targetXLowerBound) &&
        axePos.x <= targetXUppderBound &&
        axePos.y <= targetYLowerBound &&
        axePos.y >= targetYUppderBound)
    {
        puts("We are inside the target!");
        return true;
    }

    return false;
}

/////////////
// Depricated
/////////////
Vector2 moveTarget(Vector2 circPos)
{
    Vector2 newPos = circPos;
    const int circVelocity = 10;

    if (IsKeyDown(KEY_A))
    {
        newPos.x -= circVelocity;
    }

    if (IsKeyDown(KEY_D))
    {
        newPos.x += circVelocity;
    }

    if (IsKeyDown(KEY_W))
    {
        newPos.y -= circVelocity;
    }

    if (IsKeyDown(KEY_S))
    {
        newPos.y += circVelocity;
    }

    // Block movement if we reach bounding box x.
    if (static_cast<int>((newPos.x - TARGET_RADIUS)) <= 0 || static_cast<int>((newPos.x + TARGET_RADIUS)) >= X_BOUNDS)
    {
        newPos.x = circPos.x;
    }

    // Block movement if we reach bounding box y.
    if (static_cast<int>((newPos.y - TARGET_RADIUS)) <= 0 || static_cast<int>((newPos.y + TARGET_RADIUS)) >= Y_BOUNDS)
    {
        newPos.y = circPos.y;
    }

    return newPos;
}

Vector2 moveRect(int &direction, Vector2 rectPos)
{
    // printf("Incoming direction value: %d\n", direction);
    rectPos.y += direction;
    if (rectPos.y <= 0 || (rectPos.y + 50) >= Y_BOUNDS)
    {
        direction = -direction;
    }

    return rectPos;
}