#pragma once
#include <variant>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <array>
#include "raylib.h"
#include "options.h"

/** \file utils.h
* @brief Contains very special methods, types and enumerations for different parts of the game.
*/

typedef std::variant<KeyboardKey, MouseButton> anyinput_t; ///< The std::variant that can store KeyboardKey or MouseButton.
typedef std::vector<Texture2D> animation_sequence_t; ///< Sequence that is used in \ref Animation::m_animationSequence.

enum class TileType {
    WALL, FLOOR, UNKNOWN, AIR
};


enum class TileRotation { // TODO: make this enum float
    LEFT = 0,
    UP = 90,
    RIGHT = 180, 
    DOWN = -90,
    LEFTUP = 45,
    RIGHTUP = 135,
    LEFTDOWN = -45,
    RIGHTDOWN = -135
};

/**
* @enum TILEMAP_LAYER
* @brief stores indexes of all tilemap layers
*/
enum TILEMAP_LAYER {
    TILEMAP_LAYER_FLOOR,
    TILEMAP_LAYER_WALL,
    TILEMAP_LAYER_ROOF
};

struct Tile {
    TileType texture = TileType::AIR;                   ///< Default texture
    TileRotation rotation = TileRotation::LEFT;     ///< Default rotation degree
};

typedef Tile Tile;


/**
* @struct Tilemap
* @brief Tilemap struct for use in \ref Environment class.
*/
/*
struct Tilemap {
    float x, y; ///< Position of the tilemap
    float size; ///< Size of entire tilemap
    float tile_width, tile_height; ///< Width and height of every tile in the tilemap
    std::unique_ptr<TilemapBuffer_t> buffer; ///< Stores info about all tiles

    Tilemap(float x, float y, float size, float tile_width, float tile_height);
};
*/

/**
* @struct PlayerControls
* @brief The struct for storing \ref GamePlayer's controls.
*/
struct playercontrols_t {
    anyinput_t move_up = KEY_W;                    ///< Moving up key
    anyinput_t move_down = KEY_S;                  ///< Moving down key
    anyinput_t move_right = KEY_D;                 ///< Moving right key
    anyinput_t move_left = KEY_A;                  ///< Moving left key
    anyinput_t camera = KEY_LEFT_SHIFT;            ///< A key to move the Camera away from the GamePlayer.
    anyinput_t attack = MOUSE_BUTTON_LEFT;         ///< Attacking key
    anyinput_t item_throw = MOUSE_BUTTON_RIGHT;    ///< Item throwing key
    anyinput_t special = KEY_Z;                    ///< Special ability key
};

typedef playercontrols_t playercontrols_t;

/*!
@brief Packs textures for further use in \ref Animation "Animations".
*/
animation_sequence_t packAnimationSequence(const std::string dirpath);


enum class TexturePoolTextureType {
    Building, AnimationFrame
};

/**
* @enum CHARACTER_STATE
* @brief Different \ref Character states for use in \ref Character::state.
*/
enum class CharacterState { // TODO: L10
	Idle, ///< Not moving
    Walking, ///< Moving
    Attacking, ///< Attacking
    Lying, ///< Lying on the floor
    Dead ///< Dead
};

/**
* @enum ANIMATION_STATE
* @brief State of \ref Animation
*/
enum class AnimationState {
    Playing,         ///< Animation is playing right now or resumed.
    Stopped,         ///< Animation is stopped by \ref Animator::Stop()
    Ended,           ///< Animation isn't looped and played till the last frame.
    Playable         ///< Animation can be played, but not playing.
};

/**
* @enum CHAR_ANIMATION_TYPE
* @brief Type of \ref Animation. Primary used as indexes for \ref Animator::m_AnimationBuffer
*/
enum class CharAnimationType {
    Idle, ///< Idle \ref Animation
    Walking, ///< Walking \ref Animation
    Punching, ///< Punching \ref Animation
    Legs, ///< Legs \ref Animation
    Unknown ///< Unknown \ref Animation
};

/**
* @enum CHARACTER_APPEARANCE
* @brief Contains different \ref Character appearances to use in \ref Character::appearance. Works just like skins in the other games
*/
enum class CharacterAppearance {
    russian_mafia = 0 ///< Russian mafia
};