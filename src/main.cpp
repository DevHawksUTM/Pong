#include <raylib.h> // Include the raylib library header file
#include <string>

using namespace std;

// Dimensions of the window
const int win_width = 600;
const int win_height = 400;

// Paddle Properties
#define PADDLE_SPEED 240
#define LEFT_PADDLE_X 20
#define RIGHT_PADDLE_X 555
#define PADDLE_WIDTH 25
#define PADDLE_HEIGHT 100

// Ball Properties
#define BALL_SPEED 200
#define VELOCITY_TRANSFER_FACTOR 0.80
#define BALL_RADIUS 7.5
#define MAX_VELOCITY 2.3

// Formatting Properties
#define FONT_SIZE 25
#define LINE 3

// Update the y-position of both paddles by getting user input
void update_y(float& left, float& right);

// Update the position of the ball using its current velocity
void update_ball(Vector2& ball, Vector2 ball_velocity);

// The current veloctities of both paddles
int left_velocity = 0;
int right_velocity = 0;

// The scores of each player
int left_score = 0;
int right_score = 0;

int main(){

     // Create the window instance
    InitWindow(win_width, win_height, "Test window");
    SetTargetFPS(60); // 60 FPS

    // The y-coordinates of both paddles
    float left_y = 0;
    float right_y = 0;

    // Ball location (set to center)
    Vector2 ball = {
        win_width/2,
        win_height/2
    };

    // Ball direction & speed
    Vector2 ball_velocity = {
        1,
        1
    };

    while(!WindowShouldClose()){ // While the window remains open

        // Update game attributes
        update_y(left_y, right_y);
        update_ball(ball, ball_velocity);

        // Hit the top or bottom
        if(ball.y - BALL_RADIUS < 0 || ball.y + BALL_RADIUS > win_height){

            ball_velocity.y *= -1;
        }

        // Missed on the left side
        if(ball.x + BALL_RADIUS < 0){
            right_score++;
            ball.x = win_width/2;
            ball.y = win_height/2;
            ball_velocity.x *= -1;
            ball_velocity.y *= -1;
        }

        // Missed on the right side
        if(ball.x - BALL_RADIUS > win_width){
            left_score++;
            ball.x = win_width/2;
            ball.y = win_height/2;
            ball_velocity.x *= -1;
            ball_velocity.y *= -1;
        }

        // Hits a paddle
        if(ball.x - BALL_RADIUS > LEFT_PADDLE_X + PADDLE_WIDTH - LINE && 
            ball.x - BALL_RADIUS < RIGHT_PADDLE_X + LINE){
                
                // Create collision detection recs
                Rectangle left_rec = {
                    LEFT_PADDLE_X,
                    left_y,
                    PADDLE_WIDTH,
                    PADDLE_HEIGHT
                };
                Rectangle right_rec = {
                    RIGHT_PADDLE_X,
                    right_y,
                    PADDLE_WIDTH,
                    PADDLE_HEIGHT
                };
                
                // Hit left paddle
                if(CheckCollisionCircleRec(ball, BALL_RADIUS, left_rec)){

                    ball_velocity.x *= -1;
                    ball_velocity.y += left_velocity * VELOCITY_TRANSFER_FACTOR;
                }

                // Hit right paddle
                else if(CheckCollisionCircleRec(ball, BALL_RADIUS, right_rec)){

                    ball_velocity.x *= -1;
                    ball_velocity.y += right_velocity * VELOCITY_TRANSFER_FACTOR;
                }   
                
                // Constrict to max velocity
                if(ball_velocity.y > MAX_VELOCITY){
                    ball_velocity.y = MAX_VELOCITY;
                }
                else if(ball_velocity.y*-1 > MAX_VELOCITY){
                    ball_velocity.y = MAX_VELOCITY * -1;
                }
            }

        BeginDrawing();// Being drawing sequence
            ClearBackground(BLACK); // Clear the background

            // Draw score
            DrawText(to_string(left_score).c_str(), 200, 5, FONT_SIZE, WHITE);
            DrawText(to_string(right_score).c_str(), win_width-220, 5, FONT_SIZE, WHITE);

            //Draw game elements
            DrawRectangle(LEFT_PADDLE_X, left_y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
            DrawRectangle(RIGHT_PADDLE_X, right_y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
            DrawCircle(ball.x, ball.y, BALL_RADIUS, WHITE);

        EndDrawing(); // End drawing sequence
    }

    CloseWindow(); // Ensure that the window is closed
    return 0;
}

void update_ball(Vector2& ball, Vector2 ball_velocity){

    // Update time, keeps speed independent of frame-rate
    static double last_update_time = GetTime();
    double current_time = GetTime();

    // Move the ball
    ball.x += ball_velocity.x * BALL_SPEED * (current_time - last_update_time);
    ball.y += ball_velocity.y * BALL_SPEED * (current_time - last_update_time);

    // Reset update time
    last_update_time = current_time;
}

void update_y(float& left, float& right){


    static double last_update_time = GetTime();
    double current_time = GetTime();
    
    // The amount to add or detract from both paddless y-coordinates
    float left_add = 0;
    float right_add = 0;

    left_velocity = 0;
    // Left paddle
    if(IsKeyDown(KEY_S)){// Move down
        left_add += (current_time - last_update_time) * PADDLE_SPEED;
        left_velocity = 1;
    }
    else if(IsKeyDown(KEY_Q)){// Move up
        left_add -= (current_time - last_update_time) * PADDLE_SPEED;
        left_velocity = -1;
    }
    if(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_LEFT_CONTROL)){// Boost speeed
        left_add *= 2;
        left_velocity *= 2;
    }

    right_velocity = 0;
    // Right paddle
    if(IsKeyDown(KEY_APOSTROPHE)){// Move down
        right_add += (current_time - last_update_time) * PADDLE_SPEED;
        right_velocity = 1;
    }
    else if(IsKeyDown(KEY_BACKSLASH)){// Move up
        right_add -= (current_time - last_update_time) * PADDLE_SPEED;
        right_velocity = -1;
    }
    if(IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_RIGHT_ALT)){// Boost speed
        right_add *= 2;
        right_velocity *= 2;
    }

    // Add / detract from y-coordinates
    left += left_add;
    right += right_add;

    // Check bounds
    if(left < 0){// Left at top
        left = 0;
        left_velocity = 0;
    }
    else if(left + PADDLE_HEIGHT > win_height){// Left at bottom
        left = win_height - PADDLE_HEIGHT;
        left_velocity = 0;
    }
    if(right < 0){// Right at top
        right = 0;
        right_velocity = 0;
    }
    else if(right + PADDLE_HEIGHT > win_height){// Right at bottom
        right = win_height - PADDLE_HEIGHT;
        right_velocity = 0;
    }

    // Reset update time
    last_update_time = current_time;
}