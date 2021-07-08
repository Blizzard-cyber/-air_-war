#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 800

#define HERO_WIDTH 102
#define HERO_HEIGHT 126

#define HERO_SPEED 5.0f
#define BULLET_SPEED 3.0
#define ENEMY1_WIDTH 57
#define ENEMY1_HEIGHT 43
#define ENEMY2_WIDTH 69
#define ENEMY2_HEIGHT 99

#define MAX_BULLETS 100
#define MAX_ENEMY 10

const Rectangle BULLET_CLIP = { 1004,987,9,21 };
const Rectangle ENEMY_CLIP1 = { 0, 0, ENEMY1_WIDTH, ENEMY1_HEIGHT };
const Rectangle ENEMY_CLIP2 = { 0, 0, ENEMY2_WIDTH, ENEMY2_HEIGHT };
const Rectangle cilp3 = { 0,0, SCREEN_WIDTH , SCREEN_HEIGHT };
const Rectangle cilp1 = { 0,0, HERO_WIDTH,HERO_HEIGHT };
typedef struct Bullet {
    Vector2 position;
}Bullet;

typedef struct Enemy
{
    Vector2 pos_enemy;
    int type;
    int hp;
    float speed;
}Enemy;
Enemy* enemys[MAX_ENEMY];


Texture2D texture1;
Texture2D texture_enemy1;
Texture2D texture_enemy2;
Texture2D texture_enemy1_down1;
Texture2D texture_enemy1_down2;
Texture2D texture_enemy1_down3;
Texture2D texture_enemy1_down4;
Texture2D texture_enemy2_down1;
Texture2D texture_enemy2_down2;
Texture2D texture_enemy2_down3;
Texture2D texture_enemy2_down4;
Texture2D texture_hero_down;


Bullet* my_bullets[MAX_BULLETS];

void hero_fire(float hero_x, float hero_y);//创造包含子弹坐标的数组

Bullet* create_bullet(float x, float y);//生成子弹的坐标数据

void insert_bullet(Bullet* pBullet);//插入子弹数据到数组中
void update_bullets();//更新子弹坐标
void draw_my_bullets();//画出子弹

Enemy* Create_enemy(int type);//生成敌机数据
void insert_enemy(Enemy* penemy);//插入敌机数据到数组

void Move_Enemy();//敌机移动

void Draw_Enemy();//画出敌机

int Condition(float x, float y);//游戏结束条件判断

void FreeMemory();//释放空间

void paly_Plane();//子弹击中敌机
//游戏主函数
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "flighters");
    InitAudioDevice();

    Music music = LoadMusicStream("resources/sound/game_music.ogg");//加载音乐
    PlayMusicStream(music);

    Texture2D texture3 = LoadTexture("resources/image/background.png");//加载图片
    texture1 = LoadTexture("resources/image/hero.png");
    texture_enemy1 = LoadTexture("resources/image/enemy1.png");
    texture_enemy2 = LoadTexture("resources/image/enemy2.png");
    texture_enemy1_down1 = LoadTexture("resources/image/enemy1_down1.png");
    texture_enemy1_down2 = LoadTexture("resources/image/enemy1_down2.png");
    texture_enemy1_down3 = LoadTexture("resources/image/enemy1_down3.png");
    texture_enemy1_down4 = LoadTexture("resources/image/enemy1_down4.png");
    texture_enemy2_down1 = LoadTexture("resources/image/enemy2_down1.png");
    texture_enemy2_down2 = LoadTexture("resources/image/enemy2_down2.png");
    texture_enemy2_down3 = LoadTexture("resources/image/enemy2_down3.png");
    texture_enemy2_down4 = LoadTexture("resources/image/enemy2_down4.png");
    texture_hero_down = LoadTexture("resources/image/hero_down.png");

    float hero_x = (SCREEN_WIDTH - HERO_WIDTH) / 2;
    float hero_y = SCREEN_HEIGHT - HERO_HEIGHT;//我方飞机坐标
    int count = 0;//间隔时间画出敌机
    int flag = 0;//判定游戏结束

    SetTargetFPS(60);

    while (!WindowShouldClose()&&flag==0)
    {
        BeginDrawing();
        UpdateMusicStream(music);

        //按键操作
        if (IsKeyDown(KEY_RIGHT) && hero_x < SCREEN_WIDTH - HERO_WIDTH)hero_x += HERO_SPEED;
        if (IsKeyDown(KEY_LEFT) && hero_x > 0)hero_x -= HERO_SPEED;
        if (IsKeyDown(KEY_UP) && hero_y > 0)hero_y -= HERO_SPEED;
        if (IsKeyDown(KEY_DOWN) && hero_y < SCREEN_HEIGHT - HERO_HEIGHT)hero_y += HERO_SPEED;
        if (IsKeyDown(KEY_SPACE)) {
            hero_fire(hero_x, hero_y);
        }
        update_bullets();//更新子弹坐标
        ClearBackground(RAYWHITE);

        Vector2 position3 = { 0,0 };//背景
        Vector2 position1 = { hero_x,hero_y };//我方飞机

        DrawTextureRec(texture3, cilp3, position3, WHITE);//画出背景
        DrawTextureRec(texture1, cilp1, position1, WHITE);//画出我方飞机
        draw_my_bullets();//画出子弹

        if (count%1000==0)//每1000次循环画一个大飞机
        {
            Enemy* enemy = Create_enemy(1);
            insert_enemy(enemy);
        }
        if (count%500==0)//每500次循环画一个小飞机
        {
            Enemy* enemy = Create_enemy(2);
            insert_enemy(enemy);
        }
        count++;

        Move_Enemy();//敌机移动
        Draw_Enemy();//画出敌机
        paly_Plane();//击杀敌机
        //判定游戏结束
        if (Condition(hero_x,hero_y) == 1){
            flag = 1; 
        }
       
        EndDrawing();
    }
     FreeMemory();//释放动态分配的空间
     system("cmd /c mshta vbscript:msgbox(\"GAME OVER!!!\",64,\" FLIGHTERS;\")(window.close)");//游戏结束显示GAME OVER!!!

    CloseWindow();
    return 0;
}

//更新子弹坐标
void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (my_bullets[i]) {
            my_bullets[i]->position.y -= BULLET_SPEED;
            if (my_bullets[i]->position.y < 0) {
                free(my_bullets[i]);
                my_bullets[i] = NULL;
            }
        }
    }
}
//画出子弹
void draw_my_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (my_bullets[i]) {
            DrawTextureRec(texture1, BULLET_CLIP, my_bullets[i]->position, RED);
        }
    }
}
//加入子弹数据
void hero_fire(float hero_x, float hero_y) {
    Bullet* bullet = create_bullet(hero_x + HERO_WIDTH / 2 - 3, hero_y - 20);
    insert_bullet(bullet);
}
//插入子弹数据到数组
void insert_bullet(Bullet* pBullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!my_bullets[i]) {
            my_bullets[i] = pBullet;
            return;
        }
    }
}
//创造子弹坐标
Bullet* create_bullet(float x, float y) {
    Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));
    if (!bullet) return NULL;
    Vector2 position = { x, y };
    bullet->position = position;
    return bullet;
}

//产生敌机数据
Enemy* Create_enemy(int type)
{
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    if (!enemy) return NULL;

    if (type == 1)
    {
        enemy->type = 1;
        enemy->pos_enemy.x = rand() % (SCREEN_WIDTH - ENEMY1_WIDTH);
        enemy->pos_enemy.y = 0;
        enemy->speed = 1;
        enemy->hp = 2;
    }

    if (type == 2)
    {
        enemy->type = 2;
        enemy->pos_enemy.x = rand() % (SCREEN_WIDTH - ENEMY2_WIDTH);
        enemy->pos_enemy.y = 0;
        enemy->speed = 0.5;
        enemy->hp = 6;
    }
    return enemy;
}
//插入敌机数据到数组
void insert_enemy(Enemy* penemy)
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (!enemys[i])
        {
            enemys[i] = penemy;
            return;
        }
    }
}


//敌机的移动
void Move_Enemy() {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemys[i]) {

           enemys[i]->pos_enemy.y += enemys[i]->speed;
           if (enemys[i]->pos_enemy.y > SCREEN_HEIGHT) {
               free(enemys[i]);
               enemys[i] = NULL;
           }
        }
    }
}

//绘制敌军飞机
void Draw_Enemy() {

    
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (enemys[i]) {
            if (enemys[i]->type == 1)
            {
                DrawTextureRec(texture_enemy1, ENEMY_CLIP1, enemys[i]->pos_enemy, WHITE);
            }

            if (enemys[i]->type == 2)
            {
                DrawTextureRec(texture_enemy2, ENEMY_CLIP2, enemys[i]->pos_enemy, WHITE);
            }
        }
    }
}
//判断游戏结束条件
int Condition(float x, float y) {
    //当敌机与我军飞机发生碰撞时，游戏结束
    Vector2 hero_pos = { x,y };
    for (int i = 0; i < 2; i++) {
       
        if (enemys[i]&&enemys[i]->type == 1) {
            if (enemys[i]->pos_enemy.x + (ENEMY1_WIDTH / 2) >= x && enemys[i]->pos_enemy.x + (ENEMY1_WIDTH / 2) <= x + HERO_WIDTH && enemys[i]->pos_enemy.y + ENEMY1_HEIGHT -10==y) {
                DrawTextureRec(texture_hero_down, cilp1, hero_pos, WHITE);
                return 1;//满足条件则返回1
            }
        }
         if (enemys[i]&&enemys[i]->type == 2) {
            if (enemys[i]->pos_enemy.x + (ENEMY2_WIDTH / 2) >= x && enemys[i]->pos_enemy.x + (ENEMY2_WIDTH / 2) <= x + HERO_WIDTH && enemys[i]->pos_enemy.y + ENEMY2_HEIGHT -10== y) {
                DrawTextureRec(texture_hero_down, cilp1, hero_pos, WHITE);
                return 1;//满足条件则返回1
                }
         }
     
        }
        return 0;//未满足条件则返回0
    }


//释放空间
void FreeMemory() {
    

    //释放敌军所占空间
    for (int i = 0; i < MAX_ENEMY; i++) {
        free(enemys[i]);
        enemys[i] = NULL;
    }
    //释放子弹空间
    for (int i = 0; i < MAX_BULLETS; i++) {
        free(my_bullets[i]);
        my_bullets[i] = NULL;
    }
}

 //子弹击中敌机敌机消失/血量减少
void paly_Plane()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
       
        for (int k = 0; k < MAX_BULLETS; k++)
        {
            //判定碰撞
            if (enemys[i]&&my_bullets[k]&&enemys[i]->type == 1) {
                if (my_bullets[k]->position.x > enemys[i]->pos_enemy.x && my_bullets[k]->position.x < enemys[i]->pos_enemy.x + ENEMY1_WIDTH
                    && my_bullets[k]->position.y == enemys[i]->pos_enemy.y + ENEMY1_HEIGHT)
                {
                    free(my_bullets[k]);
                    my_bullets[k] = NULL;
                    enemys[i]->hp--;
                }
                
            }
            if (enemys[i]&&my_bullets[k]&&enemys[i]->type == 2) {
                if (my_bullets[k]->position.x > enemys[i]->pos_enemy.x && my_bullets[k]->position.x < enemys[i]->pos_enemy.x + ENEMY2_WIDTH
                    && my_bullets[k]->position.y == enemys[i]->pos_enemy.y + ENEMY2_HEIGHT)
                {
                    free(my_bullets[k]);
                    my_bullets[k] = NULL;
                    enemys[i]->hp--;
                }
               
            }
            //血量为0，敌机消失
            if (enemys[i]&&enemys[i]->hp <= 0)
            {
                if (enemys[i]->type == 1) {
                    DrawTextureRec(texture_enemy1_down1, ENEMY_CLIP1, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy1_down2, ENEMY_CLIP1, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy1_down3, ENEMY_CLIP1, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy1_down4, ENEMY_CLIP1, enemys[i]->pos_enemy, WHITE);
                }
                if (enemys[i]->type == 2) {
                    DrawTextureRec(texture_enemy2_down1, ENEMY_CLIP2, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy2_down2, ENEMY_CLIP2, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy2_down3, ENEMY_CLIP2, enemys[i]->pos_enemy, WHITE);
                    DrawTextureRec(texture_enemy2_down4, ENEMY_CLIP2, enemys[i]->pos_enemy, WHITE);
                }
                free(enemys[i]);
                enemys[i] = NULL;
            }
        }
    }
}