// In partner.h
#ifndef PARTNER_H
#define PARTNER_H

#include "type.h"  // Ensure this includes Vector2 definition and any needed types
void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type);
void UpdatePartner(Partner *partner, Vector2 playerPosition,Player *player);
void CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, int *enemiesKill, int *total_count_Enemies, Shoot *shoot);
void DrawPartner(Partner *partner);
void DrawPartnerHealth(const Partner *partner);
void InitPartnerShoot(Partner *partner, Shoot *shoot, int NUM_SHOOTS);
// void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets);
void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets, Enemy *enemies, int numEnemies, int *enemiesKill, int *total_count_Enemies, int *totalEnemies);
void UpdatePartnerShoot(Partner *partner, Shoot *partnerBullets, int numBullets, Enemy *enemies, int numEnemies, int *enemiesKill);
void DrawPartnerShoot(Shoot *partnerBullets, int numBullets);
#endif // PARTNER_H
