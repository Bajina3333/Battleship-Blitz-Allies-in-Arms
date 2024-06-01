// In partner.h
// In partner.h
#ifndef PARTNER_H
#define PARTNER_H

#include "type.h"  // Ensure this includes Vector2 definition and any needed types
void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type);
// void UpdatePartner(Partner *partner, Vector2 playerPosition);
void UpdatePartner(Partner *partner, Vector2 playerPosition, Player *player);
void CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, int *enemiesKill);
void DrawPartner(Partner *partner);
// bool CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy);
void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets, Enemy *enemies, int numEnemies, int *enemiesKill);

#endif
