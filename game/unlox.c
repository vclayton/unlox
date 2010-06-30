#include "g_local.h"

grapplestuff_t grapple;

// Returns the ammo type used by the given weapon type
int ammoType( int weapon )
{
	switch(weapon) {
		case WP_SHOTRAIL: return WP_RAILGUN;
		case WP_CHAINRAIL: return WP_RAILGUN;
		default: return weapon;
	}
}
