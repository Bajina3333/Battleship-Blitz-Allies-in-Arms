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
void DrawPartnerHealth(const Partner *partner);
void InitPartnerShoot(Partner *partner, Shoot *shoot, int NUM_SHOOTS);
// void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets);
void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets, Enemy *enemies, int numEnemies, int *score, int *enemiesKill);
void buyPartner(int chosenType, Partner *partner, Vector2 playerPosition);
#endif // PARTNER_H
