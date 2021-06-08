#define GL_SILENCE_DEPRECATION




#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, x_wing, second_x_wing, third_x_wing, death_star, projectionMatrix;
glm::mat4 fight_ground;
glm::mat4 darth_vader, luke;

//glm::mat4 death_star;

float player_x = 0;
float player_rotate = 0;
float star_rotate = 0;

GLuint tie_fighter_textureID, death_star_textureID;
GLuint fight_ground_textureID;
GLuint darth_vader_textureID, luke_textureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}



void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 1!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

 

    viewMatrix = glm::mat4(1.0f);
    x_wing = glm::mat4(1.0f);
    second_x_wing = glm::mat4(1.0f);
    third_x_wing = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    death_star = glm::mat4(1.0f);

    darth_vader = glm::mat4(1.0f);
    luke = glm::mat4(1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    tie_fighter_textureID = LoadTexture("x_wing.png");
    death_star_textureID = LoadTexture("death_star.png");
    fight_ground_textureID = LoadTexture("floor.png");
    darth_vader_textureID = LoadTexture("darth.png");
    luke_textureID = LoadTexture("luke.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() 
{   
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (player_x <= 10) {
        player_x += 1.0f * deltaTime;
        player_rotate += 90.0f * deltaTime;

    }

    star_rotate += 90.0f * deltaTime;
    
    x_wing = glm::mat4(1.0f);
    x_wing = glm::translate(x_wing, glm::vec3(-4.5f, -1.0f, 0.0f));

    x_wing = glm::rotate(x_wing, glm::radians(player_rotate), glm::vec3(0.5f, 0.0f, 0.0f));
    x_wing = glm::translate(x_wing, glm::vec3(player_x, 0.0f, 0.0f));

    second_x_wing = glm::mat4(1.0f);
    second_x_wing = glm::translate(second_x_wing, glm::vec3(-4.5f, 1.0f, 0.0f));
    second_x_wing = glm::rotate(second_x_wing, glm::radians(player_rotate), glm::vec3(0.5f, 0.0f, 0.0f));
    second_x_wing = glm::translate(second_x_wing, glm::vec3(player_x, 0.0f, 0.0f));

    third_x_wing = glm::mat4(1.0f);
    third_x_wing = glm::translate(third_x_wing, glm::vec3(-4.5f, 3.0f, 0.0f));
    third_x_wing = glm::rotate(third_x_wing, glm::radians(player_rotate), glm::vec3(0.5f, 0.0f, 0.0f));
    third_x_wing = glm::translate(third_x_wing, glm::vec3(player_x, 0.0f, 0.0f));

    death_star = glm::mat4(1.0f);
    death_star = glm::scale(death_star, glm::vec3(2.0f, 2.0f, 1.0f));
    death_star = glm::translate(death_star, glm::vec3(1.5f, 0.0f, 0.0f));
    death_star = glm::rotate(death_star, glm::radians(star_rotate/20), glm::vec3(0.0f, 1.0f, 0.0f));


    fight_ground = glm::mat4(1.0f);
    fight_ground = translate(fight_ground, glm::vec3(-2.0f, -3.7f, 0.0f));
    fight_ground = glm::scale(fight_ground, glm::vec3(15.0f, 1.0f, 1.0f));

    luke = glm::mat4(1.0f);
    luke = translate(luke, glm::vec3(2.0f, -2.7f, 0.0f));
    luke = glm::scale(luke, glm::vec3(1.5f, 1.5f, 1.0f));

    darth_vader = glm::mat4(1.0f);
    darth_vader = translate(darth_vader, glm::vec3(-2.0f, -2.7f, 0.0f));
    darth_vader = glm::scale(darth_vader, glm::vec3(1.5f, 1.5f, 1.0f));
}


void Draw_x_wing(glm::mat4 x_wing) {
    program.SetModelMatrix(x_wing);
    glBindTexture(GL_TEXTURE_2D, tie_fighter_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Draw_death_star() {
    program.SetModelMatrix(death_star);
    glBindTexture(GL_TEXTURE_2D, death_star_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Draw_fight_ground() {
    program.SetModelMatrix(fight_ground);
    glBindTexture(GL_TEXTURE_2D, fight_ground_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Draw_luke() {
    program.SetModelMatrix(luke);
    glBindTexture(GL_TEXTURE_2D, luke_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Draw_darth() {
    program.SetModelMatrix(darth_vader);
    glBindTexture(GL_TEXTURE_2D, darth_vader_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


    

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    Draw_x_wing(x_wing);
    Draw_x_wing(second_x_wing);
    Draw_x_wing(third_x_wing);
    Draw_death_star();
    Draw_fight_ground();
    Draw_luke();
    Draw_darth();


    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);

}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
