#include "pch.h"
#include "setup.h"
#include "objects.h"
#include "box.h"
#include "pickup.h"
#include "collide.h"
#include "init.h"
#include "elements.h"
#include "ahmet.h"
#include "effect2.h"
#include "effects.h"
#include "clockworkbeetle.h"
#include "traps.h"
#include "deathsld.h"
#include "specificfx.h"
#include "door.h"
#include "bike.h"
#include "jeep.h"
#include "voncroy.h"
#include "sphinx.h"
#include "laradouble.h"
#include "croc.h"
#include "demigod.h"
#include "wildboar.h"
#include "baboon.h"
#include "train.h"
#include "bat.h"
#include "jean.h"
#include "senet.h"
#include "wraith.h"
#include "harpy.h"
#include "rope.h"
#include "moveblok.h"
#include "objlight.h"
#include "laraflar.h"
#include "troops.h"
#include "mummy.h"
#include "laramisc.h"
#include "sas.h"
#include "hammerhead.h"
#include "tomb4fx.h"
#include "draw.h"
#include "hair.h"
#include "items.h"
#include "function_stubs.h"
#include "flmtorch.h"
#include "scarab.h"
#include "sentrygun.h"
#include "lara1gun.h"
#include "laradraw.h"
#include "switch.h"
#include "missile.h"
#include "dog.h"
#include "seth.h"
#include "raghead.h"
#include "templar.h"
#include "skeleton.h"
#include "scorpion.h"
#include "footprnt.h"
#include "guide.h"
#include "control.h"
#include "camera.h"
#include "horse.h"
#include "3dmath.h"
#include "lara.h"
#include "deltapak.h"
#include "gameflow.h"
#include "file.h"
#include "objects_register.h"

ObjectRegisterFactory factory;

static void DrawDummyItem(ITEM_INFO* item)
{
}

void ObjectObjects()
{
	factory.From(CAMERA_TARGET)->Draw(nullptr);
	factory.From(FLARE_ITEM)->Control(FlareControl)->Collision(PickUpCollision)->Draw(DrawFlareInAir)->Pivot(256)->HitPoints(256)->Save(true, false, true, false, false);
	factory.From(SMASH_OBJECT1)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT2)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT3)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT4)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT5)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT6)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT7)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(SMASH_OBJECT8)->Initialise(InitialiseSmashObject)->Collision(ObjectCollision)->Control(SmashObjectControl)->Save(false, false, true, true, false);
	factory.From(BRIDGE_FLAT)->Floor(BridgeFlatFloor)->Ceiling(BridgeFlatCeiling);
	factory.From(BRIDGE_TILT1)->Floor(BridgeTilt1Floor)->Ceiling(BridgeTilt1Ceiling);
	factory.From(BRIDGE_TILT2)->Floor(BridgeTilt2Floor)->Ceiling(BridgeTilt2Ceiling);
	factory.From(SWITCH_TYPE1)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SWITCH_TYPE2)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SWITCH_TYPE3)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SWITCH_TYPE4)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SWITCH_TYPE5)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SWITCH_TYPE6)->Control(SwitchControl)->Collision(SwitchCollision)->Save(false, false, true, true, false);
	factory.From(SEQUENCE_SWITCH1)->Control(FullBlockSwitchControl)->Collision(FullBlockSwitchCollision)->Save(false, false, true, true, false);
	factory.From(SEQUENCE_SWITCH2)->Control(FullBlockSwitchControl)->Collision(FullBlockSwitchCollision)->Save(false, false, true, true, false);
	factory.From(SEQUENCE_SWITCH3)->Control(FullBlockSwitchControl)->Collision(FullBlockSwitchCollision)->Save(false, false, true, true, false);
	factory.From(UNDERWATER_SWITCH1)->Control(SwitchControl)->Collision(SwitchCollision2)->Save(false, false, true, true, false);
	factory.From(UNDERWATER_SWITCH2)->Control(SwitchControl)->Collision(UnderwaterSwitchCollision)->Save(false, false, true, true, false);
	factory.From(TURN_SWITCH)->Control(TurnSwitchControl)->Collision(TurnSwitchCollision)->Save(false, false, true, true, false);
	factory.From(COG_SWITCH)->Control(CogSwitchControl)->Collision(CogSwitchCollision)->Save(false, false, true, true, false);
	factory.From(LEVER_SWITCH)->Control(SwitchControl)->Collision(RailSwitchCollision)->Save(false, false, true, true, false);
	factory.From(JUMP_SWITCH)->Control(SwitchControl)->Collision(JumpSwitchCollision)->Save(false, false, true, true, false);
	factory.From(CROWBAR_SWITCH)->Control(SwitchControl)->Collision(CrowbarSwitchCollision)->Save(false, false, true, true, false);
	factory.From(PULLEY)->Initialise(InitialisePulley)->Control(SwitchControl)->Collision(PulleyCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE1)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE2)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE3)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE4)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE5)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE6)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE7)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(DOOR_TYPE8)->Initialise(InitialiseDoor)->Control(DoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(UNDERWATER_DOOR)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(UnderwaterDoorCollision)->Save(false, false, true, true, false);
	factory.From(DOUBLE_DOORS)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(DoubleDoorCollision)->Save(false, false, true, true, false);
	factory.From(SEQUENCE_DOOR)->Initialise(InitialiseDoor)->Control(SequenceDoorControl)->Collision(DoorCollision)->Save(false, false, true, true, false);
	factory.From(PUSHPULL_DOOR1)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(PushPullKickDoorCollision)->Save(false, false, true, true, false);
	factory.From(PUSHPULL_DOOR2)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(PushPullKickDoorCollision)->Save(false, false, true, true, false);
	factory.From(KICK_DOOR1)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(PushPullKickDoorCollision)->Save(false, false, true, true, false);
	factory.From(KICK_DOOR2)->Initialise(InitialiseDoor)->Control(PushPullKickDoorControl)->Collision(PushPullKickDoorCollision)->Save(false, false, true, true, false);
	factory.From(FLOOR_TRAPDOOR1)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Collision(FloorTrapDoorCollision)->Save(false, false, true, true, false);
	factory.From(FLOOR_TRAPDOOR2)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Collision(FloorTrapDoorCollision)->Save(false, false, true, true, false);
	factory.From(CEILING_TRAPDOOR1)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Collision(CeilingTrapDoorCollision)->Save(false, false, true, true, false);
	factory.From(CEILING_TRAPDOOR2)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Collision(CeilingTrapDoorCollision)->Save(false, false, true, true, false);
	factory.From(TRAPDOOR1)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Save(false, false, true, true, false);
	factory.From(TRAPDOOR2)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Save(false, false, true, true, false);
	factory.From(TRAPDOOR3)->Initialise(InitialiseTrapDoor)->Control(TrapDoorControl)->Save(false, false, true, true, false);
	factory.From(PUZZLE_ITEM1)->PickupDefault();
	factory.From(PUZZLE_ITEM2)->PickupDefault();
	factory.From(PUZZLE_ITEM3)->PickupDefault();
	factory.From(PUZZLE_ITEM4)->PickupDefault();
	factory.From(PUZZLE_ITEM5)->PickupDefault();
	factory.From(PUZZLE_ITEM6)->PickupDefault();
	factory.From(PUZZLE_ITEM7)->PickupDefault();
	factory.From(PUZZLE_ITEM8)->PickupDefault();
	factory.From(PUZZLE_ITEM9)->PickupDefault();
	factory.From(PUZZLE_ITEM10)->PickupDefault();
	factory.From(PUZZLE_ITEM11)->PickupDefault();
	factory.From(PUZZLE_ITEM12)->PickupDefault();
	factory.From(PUZZLE_ITEM1_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM1_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM2_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM2_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM3_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM3_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM4_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM4_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM5_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM5_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM6_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM6_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM7_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM7_COMBO2)->PickupDefault();
	factory.From(PUZZLE_ITEM8_COMBO1)->PickupDefault();
	factory.From(PUZZLE_ITEM8_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM1)->PickupDefault();
	factory.From(KEY_ITEM2)->PickupDefault();
	factory.From(KEY_ITEM3)->PickupDefault();
	factory.From(KEY_ITEM4)->PickupDefault();
	factory.From(KEY_ITEM5)->PickupDefault();
	factory.From(KEY_ITEM6)->PickupDefault();
	factory.From(KEY_ITEM7)->PickupDefault();
	factory.From(KEY_ITEM8)->PickupDefault();
	factory.From(KEY_ITEM9)->PickupDefault();
	factory.From(KEY_ITEM10)->PickupDefault();
	factory.From(KEY_ITEM11)->PickupDefault();
	factory.From(KEY_ITEM12)->PickupDefault();
	factory.From(KEY_ITEM1_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM1_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM2_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM2_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM3_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM3_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM4_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM4_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM5_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM5_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM6_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM6_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM7_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM7_COMBO2)->PickupDefault();
	factory.From(KEY_ITEM8_COMBO1)->PickupDefault();
	factory.From(KEY_ITEM8_COMBO2)->PickupDefault();
	factory.From(PICKUP_ITEM1)->PickupDefault();
	factory.From(PICKUP_ITEM2)->PickupDefault();
	factory.From(PICKUP_ITEM3)->PickupDefault();
	factory.From(PICKUP_ITEM4)->PickupDefault();
	factory.From(PICKUP_ITEM1_COMBO1)->PickupDefault();
	factory.From(PICKUP_ITEM1_COMBO2)->PickupDefault();
	factory.From(PICKUP_ITEM2_COMBO1)->PickupDefault();
	factory.From(PICKUP_ITEM2_COMBO2)->PickupDefault();
	factory.From(PICKUP_ITEM3_COMBO1)->PickupDefault();
	factory.From(PICKUP_ITEM3_COMBO2)->PickupDefault();
	factory.From(PICKUP_ITEM4_COMBO1)->PickupDefault();
	factory.From(PICKUP_ITEM4_COMBO2)->PickupDefault();
	factory.From(EXAMINE1)->PickupDefault();
	factory.From(EXAMINE2)->PickupDefault();
	factory.From(EXAMINE3)->PickupDefault();
	factory.From(CROWBAR_ITEM)->PickupDefault();
	factory.From(BURNING_TORCH_ITEM)->PickupDefault();
	factory.From(CLOCKWORK_BEETLE)->PickupDefault();
	factory.From(CLOCKWORK_BEETLE_COMBO1)->PickupDefault();
	factory.From(CLOCKWORK_BEETLE_COMBO2)->PickupDefault();
	factory.From(MINE_DETECTOR)->PickupDefault();
	factory.From(QUEST_ITEM1)->PickupDefault();
	factory.From(QUEST_ITEM2)->PickupDefault();
	factory.From(QUEST_ITEM3)->PickupDefault();
	factory.From(QUEST_ITEM4)->PickupDefault();
	factory.From(QUEST_ITEM5)->PickupDefault();
	factory.From(QUEST_ITEM6)->PickupDefault();
	factory.From(MAP)->PickupDefault();
	factory.From(SECRET_MAP)->PickupDefault();
	factory.From(PISTOLS_ITEM)->PickupDefault();
	factory.From(PISTOLS_AMMO_ITEM)->PickupDefault();
	factory.From(UZI_ITEM)->PickupDefault();
	factory.From(UZI_AMMO_ITEM)->PickupDefault();
	factory.From(SHOTGUN_ITEM)->PickupDefault();
	factory.From(SHOTGUN_AMMO1_ITEM)->PickupDefault();
	factory.From(SHOTGUN_AMMO2_ITEM)->PickupDefault();
	factory.From(CROSSBOW_ITEM)->PickupDefault();
	factory.From(CROSSBOW_AMMO1_ITEM)->PickupDefault();
	factory.From(CROSSBOW_AMMO2_ITEM)->PickupDefault();
	factory.From(CROSSBOW_AMMO3_ITEM)->PickupDefault();
	factory.From(GRENADE_GUN_ITEM)->PickupDefault();
	factory.From(GRENADE_GUN_AMMO1_ITEM)->PickupDefault();
	factory.From(GRENADE_GUN_AMMO2_ITEM)->PickupDefault();
	factory.From(GRENADE_GUN_AMMO3_ITEM)->PickupDefault();
	factory.From(SIXSHOOTER_ITEM)->PickupDefault();
	factory.From(SIXSHOOTER_AMMO_ITEM)->PickupDefault();
	factory.From(BIGMEDI_ITEM)->PickupDefault();
	factory.From(SMALLMEDI_ITEM)->PickupDefault();
	factory.From(LASERSIGHT_ITEM)->PickupDefault();
	factory.From(BINOCULARS_ITEM)->PickupDefault();
	factory.From(FLARE_INV_ITEM)->PickupDefault();
	factory.From(COMPASS_ITEM)->PickupDefault();
	factory.From(WATERSKIN1_EMPTY)->PickupDefault();
	factory.From(WATERSKIN2_EMPTY)->PickupDefault();
	factory.From(BURNING_TORCH_ITEM)->Control(FlameTorchControl)->Save(true, false, true, false, false);
	factory.From(CROSSBOW_BOLT)->Control(ControlCrossbow)->Draw(DrawWeaponMissile)->Save(true, false, false, false, false);
	factory.From(GRENADE)->Control(ControlGrenade)->Draw(DrawWeaponMissile)->Save(true, false, false, false, false);
	factory.From(KEY_HOLE1)->KeyHoleDefault();
	factory.From(KEY_HOLE2)->KeyHoleDefault();
	factory.From(KEY_HOLE3)->KeyHoleDefault();
	factory.From(KEY_HOLE4)->KeyHoleDefault();
	factory.From(KEY_HOLE5)->KeyHoleDefault();
	factory.From(KEY_HOLE6)->KeyHoleDefault();
	factory.From(KEY_HOLE7)->KeyHoleDefault();
	factory.From(KEY_HOLE8)->KeyHoleDefault();
	factory.From(KEY_HOLE9)->KeyHoleDefault();
	factory.From(KEY_HOLE10)->KeyHoleDefault();
	factory.From(KEY_HOLE11)->KeyHoleDefault();
	factory.From(KEY_HOLE12)->KeyHoleDefault();
	factory.From(PUZZLE_HOLE1)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE2)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE3)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE4)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE5)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE6)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE7)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE8)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE9)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE10)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE11)->PuzzleHoleDefault();
	factory.From(PUZZLE_HOLE12)->PuzzleHoleDefault();
	factory.From(PUZZLE_DONE1)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE2)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE3)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE4)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE5)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE6)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE7)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE8)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE9)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE10)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE11)->PuzzleDoneDefault();
	factory.From(PUZZLE_DONE12)->PuzzleDoneDefault();
	factory.From(SARCOPHAGUS)->Control(ControlAnimatingSlots)->Collision(SarcophagusCollision)->Save(false, false, true, true, false);
	factory.From(ANIMATING1)->AnimatingDefault();
	factory.From(ANIMATING2)->AnimatingDefault();
	factory.From(ANIMATING3)->AnimatingDefault();
	factory.From(ANIMATING4)->AnimatingDefault();
	factory.From(ANIMATING5)->AnimatingDefault();
	factory.From(ANIMATING6)->AnimatingDefault();
	factory.From(ANIMATING7)->AnimatingDefault();
	factory.From(ANIMATING8)->AnimatingDefault();
	factory.From(ANIMATING9)->AnimatingDefault();
	factory.From(ANIMATING10)->AnimatingDefault();
	factory.From(ANIMATING11)->AnimatingDefault();
	factory.From(ANIMATING12)->AnimatingDefault();
	factory.From(ANIMATING13)->AnimatingDefault(false);
	factory.From(ANIMATING14)->AnimatingDefault(false);
	factory.From(ANIMATING15)->AnimatingDefault(false);
	factory.From(ANIMATING16)->AnimatingDefault(false);
	factory.From(FIREROPE)->Control(ControlBurningRope)->Collision(BurningRopeCollision)->Save(false, false, true, true, false);
	factory.From(EXPANDING_PLATFORM)->Initialise(InitialiseRaisingBlock)->Control(ControlRaisingBlock)->Draw(DrawScaledSpike)->Save(false, false, true, false, false);
	factory.From(SQUISHY_BLOCK1)->Control(ControlLRSquishyBlock)->Collision(ObjectCollision)->Save(false, false, true, true, false);
	factory.From(SQUISHY_BLOCK2)->Control(ControlFallingSquishyBlock)->Collision(FallingSquishyBlockCollision)->Save(false, false, true, true, false);
	factory.From(MAPPER)->Initialise(InitialiseMapper)->Control(ControlMapper)->Save(false, false, true, true, false);
	factory.From(OBELISK)->Initialise(InitialiseObelisk)->Control(ControlObelisk)->Collision(ObjectCollision)->Save(true, false, true, false, false);
	factory.From(ELEMENT_PUZZLE)->Initialise(InitialiseElementPuzzle)->Control(ControlElementPuzzle)->Collision(ElementPuzzleCollision)->Save(false, false, true, false, true);
	factory.From(STATUE_PLINTH)->Initialise(InitialiseStatuePlinth)->Collision(StatuePlinthCollision)->Save(false, false, true, false, true);
	factory.From(SHOOT_SWITCH1)->Control(ControlAnimatingSlots)->Collision(SwitchType78Collision)->Save(false, false, true, true, true);
	factory.From(SHOOT_SWITCH2)->Control(ControlAnimatingSlots)->Collision(SwitchType78Collision)->Save(false, false, true, true, false);
	factory.From(SCALES)->Control(ScalesControl)->Collision(ScalesCollision)->Save(false, false, true, true, false);
	factory.From(TEETH_SPIKES)->Initialise(InitialiseScaledSpike)->Control(ControlScaledSpike)->Draw(DrawScaledSpike)->Save(false, false, true, false, false);
	factory.From(JOBY_SPIKES)->Initialise(InitialiseJobySpike)->Control(ControlJobySpike)->Draw(DrawScaledSpike)->Save(false, false, true, false, false);
	factory.From(SLICER_DICER)->Initialise(InitialiseSlicerDicer)->Control(ControlSlicerDicer)->Collision(GenericDeadlyBoundingBoxCollision)->Save(false, false, true, false, false);
	factory.From(SARCOPHAGUS_CUT)->Save(false, false, false, false, true);
	factory.From(HORUS_STATUE)->Collision(ObjectCollision)->Save(false, false, false, false, true);
	factory.From(RAISING_BLOCK1)->Initialise(InitialiseRaisingBlock)->Control(ControlRaisingBlock)->Draw(DrawScaledSpike)->Save(false, false, true, false, false);
	factory.From(RAISING_BLOCK2)->Initialise(InitialiseRaisingBlock)->Control(ControlRaisingBlock)->Draw(DrawScaledSpike)->Save(false, false, true, false, false);
	factory.From(SMOKE_EMITTER_WHITE)->Initialise(InitialiseSmokeEmitter)->Control(ControlSmokeEmitter)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(SMOKE_EMITTER_BLACK)->Initialise(InitialiseSmokeEmitter)->Control(ControlSmokeEmitter)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(STEAM_EMITTER)->Initialise(InitialiseSmokeEmitter)->Control(ControlSmokeEmitter)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(RED_LIGHT)->Control(ControlColouredLights)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(GREEN_LIGHT)->Control(ControlColouredLights)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(BLUE_LIGHT)->Control(ControlColouredLights)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(AMBER_LIGHT)->Control(ControlPulseLight)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(WHITE_LIGHT)->Control(ControlElectricalLight)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(BLINKING_LIGHT)->Control(ControlBlinker)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(LIGHTNING_CONDUCTOR)->Initialise(InitialiseLightningConductor)->Control(ControlLightningConductor)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(BUBBLES)->LoadedByDefault()->Control(ControlEnemyMissile)->Draw(DrawDummyItem)->NoMeshes()->Save(false, false, true, false, false);
	factory.From(WATERFALLMIST)->Control(WaterFall)->Draw(nullptr);
	factory.From(LENS_FLARE)->Draw(DrawLensFlares);
	factory.From(WATERFALL1)->Control(ControlWaterfall)->Save(false, false, true, false, false);
	factory.From(WATERFALL2)->Control(ControlWaterfall)->Save(false, false, true, false, false);
	factory.From(WATERFALL3)->Control(ControlWaterfall)->Save(false, false, true, false, false);
	factory.From(CLOCKWORK_BEETLE)->Control(ControlClockworkBeetle);
	factory.From(GOD_HEAD)->Control(ControlGodHead)->Draw(DrawGodHead)->Save(false, false, true, false, false);
	factory.From(EARTHQUAKE)->Control(EarthQuake)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(BODY_PART)->LoadedByDefault()->Control(ControlBodyPart)->Draw(DrawDummyItem)->NoMeshes();
}

void TrapObjects()
{
	factory.From(ROLLINGBALL)->Control(ControlRollingBall)->Collision(RollingBallCollision)->Save(true, false, true, false, false);
	factory.From(CHAIN)->Control(ControlChain)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(CHAIN)->Control(ControlPlough)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(STARGATE)->Control(ControlStargate)->Collision(StargateCollision)->Save(false, false, true, true, false);
	factory.From(HAMMER)->Control(ControlHammer)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(BURNING_FLOOR)->Initialise(InitialiseBurningFloor)->Control(ControlHammer)->Save(false, false, true, true, false);
	factory.From(COG)->Control(ControlAnimatingSlots)->Collision(CogCollision)->Save(false, false, true, true, false);
	factory.From(SPIKEBALL)->Control(ControlSpikeball)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(TWOBLOCK_PLATFORM)->Initialise(InitialiseTwoBlockPlatform)->Control(ControlTwoBlockPlatform)->Floor(TwoBlockPlatformFloor)->Ceiling(TwoBlockPlatformCeiling)->Save(true, false, true, false, false);
	factory.From(FLOOR_4BLADE)->Control(Control4xFloorRoofBlade)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(ROOF_4BLADE)->Control(Control4xFloorRoofBlade)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(BIRD_BLADE)->Control(ControlBirdBlade)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(CATWALK_BLADE)->Control(ControlCatwalkBlade)->Collision(GenericDeadlyBoundingBoxCollision)->Save(false, false, true, true, false);
	factory.From(MOVING_BLADE)->Control(ControlMovingBlade)->Collision(GenericDeadlyBoundingBoxCollision)->Save(false, false, true, true, false);
	factory.From(PLINTH_BLADE)->Control(ControlPlinthBlade)->Collision(GenericDeadlyBoundingBoxCollision)->Save(false, false, true, true, false);
	factory.From(SETH_BLADE)->Initialise(InitialiseSethBlade)->Control(ControlSethBlade)->Collision(GenericSphereBoxCollision)->Save(false, false, true, true, false);
	factory.From(KILL_ALL_TRIGGERS)->Control(KillAllCurrentItems)->Draw(nullptr)->Save(true, false, true, true, false);
	factory.From(FALLING_BLOCK)->Initialise(InitialiseFallingBlock2)->Control(FallingBlock)->Collision(FallingBlockCollision)->Floor(FallingBlockFloor)->Ceiling(FallingBlockCeiling)->Save(true, false, true, true, false);
	factory.From(FALLING_BLOCK2)->Initialise(InitialiseFallingBlock2)->Control(FallingBlock)->Collision(FallingBlockCollision)->Floor(FallingBlockFloor)->Ceiling(FallingBlockCeiling)->Save(true, false, true, true, false);
	factory.From(FALLING_CEILING)->Control(FallingCeiling)->Collision(TrapCollision)->Save(true, false, true, true, false);
	factory.From(SMASHABLE_BIKE_WALL)->Control(InitialiseFallingBlock2)->Control(ControlSmashableBikeWall)->Collision(ObjectCollision)->Save(false, false, true, false, false);
	factory.From(SMASHABLE_BIKE_FLOOR)->Control(InitialiseFallingBlock2)->Control(ControlFallingBlock2)->Collision(ObjectCollision)->Floor(TwoBlockPlatformFloor)->Ceiling(TwoBlockPlatformCeiling)->Save(true, false, true, true, false);
	factory.From(PUSHABLE_OBJECT1)->Initialise(InitialiseMovingBlock)->Control(MovableBlock)->Collision(MovableBlockCollision)->Save(true, false, true, false, false);
	factory.From(PUSHABLE_OBJECT2)->Initialise(InitialiseMovingBlock)->Control(MovableBlock)->Collision(MovableBlockCollision)->Save(true, false, true, false, false);
	factory.From(PUSHABLE_OBJECT3)->Initialise(InitialiseMovingBlock)->Control(MovableBlock)->Collision(MovableBlockCollision)->Save(true, false, true, false, false);
	factory.From(PUSHABLE_OBJECT4)->Initialise(InitialiseMovingBlock)->Control(MovableBlock)->Collision(MovableBlockCollision)->Save(true, false, true, false, false);
	factory.From(PUSHABLE_OBJECT5)->Initialise(InitialiseMovingBlock)->Control(MovableBlock)->Collision(MovableBlockCollision)->Save(true, false, true, false, false);
	factory.From(SAS_DRAG_BLOKE)->Control(ControlAnimatingSlots)->Collision(DragSASCollision)->Save(true, false, true, true, false);
	factory.From(DART)->Control(DartsControl)->Collision(ObjectCollision)->Draw(S_DrawDarts)->Shadow(128)->Save(true, false, true, true, false);
	factory.From(DART_EMITTER)->Control(DartEmitterControl)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(HOMING_DART_EMITTER)->Control(DartEmitterControl)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(FLAME)->Control(FlameControl)->Draw(nullptr);
	factory.From(FLAME_EMITTER)->Initialise(InitialiseFlameEmitter)->Control(FlameEmitterControl)->Collision(FireCollision)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(FLAME_EMITTER2)->Initialise(InitialiseFlameEmitter2)->Control(FlameEmitter2Control)->Collision(FireCollision)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(FLAME_EMITTER3)->Initialise(InitialiseFlameEmitter3)->Control(FlameEmitter3Control)->Draw(nullptr)->Save(false, false, true, false, false);
	init_all_ropes();
	factory.From(ROPE)->Initialise(InitialiseRope)->Control(RopeControl)->Collision(RopeCollision)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(POLEROPE)->Collision(PoleCollision)->Save(false, false, true, false, false);
	factory.From(MINE)->Initialise(InitialiseMineHelicopter)->Control(ControlMineHelicopter)->Collision(MineCollision)->HitEffect(3);
	factory.From(SPRINKLER)->Control(ControlSprinkler)->Collision(FireCollision)->Save(false, false, true, false, false);
	factory.From(TRIGGER_TRIGGERER)->Initialise(ControlTriggerTriggerer)->Control(ControlTriggerTriggerer)->Draw(nullptr)->Save(false, false, true, false, false);
	factory.From(PLANET_EFFECT)->Initialise(InitialisePlanetEffect)->Control(ControlPlanetEffect)->Draw(DrawPlanetEffect)->Save(false, false, true, false, true);
}

void BaddyObjects()
{
	factory.From(LARA)->Initialise(InitialiseLaraLoad)->Draw(nullptr)->Shadow(160)->HitPoints(LARA_HITPOINTS)->SaveDefault();
	factory.From(LARA_DOUBLE)->CreatureDefault()->Initialise(InitialiseLaraDouble)->Control(LaraDoubleControl)->HitPoints(LARA_HITPOINTS)->Pivot(50)->HitEffect(3)->NonLot();
	factory.From(MOTORBIKE)->Initialise(InitialiseBike)->Control(BikeControl)->Collision(BikeCollision)->DrawExtra(DrawBikeExtras)->Bone(1, BN_ROT_X)->Bone(3, BN_ROT_X)->Bone(7, BN_ROT_X)->SaveDefault();
	factory.From(JEEP)->Initialise(InitialiseJeep)->Control(JeepControl)->Collision(JeepCollision)->DrawExtra(DrawJeepExtras)->Bone(8, BN_ROT_X)->Bone(9, BN_ROT_X)->Bone(11, BN_ROT_X)->Bone(12, BN_ROT_X)->SaveDefault();
	factory.From(SKELETON)->CreatureDefault(true)->Initialise(InitialiseSkeleton)->Control(SkeletonControl)->HitPoints(15)->Pivot(50)->Undead()->HitEffect(2)->ExplosionMesh(9)->ExplosionMesh(11); // Head, Shield
	factory.From(VON_CROY)->CreatureDefault(true)->Initialise(InitialiseVoncroy)->Control(gfCurrentLevel != 1 ? VoncroyRaceControl : VoncroyControl)->HitPoints(NOT_TARGETABLE)->HitEffect(2)->Bone(6, BN_ROT_X | BN_ROT_Y)->Bone(20, BN_ROT_X | BN_ROT_Y)->MeshFrom(15, MESHSWAP1)->MeshFrom(31, MESHSWAP1)->MeshFrom(37, MESHSWAP1);
	factory.From(GUIDE)->CreatureDefault(true)->Initialise(InitialiseGuide)->Control(GuideControl)->HitPoints(NOT_TARGETABLE)->HitEffect(2)->Bone(6, BN_ROT_X | BN_ROT_Y)->Bone(20, BN_ROT_X | BN_ROT_Y)->MeshFrom(31, MESHSWAP2)->MeshFrom(37, MESHSWAP2)->MeshFrom(45, MESHSWAP2);
	factory.From(RAGHEAD)->CreatureDefault(true)->Initialise(InitialiseRaghead)->Control(RagheadControl)->HitPoints(25)->Pivot(50)->Bone(7, BN_ROT_X | BN_ROT_Y)->Bone(22, BN_ROT_X | BN_ROT_Y)->MeshFrom(9, MESHSWAP3)->MeshFrom(15, MESHSWAP3);
	factory.From(SUPER_RAGHEAD)->CreatureDefault(true)->Initialise(InitialiseRaghead)->Control(RagheadControl)->HitPoints(35)->Pivot(50)->Bone(7, BN_ROT_X | BN_ROT_Y)->Bone(22, BN_ROT_X | BN_ROT_Y)->MeshFrom(9, MESHSWAP2)->MeshFrom(15, MESHSWAP2);
	factory.From(SCORPION)->CreatureDefault()->Initialise(InitialiseScorpion)->Control(ScorpionControl)->HitPoints(80)->Radius(512);
	factory.From(SMALL_SCORPION)->CreatureDefault()->Initialise(InitialiseSmallScorpion)->Control(SmallScorpionControl)->HitPoints(8)->Pivot(20);
	factory.From(MUMMY)->CreatureDefault()->Initialise(InitialiseMummy)->Control(MummyControl)->HitPoints(15)->Undead()->HitEffect(2)->Radius(170)->Pivot(50)->Bone(7, BN_ROT_X | BN_ROT_Y)->Bone(18, BN_ROT_Y);
	factory.From(KNIGHTS_TEMPLAR)->CreatureDefault()->Initialise(InitialiseTemplar)->Control(TemplarControl)->HitPoints(15)->Undead()->Pivot(50)->HitEffect(2)->Bone(6, BN_ROT_X | BN_ROT_Y)->Bone(14, BN_ROT_Y);
	factory.From(SPHINX)->CreatureDefault()->Initialise(InitialiseSphinx)->Control(SphinxControl)->HitPoints(1000)->Undead()->Pivot(500)->Radius(512)->HitEffect(3);
	factory.From(SETHA)->CreatureDefault()->Initialise(InitialiseSeth)->Control(SethControl)->HitPoints(500)->Undead()->Pivot(50)->Radius(341);
	factory.From(HORSEMAN)->CreatureDefault(true)->Initialise(InitialiseHorseman)->Control(HorsemanControl)->HitPoints(25)->Undead()->HitEffect(3)->Radius(409);
	factory.From(HAMMERHEAD)->CreatureDefault()->Initialise(InitialiseHammerhead)->Control(HammerheadControl)->HitPoints(45)->WaterCreature()->Radius(341)->Pivot(300)->Bone(0, BN_ROT_Y)->Bone(1, BN_ROT_Y)->Bone(2, BN_ROT_Y)->Bone(9, BN_ROT_Y);
	factory.From(CROCODILE)->CreatureDefault()->Initialise(InitialiseCroc)->Control(CrocControl)->HitPoints(36)->Pivot(300)->Radius(409)->WaterCreature()->Bone(0, BN_ROT_Y)->Bone(7, BN_ROT_Y)->Bone(9, BN_ROT_Y)->Bone(9, BN_ROT_Y);
	factory.From(DEMIGOD1)->CreatureDefault()->Initialise(InitialiseDemigod)->Control(DemigodControl)->HitPoints(200)->Undead()->Pivot(50)->Radius(341)->HitEffect(3)->Bone(8, BN_ROT_X | BN_ROT_Y | BN_ROT_Z)->Bone(20, BN_ROT_Y);
	factory.From(DEMIGOD2)->CreatureDefault()->Initialise(InitialiseDemigod)->Control(DemigodControl)->HitPoints(200)->Pivot(50)->Radius(341)->Bone(8, BN_ROT_X | BN_ROT_Y | BN_ROT_Z)->Bone(20, BN_ROT_Y);
	factory.From(DEMIGOD3)->CreatureDefault()->Initialise(InitialiseDemigod)->Control(DemigodControl)->HitPoints(200)->Pivot(50)->Radius(341)->Bone(8, BN_ROT_X | BN_ROT_Y | BN_ROT_Z)->Bone(20, BN_ROT_Y);
	factory.From(MUTANT)->CreatureDefault()->Initialise(InitialiseCrocgod)->Control(CrocgodControl)->HitPoints(15)->Undead()->Pivot(50)->HitEffect(3)->Bone(6, BN_ROT_X | BN_ROT_Y)->Bone(7, BN_ROT_X | BN_ROT_Y);
	factory.From(TROOPS)->CreatureDefault()->Initialise(InitialiseTroop)->Control(TroopControl)->HitPoints(40)->Pivot(50)->Bone(0, BN_ROT_X | BN_ROT_Y)->Bone(7, BN_ROT_X | BN_ROT_Y);
	factory.From(SAS)->CreatureDefault()->Initialise(InitialiseSas)->Control(SasControl)->HitPoints(40)->Pivot(50)->Bone(0, BN_ROT_X | BN_ROT_Y)->Bone(7, BN_ROT_X | BN_ROT_Y);
	factory.From(HARPY)->CreatureDefault()->Initialise(InitialiseHarpy)->Control(HarpyControl)->HitPoints(60)->Pivot(50)->Radius(409);
	factory.From(WILD_BOAR)->CreatureDefault()->Initialise(InitialiseWildboar)->Control(WildboarControl)->HitPoints(40)->Pivot(50)->Bone(12, BN_ROT_Y | BN_ROT_Z)->Bone(13, BN_ROT_Y | BN_ROT_Z);
	factory.From(DOG)->CreatureDefault()->Initialise(InitialiseDog)->Control(DogControl)->HitPoints(16)->Pivot(300)->Radius(341)->Bone(0, BN_ROT_Y)->Bone(2, BN_ROT_X | BN_ROT_Y);
	factory.From(AHMET)->CreatureDefault()->Initialise(InitialiseAhmet)->Control(AhmetControl)->HitPoints(80)->Pivot(300)->Radius(341)->Bone(9, BN_ROT_Y);
	factory.From(BABOON_NORMAL)->CreatureDefault()->Initialise(InitialiseBaboon)->Control(BaboonControl)->HitPoints(30)->Pivot(200)->Radius(256);
	factory.From(BABOON_INV)->CreatureDefault()->Initialise(InitialiseBaboon)->Control(BaboonControl)->HitPoints(30)->Pivot(200)->Radius(256)->AnimIndexFromObject(BABOON_NORMAL);
	factory.From(BABOON_SILENT)->CreatureDefault()->Initialise(InitialiseBaboon)->Control(BaboonControl)->HitPoints(30)->Pivot(200)->Radius(256)->AnimIndexFromObject(BABOON_NORMAL);
	if (gfLevelFlags & GF_TRAIN)
		factory.From(ENEMY_JEEP)->CreatureDefault()->Initialise(InitialiseTrainJeep)->Control(TrainJeepControl);
	else
		factory.From(ENEMY_JEEP)->CreatureDefault()->Initialise(InitialiseEnemyJeep)->Control(EnemyJeepControl)->HitPoints(40)->Undead()->HitEffect(2)->Radius(512)->Pivot(500)->Bone(8, BN_ROT_X)->Bone(9, BN_ROT_X)->Bone(11, BN_ROT_X)->Bone(12, BN_ROT_X);
	factory.From(BAT)->CreatureDefault()->Initialise(InitialiseBat)->Control(BatControl)->HitPoints(5)->Pivot(10);
	factory.From(BIG_BEETLE)->CreatureDefault()->Initialise(InitialiseScarab)->Control(ScarabControl)->HitPoints(30)->Pivot(50)->Radius(204);
	factory.From(SENTRY_GUN)->CreatureDefault()->Initialise(InitialiseAutogun)->Control(AutogunControl)->HitPoints(30)->Undead()->Radius(204)->Pivot(50)->NonLot()->HitEffect(3)->ExplosionMesh(6)->Bone(0, BN_ROT_Y)->Bone(1, BN_ROT_X)->Bone(2, BN_ROT_Z)->Bone(3, BN_ROT_Z);
	factory.From(HORSE)->Initialise(InitialiseHorse)->Collision(ObjectCollision)->Save(false, false, true, true, false);
	factory.From(SAS_DYING)->Initialise(InitialiseInjuredSas)->Control(InjuredSasControl)->Collision(ObjectCollision)->Save(true, false, true, true, false);
	factory.From(SAS_DYING_MIP)->Initialise(InitialiseInjuredSas)->Control(InjuredSasControl)->Collision(ObjectCollision)->Save(true, false, true, true, false);
	factory.From(JEAN_YVES)->Initialise(InitialiseJeanYves)->Control(JeanYvesControl)->Collision(ObjectCollision)->Save(true, false, true, true, false);
	factory.From(GAME_PIECE1)->Initialise(InitialiseSenet)->Control(SenetControl)->Collision(ObjectCollision)->Save(true, true, true, false, false);
	factory.From(GAME_PIECE2)->Initialise(InitialiseSenet)->Control(SenetControl)->Collision(ObjectCollision)->Save(true, true, true, false, false);
	factory.From(GAME_PIECE3)->Initialise(InitialiseSenet)->Control(SenetControl)->Collision(ObjectCollision)->Save(true, true, true, false, false);
	factory.From(ENEMY_PIECE)->Collision(ObjectCollision)->Save(true, false, true, false, false);
	factory.From(WHEEL_OF_FORTUNE)->Initialise(InitialiseGameStix)->Control(GameStixControl)->Collision(GameStixCollision)->HitPoints(1)->Save(false, false, true, true, false)->Bone(0, BN_ROT_Z)->Bone(1, BN_ROT_Z)->Bone(2, BN_ROT_Z)->Bone(3, BN_ROT_Z);
	factory.From(WRAITH1)->Initialise(InitialiseWraith)->Control(WraithControl)->DrawExtra(DrawWraithTrail)->SaveDefault();
	factory.From(WRAITH2)->Initialise(InitialiseWraith)->Control(WraithControl)->DrawExtra(DrawWraithTrail)->SaveDefault();
	factory.From(WRAITH3)->Initialise(InitialiseWraith)->Control(WraithControl)->DrawExtra(DrawWraithTrail)->SaveDefault();
	factory.From(WRAITH4)->Initialise(InitialiseWraith)->Control(WraithControl)->DrawExtra(DrawWraithTrail)->SaveDefault();
	factory.From(LITTLE_BEETLE)->Initialise(InitialiseScarabGenerator)->Control(TriggerScarab)->Draw(nullptr);
	factory.From(LOCUST_EMITTER)->Initialise(InitialiseLocustEmitter)->Control(ControlLocustEmitter)->Draw(nullptr)->Save(false, false, true, false, false);
}

void BuildOutsideTable()
{
	ROOM_INFO* r;
	uchar* pTable;
	uchar* oTable;
	uchar* cTable;
	long max_slots, roomx, roomy, cont, offset, z, z2;
	char flipped[256];

	max_slots = 0;
	OutsideRoomOffsets = (short*)malloc(0x5B2);
	OutsideRoomTable = (char*)malloc(0xB640);
	memset(OutsideRoomTable, 0xFF, 0xB640);
	memset(flipped, 0, 255);

	for (int i = 0; i < number_rooms; i++)
	{
		r = &room[i];

		if (r->flipped_room != -1)
			flipped[r->flipped_room] = 1;
	}

	r = &room[0];
	printf("X %d, Y %d, Z %d, Xs %d, Ys %d\n", r->x, r->y, r->z, r->x_size, r->y_size);

	for (int y = 0; y < 108; y += 4)
	{
		for (int x = 0; x < 108; x += 4)
		{
			for (int i = 0; i < number_rooms; i++)
			{
				r = &room[i];

				if (flipped[i])
					continue;

				roomx = (r->z >> 10) + 1;
				roomy = (r->x >> 10) + 1;
				cont = 0;

				for (int ry = 0; ry < 4; ry++)
				{
					for (int rx = 0; rx < 4; rx++)
					{
						if (x + rx >= roomx && x + rx < roomx + r->x_size - 2 && y + ry >= roomy && y + ry < roomy + r->y_size - 2)
						{
							cont = 1;
							break;
						}
					}
				}

				if (cont)
				{
					pTable = (uchar*)&OutsideRoomTable[1728 * (y >> 2) + 64 * (x >> 2)];

					for (int j = 0; j < 64; j++)
					{
						if (pTable[j] == 255)
						{
							pTable[j] = i;

							if (j > max_slots)
								max_slots = j;

							break;
						}
					}
				}
			}
		}
	}

	oTable = (uchar*)OutsideRoomTable;

	for (int y = 0; y < 27; y++)
	{
		for (int x = 0; x < 27; x++)
		{
			z = 0;
			offset = x + 27 * y;
			pTable = (uchar*)&OutsideRoomTable[1728 * y + 64 * x];

			while (pTable[z] != 255) z++;

			if (!z)
				OutsideRoomOffsets[offset] = -1;
			else if (z == 1)
				OutsideRoomOffsets[offset] = *pTable | 0x8000;
			else
			{
				cTable = (uchar*)OutsideRoomTable;

				while (cTable < oTable)
				{
					if (!memcmp(cTable, pTable, z))
					{
						OutsideRoomOffsets[offset] = short((long)cTable - (long)OutsideRoomTable);
						break;
					}

					z2 = 0;

					while (cTable[z2] != 255) z2++;

					cTable += z2 + 1;
				}

				if (cTable >= oTable)
				{
					OutsideRoomOffsets[offset] = short((long)oTable - (long)OutsideRoomTable);
					memcpy(oTable, pTable, z);
					oTable += z;
					*oTable++ = -1;
				}
			}
		}
	}

	printf("Ouside room table = %d bytes, max_slots = %d\n", oTable - (uchar*)OutsideRoomTable, max_slots);
}

void reset_cutseq_vars()
{
	cutseq_num = 0;
	cutseq_trig = 0;
	GLOBAL_playing_cutseq = 0;
	GLOBAL_cutseq_frame = 0;
	SetFadeClip(0, 1);
}

void ClearFootPrints()
{
	for (int i = 0; i < 32; i++)
		FootPrint[i].Active = 0;

	FootPrintNum = 0;
}

void InitialiseGameFlags()
{
	memset(flipmap, 0, sizeof(flipmap));
	memset(flip_stats, 0, sizeof(flip_stats));
	memset(cd_flags, 0, sizeof(cd_flags));
	flipeffect = -1;
	flip_status = 0;
	IsAtmospherePlaying = 0;
	camera.underwater = 0;
}

void InitialiseLara()
{
	short item_num, gun;

	if (lara.item_number == NO_ITEM)
		return;

	lara_item->data = &lara;
	lara_item->collidable = 0;
	item_num = lara.item_number;
	memset(&lara, 0, sizeof(LARA_INFO));
	lara.look = 1;
	lara.item_number = item_num;
	lara.hit_direction = -1;
	lara.air = 1800;
	lara.vehicle = NO_ITEM;
	lara.weapon_item = NO_ITEM;
	lara.water_surface_dist = 100;
	lara.holster_l = LARA_HOLSTERS_PISTOLS;
	lara.holster_r = LARA_HOLSTERS_PISTOLS;
	lara.location = -1;
	lara.highest_location = -1;
	lara.RopePtr = -1;
	lara_item->hit_points = 1000;
	lara.gun_status = LG_NO_ARMS;

	if (gfLevelFlags & GF_YOUNGLARA)
		gun = WEAPON_NONE;
	else
		gun = WEAPON_PISTOLS;
	
	lara.last_gun_type = gun;
	lara.gun_type = gun;
	lara.request_gun_type = gun;
	LaraInitialiseMeshes();
	lara.pistols_type_carried = W_PRESENT | W_AMMO1;
	lara.binoculars = 1;
	lara.num_flares = 3;
	lara.num_small_medipack = 3;
	lara.num_large_medipack = 1;
	lara.num_pistols_ammo = -1;

	if (Gameflow->DemoDisc)
		lara.crowbar = 1;

	lara.beetle_uses = 3;
	InitialiseLaraAnims(lara_item);
	DashTimer = 120;
}

void InitialiseObjects()
{
	for (int i = 0; i < NUMBER_OBJECTS; i++)
		factory.From(i)->Draw(DrawAnimatingItem)->HitPoints(NOT_TARGETABLE)->Radius(10)->FrameBase(frames);

	BaddyObjects();
	ObjectObjects();
	TrapObjects();
	InitialiseHair();
	InitialiseEffects();

	for (int i = 0; i < 6; i++)
		SequenceUsed[i] = 0;
	for (int i = 0; i < 8; i++)
		LibraryTab[i] = 0;

	NumRPickups = 0;
	CurrentSequence = 0;
	SequenceResults[0][1][2] = 0;
	SequenceResults[0][2][1] = 1;
	SequenceResults[1][0][2] = 2;
	SequenceResults[1][2][0] = 3;
	SequenceResults[2][0][1] = 4;
	SequenceResults[2][1][0] = 5;
}

void GetAIPickups()
{
	ITEM_INFO* item;
	AIOBJECT* aiObj;
	short aiObjNum;

	for (int i = 0; i < level_items; i++)
	{
		item = &items[i];

		if (objects[item->object_number].intelligent)
		{
			item->ai_bits = 0;

			for (aiObjNum = 0; aiObjNum < nAIObjects; aiObjNum++)
			{
				aiObj = &AIObjects[aiObjNum];

				if (aiObj->x == item->pos.x_pos && aiObj->z == item->pos.z_pos &&
					aiObj->room_number == item->room_number && aiObj->object_number < AI_PATROL2)
				{
					item->ai_bits |= 1 << (aiObj->object_number - AI_GUARD);
					item->item_flags[3] = aiObj->trigger_flags;
					aiObj->room_number = NO_ROOM;
				}
			}
		}
	}
}

void GetCarriedItems()
{
	for (int i = 0; i < level_items; i++)
	{
		auto* baddy = &items[i];
		baddy->carried_item = NO_ITEM;

		auto* baddyobj = &objects[baddy->object_number];
		if (baddyobj->intelligent)
		{
			short item_num = room[baddy->room_number].item_number;
			while (item_num != NO_ITEM)
			{
				auto* pickup = &items[item_num];
				auto* obj = &objects[pickup->object_number];
				if (baddy->pos.x_pos == pickup->pos.x_pos && abs(baddy->pos.y_pos - pickup->pos.y_pos) < 256 && baddy->pos.z_pos == pickup->pos.z_pos && obj->collision && obj->is_pickup)
				{
					pickup->carried_item = baddy->carried_item;
					baddy->carried_item = item_num;
					RemoveDrawnItem(item_num);
					pickup->room_number = NO_ROOM;
				}
				item_num = pickup->next_item;
			}
		}
	}
}

void InitTarget()
{
	short** meshpp;
	short* meshp;
	short nVtx;

	meshpp = &meshes[objects[TARGET_GRAPHICS].mesh_index];
	meshp = *meshpp;
	targetMeshP = (MESH_DATA*)meshp;
	nVtx = meshp[4] & 0xFF;
	meshp += 6;

	for (int i = 0; i < nVtx; i++)
	{
		meshp[0] = short(phd_centerx + 80 * meshp[0] / 96);
		meshp[1] = short(phd_centery + 60 * meshp[1] / 224);
		meshp += 4;
	}
}

void SetupGame()
{
	SeedRandomDraw(0xD371F947);
	SeedRandomControl(0xD371F947);
	wibble = 0;
	ClearFootPrints();
	InitBinoculars();
	InitTarget();
	trainmappos = 0;
	InitialiseGameFlags();
	InitialiseLara();
	GetCarriedItems();
	GetAIPickups();
	SeedRandomDraw(0xD371F947);
	SeedRandomControl(0xD371F947);
}
