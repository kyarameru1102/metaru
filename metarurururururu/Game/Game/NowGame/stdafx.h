#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>
#include <string>
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTK�̃C���N���[�h�t�@�C���B
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "physics/Physics.h"
#include "HID/Pad.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"

#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"
#include "GameObjectManager.h"
#include "Game.h"
#include "level/Level.h"
#include "SkinModelRender.h"
#include "StopWatch.h"
#include "GameTime.h"


const float FRAME_BUFFER_W = 1280.0f;				//�t���[���o�b�t�@�̕��B
const float FRAME_BUFFER_H = 720.0f;				//�t���[���o�b�t�@�̍����B

static const int MAX_BONE = 512;	//!<�{�[���̍ő吔�B