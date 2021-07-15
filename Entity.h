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


enum EntityType {PLAYER, PLATFORM, ENEMY};
enum AIType {WALKER, WAITANDGO, PATROL};
enum AIState {IDLE, WALKING, ATTACKING};


class Entity {
public:

    EntityType entityType;
    AIType aiType;
    AIState aiState;
    EntityType lastCollision;


    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 accelaration;
    float speed;

    float width = 1;
    float height = 1;
    bool jump = false;
    float jumpPower = 0;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;


    bool isActive = true;
    bool gameover = false;


    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;

    int x = 1;
    int y = 1;


    Entity();


    // collisions
    bool CheckCollision(Entity* other);
    bool SensorCheckCollision(Entity* other, glm::vec3 sensorPoint);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);

    // general
    void Update(float deltaTime, Entity* player, Entity* platforms, int platformCount, Entity* enemies, int enemyCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    // enemy 
    void AI(Entity* player, Entity* platforms, int platformCount);
    void AIWalker(Entity* platforms, int platformCount);
    void AIWaitAndGo(Entity* player, Entity* platforms, int platformCount);
    void AIPatrol(Entity* player, Entity* platforms, int platformCount);
};