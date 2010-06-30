typedef enum {
	WH_ROCKET,
	WH_CLUSTER,
} warhead_t;


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
