#include "g_local.h"

void G_ExplodeMissile( gentity_t *ent );

// All originally taken from http://www.quake3hut.co.uk/q3coding/Turrets%20Part%201.htm
// Cleaned up some and modified some



/*
=======================
Turret code. for spawning and seting the spawn location of turrets
=======================
*/

/*
=================
turret_findenemy
This function goes throught the entity list and check the distance to every entity untill it finds one that fits these rules:

it's not the turret itself.
it's not the creator of the turret (ent->parent)
it's still alive;
it's within a certain distance of the turret (set by rad. rad is current;y set to 500 q3 units. if you change it here, you have to change it in turretthink)
it has to be a client (player or bot)

if it fits these rules, the entity found is set as the turrets enemy.
if no entity is found, enemy is set to NULL;
=================
*/
void turret_findenemy( gentity_t *ent ) {
	gentity_t *target;
	vec3_t	eorg,org;
	int		j,rad;
	
	rad=500;
	VectorCopy(ent->r.currentOrigin,org);
	
	target = g_entities;
	
	for (; target < &g_entities[level.num_entities]; target++)
	{
		if (target==ent) // Don't target self
			continue;
		if (target==ent->parent) // Don't target owner
			continue;
		if (target->health<0) // Don't target the dead
			continue;
		if (!target->inuse) // ???
			continue;
		for (j=0; j<3; j++)
			eorg[j] = org[j] - (target->r.currentOrigin[j] + (target->r.mins[j] + target->r.maxs[j])*0.5);
		if (VectorLength(eorg) > rad)
			continue;
		if(!target->client) // Only target players
			continue;
		ent->enemy=target;
		return;
	}
	
	ent->enemy=NULL;
}



/*
Simple one this
it merely works out the angle to the enemy, then sets ent->turloc (in this case being used to store the angle (as a normalised difference vector)) to the enemy, and the turrets angles (ent->s.apos.trbase) to face the enemy
it then relinks the turret to finalise the changes
This is probably the final version.
*/
void turret_trackenemy( gentity_t *ent ) {
	vec3_t dir;
	
	VectorSubtract(ent->enemy->r.currentOrigin,ent->r.currentOrigin,dir);
	VectorNormalize(dir);
	vectoangles(dir,dir);
	VectorCopy( dir,ent->s.apos.trBase );
	trap_LinkEntity (ent);
}

/*
Fires the plasma at the enemy. 
Doesn't check to see if it will hit, and doesn't lead the enemy at all.

The event is being added so you can see what is happening (the models and such are handled by cgame. the event just tells it to display them)

ent->count is there to stop the gun firing as fast as the turret thinks. decrease the 200 to make it fire faster.

this function will be refined in a later tutorial
*/
void turret_fireonenemy( gentity_t *ent ) {
	vec3_t forward, right, up, origin;
	AngleVectors( ent->s.apos.trBase, forward, right, up );
	// Offset firing so we don't hit ourself
	VectorNormalize(forward);
	VectorMA( ent->r.currentOrigin, 15, forward, origin );
	fire_plasma( ent->parent, origin, forward );
	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
	ent->count=level.time+200;
}


/*
This function is called very regularly.
if the turret has no enemy, it tries to find one.
if the enemy is dead it tries to find a new one
if the enemy has moved out of range it tries to find a new one (the 500 is the number of quake units the enemy should be within)

The turret is then told to track onto the enemy
If a certain amount of time has passed since it last fired, it fires again

Very possible the final version
*/
void turret_think( gentity_t *ent ) {
	vec3_t distance;
	
	ent->nextthink=level.time+10;
	
	if (!ent->enemy)
		turret_findenemy(ent);
	if (!ent->enemy)
		return;
	if (ent->enemy->health<0)
		turret_findenemy(ent);
	if (!ent->enemy)
		return;
	VectorSubtract(ent->r.currentOrigin,ent->enemy->r.currentOrigin,distance);
	if (VectorLength(distance)>500)
		turret_findenemy(ent);
	
	if(!ent->enemy)
		return;
	
	turret_trackenemy(ent);
	if (ent->count<level.time && visible(ent, ent->enemy))
		turret_fireonenemy(ent);
}

/*
================
G_MissileDie

Lancer - Destroy a missile
================
*/
void turret_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	if (inflictor == self)
		return;
	self->takedamage = qfalse;
	self->think = G_ExplodeMissile;
	self->nextthink = level.time + 10;
}

void turret_spawn( gentity_t *ent ) {
	gentity_t *turret; 	// The object to hold the turrets details.
	
	turret = G_Spawn();
	turret->parent = ent;
	turret->eventTime = 200; // makes the firing sequence go away after 200 milliseconds.
	turret->s.weapon = WP_PLASMAGUN; // which weapon will be fired (graphics only)
	turret->classname = "turret";	// not really needed yet. it may be later.
	turret->s.modelindex = G_ModelIndex("models/weapons2/rocketl/rocketl.md3");
	turret->model = "models/weapons2/rocketl/rocketl.md3";
	turret->s.modelindex2 = G_ModelIndex("models/weapons2/rocketl/rocketl.md3");
// the three lines above set the model to be displayed. currently its just the machinegun.
	VectorSet( turret->r.mins, -15, -15, -15 );
	VectorSet( turret->r.maxs, 30, 30, 30);
// these two lines set the size of the turret. doesn't do anything as the turret is not solid, but this will change
	turret->think = turret_think; // what the turret does
	turret->nextthink = level.time+100; // when the turret will activate
	G_SetOrigin( turret, ent->client->ps.origin ); // sets where the turret is
	
	// Make it destroyable
	turret->health = 25;
	turret->takedamage = qtrue;
	turret->die = turret_die;
	turret->r.contents = CONTENTS_CORPSE;
	// Set bounding box
	VectorSet(turret->r.mins, -10, -3, 0);
	VectorSet(turret->r.maxs, 10, 3, 6);
	VectorCopy(turret->r.mins, turret->r.absmin);
	VectorCopy(turret->r.maxs, turret->r.absmax);	
	trap_LinkEntity (turret); // adds the finalized turret.
}

// (NOBODY): Code helper function
// Returns qtrue/qfalse if there's nothing between ent1 and ent2
qboolean visible( gentity_t *ent1, gentity_t *ent2 ) {
	trace_t  trace;
	
	trap_Trace (&trace, ent1->s.pos.trBase, NULL, NULL, ent2->s.pos.trBase, ent1->s.number, MASK_SHOT );
	if ( trace.contents & CONTENTS_SOLID ) {
		return qfalse;
	}
	return qtrue;
}
