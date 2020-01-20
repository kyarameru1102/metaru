#include "stdafx.h"
#include "SkinModelRender.h"
#include "GameTime.h"

SkinModelRender::SkinModelRender()
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

void SkinModelRender::InitAnimation(AnimationClip * animationClips, int numAnimationClips)
{
	m_animationClips = animationClips;
	m_numAnimationClips = numAnimationClips;
	if (m_animationClips != nullptr) {
		m_animation.Init(m_skinModel, m_animationClips, m_numAnimationClips);
		m_animation2.Init(m_skinModel, m_animationClips, m_numAnimationClips);
	}
}

bool SkinModelRender::Start()
{
	return true;
}

void SkinModelRender::Update()
{
	m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);	
	m_animation.Update(GameTime().GetFrameDeltaTime());
	m_animation2.Update(GameTime().GetFrameDeltaTime());
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

