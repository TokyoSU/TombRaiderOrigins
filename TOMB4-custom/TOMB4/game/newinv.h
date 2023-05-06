#pragma once
#include "global/types.h"

long LoadGame();
long SaveGame();
void init_new_inventry();
void do_debounced_joystick_poo();
void DrawInventoryItemMe(INVDRAWITEM* item, long shade, long overlay, long shagflag);
void DrawThreeDeeObject2D(long x, long y, long num, long shade, long xrot, long yrot, long zrot, long bright, long overlay);
long go_and_load_game();
long go_and_save_game();
void insert_object_into_list_v2(short num);
void construct_combine_object_list();
void insert_object_into_list(short num);
void construct_object_list();
void handle_object_changeover(long ringnum);
void fade_ammo_selector();
void spinback(unsigned short* cock);
void update_laras_weapons_status();
long have_i_got_item(short obj);
void combine_revolver_lasersight(long flag);
void combine_crossbow_lasersight(long flag);
void combine_PuzzleItem1(long flag);
void combine_PuzzleItem2(long flag);
void combine_PuzzleItem3(long flag);
void combine_PuzzleItem4(long flag);
void combine_PuzzleItem5(long flag);
void combine_PuzzleItem6(long flag);
void combine_PuzzleItem7(long flag);
void combine_PuzzleItem8(long flag);
void combine_KeyItem1(long flag);
void combine_KeyItem2(long flag);
void combine_KeyItem3(long flag);
void combine_KeyItem4(long flag);
void combine_KeyItem5(long flag);
void combine_KeyItem6(long flag);
void combine_KeyItem7(long flag);
void combine_KeyItem8(long flag);
void combine_PickupItem1(long flag);
void combine_PickupItem2(long flag);
void combine_PickupItem3(long flag);
void combine_PickupItem4(long flag);
void combine_ClockWorkBeetle(long flag);
long do_special_waterskin_combine_bullshit(long flag);
void setup_objectlist_startposition(short newobj);
void setup_objectlist_startposition2(short newobj);
long have_i_got_object(short object_number);
void remove_inventory_item(short object_number);
long convert_obj_to_invobj(short obj);
void draw_compass();
void do_examine_mode();
void dels_give_lara_items_cheat();
void use_current_item();
void DEL_picked_up_object(short objnum);
long is_item_currently_combinable(short obj);
long do_these_objects_combine(long obj1, long obj2);
void combine_these_two_objects(short obj1, short obj2);
void seperate_object(short obj);
void draw_ammo_selector();
void handle_inventry_menu();
void draw_current_object_list(long ringnum);
long S_CallInventory2();

extern INVOBJ inventry_objects_list[];
extern long GLOBAL_enterinventory;
extern long GLOBAL_inventoryitemchosen;
extern long GLOBAL_lastinvitem;
extern long InventoryActive;

enum ring_types
{
	RING_INVENTORY,
	RING_AMMO
};

struct OPTION_TABLE
{
	INVOBJ_TYPES inv_item = INV_NOITEM;
	bool equip = false;
	bool combine = false;
	bool separate = false;
	bool use = false;
	bool examine = false;
	bool choose_ammo = false;
	bool show_ammo = false;

	OPTION_TABLE() {}
	OPTION_TABLE(INVOBJ_TYPES item) { inv_item = item; }
	OPTION_TABLE(INVOBJ_TYPES item, bool _equip, bool _combine, bool _separate, bool _use, bool _examine, bool _chooseammo, bool _showammo)
	{
		inv_item = item;
		equip = _equip;
		combine = _combine;
		separate = _separate;
		use = _use;
		examine = _examine;
		choose_ammo = _chooseammo;
		show_ammo = _showammo;
	}
};
