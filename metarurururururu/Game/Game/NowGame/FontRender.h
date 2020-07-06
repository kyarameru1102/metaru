#pragma once
#include "Font.h"
class FontRender : public IGameObject
{
public:
	/// <summary>
	/// フォントを設定する関数。
	/// </summary>
	/// <param name="font">
	/// フォント　SpriteFont*
	/// </param>
	void SetFont(DirectX::SpriteFont* font)
	{
		m_font.SetFont(font);
	}
	/// <summary>
	/// テキストを設定する関数。
	/// </summary>
	/// <param name="text">
	/// テキスト　const wchar_t*
	/// </param>
	void SetText(const wchar_t* text)
	{
		if (wcscmp(m_text.c_str(), text) != 0) {
			//文字列が変わった。
			m_text = text;
		}

	}
	/// <summary>
	/// 座標を設定する関数。
	/// </summary>
	/// <param name="pos">
	/// 座標　CVector2
	/// </param>
	void SetPosition(const CVector2& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// カラーを設定する関数。
	/// </summary>
	/// <param name="color">
	/// カラー　CVector3
	/// </param>
	void SetColor(const CVector3& color)
	{
		m_color = color;
	}
	/// <summary>
	/// 回転を設定する関数。
	/// </summary>
	/// <param name="rotation">
	/// 回転　float
	/// </param>
	void SetRotation(float rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定する関数。
	/// </summary>
	/// <param name="scale">
	/// スケール　float
	/// </param>
	void SetScale(float scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ピボットを設定する関数。
	/// </summary>
	/// <param name="pivot">
	/// ピボット　CVector2
	/// </param>
	void SetPivot(const CVector2& pivot)
	{
		m_pivot = pivot;
	}

	void RenderFont();
private:
	Font m_font;
	std::wstring m_text;						//テキスト。
	const wchar_t* m_textUnsafe = nullptr;		//アンセーフ版のテキスト。
	CVector2 m_position = CVector2::Zero();		//座標。x = 0.0f, y = 0.0fで画面の中心。
	CVector4 m_color = CVector4::White();		//カラー。
	float m_rotation = 0.0f;					//回転。
	float m_scale = 1.0f;						//拡大率。
	CVector2 m_pivot;							//ピボット。
};

