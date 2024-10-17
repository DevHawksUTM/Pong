#include <raylib.h> // Include the raylib library header file
#include <string>
#include <ctime>

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

// Ball properties
#define BALL_SPEED 200
#define VELOCITY_TRANSFER_FACTOR 0.80
#define BALL_RADIUS 7.5
#define MAX_VELOCITY 6

#define FONT_SIZE 20
#define SCORE_MARGIN 10
#define LINE 3

void update_y(float& left, float& right);

void update_ball(Vector2& ball, Vector2 ball_velocity);

int left_velocity = 0;
int right_velocity = 0;

int left_score = 0;
int right_score = 0;

int main(){

    InitWindow(win_width, win_height, "Test window"); // Create the window instance
    SetTargetFPS(60); // 60 FPS
    SetRandomSeed(time(NULL));

    float left_y = 0;
    float right_y = 0;

    Vector2 ball = {
        win_width/2,
        win_height/2
    };

    Vector2 ball_velocity = {
        1,
        1
    };

    while(!WindowShouldClose()){ // While the window remains open

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
        }

        // Missed on the right side
        if(ball.x - BALL_RADIUS > win_width){
            left_score++;
            ball.x = win_width/2;
            ball.y = win_height/2;
        }

        // Hits a paddle
        if(ball.x - BALL_RADIUS > LEFT_PADDLE_X + PADDLE_WIDTH - LINE && 
            ball.x - BALL_RADIUS < RIGHT_PADDLE_X + LINE){

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

                if(CheckCollisionCircleRec(ball, BALL_RADIUS, left_rec)){

                    ball_velocity.x *= -1;
                    ball_velocity.y += left_velocity * VELOCITY_TRANSFER_FACTOR;
                }
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
            DrawText(to_string(left_score).c_str(), 0, 0, FONT_SIZE, WHITE);
            DrawText(to_string(right_score).c_str(), win_width-100, 0 , FONT_SIZE, WHITE);

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

    static double last_update_time = GetTime();
    double current_time = GetTime();

    ball.x += ball_velocity.x * BALL_SPEED * (current_time - last_update_time);
    ball.y += ball_velocity.y * BALL_SPEED * (current_time - last_update_time);
    last_update_time = current_time;
}

void update_y(float& left, float& right){

    static double last_update_time = GetTime();
    double current_time = GetTime();

    float left_add = 0;
    float right_add = 0;

    left_velocity = 0;
    // Left paddle
    if(IsKeyDown(KEY_S)){
        left_add += (current_time - last_update_time) * PADDLE_SPEED;
        left_velocity = 1;
    }
    else if(IsKeyDown(KEY_Q)){
        left_add -= (current_time - last_update_time) * PADDLE_SPEED;
        left_velocity = -1;
    }
    if(IsKeyDown(KEY_LEFT_ALT)){
        left_add *= 2;
        left_velocity *= 2;
    }

    right_velocity = 0;
    // Right paddle
    if(IsKeyDown(KEY_APOSTROPHE)){
        right_add += (current_time - last_update_time) * PADDLE_SPEED;
        right_velocity = 1;
    }
    else if(IsKeyDown(KEY_BACKSLASH)){
        right_add -= (current_time - last_update_time) * PADDLE_SPEED;
        right_velocity = -1;
    }
    if(IsKeyDown(KEY_RIGHT_CONTROL)){
        right_add *= 2;
        right_velocity *= 2;
    }

    left += left_add;
    right += right_add;

    if(left < 0){
        left = 0;
        left_velocity = 0;
    }
    else if(left + PADDLE_HEIGHT > win_height){
        left = win_height - PADDLE_HEIGHT;
        left_velocity = 0;
    }
    if(right < 0){
        right = 0;
        right_velocity = 0;
    }
    else if(right + PADDLE_HEIGHT > win_height){
        right = win_height - PADDLE_HEIGHT;
        right_velocity = 0;
    }
    last_update_time = current_time;
}