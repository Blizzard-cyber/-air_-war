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

void hero_fire(float hero_x, float hero_y);//��������ӵ����������

Bullet* create_bullet(float x, float y);//�����ӵ�����������

void insert_bullet(Bullet* pBullet);//�����ӵ����ݵ�������
void update_bullets();//�����ӵ�����
void draw_my_bullets();//�����ӵ�

Enemy* Create_enemy(int type);//���ɵл�����
void insert_enemy(Enemy* penemy);//����л����ݵ�����

void Move_Enemy();//�л��ƶ�

void Draw_Enemy();//�����л�

int Condition(float x, float y);//��Ϸ���������ж�

void FreeMemory();//�ͷſռ�

void paly_Plane();//�ӵ����ел�
//��Ϸ������
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "flighters");
    InitAudioDevice();

    Music music = LoadMusicStream("resources/sound/game_music.ogg");//��������
    PlayMusicStream(music);

    Texture2D texture3 = LoadTexture("resources/image/background.png");//����ͼƬ
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
    float hero_y = SCREEN_HEIGHT - HERO_HEIGHT;//�ҷ��ɻ�����
    int count = 0;//���ʱ�仭���л�
    int flag = 0;//�ж���Ϸ����

    SetTargetFPS(60);

    while (!WindowShouldClose()&&flag==0)
    {
        BeginDrawing();
        UpdateMusicStream(music);

        //��������
        if (IsKeyDown(KEY_RIGHT) && hero_x < SCREEN_WIDTH - HERO_WIDTH)hero_x += HERO_SPEED;
        if (IsKeyDown(KEY_LEFT) && hero_x > 0)hero_x -= HERO_SPEED;
        if (IsKeyDown(KEY_UP) && hero_y > 0)hero_y -= HERO_SPEED;
        if (IsKeyDown(KEY_DOWN) && hero_y < SCREEN_HEIGHT - HERO_HEIGHT)hero_y += HERO_SPEED;
        if (IsKeyDown(KEY_SPACE)) {
            hero_fire(hero_x, hero_y);
        }
        update_bullets();//�����ӵ�����
        ClearBackground(RAYWHITE);

        Vector2 position3 = { 0,0 };//����
        Vector2 position1 = { hero_x,hero_y };//�ҷ��ɻ�

        DrawTextureRec(texture3, cilp3, position3, WHITE);//��������
        DrawTextureRec(texture1, cilp1, position1, WHITE);//�����ҷ��ɻ�
        draw_my_bullets();//�����ӵ�

        if (count%1000==0)//ÿ1000��ѭ����һ����ɻ�
        {
            Enemy* enemy = Create_enemy(1);
            insert_enemy(enemy);
        }
        if (count%500==0)//ÿ500��ѭ����һ��С�ɻ�
        {
            Enemy* enemy = Create_enemy(2);
            insert_enemy(enemy);
        }
        count++;

        Move_Enemy();//�л��ƶ�
        Draw_Enemy();//�����л�
        paly_Plane();//��ɱ�л�
        //�ж���Ϸ����
        if (Condition(hero_x,hero_y) == 1){
            flag = 1; 
        }
       
        EndDrawing();
    }
     FreeMemory();//�ͷŶ�̬����Ŀռ�
     system("cmd /c mshta vbscript:msgbox(\"GAME OVER!!!\",64,\" FLIGHTERS;\")(window.close)");//��Ϸ������ʾGAME OVER!!!

    CloseWindow();
    return 0;
}

//�����ӵ�����
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
//�����ӵ�
void draw_my_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (my_bullets[i]) {
            DrawTextureRec(texture1, BULLET_CLIP, my_bullets[i]->position, RED);
        }
    }
}
//�����ӵ�����
void hero_fire(float hero_x, float hero_y) {
    Bullet* bullet = create_bullet(hero_x + HERO_WIDTH / 2 - 3, hero_y - 20);
    insert_bullet(bullet);
}
//�����ӵ����ݵ�����
void insert_bullet(Bullet* pBullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!my_bullets[i]) {
            my_bullets[i] = pBullet;
            return;
        }
    }
}
//�����ӵ�����
Bullet* create_bullet(float x, float y) {
    Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));
    if (!bullet) return NULL;
    Vector2 position = { x, y };
    bullet->position = position;
    return bullet;
}

//�����л�����
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
//����л����ݵ�����
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


//�л����ƶ�
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

//���Ƶо��ɻ�
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
//�ж���Ϸ��������
int Condition(float x, float y) {
    //���л����Ҿ��ɻ�������ײʱ����Ϸ����
    Vector2 hero_pos = { x,y };
    for (int i = 0; i < 2; i++) {
       
        if (enemys[i]&&enemys[i]->type == 1) {
            if (enemys[i]->pos_enemy.x + (ENEMY1_WIDTH / 2) >= x && enemys[i]->pos_enemy.x + (ENEMY1_WIDTH / 2) <= x + HERO_WIDTH && enemys[i]->pos_enemy.y + ENEMY1_HEIGHT -10==y) {
                DrawTextureRec(texture_hero_down, cilp1, hero_pos, WHITE);
                return 1;//���������򷵻�1
            }
        }
         if (enemys[i]&&enemys[i]->type == 2) {
            if (enemys[i]->pos_enemy.x + (ENEMY2_WIDTH / 2) >= x && enemys[i]->pos_enemy.x + (ENEMY2_WIDTH / 2) <= x + HERO_WIDTH && enemys[i]->pos_enemy.y + ENEMY2_HEIGHT -10== y) {
                DrawTextureRec(texture_hero_down, cilp1, hero_pos, WHITE);
                return 1;//���������򷵻�1
                }
         }
     
        }
        return 0;//δ���������򷵻�0
    }


//�ͷſռ�
void FreeMemory() {
    

    //�ͷŵо���ռ�ռ�
    for (int i = 0; i < MAX_ENEMY; i++) {
        free(enemys[i]);
        enemys[i] = NULL;
    }
    //�ͷ��ӵ��ռ�
    for (int i = 0; i < MAX_BULLETS; i++) {
        free(my_bullets[i]);
        my_bullets[i] = NULL;
    }
}

 //�ӵ����ел��л���ʧ/Ѫ������
void paly_Plane()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
       
        for (int k = 0; k < MAX_BULLETS; k++)
        {
            //�ж���ײ
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
            //Ѫ��Ϊ0���л���ʧ
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