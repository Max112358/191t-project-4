// EnemyManager.cpp
#include "EnemyManager.h"

EnemyManager::EnemyManager() {
    currentModelPath = nullptr;
    enemyScale = 1.f;
    spawnTimer = new _Timer();
}

EnemyManager::~EnemyManager() {
    for(auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    delete spawnTimer;
}

bool EnemyManager::shouldSpawnEnemy() {
    return false;
    // Check if enough time has passed since last spawn
    if(clock() - spawnTimer->startTime > spawnInterval) {
        spawnTimer->startTime = clock(); // Reset timer
        return true;
    }
    return false;
}

void EnemyManager::checkSpawnTimer() {
    if(shouldSpawnEnemy()) {
        spawnEnemy(spawnX, spawnY, spawnZ);
    }
}

void EnemyManager::initEnemy(const char* modelPath) {
    currentModelPath = modelPath;
    // Set scale based on model type
    if(strstr(modelPath, "carrot") != nullptr) {
        enemyScale = 0.02f;  // Much smaller scale for carrots
    } else {
        enemyScale = 0.1f;   // Default scale for other enemies
    }
}

void EnemyManager::updateEnemies() {
    float deltaTime = 1.0f/60.0f;

    // Update existing enemies
    for(auto enemy : enemies) {
        enemy->updateAnimation(deltaTime);
        enemy->setAnimationState(_ModelLoaderMD2::ANIM_STAND);
        enemy->dirAngleZ = 180; // Face left
        //enemy->pos.x -= enemySpeed; // Move left
    }

    // Check for spawning new enemies
    //checkSpawnTimer();

    // Clean up offscreen enemies
    removeOffscreenEnemies();
}

void EnemyManager::spawnEnemy(float x, float y, float z) {
    if(!currentModelPath) return;

    _ModelLoaderMD2* enemy = new _ModelLoaderMD2();
    enemy->initModel(currentModelPath);
    enemy->pos.x = x;
    enemy->pos.y = y;
    enemy->pos.z = z;

    // Set specific scale for carrot enemies
    if(strstr(currentModelPath, "carrot") != nullptr) {
        enemy->scale = {0.03f, 0.03f, 0.03f};  // Make carrots smaller
    } else {
        enemy->scale = {0.1f, 0.1f, 0.1f};  // Default scale for other models
    }

    enemy->setAnimationState(_ModelLoaderMD2::ANIM_STAND);
    enemies.push_back(enemy);
}

void EnemyManager::removeOffscreenEnemies() {
    // Remove enemies that have moved too far left (off screen)
    auto it = enemies.begin();
    while(it != enemies.end()) {
        if((*it)->pos.x < -600.0f) { // If enemy is far off left side
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}


void EnemyManager::drawEnemies() {
    for(auto enemy : enemies) {
        enemy->DrawWithScale(0.02f);  // Use the new scale override method
    }
}


bool EnemyManager::isColliding(const _ModelLoaderMD2* enemy, const _ModelLoaderMD2* player, bool isPlayerJumping) {

    return false;
    // Get the distance between closest edges
    float edgeDistance = getEdgeDistance(enemy, player);
    cout << edgeDistance << endl;

    // If distance is 0 or negative, we have a collision
    return edgeDistance <= 2.0f;
}

void EnemyManager::checkCollisions(const _ModelLoaderMD2* player, bool isPlayerJumping, int& score) {
    return;
    auto it = enemies.begin();
    while (it != enemies.end()) {
        float distance = getEdgeDistance(*it, player);

        // If we hit an enemy while not jumping, reset score and remove enemy
        if (distance <= 2.0f && !isPlayerJumping) {
            score = 0;  // Reset score on collision
            delete *it;
            it = enemies.erase(it);
            continue;
        }

        // If distance is increasing and we're just past collision distance,
        // that means we successfully jumped over
        if (distance > 2.0f && distance < 3.0f && isPlayerJumping) {
            score += 10;  // Increase score for successful jump
        }

        ++it;
    }
}
