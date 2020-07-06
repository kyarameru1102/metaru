#include "stdafx.h"
#include "SkinModelRender.h"
#include "GameTime.h"

SkinModelRender::SkinModelRender()
{
}
SkinModelRender::~SkinModelRender()
{

}
void SkinModelRender::OnDestroy()
{
}

void SkinModelRender::Init(const wchar_t * filePath, AnimationClip * animationClips, int numAnimationClips, EnFbxUpAxis fbxUpAxis)
{
	m_skinModel.Init(filePath,fbxUpAxis);
	InitAnimation(animationClips, numAnimationClips);
}
void SkinModelRender::Init(const wchar_t * filePath,bool blend, AnimationClip * animationClips, int numAnimationClips, EnFbxUpAxis fbxUpAxis)
{
	m_blend = blend;
	m_skinModel.Init(filePath, fbxUpAxis);
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	m_skeletonUpper.Load(skeletonFilePath.c_str());
	m_skeletonLower.Load(skeletonFilePath.c_str());

	InitAnimation(animationClips, numAnimationClips,m_skeletonUpper,m_skeletonLower);
	if (m_blend) {
		m_blendSkeleton.Load(skeletonFilePath.c_str());
		m_skinModel.SetSkeleton(m_blendSkeleton);
	}
}

void SkinModelRender::InitAnimation(AnimationClip * animationClips, int numAnimationClips)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClips != nullptr) {
		m_animation.Init(m_skinModel, m_animationClips, m_numAnimationClips);
		m_animation2.Init(m_skinModel, m_animationClips, m_numAnimationClips);
	}
}

void SkinModelRender::InitAnimation(AnimationClip * animationClips, int numAnimationClips, Skeleton& skeletonUpper,Skeleton& skeletonLower)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClips != nullptr) {
		m_animation.Init(skeletonUpper, m_animationClips, m_numAnimationClips);
		m_animation2.Init(skeletonLower, m_animationClips, m_numAnimationClips);
	}
}

bool SkinModelRender::Start()
{
	return true;
}

void SkinModelRender::Update()
{
	m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);	
	if(m_blend){
		m_animation.Update(GameTime().GetFrameDeltaTime(), m_blend);
		m_animation2.Update(GameTime().GetFrameDeltaTime(), m_blend);
		float weight[31];		//ウエイトテーブル。
		weight[0] = 0.0f;		//基点。0でいい。
		weight[1] = 0.0f;
		weight[2] = 0.0f;
		weight[3] = 0.0f;
		weight[4] = 0.0f;
		weight[5] = 0.0f;
		weight[6] = 0.0f;
		weight[7] = 0.0f;
		weight[8] = 0.0f;
		weight[9] = 0.0f;
		weight[10] = 0.0f;
		weight[11] = 0.0f;
		weight[12] = 0.2f;
		weight[13] = 0.2f;
		weight[14] = 0.2f;
		weight[15] = 0.5f;
		weight[16] = 0.5f;
		weight[17] = 0.5f;
		weight[18] = 0.5f;
		weight[19] = 0.5f;		//左足下半身付け根。
		weight[20] = 0.8f;		//
		weight[21] = 1.0f;		//左足下半身つま先。
		weight[22] = 0.5f;		//右足下半身付け根。
		weight[23] = 0.8f;		//
		weight[24] = 1.0f;		//右足下半身つま先。
		weight[25] = 0.0f;
		weight[26] = 0.0f;
		weight[27] = 0.0f;
		weight[28] = 0.0f;
		weight[29] = 0.0f;
		weight[30] = 0.0f;

		m_blendSkeleton.BlendBoneMatrix(m_skeletonLower, m_skeletonUpper, weight);
	}
	else {
		m_animation.Update(GameTime().GetFrameDeltaTime(), m_blend);
		m_animation2.Update(GameTime().GetFrameDeltaTime(), m_blend);
	}
	
	//シャドウレシーバーを登録。
	m_skinModel.SetShadowReciever(m_shadowRecieverOn);
	if (m_shadowCasterOn) {
		//シャドウマップに登録
		ShadowMap::GetShadowMap().RegistShadowCaster(&m_skinModel);
	}
	m_renderOK = true;
}


void SkinModelRender::Render()
{
	if (m_renderOK) {
		//通常レンダリング。
		if (m_RenderOn) {
			m_skinModel.Draw(
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}
}

