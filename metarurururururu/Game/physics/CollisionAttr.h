/*!
 * @brief	コリジョン属性
 */
#pragma once

/*!
 * @brief	コリジョン属性の大分類。
 */
enum EnCollisionAttr{
	enCollisionAttr_map,
	enCollisionAttr_Character,
	enCollisionAttr_User,
	enCollisionAttr_ground,				//地面。
	enCollisionAttr_obstacle,			//カメラの判定から外したいときにこの属性を入れる。
};