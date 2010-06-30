#include "g_local.h"


void Warhead_Rocket( gentity_t *ent )
{
	trap_SendServerCommand( ent->r.ownerNum, va("print \"Rocket goes boom!\n\""));
}


void Warhead_ClusterGrenade( gentity_t *ent )
{
	vec3_t dir;
	
	VectorSet(dir, 20, 20, 40);
	fire_grenade(ent->parent, ent->r.currentOrigin, dir);
	VectorSet(dir, -20, 20, 40);
	fire_grenade(ent->parent, ent->r.currentOrigin, dir);
	VectorSet(dir, 20, -20, 40);
	fire_grenade(ent->parent, ent->r.currentOrigin, dir);
	VectorSet(dir, -20, -20, 40);
	fire_grenade(ent->parent, ent->r.currentOrigin, dir);
}

void Warhead_Teleport( gentity_t *ent )
{
	
	int		i, j;
	int		event;
	gitem_t *item;
	gentity_t *drop;
	vec3_t	origin, angles;
	gentity_t *parent;
	
	trap_SendServerCommand( ent->r.ownerNum, va("print \"Teleport Rocket goes boom!\n\""));
	
	parent = ent->parent;
	if( !parent->client ) {
		return;
	}
	
	
			// drop flags in CTF
			item = NULL;
			j = 0;
			
			if ( parent->client->ps.powerups[ PW_REDFLAG ] ) {
				item = BG_FindItemForPowerup( PW_REDFLAG );
				j = PW_REDFLAG;
			} else if ( parent->client->ps.powerups[ PW_BLUEFLAG ] ) {
				item = BG_FindItemForPowerup( PW_BLUEFLAG );
				j = PW_BLUEFLAG;
			} else if ( parent->client->ps.powerups[ PW_NEUTRALFLAG ] ) {
				item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
				j = PW_NEUTRALFLAG;
			}
			
			if ( item ) {
				drop = Drop_Item( parent, item, 0 );
				// decide how many seconds it has left
				drop->count = ( parent->client->ps.powerups[ j ] - level.time ) / 1000;
				if ( drop->count < 1 ) {
					drop->count = 1;
				}
				parent->client->ps.powerups[ j ] = 0;
			}
			
	VectorCopy( ent->r.currentOrigin, origin );
	origin[2] += 5; // Add 5 to Z axis to prevent spawning in the ground
	
	VectorCopy( parent->client->ps.viewangles, angles );
	
	TeleportPlayer( parent, origin, angles );
}


void Warhead_Explode( gentity_t *ent )
{
//	trap_SendServerCommand( ent->r.ownerNum, va("print \"Warhead type %d\n\"", ent->warhead));
	switch(ent->warhead ) 
	{
		case WH_ROCKET: 
			Warhead_Rocket( ent );
			break;
		case WH_CLUSTER:
			Warhead_ClusterGrenade( ent );
			break;
		case WH_TELEPORT:
			Warhead_Teleport( ent );
			break;
		default:
			trap_SendServerCommand( ent->r.ownerNum, va("print \"Unknown warhead type %d\n\"", ent->warhead));
	}
}


