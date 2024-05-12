// In partner.h
#ifndef PARTNER_H
#define PARTNER_H

#include "type.h"  // Ensure this includes Vector2 definition and any needed types

void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type);
void UpdatePartner(Partner *partner, Vector2 playerPosition);
void PartnerShoot(void);
bool CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, float currentTime);
void DrawPartner(Partner *partner);
void DrawPartnerHealth(const Partner *partner);

#endif // PARTNER_H
