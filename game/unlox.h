typedef enum {
	WH_ROCKET,
	WH_CLUSTER,
	WH_TELEPORT,
	WH_RAIL,
	WH_PLASMA,
	WH_MAX,		// Must always be the LAST entry in the enum!
} warhead_t;


#define MAX_RAIL_BOUNCE 4

typedef struct grapplestuff_s {
	unsigned int damage;
	unsigned int teamdamage;
	unsigned int reload;
	unsigned int speed;
	unsigned int pull;
	unsigned int altdamage;
	unsigned int altteamdamage;
	unsigned int altreload;
	unsigned int altspeed;
	unsigned int altpull;
	unsigned int offhand;
	unsigned int hookitems;
	unsigned int charge;
	unsigned int altcharge;
	unsigned int hooksky;
	unsigned int hooktime;
	unsigned int althooktime;
} grapplestuff_t;

extern grapplestuff_t grapple;

int ammoType( int weapon );
qboolean visible( gentity_t *ent1, gentity_t *ent2 );
void turret_think( gentity_t *ent );
void turret_spawn( gentity_t *ent );
